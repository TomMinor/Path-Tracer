#include "raytracer/tracemath.h"
#include <ngl/Vec4.h>

bool SolveQuadratic(float _a, float _b, float _c, float &o_x0, float &o_x1)
{
    float discriminant = (_b*_b) - (4*_a*_c);

    if(discriminant > 0) // 2 real roots
    {
        o_x0 = (-(_b) - discriminant )/ (2 * _a);
        o_x1 = (-(_b) + discriminant )/ (2 * _a);
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

float integrate( std::function< float(float) >& F,
                 std::function< float(float) >& f,
                 float a,
                 float b,
                 unsigned int steps = 100 )
{
    float exact = F(b) - F(a);

    // compute approximation using Riemann sum
    float dt = (b - a) / steps;
    float time = dt;
    float sum = 0;

    for (unsigned int i = 1; i <= steps; ++i) {
        float delta_d = f(time) * dt;
        sum += delta_d;
        time += dt;
    }

    return exact;
}

ngl::Vec3 transformPosition(const ngl::Vec3& _point, const ngl::Mat4& _transform)
{
    ngl::Mat4 tmp = _transform;
    tmp.transpose();
    ngl::Vec3 transformedPoint = (tmp * ngl::Vec4(_point)).toVec3();

    //rayOrigin.normalize();

    return transformedPoint;
}

ngl::Vec3 transformNormal(const ngl::Vec3& _normal, const ngl::Mat4& _transform)
{
    ngl::Mat4 tmp = _transform;
    tmp.m_30 = 0;
    tmp.m_31 = 0;
    tmp.m_32 = 0;
//    tmp.transpose();

    ngl::Vec3 transformedNormal = ( tmp * ngl::Vec4(_normal)).toVec3(); // Apply world transforms to ray direction
    transformedNormal.normalize();

    return transformedNormal;

}
