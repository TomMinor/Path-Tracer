#ifndef __RAY_H_
#define __RAY_H_

#include <limits>
#include <ngl/Vec3.h>

enum Raytype
{
    UNKNOWNRAY = 0,
    CAMERARAY,
    SHADOWRAY
};

/**
 * @brief The Ray class represents a ray with it's origin, direction and t min/max.
 */
class Ray
{
public:
    Ray(ngl::Vec3 &_origin, ngl::Vec3 &_direction,  ngl::Real _near = 0.0, ngl::Real _far = std::numeric_limits<ngl::Real>::max());

    // Unused right now, seems handy
    ngl::Vec3 operator () (const ngl::Real &_t) const;

    ngl::Vec3 m_origin;
    ngl::Vec3 m_direction;

    // Ray min and max distances
    ngl::Real m_tmin;
    ngl::Real m_tmax;

    // Precomputed for ray-box intersection
    ngl::Vec3 m_inverseDir;
    int m_sign[3];

    // This isn't useful yet, but it will be when shadow rays are implemented
    Raytype m_type;

private:

    ///
    /// \brief inverseDirection
    /// \return The inverse vector of the current direction
    ///
    ngl::Vec3 inverseDirection() const;
};

#endif
