#include <ngl/Vec4.h>
#include <ngl/NGLStream.h>
#include <fstream>
#include <algorithm>

#include <tracer/Scene.h>
#include <tracer/Framebuffer.h>

ngl::Colour Scene::shade(const Ray& _ray, const Primitive& _primitive)
{
    return _primitive.getSurfaceColour();
}

bool Scene::trace(const Ray& _ray, ngl::Colour &_spectrum)
{
    ngl::Real tClosest = _ray.m_tmax;
    ngl::Colour primitiveColour;
    Primitive* hitObject = NULL;

    for(size_t i=0; i < m_context->m_primitives.size(); ++i)
    {
        ngl::Real t = 0.0f;
        if(m_context->m_primitives[i]->intersect(_ray, t))
        {
            if( t < tClosest && t > _ray.m_tmin)
            {
                tClosest = t;
                hitObject = m_context->m_primitives[i];

                ngl::Vec3 hit = ngl::Vec4(ngl::Vec4(_ray(t)) * m_context->m_camera.m_cameraToWorld).toVec3();
                ngl::Colour result;

                if(_ray.m_type == CAMERARAY)
                {
                    ngl::Vec4 lightPos = ngl::Vec4(1, 6, -1.0f);

                    ngl::Vec3 rayOrigin = ngl::Vec4(lightPos * m_context->m_camera.m_cameraToWorld).toVec3();
                    ngl::Vec3 rayDirection = hit - rayOrigin;
                    rayDirection.normalize();

                    Ray ray(rayOrigin, rayDirection, m_context->m_camera.m_nearClippingPlane, m_context->m_camera.m_farClippingPlane, SHADOWRAY);

                    _spectrum = m_context->m_primitives[i]->getSurfaceColour();

                    if(!trace(ray, result))
                    {
                        _spectrum *= 0.2;
                    }
                }
                else
                {
                    _spectrum = primitiveColour;
                }

                return true;
            }
        }
    }

    // Set the background if no rays intersect any objects
    _spectrum =  m_context->m_background;

    return false;
}

Framebuffer Scene::render(const RendererContext* _context)
{
    m_context = _context;

    Framebuffer image(_context->m_imageWidth, _context->m_imageHeight);

    ngl::Vec3 rayOrigin = ngl::Vec4(ngl::Vec4() * _context->m_camera.m_cameraToWorld).toVec3();

    for(uint32_t j = 0; j < _context->m_imageHeight; ++j)
    {
        for(uint32_t i = 0; i < _context->m_imageWidth; ++i)
        {
            // Split this up into a separate function
            float x = (2 * ((i + 0.5) / _context->m_imageWidth) - 1)  * _context->m_aspectRatio * _context->m_camera.m_angle;
            float y = (1 - 2 * ((j + 0.5) / _context->m_imageHeight)) * _context->m_camera.m_angle;

            ngl::Vec4 cameraPos = ngl::Vec4(x,y,-1.0f) * _context->m_camera.m_cameraToWorld;
            ngl::Vec3 rayDirection = cameraPos.toVec3() - rayOrigin;
            rayDirection.normalize();

            Ray ray(rayOrigin, rayDirection, _context->m_camera.m_nearClippingPlane, _context->m_camera.m_farClippingPlane);

            ngl::Colour rayColour;
            trace(ray, rayColour);

            image.setPixel(i, j, rayColour);
        }
    }

    return image;
}

