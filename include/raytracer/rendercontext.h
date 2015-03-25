#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <vector>
#include <ngl/Mat4.h>
#include <ngl/Colour.h>
#include <ngl/Camera.h>

#include "raytracer/primitive.h"
#include "raytracer/camera.h"
#include "raytracer/scene.h"

namespace Renderer
{


class Scene;

/**
 * @brief The RendererContext class is a convenience class that allows us to pass around scene information
 */
struct RenderContext
{
    RenderContext(Scene *_scene,
                  Camera *_camera,
                   unsigned int _imageWidth,
                   unsigned int _imageHeight,
                   int _samples = 16,
                   const std::string& _outputPath = ""
                 );

    ~RenderContext();

    //-------------------------------------------------
    // Output Image Settings
    //-------------------------------------------------

    ///
    /// \brief m_imageWidth is the rendered image width
    ///
    unsigned int m_imageWidth;

    ///
    /// \brief m_imageHeight is the rendered image height
    ///
    unsigned int m_imageHeight;

    ///
    /// \brief m_samples is the number of samples to fire per pixel
    ///
    unsigned int m_samples;

    ///
    /// \brief m_aspectRatio is the render image aspect ratio
    ///
    float m_aspectRatio;

    ///
    /// \brief m_depth stores the current ray depth
    ///
    int m_depth; // Reflection/refraction depth

    ///
    /// \brief m_renderCamera is the render camera, necessary by many parts of the raytracer
    ///
    Camera* m_renderCamera;

    ///
    /// \brief m_scene stores all the primitives in the world
    ///
    Scene* m_scene;

    ///
    /// \brief m_outputPath is the path to save the rendered image to
    ///
    std::string m_outputPath;

    ///
    /// \brief m_hit stores data about the last ray-primitive intersection
    ///
    HitData m_hit;
};

}

#endif // RENDERCONTEXT_H

