#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Colour.h>
#include <ngl/Camera.h>

#include <tracer/Ray.h>


/**
 * @brief The Primitive class is the abstract class representing a renderer primitive,
 * the intersect method must be overloaded in all child classes.
 */
class Primitive
{
public:
    Primitive(const ngl::Mat4 &_objectToWorld, ngl::Camera *_sceneCamera);

    virtual bool intersect(const Ray& _ray, ngl::Real &_t) const = 0;
    virtual void draw(ngl::Mat4 &_globalMouseTx) {;}

    ngl::Colour getSurfaceColour() const { return m_colour; }

//protected:
    // Used to easily convert between world and object space
    ngl::Mat4 m_objectToWorld;
    ngl::Mat4 m_worldToObject;

    // These are just used for drawing right now
    ngl::Vec3 m_pos;
    ngl::Camera* m_sceneCamera;

    ngl::Colour m_colour;
};

#endif // PRIMITIVE_H
