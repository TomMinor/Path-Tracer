#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <vector>
#include <ngl/Mat4.h>
#include <ngl/Colour.h>
#include <ngl/Camera.h>

#include "raytracer/primitive.h"
#include "raytracer/renderercamera.h"

/// @todo Implement this class
class TestLight;
class Primitive;

/**
 * @brief The RendererContext class is a convenience class that allows us to pass around scene information
 */
struct RenderContext
{
    RenderContext( const std::vector<Primitive*> &_primitives,
                   const std::vector<TestLight*> &_lights,
                   const ngl::Mat4 &_toWorldSpace,
                   float _fov,
                   unsigned int _imageWidth,
                   unsigned int _imageHeight,
                   ngl::Colour _background = ngl::Colour(0.f, 0.f, 0.f) // Default to black
                   );

    ~RenderContext();

    //-------------------------------------------------
    // Output Image Settings
    //-------------------------------------------------
    unsigned int m_imageWidth;
    unsigned int m_imageHeight;
    float m_aspectRatio;

    //-------------------------------------------------
    // Scene Settings /// @todo Maybe this should be it's own Scene class?
    //-------------------------------------------------
    const std::vector<Primitive*> m_primitives;
    const std::vector<TestLight*> m_lights;
    RendererCamera *m_camera;
    ngl::Camera *m_sceneCamera;

    ngl::Colour m_background;
    ngl::Colour *m_pixels; /// @todo This should have its own, more efficient type

};

#endif // RENDERCONTEXT_H

