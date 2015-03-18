#ifndef TRACEMATH_H
#define TRACEMATH_H

#include <cmath>
#include <functional>

bool SolveQuadratic(float _a, float _b, float _c, float& o_x0, float & o_x1);

float degreesToRadians(float _deg);

float integrate( std::function< float(float) >& F, std::function< float(float) >& f, float a, float b, unsigned int steps );

#endif // TRACEMATH_H

