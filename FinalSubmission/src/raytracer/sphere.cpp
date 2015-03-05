#include <utility>
#include <cstring>

#include <ngl/Vec4.h>
#include <ngl/VAOPrimitives.h>

#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>

#include "raytracer/sphere.h"
#include "raytracer/tracemath.h"


bool Sphere::intersect(const Ray<float> &_ray, float &o_t) const
{
    ngl::Vec3 rayOrigin;
    ngl::Vec3 rayDirection = ngl::Vec4( m_toWorldSpace * ngl::Vec4(_ray.m_direction)).toVec3(); // Apply world transforms to ray direction

    // Translate the origin coordinate ( this code was copied straight from the scratchapixel math header, needs tidying )
    // @todo Convert this to use ngl::Mat4
    float m[4][4];
    memcpy(m, m_toWorldSpace.m_openGL, sizeof(float)*16);

    float src[3] = { _ray.m_origin.m_x, _ray.m_origin.m_y, _ray.m_origin.m_z };
    float x = src[0] * m[0][0] + src[1] * m[1][0] + src[2] * m[2][0] + m[3][0];
    float y = src[0] * m[0][1] + src[1] * m[1][1] + src[2] * m[2][1] + m[3][1];
    float z = src[0] * m[0][2] + src[1] * m[1][2] + src[2] * m[2][2] + m[3][2];
    float w = src[0] * m[0][3] + src[1] * m[1][3] + src[2] * m[2][3] + m[3][3];
    rayOrigin.m_x = x / w;
    rayOrigin.m_y = y / w;
    rayOrigin.m_z = z / w;

    float a = rayDirection.dot(rayDirection);
    float b = 2 * rayDirection.dot(rayOrigin);
    float c = rayOrigin.dot(rayOrigin) - m_squaredRadius;

    float t0, t1;

    if(!SolveQuadratic(a,b,c, t0, t1) || t1 < 0)
    {
        return false;
    }

    //@todo What's this doing?
    if(t0 > t1)
    {
        std::swap(t0, t1);
    }

    o_t = (t0 < 0) ? t1 : t0;

    return true;
}

void Sphere::draw(ngl::Mat4 &_globalMouseTx, const RenderContext &_context)
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();

    ngl::Material mat;
    mat.setDiffuse(m_colour);
    mat.loadToShader("material");

    //@todo I need to know this
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat4 normalMatrix;

    if(_context.m_camera)
    {
        MV = m_toObjectSpace  * _globalMouseTx * _context.m_camera->
    }
}
