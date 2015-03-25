#ifndef TRACEMATH_H
#define TRACEMATH_H

#include <cmath>
#include <functional>

#include "ngl/Vec3.h"
#include "ngl/Mat4.h"

///
/// \brief SolveQuadratic
/// \param _a
/// \param _b
/// \param _c
/// \param o_x0 Output value of first solution
/// \param o_x1 Output value of second solution
/// \return true if any solutions were found, else false
///
bool SolveQuadratic(float _a, float _b, float _c, float& o_x0, float & o_x1);

///
/// \brief degreesToRadians
/// \param _deg
/// \return
///
float degreesToRadians(float _deg);

//unused
float integrate( std::function< float(float) >& F, std::function< float(float) >& f, float a, float b, unsigned int steps );

inline float inverseSquare(float _distanceSquared)
{
    return 1.0f / _distanceSquared;
}

///
/// \brief transformPosition transforms a point vector by a transform matrix
/// \param _point
/// \param _transform
/// \return the transformed vector
///
ngl::Vec3 transformPosition(const ngl::Vec3& _point, const ngl::Mat4& _transform);

///
/// \brief transformNormal
/// \param _normal
/// \param _transform
/// \return
///
ngl::Vec3 transformNormal(const ngl::Vec3& _normal, const ngl::Mat4& _transform);

#endif // TRACEMATH_H

