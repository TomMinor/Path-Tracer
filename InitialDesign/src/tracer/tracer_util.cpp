#include "tracer/tracer_util.h"

bool SolveQuadratic(const ngl::Real &_a, const ngl::Real &_b, const ngl::Real &_c, ngl::Real &o_x0, ngl::Real &o_x1)
{
    ngl::Real discriminant = _b*_b - 4*_a*_c;

    // No real roots
    if(discriminant < 0)
    {
        return false;
    }
    // 1 root
    else if(discriminant == 0)
    {
        o_x0 = -0.5 * _b / _a;
        o_x1 = o_x0;
    }
    // 2 roots
    else
    {
        ngl::Real q = (_b>0) ? -0.5 * (_b + sqrt(discriminant)) : -0.5 * (_b - sqrt(discriminant));
        o_x0 = q / _a;
        o_x1 = _c / q;
    }
    if(o_x0 > o_x1)
    {
        std::swap(o_x0, o_x1);
    }
    return true;
}
