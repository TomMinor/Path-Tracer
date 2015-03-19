#include <utility>
#include <cstring>

#include <ngl/Vec4.h>
#include <ngl/VAOPrimitives.h>

#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>

#include "raytracer/sphere.h"
#include "raytracer/tracemath.h"

namespace Renderer
{

ngl::Vec3 Sphere::getNormal(ngl::Vec3 _point) const
{
    ngl::Vec3 origin(m_toWorldSpace.m_03, m_toWorldSpace.m_13, m_toWorldSpace.m_23);
    ngl::Vec3 dir(_point - origin);
    dir.normalize();

    return dir;
}

bool Sphere::intersect(const Ray &_ray, HitData& _hit) const
{
    Ray objectSpaceRay = rayToObjectSpace(_ray);

    const ngl::Vec3 rayDirection = objectSpaceRay.m_direction;
    const ngl::Vec3 rayOrigin = objectSpaceRay.m_origin;

    const float a = rayDirection.dot(rayDirection);
    const float b = rayDirection.dot(rayOrigin) * 2.0f;
    const float c = rayOrigin.dot(rayOrigin) - m_squaredRadius;

    float t0, t1;
    if(!SolveQuadratic(a,b,c, t0, t1) || t1 < 0)
    {
        // No valid intersections
        // (we didn't touch the sphere or the intersection was behind the camera)
        return false;
    }

    // t1 is expected to be further away
    if(t1 < t0)
    {
        std::swap(t0, t1);
    }

    // Prefer a solution infront of the camera
    _hit.m_t = (t0 < 0) ? t1 : t0;
    _hit.m_impact = _ray(_hit.m_t);
    _hit.m_normal = getNormal(_hit.m_impact);

    //Generate UVs (http://www.mvps.org/directx/articles/spheremap.htm)
    _hit.m_u = asinf(_hit.m_normal.m_x)/M_PI + 0.5;
    _hit.m_v = asinf(_hit.m_normal.m_y)/M_PI + 0.5;
    _hit.m_w = 1 - _hit.m_u - _hit.m_v;
    _hit.m_material = m_material;

    return true;
}

ngl::Vec3 Sphere::sample() const
{
    ///@todo Make this more uniform
    float x,y,z;

    // Rejection sampling
    do
    {
        x = drand48();
        y = drand48();
        z = drand48();
    }while(x*x + y*y + z*z > 1);

    return ngl::Vec3(x,y,z);
}

void Sphere::draw() const
{
    //glCullFace(GL_FRONT);
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    prim->draw("sphere");
    //glCullFace(GL_BACK);
}

}
