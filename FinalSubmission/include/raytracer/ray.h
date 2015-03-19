#ifndef __RAY_H_
#define __RAY_H_

#include <ngl/Vec3.h>
#include <limits>

namespace Renderer
{

struct Ray
{
public:
    enum RayType { UNKNOWN = 0, CAMERA, SHADOW }; ///@todo Make the ray polymorphic?

    Ray(const ngl::Vec3& _origin, const ngl::Vec3& _direction, RayType _type = UNKNOWN, float _near = 0.0, float _far = std::numeric_limits<float>::max())
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
    inline ngl::Vec3 operator () (float _t) const
    {
        return m_origin + (m_direction * _t);
    }

    static inline ngl::Vec3 reflect(ngl::Vec3 _incident, ngl::Vec3 _normal)
    {
      return ngl::Vec3(_incident - (2 * _incident.dot(_normal) * _normal));
    }

public:
    ngl::Vec3 m_origin;
    ngl::Vec3 m_direction;

    // Precomputed for ray-box intersection
    ngl::Vec3 m_inverseDirection;
    int m_sign[3];

    // Maximum/minimum distance
    float m_tmin;
    float m_tmax;

    RayType m_type;

protected:
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

}

#endif
