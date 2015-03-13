#ifndef SPHERE_H
#define SPHERE_H

#include "raytracer/primitive.h"

namespace Renderer
{

class Sphere : public Primitive
{
public:
    Sphere(const ngl::Mat4 &_toWorldSpace, float _radius = 1.0f)
        : Primitive(_toWorldSpace), m_radius(_radius), m_squaredRadius(_radius * _radius)
    {;}

    virtual bool intersect(const Ray<float> &_ray, float &o_t) const;

private:
    float m_radius;
    float m_squaredRadius;

};

}

#endif // SPHERE_H

