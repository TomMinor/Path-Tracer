#ifndef TRACER_UTIL_H
#define TRACER_UTIL_H

#include <cmath>
#include <utility>

#include <ngl/Vec3.h>

bool SolveQuadratic(const ngl::Real &_a, const ngl::Real &_b, const ngl::Real &_c, ngl::Real &o_x0, ngl::Real &o_x1);

#endif // TRACER_UTIL_H
