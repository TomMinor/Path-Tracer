#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "raytracer/primitive.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <vector>

namespace Renderer
{

class Triangle : public Primitive
{
public:
    Triangle(ngl::Vec3 _v0,
             ngl::Vec3 _v1,
             ngl::Vec3 _v2,
             const ngl::Mat4 _toWorldSpace,
             const Material& _material,
             bool _singleSided = true);

    virtual bool intersect(const Ray& _ray, HitData& _hit) const;

    virtual void draw() const;

    virtual ngl::Vec3 sample() const;

    virtual ngl::Vec3 getNormal(ngl::Vec3 _point) const;

    ~Triangle();


private:
    ngl::Vec3 m_vtx[3];
    ngl::Vec3 m_normals[3];

    ngl::VertexArrayObject *m_vao;

    bool m_singleSided;
};

}

#endif // TRIANGLEMESH_H

