#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "raytracer/ray.h"
#include "raytracer/rendercontext.h"
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Colour.h>

// We pass in a reference to RenderContext when drawing
struct RenderContext;

/**
 * @brief The Primitive class is the abstract class representing a renderer primitive,
 * the intersect and draw methods must be overloaded in child classes.
 */
class Primitive
{
public:
    Primitive(const ngl::Mat4 &_toWorldSpace);

    virtual bool intersect(const Ray<float>& _ray, ngl::Real &_t) const = 0;
    virtual void draw(ngl::Mat4 &_globalMouseTx, const RenderContext& _context) = 0;

    /**
     * @brief convertRayToObjectSpace is a helper method to transform a ray into this primitives object space
     * @param _ray
     * @return
     */
    Ray<float> rayToObjectSpace(const Ray<float>& _ray) const;

    ngl::Colour getSurfaceColour() const { return m_colour; }
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

    // These are just used for drawing right now
    ngl::Vec3 m_position;

    ngl::Colour m_colour;
};

#endif // PRIMITIVE_H

