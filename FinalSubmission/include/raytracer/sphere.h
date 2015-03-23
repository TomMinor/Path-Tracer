#ifndef SPHERE_H
#define SPHERE_H

#include "raytracer/primitive.h"

namespace Renderer
{

///
/// \brief The Sphere class represents a sphere defined by a radius and transform
///
class Sphere : public Primitive
{
public:
    ///
    /// \brief Sphere ctor
    /// \param _toWorldSpace
    /// \param _material
    /// \param _radius
    ///
    Sphere(const ngl::Mat4 &_toWorldSpace, const Material& _material, float _radius = 1.0f)
        : Primitive(_toWorldSpace, _material), m_radius(_radius), m_squaredRadius(_radius * _radius)
    {;}

    virtual bool intersect(const Ray &_ray, HitData& _hit) const;

    virtual void draw() const;

    virtual ngl::Vec3 sample() const;

    virtual ngl::Vec3 getNormal(ngl::Vec3 _point) const;

    //http://people.cs.kuleuven.be/~philip.dutre/GI/TotalCompendium.pdf pg 17
    ngl::Vec3 tangentSphere(const ngl::Vec3 _point, const ngl::Vec3 _direction) const;

private:
    float m_radius;
    float m_squaredRadius;

};

}

#endif // SPHERE_H

