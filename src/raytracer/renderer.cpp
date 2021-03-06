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

          /// @brief Don't bother with checking objects in the scene if the surface normal & light direction are in opposing directions
          /// Modified from :-
          /// Codermind team, Ray Tracing Tutorial [online].
          /// [Accessed 2015]. Available from: <http://www.ics.uci.edu/~gopi/CS211B/RayTracing%20tutorial.pdf> Pg 8.
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
                  // Convert to object space for intersection
                  newRay = (*object)->Primitive::rayToObjectSpace(newRay);

                  if((*object)->intersect(newRay, hitResult))
                  {
                     isShaded = true;
                     break; // If we collide with one object we know this pixel is shaded, no need to check the rest of the scene
                  }
              }
          }

          if(!isShaded)
          {
              Ray tmp(hitResult.m_ray);

              ngl::Colour tint = (*light)->getSurfaceMaterial().m_diffuse;
              ngl::Vec3 camera = shadowOrigin;
              ngl::Vec3 shadow = shadowEnd;

              // Convert sample point to world space
              //shadow = transformPosition(shadow, (*light)->worldTransform());

              float distance = (camera - shadow).length(); // This should be squared, for some reason the actual length computes a nicer falloff

              emitted += Image::Pixel(tint.m_r, tint.m_g, tint.m_b) * inverseSquare(distance);
          }
      }
    }

    // Reflections look terrible right now, let's turn them off
    bool enableReflectionTest = false;

    _context->m_depth++;

    // Hard limit to the depth (4 is a magic number for now, we should move it into render settings)
    if(enableReflectionTest && _context->m_depth < 4)
    {
        // Assume equal chance of reflection or refraction
        if(drand48() > 0.5)
        {
            ngl::Vec3 reflectedDir(shadowOrigin.reflect(worldSpaceNormal));

            reflectedDir += ngl::Vec3(drand48(), drand48(),drand48() );

            Ray reflectedRay(shadowOrigin, reflectedDir, Ray::REFLECT);
            reflected = trace(reflectedRay, _context) * pow(0.25, _context->m_depth);
        }
        else
        {
            ///@todo Refraction, snell's law?
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


    HitData hitResult;
    const Primitive* collidingObject = NULL;

    float nearestHit = _ray(_ray.m_tmax).length(); // Get maximum length ray
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
        /// Objects are drawn in order they were read from the file, this could be related to the
        /// sphere rendering errors
        if(distance < nearestHit)
        {
          nearestHit = -distance;
          collidingObject = hitResult.m_object;
        }
      }
    }

    Image::Pixel finalColour;
    if(collidingObject != NULL)
    {
        _context->m_hit = hitResult;

        finalColour = Image::Pixel(_context->m_hit.m_object->getSurfaceMaterial().m_diffuse) * shade(_context);
    }
    else
    {
        finalColour = _context->m_scene->getBackgroundColour();
    }

    return finalColour;
  }

  void render(RenderContext *_context)
  {
    ImagePPM image(_context->m_imageWidth, _context->m_imageHeight);

    // Convert world origin to camera's world space
    ngl::Vec3 rayOrigin = (ngl::Vec4() * _context->m_renderCamera->getWorldSpaceMatrix()).toVec3();

    /// @brief Much improved performance (even without using threads)
    /// Modified from :-
    /// Iñigo Quílez, Tile Rendering [online].
    /// [Accessed 2015]. Available from: <http://www.iquilezles.org/www/articles/cputiles/cputiles.htm>.
    const int tilesize = 8;

    const int numxtiles = _context->m_imageWidth / tilesize;
    const int numytiles = _context->m_imageHeight / tilesize;
    const int numtiles = numxtiles * numytiles;

    // Render tiles
    for(int tile=0; tile < numtiles; tile++)
    {
      const int ia = tilesize * (tile % numxtiles);
      const int ja = tilesize * (tile / numxtiles);

      // Compute colour for each pixel in this tile
      for(unsigned int j = 0; j < tilesize; ++j)
      {
        for(unsigned int i = 0; i < tilesize; ++i)
        {
            Image::Pixel result;

            const int samples = _context->m_samples;
            for(int s = 0; s < samples; s++)
            {
                ///@todo Add a better sampler (stratified?), move into Sampler class, put in render context
                /// https://www.cs.duke.edu/courses/spring09/cps111/notes/sampling.pdf
                float jitterX = drand48();
                float jitterY = drand48();

                // Canonical Coordinates (camera space)
                // The jitter is necessary for path tracing to get a variety of samples, but also provides free anti-aliasing
                float x = (2 * ((ia + i + jitterX) / _context->m_imageWidth) - 1) * _context->m_aspectRatio * _context->m_renderCamera->getAngle();
                float y = (1 - 2 *((ja + j + jitterY) / _context->m_imageHeight)) * _context->m_renderCamera->getAngle();

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

          image.setPixel(result, ia + i, ja + j);
        }
      }

      const float percentile = (float)(tile * 1.0 / numtiles) * 100;
      qDebug("Render progress : %.2f%%", percentile);
    }
    qDebug("Render complete.");

    if(_context->m_outputPath.length() > 0)
    {
      image.save(_context->m_outputPath);
    }
    else
    {
      qWarning("Empty output path");
    }
  }
}
