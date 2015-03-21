#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>

#include "raytracer/ray.h"
#include "raytracer/material.h"

namespace Renderer
{

struct HitData
{
    Ray m_ray;
    float m_distanceSqr;
    float m_t;
    float m_u;
    float m_v;
    float m_w;
    ngl::Vec3 m_surfaceImpact;
    ngl::Vec3 m_surfaceNormal;
    const class Primitive* m_object;

    HitData()
        : m_ray(ngl::Vec3(), ngl::Vec3()), m_object(NULL)
    {;}
};

/**
 * @brief The Primitive class is the abstract class representing a renderer primitive,
 * the intersect and draw methods must be overloaded in child classes.
 */
class Primitive
{
public:
    Primitive(const ngl::Mat4 &_toWorldSpace, const Material &_material);
    virtual ~Primitive() {;}

    virtual bool intersect(const Ray& _ray, HitData& _hit) const = 0;

    virtual void draw() const = 0;

    virtual ngl::Vec3 sample() const = 0;

    virtual ngl::Vec3 getNormal(ngl::Vec3 _point) const = 0;

    /**
     * @brief convertRayToObjectSpace is a helper method to transform a ray into this primitives object space
     * @param _ray
     * @return
     */
    Ray rayToObjectSpace(const Ray& _ray) const;

    inline Material getSurfaceMaterial() const { return m_material; }

    inline ngl::Colour getSurfaceColour() const { return m_material.m_diffuse; }

    inline const ngl::Mat4& objectTransform() const { return m_toObjectSpace; }
    inline const ngl::Mat4& worldTransform() const { return m_toWorldSpace; }

//    inline void populateHitData(Hitdata& _hit)
//    {

//    }

protected:
    // Used to easily convert between world and object space

    /**
     * @brief m_toWorldSpace stores the object space to world space transformation matrix for this primitive.
     */
    ngl::Mat4 m_toWorldSpace;

    /**
     * @brief m_toObjectSpace stores the world space to object space transformation matrix for this primitive.
     */
    ngl::Mat4 m_toObjectSpace;

    Material m_material;
};

}

#endif // PRIMITIVE_H


