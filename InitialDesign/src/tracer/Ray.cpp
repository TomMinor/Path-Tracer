#include <tracer/Ray.h>
#include <ngl/Camera.h>

Ray::Ray(ngl::Vec3 &_origin, ngl::Vec3 &_direction, ngl::Real _near, ngl::Real _far, Raytype type)
    : m_origin(_origin), m_direction(_direction), m_tmin(_near), m_tmax(_far), m_type(type)
{
    // Precompute the inverse
    m_inverseDir = inverseDirection();

    // Figure out the sign of the x,y,z components of direction
    m_sign[0] = (m_inverseDir[0] < 0);
    m_sign[1] = (m_inverseDir[1] < 0);
    m_sign[2] = (m_inverseDir[2] < 0);
}

ngl::Vec3 Ray::operator () (const ngl::Real &_t) const
{
    return m_origin + (m_direction * _t);
}

ngl::Vec3 Ray::inverseDirection() const
{
    ngl::Real numerator = 1.0f;
    return ngl::Vec3( numerator / m_direction.m_x,
                      numerator / m_direction.m_y,
                      numerator / m_direction.m_z );
}
