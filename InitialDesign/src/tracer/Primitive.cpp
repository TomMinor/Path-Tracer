#include <ngl/Random.h>
#include "tracer/Primitive.h"

Primitive::Primitive(const ngl::Mat4 &_objectToWorld, ngl::Camera *_sceneCamera)
    : m_objectToWorld(_objectToWorld), m_sceneCamera(_sceneCamera)
{
    ngl::Random *random = ngl::Random::instance();

    m_pos = ngl::Vec3(_objectToWorld.m_30, _objectToWorld.m_31, _objectToWorld.m_32);

    ngl::Mat4 inverse = _objectToWorld;
    m_worldToObject = inverse.inverse();
    m_colour = random->getRandomColour();
}
