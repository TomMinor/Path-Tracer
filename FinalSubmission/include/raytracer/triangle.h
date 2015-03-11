#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "raytracer/primitive.h"

namespace Renderer
{

class Triangle : public Primitive
{
public:
    Triangle(ngl::Vec3 _v0, ngl::Vec3 _v1, ngl::Vec3 _v2, const ngl::Mat4 _toWorldSpace, bool _singleSided = true)
        : Primitive(_toWorldSpace), m_v0(_v0), m_v1(_v1), m_v2(_v2), m_singleSided(_singleSided)
    {}

    virtual bool intersect(const Ray<float>& _ray, ngl::Real &_t) const override;
    virtual void draw(ngl::Mat4 &_globalMouseTx, const RenderContext& _context) override;

private:
    ngl::Vec3 m_v0;
    ngl::Vec3 m_v1;
    ngl::Vec3 m_v2;

    bool m_singleSided;
};

}

#endif // TRIANGLEMESH_H

