#include "raytracer/rendercontext.h"

RenderContext::RenderContext(const Scene* _scene,
                             unsigned int _imageWidth,
                             unsigned int _imageHeight)
    : m_imageWidth(_imageWidth),
      m_imageHeight(_imageHeight)

{
    m_aspectRatio = (float)m_imageWidth / (float)m_imageHeight;

}
