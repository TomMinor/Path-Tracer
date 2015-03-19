#ifndef SPHERE_H
#define SPHERE_H

#include "raytracer/primitive.h"

namespace Renderer
{

class Sphere : public Primitive
{
public:
    Sphere(const ngl::Mat4 &_toWorldSpace, const Material& _material, float _radius = 1.0f)
        : Primitive(_toWorldSpace, _material), m_radius(_radius), m_squaredRadius(_radius * _radius)
    {;}

    virtual bool intersect(const Ray &_ray, HitData& _hit) const;

    virtual void draw() const;

    virtual ngl::Vec3 sample() const;

    virtual ngl::Vec3 getNormal(ngl::Vec3 _point) const;

private:
    float m_radius;
    float m_squaredRadius;

};

}

#endif // SPHERE_H

