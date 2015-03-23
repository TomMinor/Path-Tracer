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
    unsigned int m_imageWidth;
    unsigned int m_imageHeight;
    unsigned int m_samples;
    float m_aspectRatio;


    int m_depth; // Reflection/refraction depth

    Camera* m_renderCamera;
    Scene* m_scene;

    std::string m_outputPath;

    HitData m_hit;
};

}

#endif // RENDERCONTEXT_H

