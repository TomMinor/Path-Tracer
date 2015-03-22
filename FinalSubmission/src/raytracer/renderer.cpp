#include "raytracer/renderer.h"
#include "raytracer/imagePPM.h"
#include "raytracer/sphere.h"
#include "raytracer/triangle.h"
#include <QDebug>

namespace Renderer
{
  Image::Pixel shade(RenderContext *_context)
  {
    //http://www.cs.utah.edu/~shirley/books/fcg2/rt.pdf
    ngl::Colour colour(0, 0, 0);

    // Inverse square falloff

    /*  for each light source
     *      test visibility of random position on light source
     *          if visible
     *              color += direct illumination
     *  color += trace(random ray)
     */

    ngl::Vec3 origin = _context->m_hit.m_surfaceImpact + (_context->m_hit.m_surfaceNormal * 0.0005);
    origin = transformPosition(origin, _context->m_hit.m_object->worldTransform());

    for(Scene::objectListIterator light = _context->m_scene->objectBegin();
        light != _context->m_scene->objectEnd();
        light++)
    {
      Material::SurfaceProperty type = (*light)->getSurfaceMaterial().m_type;
      if( type == Material::EMISSIVE)
      {
          ngl::Vec3 end = (*light)->sample();
          end = transformPosition(end, (*light)->worldTransform());

          ngl::Vec3 direction = origin - end;
          direction.normalize();

          Ray newRay(origin, direction, Ray::SHADOW);
          HitData hitResult;

          bool isHit = false;
          float nearestT = newRay.m_tmax;
          for(Scene::objectListIterator object = _context->m_scene->objectBegin();
              object != _context->m_scene->objectEnd();
              object++)
          {
                  Material::SurfaceProperty type = (*object)->getSurfaceMaterial().m_type;
                  if( type != Material::EMISSIVE)
                  {
                      newRay = (*object)->Primitive::rayToObjectSpace(newRay);
                      if((*object)->intersect(newRay, hitResult))
                      {
                          //if(hitResult.m_t < nearestT && hitResult.m_t > newRay.m_tmin)
                          {
                             isHit = true;
                             break;
                          }
                      }
                  }
          }

          if(isHit)
          {
                colour += ngl::Colour(0,0,0 );
          }
          else
          {
                Ray tmp(hitResult.m_ray);

                ngl::Colour tint = (*light)->getSurfaceMaterial().m_diffuse;
                colour += tint ;
          }
      }
    }

    //colour += trace

    //_context->m_hit.m_surfaceNormal.normalize();

//    ngl::Vec3 a = _context->m_hit.m_surfaceNormal;
//    if(a.lengthSquared() < 0)
//        a = -a;

//    float tmp = std::max(0.f, (float)_context->m_hit.m_ray.m_direction.dot(a) * 1);

    return Image::Pixel(colour.m_r, colour.m_g, colour.m_b);
  }

  Image::Pixel trace(const Ray &_ray, RenderContext *_context)
  {
    Image::Pixel primitiveColour;
    float nearestT = _ray.m_tmax;
    float nearestHit = 2000000;

    HitData hitResult;
    const Primitive* tmp = NULL;

    for(Scene::objectListIterator object = _context->m_scene->objectBegin();
        object != _context->m_scene->objectEnd();
        object++)
    {
      Ray newRay = (*object)->rayToObjectSpace(_ray);
      if( (*object)->intersect(newRay, hitResult) )
      {
        if(hitResult.m_t < nearestT && hitResult.m_t > hitResult.m_ray.m_tmin)
        {
          nearestT = hitResult.m_t;
          tmp = hitResult.m_object;
        }
      }
    }

    Image::Pixel c;
    if(tmp != NULL)
    {
        _context->m_hit = hitResult;
        _context->m_hit.m_surfaceImpact = hitResult.m_surfaceImpact;

//        primitiveColour = (*object)->getSurfaceColour();
//        primitiveColour.m_r = fabs(std::min<float>(hitResult.m_u, 1.0));
//        primitiveColour.m_g = fabs(std::min<float>(hitResult.m_v, 1.0));
//        primitiveColour.m_b = fabs(std::min<float>(hitResult.m_w, 1.0));

//        primitiveColour.m_r = tmp->getSurfaceColour().m_r;
//        primitiveColour.m_g = tmp->getSurfaceColour().m_g;
//        primitiveColour.m_b = tmp->getSurfaceColour().m_b;

        ngl::Colour col = _context->m_hit.m_object->getSurfaceMaterial().m_diffuse;
        c.m_r = col.m_r;
        c.m_g = col.m_g;
        c.m_b = col.m_b;

        c *= shade(_context);// * primitiveColour;

//        ngl::Vec3 a = hitResult.m_surfaceImpact;
//        a.normalize();

//        primitiveColour.m_r = hitResult.m_t / (hitResult.m_ray.m_tmax / 4.0);
//        primitiveColour.m_g = hitResult.m_t / (hitResult.m_ray.m_tmax / 4.0);
//        primitiveColour.m_b = hitResult.m_t / (hitResult.m_ray.m_tmax / 4.0);

//        primitiveColour.m_r = a.m_x;
//        primitiveColour.m_g = a.m_y;
//        primitiveColour.m_b = a.m_z;

//        primitiveColour.m_r = sqrt(hitResult.m_distanceSqr) / 16;
//        primitiveColour.m_g = sqrt(hitResult.m_distanceSqr) / 16;
//        primitiveColour.m_b = sqrt(hitResult.m_distanceSqr) / 16;
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

          const int samples = 4;
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

              // World space ray
              Ray ray(rayOrigin,
                      rayDirection,
                      Ray::CAMERA,
                      _context->m_renderCamera->getNearClipPlane(),
                      _context->m_renderCamera->getfarClipPlane()
                      );

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
