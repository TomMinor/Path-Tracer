#include "raytracer/ray.h"

template <typename T>
Ray<T>::Ray(const ngl::Vec3 &_origin, const ngl::Vec3 &_direction, T _near, T _far, Ray<T>::RayType _type)
    : m_origin(_origin), m_direction(_direction), m_tmin(_near), m_tmax(_far), m_type(_type)
{
    // Precompute the inverse
    m_inverseDirection = inverseDirection();

    // Precompute the sign of the x,y,z components of direction
    m_sign[0] = (m_inverseDirection[0] < 0);
    m_sign[1] = (m_inverseDirection[1] < 0);
    m_sign[2] = (m_inverseDirection[2] < 0);
}

template <typename T>
ngl::Vec3 Ray<T>::operator () (const ngl::Real &_t) const
{
    return m_origin + (m_direction * _t);
}

template <typename T>
ngl::Vec3 Ray<T>::inverseDirection() const
{
    ngl::Real numerator = 1.0f;
    return ngl::Vec3( numerator / m_direction.m_x,
                      numerator / m_direction.m_y,
                      numerator / m_direction.m_z );
}
