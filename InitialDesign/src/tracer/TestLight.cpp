#include <tracer/TestLight.h>

TestLight::TestLight(ngl::Mat4 _lightToWorld, ngl::Colour _colour)
    : m_lightToWorld(_lightToWorld), m_colour(_colour)
{
    m_worldToLight = m_lightToWorld.inverse();
}
