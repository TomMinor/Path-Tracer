#ifndef __RAY_H_
#define __RAY_H_

#include <ngl/Vec3.h>
#include <limits>

template <typename T>
struct Ray
{
public:
    enum RayType { UNKNOWN = 0, CAMERA, SHADOW };

    Ray(const ngl::Vec3& _origin, const ngl::Vec3& _direction, T _near = 0.0, T _far = std::numeric_limits<T>::max(), RayType _type = UNKNOWN);

    ///
    /// \brief operator () Return the position of t along the ray
    /// \param _t
    /// \return
    ///
    ngl::Vec3 operator () (const ngl::Real &_t) const;

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
    ngl::Vec3 inverseDirection() const;
};

#endif
