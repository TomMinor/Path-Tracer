#ifndef RENDERERCONTEXT_H
#define RENDERERCONTEXT_H

#include <vector>
#include <ngl/Colour.h>
#include <tracer/Primitive.h>
#include <tracer/Camera.h>
#include <tracer/TestLight.h>

/**
 * @brief The RendererContext class is a convenience class that allows us to pass around important
 */
class RendererContext
{
public:
    RendererContext( std::vector<Primitive*> &_primitives,
                     std::vector<TestLight*> &_lights,
                     const ngl::Mat4 &_camToWorld,
                     float _fov,
                     unsigned int _imageWidth,
                     unsigned int _imageHeight,
                     ngl::Colour _background);

    unsigned int m_imageWidth;
    unsigned int m_imageHeight;
    float m_aspectRatio;

    ngl::Colour m_background;
    ngl::Colour *m_pixels;
    std::vector<Primitive*> &m_primitives;
    std::vector<TestLight*> &m_lights;

    Camera m_camera;
};

#endif // RENDERERCONTEXT_H