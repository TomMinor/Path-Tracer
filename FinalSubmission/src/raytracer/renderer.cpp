#include "raytracer/renderer.h"
#include "raytracer/imagePPM.h"
#include "raytracer/sphere.h"
#include "raytracer/triangle.h"
#include <QDebug>

namespace Renderer
{
  Image::Pixel shade(RenderContext *_context)
  {

    Image::Pixel emitted(0, 0, 0);
    Image::Pixel reflected(0, 0, 0);

    // Start the shadow ray from the intersection point
    // The impact point was in object space, we must convert it to world space for it to be useful
    ngl::Vec3 shadowOrigin = transformPosition(_context->m_hit.m_surfaceImpact, _context->m_hit.m_object->worldTransform());;

    // Correct possibility of intersecting with the surface itself,
    // by offsetting it very slightly along the normal (also in world space)
    ngl::Vec3 worldSpaceNormal = transformNormal(_context->m_hit.m_surfaceNormal, _context->m_hit.m_object->worldTransform());
    shadowOrigin += worldSpaceNormal * 0.005f;

    // Gather light from all emissive surfaces in the scene
    for(Scene::objectListIterator light = _context->m_scene->objectBegin();
        light != _context->m_scene->objectEnd();
        light++)
    {
      if( (*light)->getSurfaceMaterial().m_type == Material::EMISSIVE)
      {
          ngl::Vec3 sample = (*light)->sample();

          // Sample a random point on the emissive surface, then convert to world space
          ngl::Vec3 shadowEnd = transformPosition( sample, (*light)->worldTransform() );

          // Calculate the world space direction vector
          ngl::Vec3 direction = shadowOrigin - shadowEnd;
          direction.normalize();

          // Don't bother with checking objects in the scene if the surface normal & light direction are opposing
          // http://www.ics.uci.edu/~gopi/CS211B/RayTracing%20tutorial.pdf pg 8
          if(_context->m_hit.m_object->getNormal(shadowOrigin).dot(direction) <= 0.f)
          {
            continue;
          }

          HitData hitResult;
          bool isShaded = false;

          // Check for an intersection against any object in the scene
          for(Scene::objectListIterator object = _context->m_scene->objectBegin();
              object != _context->m_scene->objectEnd();
              object++)
          {
              Material::SurfaceProperty type = (*object)->getSurfaceMaterial().m_type;
              if( type != Material::EMISSIVE)
              {
                  Ray newRay(shadowOrigin, direction, Ray::SHADOW);
                  newRay = (*object)->Primitive::rayToObjectSpace(newRay);

                  if((*object)->intersect(newRay, hitResult))
                  {
                     isShaded = true;
                     break; // If we collide with one object we know this pixel is shaded
                  }
              }
          }

          if(!isShaded)
          {
                Ray tmp(hitResult.m_ray);

                ngl::Colour tint = (*light)->getSurfaceMaterial().m_diffuse;
                ngl::Vec3 camera = shadowOrigin;
                ngl::Vec3 shadow = shadowEnd;

                // Convert to world space
                //shadow = transformPosition(shadow, (*light)->worldTransform());

                float distance = (camera - shadow).length(); // This should be squared, for some reason the actual length computes a nicer falloff

                emitted += Image::Pixel(tint.m_r, tint.m_g, tint.m_b) * inverseSquare(distance);
          }
      }
    }

    _context->m_depth++;

    // Hard limit to the depth (4 is a magic number for now, we should move it into render settings)
    if(false && _context->m_depth < 4)
    {
        // Assume equal chance of reflection or refraction
        if(drand48() > 0.5)
        {
          ngl::Vec3 reflectedDir(shadowOrigin.reflect(worldSpaceNormal));

          reflectedDir += ngl::Vec3(drand48(), drand48(),drand48() );

          Ray reflectedRay(shadowOrigin, reflectedDir, Ray::REFLECT);
          reflected = trace(reflectedRay, _context) * pow(0.25, _context->m_depth) ;
        }
        else
        {
//          float theta =

//          // http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
//          Ray refracted(shadowOrigin, shadowOrigin.reflect(worldSpaceNormal), Ray::REFRACT);
//          colour += trace(refracted, _context) * pow(0.5, _context->m_depth) ;
        }
    }

    return emitted + reflected;
  }

  Image::Pixel trace(const Ray &_ray, RenderContext *_context)
  {
    float nearestHit = 20000;

    HitData hitResult;
    const Primitive* tmp = NULL;

    float distance = _ray(_ray.m_tmax).length();

    for(Scene::objectListIterator object = _context->m_scene->objectBegin();
        object != _context->m_scene->objectEnd();
        object++)
    {
      // Construct a ray in the primitive's object space and check for collision
      Ray newRay = (*object)->rayToObjectSpace(_ray);
      if( (*object)->intersect(newRay, hitResult) )
      {
        ngl::Vec3 camera = _ray.m_origin;
        ngl::Vec3 shadow = hitResult.m_surfaceImpact;

        // Convert to world space
        shadow = transformPosition(shadow, hitResult.m_object->worldTransform());

        distance = (camera - shadow).length();

        ///@bug Depth fails completely right now, checking for t value failed too
        if(distance < nearestHit)
        {
          nearestHit = -distance;
          tmp = hitResult.m_object;
        }
      }
    }

    Image::Pixel c;
    if(tmp != NULL)
    {
        _context->m_hit = hitResult;

        ngl::Colour col = _context->m_hit.m_object->getSurfaceMaterial().m_diffuse;
        c.m_r = col.m_r;
        c.m_g = col.m_g;
        c.m_b = col.m_b;

        c *= shade(_context);
    }
    else
    {
      c = _context->m_scene->getBackgroundColour();
    }

    return Image::Pixel(c.m_r, c.m_g, c.m_b);
  }

  ///@todo Refraction, snell's law?
  ///@todo Light path weight, inverse square?
  ///@todo Path tracer/Photon mapping
  void render(RenderContext *_context)
  {
    ImagePPM pixels(_context->m_imageWidth, _context->m_imageHeight);

    ngl::Vec3 rayOrigin = (ngl::Vec4() * _context->m_renderCamera->getWorldSpaceMatrix()).toVec3();

    for(unsigned int j = 0; j < _context->m_imageHeight; ++j)
    {
      for(unsigned int i = 0; i < _context->m_imageWidth; ++i)
      {
          Image::Pixel result;

          const int samples = 16;
          for(int s = 0; s < samples; s++)
          {
              ///@todo Add a better sampler (stratified?), move into Sampler class, put in render context
              /// https://www.cs.duke.edu/courses/spring09/cps111/notes/sampling.pdf
              float jitterX = drand48();
              float jitterY = drand48();

              ///@todo Move into function and explain this a bit (canonical coordinates?)
              float x = (2 * ((i + jitterX) / _context->m_imageWidth) - 1) * _context->m_aspectRatio * _context->m_renderCamera->getAngle();
              float y = (1 - 2 *((j + jitterY) / _context->m_imageHeight)) * _context->m_renderCamera->getAngle();

              ngl::Vec3 cameraPosition = (ngl::Vec4(x, y, -1, 1) * _context->m_renderCamera->getWorldSpaceMatrix()).toVec3();
              ngl::Vec3 rayDirection = cameraPosition - rayOrigin;
              rayDirection.normalize();

              // Construct a ray from camera into the world (World Space)
              Ray ray(rayOrigin,
                      rayDirection,
                      Ray::CAMERA,
                      _context->m_renderCamera->getNearClipPlane(),
                      _context->m_renderCamera->getfarClipPlane()
                      );

              _context->m_depth = 0;
              result += trace(ray, _context);
          }

        result /= samples;

        pixels.setPixel(result, i, j);
      }

      const float percentile = ((float)((_context->m_imageWidth * j)) /
                                        (_context->m_imageWidth * _context->m_imageHeight)) * 100;

      qDebug("Render progress : %.2f%%", percentile);
    }

    qDebug("Render complete.");

    if(_context->m_outputPath.length() > 0)
    {
      pixels.save(_context->m_outputPath);
    }
  }
}
