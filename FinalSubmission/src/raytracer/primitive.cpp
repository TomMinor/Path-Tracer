#include "raytracer/primitive.h"

#include <ngl/Random.h>


namespace Renderer
{

Primitive::Primitive(const ngl::Mat4 &_toWorldSpace)
    : m_toWorldSpace(_toWorldSpace)
{
    ngl::Random *random = ngl::Random::instance();

    m_toObjectSpace = m_toWorldSpace.inverse();
    m_colour = random->getRandomColour();
}

/**
 * @brief Primitive::rayToObjectSpace
 * @param _ray
 * @return
 */
Ray<float> Primitive::rayToObjectSpace(const Ray<float> &_ray) const
{
    /* To get correct object intersections the ray must also be transformed
     * to match the primitive's transform/scale.
     *
     * The primitive is transformed in world space, but as the ray is already in world space
     * it must be transformed by the inverse (object space) to match the primitive's transforms.
     */

    ngl::Vec3 rayOrigin;
    ngl::Vec3 rayDirection = ngl::Vec4( m_toObjectSpace * ngl::Vec4(_ray.m_direction)).toVec3(); // Apply world transforms to ray direction

    // Translate the origin coordinate ( this code was copied straight from the scratchapixel math header, needs tidying )
    /// @todo Convert this to use ngl::Mat4, unsure why the math differs
    float m[4][4];
    memcpy(m, m_toObjectSpace.m_openGL, sizeof(float)*16);

    float src[3] = { _ray.m_origin.m_x, _ray.m_origin.m_y, _ray.m_origin.m_z };
    float x = src[0] * m[0][0] + src[1] * m[1][0] + src[2] * m[2][0] + m[3][0];
    float y = src[0] * m[0][1] + src[1] * m[1][1] + src[2] * m[2][1] + m[3][1];
    float z = src[0] * m[0][2] + src[1] * m[1][2] + src[2] * m[2][2] + m[3][2];
    float w = src[0] * m[0][3] + src[1] * m[1][3] + src[2] * m[2][3] + m[3][3];
    rayOrigin.m_x = x / w;
    rayOrigin.m_y = y / w;
    rayOrigin.m_z = z / w;

    return Ray<float>(rayOrigin, rayDirection, _ray.m_type, _ray.m_tmin, _ray.m_tmax);
}

}
