#ifndef PLANE_H
#define PLANE_H

#include "raytracer/primitive.h"
#include "raytracer/triangle.h"

namespace Renderer
{

// Most of the implementation is heavily borrowed from this, but it is completely untested and isn't used right now
// (2 triangles are used to represent a plane instead, although the code may be easier to use with a specialised plane primitive
// https://github.com/imageworks/OpenShadingLanguage/blob/master/src/testrender/raytracer.h
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

