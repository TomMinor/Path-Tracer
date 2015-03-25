#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>

#include "raytracer/ray.h"
#include "raytracer/material.h"

namespace Renderer
{

///
/// \brief The HitData structure is passed around during the rendering process, storing important hit data required by all parts of the pipeline
///
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

const double EPSILON = 1.0e-6;

/**
 * @brief The Primitive class is the abstract class representing a renderer primitive,
 * the intersect and draw methods must be overloaded in child classes.
 */
class Primitive
{
public:
    ///
    /// \brief Primitive constructs the to world and to object transform matrices
    /// \param _toWorldSpace
    /// \param _material
    ///
    Primitive(const ngl::Mat4 &_toWorldSpace, const Material &_material);

    virtual ~Primitive() {;}

    ///
    /// \brief intersect samples all the objects in the scene to figure out if the ray intersects
    /// \param _ray expects a ray in worldspace
    /// \param _hit is filled with data if there is a collision, otherwise it is not
    /// \return true if there is a collision, false if not
    ///
    virtual bool intersect(const Ray& _ray, HitData& _hit) const = 0;

    ///
    /// \brief draws the primitive in the 3d visualisation
    ///
    virtual void draw() const = 0;

    ///
    /// \brief samples a point on the surface, used for lighting
    /// \return a point on the primitive surface
    ///
    virtual ngl::Vec3 sample() const = 0;

    ///
    /// \brief getNormal calculates the surface normal a certain point
    /// \param _point a point on the surface
    /// \return the normal on the surface at _point
    ///
    virtual ngl::Vec3 getNormal(ngl::Vec3 _point) const = 0;

    ///
    /// \brief rayToObjectSpace converts _ray to this primitive's object space
    /// \param _ray
    /// \return the transformed ray
    ///
    Ray rayToObjectSpace(const Ray& _ray) const;

    ///
    /// \brief rayToWorldSpace converts _ray to this primitive's world space
    /// \param _ray
    /// \return the transformed ray
    ///
    Ray rayToWorldSpace(const Ray& _ray) const;

    ///
    /// \brief getSurfaceMaterial
    /// \return the primitive's surface material
    ///
    inline Material getSurfaceMaterial() const { return m_material; }

    ///
    /// \brief objectTransform
    /// \return  the primitive's object space transform matrix
    ///
    inline const ngl::Mat4& objectTransform() const { return m_toObjectSpace; }

    ///
    /// \brief worldTransform
    /// \return  the primitive's object worldtransform matrix
    ///
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

    Material m_material;
};

}

#endif // PRIMITIVE_H


