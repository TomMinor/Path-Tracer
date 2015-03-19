#include "raytracer/plane.h"

namespace Renderer
{

Plane::Plane(ngl::Vec3 _p1, ngl::Vec3 _p2, ngl::Vec3 _p3, ngl::Vec3 _p4, const ngl::Mat4 &_toWorldSpace, const ngl::Colour& _colour)
    : Primitive(_toWorldSpace, _colour), m_t1(NULL), m_t2(NULL)
{
    ngl::Vec3 origin( (_p1.m_x + _p3.m_x)/2,
                                          0,
                      (_p1.m_z + _p4.m_z)/2 );

    _p1 -= origin;
    _p2 -= origin;
    _p3 -= origin;
    _p4 -= origin;

    m_t1 = new Triangle(_p1, _p2, _p3, _toWorldSpace, _colour);
    m_t2 = new Triangle(_p3, _p4, _p1, _toWorldSpace, _colour);
}

ngl::Vec3 Plane::getNormal(ngl::Vec3 _point) const
{
    return m_t1->getNormal(_point);
}

Plane::~Plane()
{
    delete m_t1;
    delete m_t2;
}

bool Plane::intersect(const Ray<float> &_ray, HitData &_hit) const
{
    return m_t1->intersect(_ray, _hit) || m_t2->intersect(_ray, _hit);
}

void Plane::draw() const
{
    m_t1->draw();
    m_t2->draw();
}

}
