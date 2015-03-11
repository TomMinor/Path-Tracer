#include "raytracer/tracemath.h"

bool SolveQuadratic(float _a, float _b, float _c, float &o_x0, float &o_x1)
{
    float discriminant = (_b*_b) - (4*_a*_c);

    if(discriminant > 0) // 2 real roots
    {
        o_x0 = (-(_b) + discriminant )/ (2 * _a);
        o_x1 = (-(_b) - discriminant )/ (2 * _a);
    }
    else if(discriminant == 0) // Only 1 real root
    {
        o_x0 = o_x1 = -(_b) / (2 * _a);
    }
    else // No real roots
    {
        o_x0 = o_x1 = 0;
        return false;
    }

    return true;
}

float degreesToRadians(float _deg)
{
    return _deg * (M_PI / 180.0f);
}
