#include "tracer/RendererContext.h"


RendererContext::RendererContext( std::vector<Primitive*> &_primitives, std::vector<TestLight*> &_lights, const ngl::Mat4 &_camToWorld, float _fov, unsigned int _imageWidth, unsigned int _imageHeight, ngl::Colour _background)
    : m_primitives(_primitives), m_lights(_lights), m_imageWidth(_imageWidth), m_imageHeight(_imageHeight), m_background(_background)
{
    m_aspectRatio = m_imageWidth / (float)m_imageHeight;
    m_camera.set(_camToWorld, _fov);
}
