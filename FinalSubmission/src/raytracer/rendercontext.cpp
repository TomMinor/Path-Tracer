#include "raytracer/rendercontext.h"

RenderContext::RenderContext(const std::vector<Primitive *> &_primitives,
                             const std::vector<TestLight *> &_lights,
                             const ngl::Mat4 &_toWorldSpace,
                             float _fov,
                             unsigned int _imageWidth,
                             unsigned int _imageHeight,
                             ngl::Colour _background)
    : m_imageWidth(_imageWidth),
      m_imageHeight(_imageHeight),
      m_primitives(_primitives),
      m_lights(_lights),
      m_background(_background)
{
    m_aspectRatio = (float)m_imageWidth / (float)m_imageHeight;
    m_camera = new RendererCamera( _toWorldSpace, _fov );
}
