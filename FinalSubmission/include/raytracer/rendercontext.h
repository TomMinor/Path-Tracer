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
                   unsigned int _imageWidth,
                   unsigned int _imageHeight,
                   const std::string& _outputPath = ""
                 );

    ~RenderContext();

    //-------------------------------------------------
    // Output Image Settings
    //-------------------------------------------------
    unsigned int m_imageWidth;
    unsigned int m_imageHeight;
    float m_aspectRatio;

    std::string m_outputPath;

    Scene* m_scene;
};

}

#endif // RENDERCONTEXT_H

