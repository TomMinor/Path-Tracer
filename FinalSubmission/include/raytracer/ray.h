#ifndef __RAY_H_
#define __RAY_H_

#include <ngl/Vec3.h>
#include <limits>

template <typename T>
struct Ray
{
public:
    enum RayType { UNKNOWN = 0, CAMERA, SHADOW };

    Ray(const ngl::Vec3& _origin, const ngl::Vec3& _direction, RayType _type = UNKNOWN, T _near = 0.0, T _far = std::numeric_limits<T>::max())
        : m_origin(_origin), m_direction(_direction), m_tmin(_near), m_tmax(_far), m_type(_type)
    {
        // Precompute the inverse
        m_inverseDirection = inverseDirection();

        // Precompute the sign of the x,y,z components of direction
        m_sign[0] = (m_inverseDirection[0] < 0);
        m_sign[1] = (m_inverseDirection[1] < 0);
        m_sign[2] = (m_inverseDirection[2] < 0);
    }

    ///
    /// \brief operator () Return the position of t along the ray
    /// \param _t
    /// \return
    ///
    inline ngl::Vec3 operator () (const T &_t) const
    {
        return m_origin + (m_direction * _t);
    }

public:
    ngl::Vec3 m_origin;
    ngl::Vec3 m_direction;

    // Precomputed for ray-box intersection
    ngl::Vec3 m_inverseDirection;
    int m_sign[3];

    // Maximum/minimum distance
    T m_tmin;
    T m_tmax;

    const RayType m_type;

private:
    ///
    /// \brief inverseDirection
    /// \return The inverse vector of the current direction
    ///
    ngl::Vec3 inverseDirection() const
    {
        ngl::Real numerator = 1.0f;
        return ngl::Vec3( numerator / m_direction.m_x,
                          numerator / m_direction.m_y,
                          numerator / m_direction.m_z );
    }
};

#endif
