#include <utility>
#include <cstring>

#include <ngl/Vec4.h>
#include <ngl/VAOPrimitives.h>

#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>

#include "raytracer/sphere.h"
#include "raytracer/tracemath.h"

namespace Renderer
{

bool Sphere::intersect(const Ray<float> &_ray, float &o_t) const
{
    Ray<float> objectSpaceRay = rayToObjectSpace(_ray);

    const ngl::Vec3 rayDirection = objectSpaceRay.m_direction;
    const ngl::Vec3 rayOrigin = objectSpaceRay.m_origin;

    const float a = rayDirection.dot(rayDirection);
    const float b = rayDirection.dot(rayOrigin) * 2.0f;
    const float c = rayOrigin.dot(rayOrigin) - m_squaredRadius;

    float t0, t1;
    if(!SolveQuadratic(a,b,c, t0, t1) || t1 < 0)
    {
        // No valid intersections
        // (we didn't touch the sphere or the intersection was behind the camera)
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

//    if(_context.m_camera)
//    {
//        MV = m_toObjectSpace  * _globalMouseTx * _context.m_camera->
//    }
}

}
