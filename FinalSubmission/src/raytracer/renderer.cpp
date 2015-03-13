#include "raytracer/renderer.h"
#include "raytracer/imagePPM.h"

namespace Renderer
{
  void trace(const Ray<float> &_ray, const RenderContext *_context, Image::Pixel& o_colour)
  {
    Primitive* hit = NULL;
    ngl::Colour primitiveColour;
    float nearestT = _ray.m_tmax;

    for(Scene::objectListIterator object = _context->m_scene->objectBegin();
        object != _context->m_scene->objectEnd();
        object++)
    {
      Primitive* tmp = *object;
      float t;
      if( (*object)->intersect(_ray, t) )
      {
        if(t < nearestT && t > _ray.m_tmin)
        {
          nearestT = t;
          hit = *object;
          primitiveColour = (*object)->getSurfaceColour();
        }
      }
    }

    ngl::Colour c;
    if(hit != NULL)
    {
      c = hit->getSurfaceColour();
    }
    else
    {
      c = _context->m_scene->getBackgroundColour();
    }

    o_colour.m_r = c.m_r;
    o_colour.m_g = c.m_g;
    o_colour.m_b = c.m_b;
  }

  void render(const RenderContext *_context)
  {
    Camera* renderCamera = _context->m_scene->getRenderCamera();
    ImagePPM pixels(_context->m_imageWidth, _context->m_imageHeight);

    ngl::Vec3 rayOrigin = (ngl::Vec4() * renderCamera->getWorldSpaceMatrix()).toVec3();

    for(unsigned int j = 0; j < _context->m_imageHeight; ++j)
    {
      for(unsigned int i = 0; i < _context->m_imageWidth; ++i)
      {
        ///@todo Move into function and explain this a bit
        float x = (2 * ((i + 0.5) / _context->m_imageWidth) - 1) * _context->m_aspectRatio * renderCamera->getAngle();
        float y = (1 - 2 *((j + 0.5) / _context->m_imageHeight)) * renderCamera->getAngle();

        ngl::Vec3 cameraPosition = (ngl::Vec4(x, y, -1, 1) * renderCamera->getWorldSpaceMatrix()).toVec3();
        ngl::Vec3 rayDirection = cameraPosition - rayOrigin;
        rayDirection.normalize();

        Ray<float> ray(rayOrigin,
                       rayDirection,
                       Ray<float>::CAMERA,
                       renderCamera->getNearClipPlane(),
                       renderCamera->getfarClipPlane()
                       );

        Image::Pixel rayColour;
        trace(ray, _context, rayColour);
        pixels.setPixel(rayColour, i, j);
      }
    }

    if(_context->m_outputPath.length() > 0)
    {
      pixels.save(_context->m_outputPath);
    }

  }

}
