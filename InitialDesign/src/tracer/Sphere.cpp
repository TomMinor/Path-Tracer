#include <utility>
#include <ngl/Vec4.h>
#include <ngl/VAOPrimitives.h>

#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>

#include <tracer/Sphere.h>
#include <tracer/tracer_util.h>

#include <cstring>

bool Sphere::intersect(const Ray &_ray, ngl::Real &_t) const
{


    ngl::Vec3 r_origin;
    ngl::Vec3 r_direction = ngl::Vec4(m_worldToObject * ngl::Vec4(_ray.m_direction)).toVec3();

    // Translate the origin coordinate ( this code was copied straight from the scratchapixel math header, needs tidying )
    float m[4][4];
    memcpy(m, m_worldToObject.m_openGL, sizeof(float)*16);
    float src[3] = { _ray.m_origin.m_x, _ray.m_origin.m_y, _ray.m_origin.m_z };
    ngl::Real x = src[0] * m[0][0] + src[1] * m[1][0] + src[2] * m[2][0] + m[3][0];
    ngl::Real y = src[0] * m[0][1] + src[1] * m[1][1] + src[2] * m[2][1] + m[3][1];
    ngl::Real z = src[0] * m[0][2] + src[1] * m[1][2] + src[2] * m[2][2] + m[3][2];
    ngl::Real w = src[0] * m[0][3] + src[1] * m[1][3] + src[2] * m[2][3] + m[3][3];
    r_origin.m_x = x / w;
    r_origin.m_y = y / w;
    r_origin.m_z = z / w;

    ngl::Real a = r_direction.dot(r_direction);
    ngl::Real b = 2 * r_direction.dot(r_origin);
    ngl::Real c = r_origin.dot(r_origin) - m_radius_sqr;
    ngl::Real t0;
    ngl::Real t1;

    if(!SolveQuadratic(a,b,c, t0, t1) || t1 < 0)
    {
        return false;
    }

    if(t1 < t0)
    {
        std::swap(t0, t1);
    }

    _t = (t0 < 0) ? t1 : t0;

    return true;
}

void Sphere::draw(ngl::Mat4 &_globalMouseTx)
{
      ngl::ShaderLib *shader=ngl::ShaderLib::instance();
      (*shader)["Phong"]->use();

      ngl::Material mat;
      mat.setDiffuse(m_colour);
      mat.loadToShader("material");

      ngl::Mat4 MV;
      ngl::Mat4 MVP;
      ngl::Mat3 normalMatrix;

      MV  = m_objectToWorld * _globalMouseTx * m_sceneCamera->getViewMatrix();
      MVP = m_objectToWorld * _globalMouseTx * m_sceneCamera->getVPMatrix();
      normalMatrix = MV;

      shader->setShaderParamFromMat4("MV",MV);
      shader->setShaderParamFromMat4("MVP",MVP);
      shader->setShaderParamFromMat3("normalMatrix",normalMatrix);

      // get an instance of the VBO primitives for drawing
      ngl::VAOPrimitives::instance()->draw("sphere1");
}
