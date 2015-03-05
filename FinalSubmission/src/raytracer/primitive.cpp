#include "raytracer/primitive.h"

#include <ngl/Random.h>

Primitive::Primitive(const ngl::Mat4 &_toWorldSpace)
    : m_toWorldSpace(_toWorldSpace)
{
    ngl::Random *random = ngl::Random::instance();

    m_position = ngl::Vec3(m_toWorldSpace.m_30, m_toWorldSpace.m_31, m_toWorldSpace.m_32);

    m_toObjectSpace= m_toWorldSpace.inverse();
    m_colour = random->getRandomColour();
}
