#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "raytracer/ray.h"
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Colour.h>

namespace Renderer
{

struct HitData
{
    float m_t;
    float m_u;
    float m_v;
    float m_w;
    ngl::Vec3 m_normal;
};

/**
 * @brief The Primitive class is the abstract class representing a renderer primitive,
 * the intersect and draw methods must be overloaded in child classes.
 */
class Primitive
{
public:
    Primitive(const ngl::Mat4 &_toWorldSpace, const ngl::Colour& _colour = ngl::Colour(0.5, 0.5, 0.5));
    virtual ~Primitive() {;}

    virtual bool intersect(const Ray<float>& _ray, HitData& _hit) const = 0;

    virtual void draw() const = 0;

    virtual ngl::Vec3 getNormal(ngl::Vec3 _point) const = 0;

    /**
     * @brief convertRayToObjectSpace is a helper method to transform a ray into this primitives object space
     * @param _ray
     * @return
     */
    Ray<float> rayToObjectSpace(const Ray<float>& _ray) const;

    ngl::Colour getSurfaceColour() const { return m_colour; }

    inline const ngl::Mat4& objectTransform() const { return m_toObjectSpace; }
    inline const ngl::Mat4& worldTransform() const { return m_toWorldSpace; }

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

    ngl::Colour m_colour;
};

}

#endif // PRIMITIVE_H


