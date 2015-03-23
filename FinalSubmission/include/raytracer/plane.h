#ifndef PLANE_H
#define PLANE_H

#include "raytracer/primitive.h"
#include "raytracer/triangle.h"

namespace Renderer
{


class Plane : public Primitive
{
public:
    Plane(ngl::Vec3 _p, ngl::Vec3 _x, ngl::Vec3 _y, const ngl::Mat4 &_toWorldSpace, const Material& _material);

    ~Plane();

    virtual bool intersect(const Ray &_ray, HitData& _hit) const;
    virtual void draw() const;
    virtual ngl::Vec3 sample() const;
    virtual ngl::Vec3 getNormal(ngl::Vec3 _point) const;

private:
    ngl::Vec3 m_normal;
    ngl::Vec3 m_p;
    ngl::Vec3 m_x;
    ngl::Vec3 m_y;
    float m_area;
    float m_u;
    float m_v;
};

}

#endif // PLANE_H

