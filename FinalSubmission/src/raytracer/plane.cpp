#include "raytracer/plane.h"
#include <ngl/Random.h>

namespace Renderer
{

// Heavily borrowed from https://github.com/imageworks/OpenShadingLanguage/blob/master/src/testrender/raytracer.h

Plane::Plane(ngl::Vec3 _p, ngl::Vec3 _x, ngl::Vec3 _y, const ngl::Mat4 &_toWorldSpace, const Material& _material)
    : Primitive(_toWorldSpace, _material)
{
    m_p = _p;
    m_x = _x;
    m_y = _y;
    m_normal = _x.cross(_y);
    m_area = m_normal.length();
    m_normal.normalize();
}

ngl::Vec3 Plane::sample() const
{
    ngl::Random* rand = ngl::Random::instance();

    // Unsure what these are meant to be, randomise all the things
    ngl::Vec3 x = rand->getRandomVec3();
    ngl::Vec3 xi = rand->getRandomVec3();
    ngl::Vec3 yi = rand->getRandomVec3();

    ngl::Vec3 l = (m_p + xi * m_x + yi * m_y) - x;
    float d2 = l.lengthSquared();
    l.normalize();
    //invpdf = m_area * fabsf(dir.dot(m_normal)) / d2;

    return l;

}

ngl::Vec3 Plane::getNormal(ngl::Vec3 _point) const
{
    return m_normal;
}

Plane::~Plane()
{
}

bool Plane::intersect(const Ray&_ray, HitData &_hit) const
{
    float dn = _ray.m_direction.dot(m_normal);
    float en = (m_p - _ray.m_origin).dot(m_normal);

    if(dn * en > 0)
    {
        float t = en / dn;
        ngl::Vec3 h = _ray(t) - m_p;
        float dx = h.dot(m_x) * m_u;
        float dy = h.dot(m_y) * m_v;

        if( dx >= 0 && dx < 1 && dy >= 0 && dy < 1)
        {
            _hit.m_object = this;
            _hit.m_ray = _ray;

            _hit.m_t = t;
            _hit.m_surfaceImpact = _ray(t);
            _hit.m_surfaceNormal = getNormal(_hit.m_surfaceImpact);
            _hit.m_distanceSqr =  _hit.m_surfaceImpact.lengthSquared() ;

            // These UVs probably aren't correct
            _hit.m_u = m_u;
            _hit.m_v = m_v;
        }

    }

    return false;
}

void Plane::draw() const
{

}

}
