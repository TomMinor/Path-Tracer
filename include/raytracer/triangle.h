#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "raytracer/primitive.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <vector>

namespace Renderer
{

///
/// \brief The Triangle class represents a triangle with 3 vertices and a transform
///
class Triangle : public Primitive
{
public:
    ///
    /// \brief Triangle
    /// \param _v0
    /// \param _v1
    /// \param _v2
    /// \param _toWorldSpace
    /// \param _material
    /// \param _singleSided
    ///
    Triangle(ngl::Vec3 _v0,
             ngl::Vec3 _v1,
             ngl::Vec3 _v2,
             const ngl::Mat4 _toWorldSpace,
             const Material& _material,
             bool _singleSided = true);

    ///
    /// \brief intersect
    /// \param _ray
    /// \param _hit
    /// \return
    ///
    virtual bool intersect(const Ray& _ray, HitData& _hit) const;

    ///
    /// \brief draw
    ///
    virtual void draw() const;

    ///
    /// \brief sample
    /// \return
    ///
    virtual ngl::Vec3 sample() const;

    ///
    /// \brief getNormal
    /// \param _point
    /// \return
    ///
    virtual ngl::Vec3 getNormal(ngl::Vec3 _point) const;

    ///
    /// \brief getTangent
    /// \return the tangent to the face
    ///
    ngl::Vec3 getTangent() const;

    ~Triangle();


private:
    ///
    /// \brief m_vtx triangle data
    ///
    ngl::Vec3 m_vtx[3];

    ///
    /// \brief m_normals normal data (face normal)
    ///
    ngl::Vec3 m_normals[3];

    ngl::VertexArrayObject *m_vao;

    ///
    /// \brief m_singleSided determines whether the face should be rendered as double sided
    ///
    bool m_singleSided;
};

}

#endif // TRIANGLEMESH_H

