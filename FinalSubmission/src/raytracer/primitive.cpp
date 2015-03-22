#include "raytracer/primitive.h"
#include "raytracer/tracemath.h"

#include <ngl/Random.h>


namespace Renderer
{

//Primitive::Primitive(const ngl::Mat4 &_toWorldSpace)
//    : m_toWorldSpace(_toWorldSpace)
//{
//    ngl::Random *random = ngl::Random::instance();

//    m_toObjectSpace = m_toWorldSpace.inverse();
//    m_colour = random->getRandomColour();
//}

Primitive::Primitive(const ngl::Mat4 &_toWorldSpace,
                     const Material& _material)
    : m_toWorldSpace(_toWorldSpace), m_material(_material)
{
  m_toObjectSpace = m_toWorldSpace.inverse();
}

/**
 * @brief Primitive::rayToObjectSpace
 * @param _ray
 * @return
 */
Ray Primitive::rayToObjectSpace(const Ray &_ray) const
{
    /* To get correct object intersections the ray must also be transformed
     * to match the primitive's transform/scale.
     *
     * The primitive is transformed in world space, but as the ray is already in world space
     * it must be transformed by the inverse (object space) to match the primitive's transforms.
     */

    ngl::Vec3 rayOrigin = transformPosition(_ray.m_origin, m_toObjectSpace);
    ngl::Vec3 rayDirection = transformNormal(_ray.m_direction , m_toObjectSpace);

//    ngl::Mat4 tmp = m_toObjectSpace;
//    tmp.transpose();
//    ngl::Vec3 rayOrigin = ngl::Vec4( tmp * ngl::Vec4(_ray.m_origin)).toVec3();;
//    ngl::Vec3 rayDirection = ngl::Vec4( m_toObjectSpace * ngl::Vec4(_ray.m_direction)).toVec3(); // Apply world transforms to ray direction

//    //rayOrigin.normalize();
//    rayDirection.normalize();

    return Ray(rayOrigin, rayDirection, _ray.m_type, _ray.m_tmin, _ray.m_tmax);
}

/**
 * @brief Primitive::rayToWorldSpace
 * @param _ray
 * @return
 */
Ray Primitive::rayToWorldSpace(const Ray &_ray) const
{
    /* To get correct object intersections the ray must also be transformed
     * to match the primitive's transform/scale.
     *
     * The primitive is transformed in world space, but as the ray is already in world space
     * it must be transformed by the inverse (object space) to match the primitive's transforms.
     */

    ngl::Vec3 rayOrigin = transformPosition(_ray.m_origin, m_toWorldSpace);
    ngl::Vec3 rayDirection = transformNormal(_ray.m_direction , m_toWorldSpace);

//    ngl::Mat4 tmp = m_toWorldSpace;
//    tmp.transpose();
//    ngl::Vec3 rayOrigin = ngl::Vec4( tmp * ngl::Vec4(_ray.m_origin)).toVec3();;
//    ngl::Vec3 rayDirection = ngl::Vec4( m_toWorldSpace * ngl::Vec4(_ray.m_direction)).toVec3(); // Apply world transforms to ray direction

    //rayOrigin.normalize();
//    rayDirection.normalize();

    return Ray(rayOrigin, rayDirection, _ray.m_type, _ray.m_tmin, _ray.m_tmax);
}

}
