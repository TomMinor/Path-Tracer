#include "raytracer/rendercontext.h"

namespace Renderer
{

RenderContext::RenderContext(Scene* _scene,
                             unsigned int _imageWidth,
                             unsigned int _imageHeight,
                             const std::string &_outputPath)
    : m_scene(_scene),
      m_imageWidth(_imageWidth),
      m_imageHeight(_imageHeight),
      m_outputPath(_outputPath)

{
    m_aspectRatio = (float)m_imageWidth / (float)m_imageHeight;

}

RenderContext::~RenderContext()
{
}

}
