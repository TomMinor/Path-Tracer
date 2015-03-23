#include "raytracer/rendercontext.h"

namespace Renderer
{

RenderContext::RenderContext(Scene* _scene,
                             Camera *_camera,
                             unsigned int _imageWidth,
                             unsigned int _imageHeight,
                             const std::string &_outputPath)
    : m_imageWidth(_imageWidth),
      m_imageHeight(_imageHeight),
      m_renderCamera(_camera),
      m_scene(_scene),
      m_outputPath(_outputPath),
      m_depth(0),
      m_samples(16)

{
    m_aspectRatio = (float)m_imageWidth / (float)m_imageHeight;

}

RenderContext::~RenderContext()
{
}

}
