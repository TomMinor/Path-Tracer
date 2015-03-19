#include "raytracer/renderer.h"
#include "raytracer/imagePPM.h"
#include <QDebug>

namespace Renderer
{
  Image::Pixel shade(RenderContext *_context)
  {
    //http://www.cs.utah.edu/~shirley/books/fcg2/rt.pdf
    ngl::Colour colour(0,0,0);


    // Inverse square falloff

    /*  for each light source
     *      test visibility of random position on light source
     *          if visible
     *              color += direct illumination
     *  color += trace(random ray)
     */

    ngl::Vec3 origin = _context->m_hit.m_impact;

    int visibleLights = 0;

    for(Scene::objectListIterator light = _context->m_scene->objectBegin();
        light != _context->m_scene->objectEnd();
        light++)
    {
      Material::SurfaceProperty type = (*light)->getSurfaceMaterial().m_type;
      if( type == Material::EMISSIVE)
      {
          ngl::Vec3 end = (*light)->sample();
          ngl::Vec3 direction =  origin - end;
          direction.normalize();

          Ray newRay(origin, direction, Ray::SHADOW);

          for(Scene::objectListIterator object = _context->m_scene->objectBegin();
              object != _context->m_scene->objectEnd();
              object++)
          {
              HitData hitResult;
              if(!(*object)->intersect(newRay, hitResult))
              {
                  hitResult.m_object = *object;
                 colour += hitResult.m_object->getSurfaceMaterial().m_diffuse * (1 / (hitResult.m_t*hitResult.m_t)); // inverse square
                 visibleLights++;
              }
//            if( (*object)->intersect(_ray, hitResult) )
//            {
//              if(hitResult.m_t < nearestT && hitResult.m_t > _ray.m_tmin)
//              {
//                nearestT = hitResult.m_t;
//                hitResult.m_object = *object;
//              }
//            }
          }


      }
    }

    if(visibleLights > 0)
    {
        colour.m_r /= visibleLights;
        colour.m_g /= visibleLights;
        colour.m_b /= visibleLights;
        colour.m_a = 1;
    }

    return Image::Pixel(colour.m_r, colour.m_g, colour.m_b);
  }

  Image::Pixel  trace(const Ray &_ray, RenderContext *_context)
  {
    Image::Pixel primitiveColour;
    float nearestT = _ray.m_tmax;

    HitData hitResult;

    for(Scene::objectListIterator object = _context->m_scene->objectBegin();
        object != _context->m_scene->objectEnd();
        object++)
    {
      if( (*object)->intersect(_ray, hitResult) )
      {
        if(hitResult.m_t < nearestT && hitResult.m_t > _ray.m_tmin)
        {
          nearestT = hitResult.m_t;
          hitResult.m_object = *object;
        }
      }
    }

    Image::Pixel c;
    if(hitResult.m_object != NULL)
    {
        _context->m_hit = hitResult;

        //primitiveColour = (*object)->getSurfaceColour();
        primitiveColour.m_r = fabs(std::min<float>(hitResult.m_u, 1.0));
        primitiveColour.m_g = fabs(std::min<float>(hitResult.m_v, 1.0));
        primitiveColour.m_b = fabs(std::min<float>(hitResult.m_w, 1.0));

        primitiveColour = shade(_context);

        c = primitiveColour;
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

              Ray ray(rayOrigin,
                      rayDirection,
                      Ray::CAMERA,
                      _context->m_renderCamera->getNearClipPlane(),
                      _context->m_renderCamera->getfarClipPlane()
                      );

              result += trace(ray, _context);

              qDebug() << "Sample : " << s << " j : " << j << " i : " << i;
          }

        result /= samples;

        pixels.setPixel(result, i, j);
      }
    }

    if(_context->m_outputPath.length() > 0)
    {
      pixels.save(_context->m_outputPath);
    }

  }

}
