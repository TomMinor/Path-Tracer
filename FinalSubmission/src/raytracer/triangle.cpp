#include "raytracer/triangle.h"
#include "raytracer/sphere.h"

namespace Renderer
{

Triangle::Triangle(ngl::Vec3 _v0,
                   ngl::Vec3 _v1,
                   ngl::Vec3 _v2,
                   const ngl::Mat4 _toWorldSpace,
                   const Material &_material,
                   bool _singleSided)
    : Primitive(_toWorldSpace, _material), m_singleSided(_singleSided)
{

    ngl::Mat4 tmp = m_toObjectSpace;
//    tmp.transpose();
    m_wsvtx[0]= ngl::Vec4(_v0 * tmp).toVec3();
    m_wsvtx[1]= ngl::Vec4(_v1 * tmp).toVec3();
    m_wsvtx[2]= ngl::Vec4(_v2 * tmp).toVec3();

    m_vtx[0] = _v0;
    m_vtx[1] = _v1;
    m_vtx[2] = _v2;

    ngl::Vec3 nrm = ngl::calcNormal(m_vtx[0], m_vtx[1], m_vtx[2]);
    nrm.normalize();
    m_normals[0] = m_normals[1] = m_normals[2] = nrm;

    m_vao =  ngl::VertexArrayObject::createVOA(GL_TRIANGLES);

    m_vao->bind();
    m_vao->setData(3 * sizeof(ngl::Vec3), m_vtx[0].m_x);
    m_vao->setVertexAttributePointer(0, 3, GL_FLOAT, 0, 0);

    m_vao->setData(3 * sizeof(ngl::Vec3), m_normals[0].m_x);
    m_vao->setNumIndices(3);
    m_vao->setVertexAttributePointer(2, 3, GL_FLOAT, 0, 0);

    m_vao->setNumIndices(3);
    m_vao->unbind();
}

void Triangle::draw() const
{
    m_vao->bind();
    m_vao->draw();
    m_vao->unbind();
}

Triangle::~Triangle()
{
    m_vao->removeVOA();
    delete m_vao;
}

ngl::Vec3 Triangle::sample() const
{
    // get two randoms
    const double sqr1 = sqrt(drand48());
    const double r2   = drand48();

    // make barycentric coords
    const double a = 1.0 - sqr1;
    const double b = (1.0 - r2) * sqr1;
    //const real64 c = r2 * sqr1;

    // make position from barycentrics
    // calculate interpolation by using two edges as axes scaled by the
    // barycentrics
    return ngl::Vec3( ((m_wsvtx[1] - m_wsvtx[0]) * a) + ((m_wsvtx[2] - m_wsvtx[0]) * b) + m_wsvtx[0] );
}

ngl::Vec3 Triangle::getNormal(ngl::Vec3 _point) const
{/*
    ngl::Vec3 nrm(m_normals[0].m_x, m_normals[0].m_y, m_normals[0].m_z);
    nrm.normalize();*/

//    ngl::Mat4 tmp = m_toObjectSpace;
//    tmp.transpose();
//    ngl::Vec3 v1 = ngl::Vec4(m_vtx[1] * tmp).toVec3();
//    ngl::Vec3 v2 = ngl::Vec4(m_vtx[2] * tmp).toVec3();

    return getTangent().cross( m_wsvtx[2] - m_wsvtx[1] );
}

ngl::Vec3 Triangle::getTangent() const
{
//    ngl::Mat4 tmp = m_toObjectSpace;
//    tmp.transpose();
//    ngl::Vec3 v0 = ngl::Vec4(m_vtx[0] * tmp).toVec3();
//    ngl::Vec3 v1 = ngl::Vec4(m_vtx[1] * tmp).toVec3();

    return m_wsvtx[1] - m_wsvtx[0];
}


const double EPSILON = 1.0 / 1048576.0;

// http://www.hxa.name/rendering/#minilight
bool Triangle::intersect(const Ray &_ray, HitData &_hit) const
{
    Ray objectSpaceRay = _ray;//rayToObjectSpace(_ray);

    ngl::Mat4 tmp = m_toWorldSpace;
    tmp.transpose();
    ngl::Vec3 origin = ngl::Vec4(_ray.m_origin * tmp).toVec3();
    ngl::Vec3 direction = ngl::Vec4(_ray.m_direction * m_toWorldSpace).toVec3();
    direction.normalize();
    objectSpaceRay = Ray(origin, direction, _ray.m_type, _ray.m_tmin);

    ngl::Vec3 v0 = m_wsvtx[0];
    ngl::Vec3 v1 = m_wsvtx[1];
    ngl::Vec3 v2 = m_wsvtx[2];

    const ngl::Vec3 edge1 = v1 - v0;
    const ngl::Vec3 edge2 = v2 - v0;

    const ngl::Vec3 pvec ( objectSpaceRay.m_direction.cross(edge2));
    const double det = edge1.dot(pvec);

    if(m_singleSided && det > 0)
        return false;

    bool isHit = false;
    if( (det <= -EPSILON) | (det >= EPSILON) )
    {
        const double inv_det = 1.0 / det;

        const ngl::Vec3 tvec = objectSpaceRay.m_origin - v0;

        const double u = tvec.dot(pvec) * inv_det;
        if( (u >= 0.0) & (u <= 1.0) )
        {
            const ngl::Vec3 qvec(tvec.cross(edge1));

            const double v = objectSpaceRay.m_direction.dot(qvec) * inv_det;
            if( (v >= 0.0) & (u + v <= 1.0) )
            {
                float t = edge2.dot(qvec) * inv_det;
                if(t >= 0)
                {
                    _hit.m_t = t;///@todo This doesn't return the right value of t
                    _hit.m_u = u;
                    _hit.m_v = v;
                    _hit.m_w = (1-u-v);
                    _hit.m_surfaceImpact = objectSpaceRay(_hit.m_t);
                    _hit.m_surfaceNormal = getNormal(_hit.m_surfaceImpact);
                    _hit.m_ray = objectSpaceRay;
                    _hit.m_object = this;

                    isHit = true;
                }
            }
        }
    }

    return isHit;

//    ngl::Mat4 a,b,c;

//    a.translate(m_vtx[0].m_x, m_vtx[0].m_y, m_vtx[0].m_z);
//    b.translate(m_vtx[1].m_x, m_vtx[1].m_y, m_vtx[1].m_z);
//    c.translate(m_vtx[2].m_x, m_vtx[2].m_y, m_vtx[2].m_z);

//    a *= m_toObjectSpace;
//    b *= m_toObjectSpace;
//    c *= m_toObjectSpace;

//    Sphere v0(a * m_toWorldSpace, 0.1f, m_colour);
//    Sphere v1(b * m_toWorldSpace, 0.1f, m_colour);
//    Sphere v2(c * m_toWorldSpace, 0.1f, m_colour);

//    return v0.intersect(_ray, _t) || v1.intersect(_ray, _t) || v2.intersect(_ray, _t);


//    _t = 0.f; // Initialise to 0 just incase

//    /* Check plane/ray intersection - Find intersection point (if any) */
//    Ray<float> objectSpaceRay = rayToObjectSpace(_ray);

//    // Triangle edges
//    const ngl::Vec3 e0 = m_vtx[1] - m_vtx[0];
//    const ngl::Vec3 e1 = m_vtx[2] - m_vtx[0];
//    const ngl::Vec3 e2 = m_vtx[0] - m_vtx[2];

//    const ngl::Vec3 planeNormal = e0.cross(e1);

//    const float rayDirectionDot = planeNormal.dot(objectSpaceRay.m_direction);
////    if( rayDirectionDot == 0 || // Ray is parallel to triangle
////        (rayDirectionDot < 0 && m_singleSided)) // Backface cull
//    if( rayDirectionDot == 0)
//    {
//        return false;
//    }

//    // Distance from origin to plane's nearest point
//    const float d = planeNormal.dot(m_vtx[0]);

//    // Calculate length of intersecting ray
//    const float t = -( planeNormal.dot(objectSpaceRay.m_origin) + d ) / rayDirectionDot;  /* Had to invert the value of this to get the right intersection point, unsure why, might be using the wrong handiness */
//    if( t > 0)
//    {
//        // Ray is behind the triangle
//        return false;
//    }

//    // Intersection point
//    const ngl::Vec3 P = objectSpaceRay(t);

//    _t = t;

//    return true;
//    /* Check triangle/ray intersection - Inside/Outside test
//     *
//     *       /|
//     *      / |
//     *     /  |
//     *    /   |      P is the intersection point,
//     *   /  P | P    if it is to the left of every edge,
//     *  /C0->\|/     we assume it lies within the triangle
//     * /______|
//     *
//     */

//    // Inside edge vectors, from each vertex in the triangle to P
////    const ngl::Vec3 C0 = P - m_vtx[0];
////    const ngl::Vec3 C1 = P - m_vtx[1];
////    const ngl::Vec3 C2 = P - m_vtx[2];

//    const ngl::Vec3 C0 = P - m_vtx[0];
//    // Inside/Out test for edge 0
//    const bool isPOnRightOfEdge0 = planeNormal.dot( e0.cross(C0) ) < 0;
//    if(isPOnRightOfEdge0)
//        return false;

//    const ngl::Vec3 C1 = P - m_vtx[1];
//    ngl::Vec3 v1v2 = m_vtx[2] - m_vtx[1];

//    // Inside/Out test for edge 1
//    const bool isPOnRightOfEdge1 = planeNormal.dot( e1.cross(C1) ) < 0;
//    if(isPOnRightOfEdge1)
//        return false;

//    const ngl::Vec3 C2 = P - m_vtx[2];
//    ngl::Vec3 v2v0 = m_vtx[0] - m_vtx[2];

//    // Inside/Out test for edge 2
//    const bool isPOnRightOfEdge2 = planeNormal.dot( e2.cross(C2) ) < 0;
//    if(isPOnRightOfEdge2)
//        return false;

//    ngl::Vec3 v0v1 =  m_vtx[1] -  m_vtx[0];
//    ngl::Vec3 v0v2 =  m_vtx[2] -  m_vtx[0];

//   // inside-out test edge0
////    ngl::Vec3 v0p = P - m_vtx[0];
////    float v = planeNormal.dot(v0v1.cross(v0p));
////    if (v > 0) return false; // P outside triangle

////    // inside-out test edge1
////    ngl::Vec3 v1p = P - m_vtx[1];
////    ngl::Vec3 v1v2 = m_vtx[2] - m_vtx[1];
////    float w = planeNormal.dot(v1v2.cross(v1p));
////    if (w > 0) return false; // P outside triangle

////    // inside-out test edge2
////    ngl::Vec3 v2p = P - m_vtx[2];
////    ngl::Vec3 v2v0 = m_vtx[0] - m_vtx[2];
////    float u = planeNormal.dot(v2v0.cross(v2p));
////    if (u > 0) return false; // P outside triangle


//    _t = t;

//    // Ray hits the triangle
//    return true;
}

/* These are handy for testing
 *
 * gnuplot> unset label; set label at 0.326836, -0.326836, 0.326836 "" point pointtype 6 pointsize 2 lc rgb "white" front
 * gnuplot> splot 'tri.dat' w pm3d notitle, 'line.dat' notitle with lines lt -1
 *
 */




}


