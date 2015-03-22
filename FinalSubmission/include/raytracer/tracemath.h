#ifndef TRACEMATH_H
#define TRACEMATH_H

#include <cmath>
#include <functional>

#include "ngl/Vec3.h"
#include "ngl/Mat4.h"

bool SolveQuadratic(float _a, float _b, float _c, float& o_x0, float & o_x1);

float degreesToRadians(float _deg);

float integrate( std::function< float(float) >& F, std::function< float(float) >& f, float a, float b, unsigned int steps );

inline float inverseSquare(float _distanceSquared)
{
    return 1.0f / _distanceSquared;
}

ngl::Vec3 transformPosition(const ngl::Vec3& _point, const ngl::Mat4& _transform);

ngl::Vec3 transformNormal(const ngl::Vec3& _normal, const ngl::Mat4& _transform);

#endif // TRACEMATH_H

