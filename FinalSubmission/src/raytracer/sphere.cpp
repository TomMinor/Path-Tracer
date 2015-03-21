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


///@todo FIND HOME
// A limit to how deeply in recursion CalculateLighting may go
// before it gives up, so as to avoid call stack overflow.
const int MAX_OPTICAL_RECURSION_DEPTH = 20;

// A limit to how weak the red, green, or blue intensity of
// a light ray may be after recursive calls from multiple
// reflections and/or refractions before giving up.
// This intensity is deemed too weak to make a significant
// difference to the image.
const double MIN_OPTICAL_INTENSITY = 0.001;





const double EPSILON = 1.0e-6;

ngl::Vec3 Sphere::getNormal(ngl::Vec3 _point) const
{
    ngl::Vec3 origin = ngl::Vec4(ngl::Vec4() * m_toObjectSpace).toVec3();
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
    if(!SolveQuadratic(a,b,c, t0, t1) || t1 < EPSILON)
    {
        // No valid intersections
        // (we didn't touch the sphere or the intersection was inside)
        return false;
    }

//     t1 is expected to be further away
    if(t1 < t0)
    {
        std::swap(t0, t1);
    }
    // Prefer a solution infront of the camera
    _hit.m_t = (t0 < 0) ? t1 : t0;
    _hit.m_surfaceImpact = objectSpaceRay(_hit.m_t);
    _hit.m_surfaceNormal = getNormal(_hit.m_surfaceImpact);
    _hit.m_distanceSqr = _hit.m_surfaceImpact.lengthSquared();

    //Generate UVs (http://www.mvps.org/directx/articles/spheremap.htm)
    _hit.m_u = asinf(_hit.m_surfaceNormal.m_x)/M_PI + 0.5;
    _hit.m_v = asinf(_hit.m_surfaceNormal.m_y)/M_PI + 0.5;
    _hit.m_w = 1 - _hit.m_u - _hit.m_v;

    _hit.m_ray = objectSpaceRay;
    _hit.m_object = this;

    return true;
}

//http://people.cs.kuleuven.be/~philip.dutre/GI/TotalCompendium.pdf pg 19
ngl::Vec3 Sphere::sample() const
{
    ngl::Vec3 center(m_toWorldSpace.m_03, m_toWorldSpace.m_13, m_toWorldSpace.m_23);
    float r1 = drand48();
    float r2 = drand48();

    ngl::Vec3 point( center.m_x + 2 * m_radius * cos(2 * M_PI * r1) * sqrt(r2 * (1 - r2)),
                      center.m_y + 2 * m_radius * sin(2 * M_PI * r1) * sqrt(r2 * (1 - r2)),
                      center.m_z + m_radius * (1 - (2*r2))
                );


    return ngl::Vec4( point * m_toObjectSpace ).toVec3();

//    ///@todo Make this more uniform
//    float x,y,z;

//    // Rejection sampling
//    do
//    {
//        x = drand48();
//        y = drand48();
//        z = drand48();
//    }while(x*x + y*y + z*z > 1);

//    return ngl::Vec3(x,y,z);
}

ngl::Vec3 Sphere::tangentSphere(const ngl::Vec3 _point, const ngl::Vec3 _direction) const
{
    float t = getNormal(_point).dot(_direction);

    return (t >= 0) ? t : 0;
}

void Sphere::draw() const
{
    //glCullFace(GL_FRONT);
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    prim->draw("sphere");
    //glCullFace(GL_BACK);
}

}
