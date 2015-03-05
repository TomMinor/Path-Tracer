#ifndef SPHERE_H
#define SPHERE_H

#include <tracer/Primitive.h>

/**
 * @brief The Sphere class is a simple sphere primitive used by the raytracer render function.
 */
class Sphere : public Primitive
{
public:
    Sphere(ngl::Mat4 _objectToWorld, ngl::Camera *_sceneCamera, ngl::Real _radius = 1)
        : Primitive(_objectToWorld, _sceneCamera), m_radius(_radius), m_radius_sqr(_radius * _radius)
    {;}

    virtual bool intersect(const Ray &_ray, ngl::Real &_t) const;
    virtual void draw(ngl::Mat4 &_globalMouseTx);

private:
    ngl::Real m_radius;
    ngl::Real m_radius_sqr;
};

#endif // SPHERE_H
