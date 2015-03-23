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
    ngl::Vec3 origin = transformPosition( ngl::Vec3(), m_toObjectSpace);
    ngl::Vec3 dir(_point - origin);
    dir.normalize();

    return dir;
}


bool Sphere::intersect(const Ray &_ray, HitData& _hit) const
{
    const ngl::Vec3 rayDirection = _ray.m_direction;
    const ngl::Vec3 rayOrigin = _ray.m_origin;

    const float a = rayDirection.dot(rayDirection);
    const float b = rayDirection.dot(rayOrigin) * 2.0f;
    const float c = rayOrigin.dot(rayOrigin) - m_squaredRadius;

    float t0 = -1.f, t1 = -1.f;
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

    // Choose the closest intersection
    _hit.m_t = (t0 < 0) ? t1 : t0;
    _hit.m_surfaceImpact = _ray(_hit.m_t);
    _hit.m_surfaceNormal = getNormal(_hit.m_surfaceImpact);

    //Generate UVs (http://www.mvps.org/directx/articles/spheremap.htm)
    _hit.m_u = asinf(_hit.m_surfaceNormal.m_x)/M_PI + 0.5;
    _hit.m_v = asinf(_hit.m_surfaceNormal.m_y)/M_PI + 0.5;
    _hit.m_w = 1 - _hit.m_u - _hit.m_v;

    _hit.m_ray = _ray;
    _hit.m_object = this;

    return true;
}


/// @brief Sphere sampling
/// Modified from :-
/// Philip Dutré (September 29, 2003). Global Illumination Compendium [online].
/// [Accessed 2015]. Available from: <http://people.cs.kuleuven.be/~philip.dutre/GI/TotalCompendium.pdf> (page 17).
ngl::Vec3 Sphere::sample() const
{
    ngl::Vec3 center = transformPosition( ngl::Vec3(), m_toObjectSpace);
    float r1 = drand48();
    float r2 = drand48();

    ngl::Vec3 point( center.m_x + 2 * m_radius * cos(2 * M_PI * r1) * sqrt(r2 * (1 - r2)),
                      center.m_y + 2 * m_radius * sin(2 * M_PI * r1) * sqrt(r2 * (1 - r2)),
                      center.m_z + m_radius * (1 - (2*r2))
                );

    return point;
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
