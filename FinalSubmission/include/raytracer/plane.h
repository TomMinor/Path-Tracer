#ifndef PLANE_H
#define PLANE_H

#include "raytracer/primitive.h"
#include "raytracer/triangle.h"

namespace Renderer
{

class Plane : public Primitive
{
public:
    Plane(ngl::Vec3 _p1, ngl::Vec3 _p2, ngl::Vec3 _p3, ngl::Vec3 _p4, const ngl::Mat4 &_toWorldSpace, const ngl::Colour& _colour);

    ~Plane();

    virtual bool intersect(const Ray<float> &_ray, float &o_t) const;
    virtual void draw() const;

private:
    Triangle* m_t1;
    Triangle* m_t2;
};

}

#endif // PLANE_H

