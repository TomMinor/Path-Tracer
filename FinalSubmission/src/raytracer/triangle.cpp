#include "raytracer/triangle.h"

namespace Renderer
{

bool Triangle::intersect(const Ray<float> &_ray, ngl::Real &_t) const
{
    _t = 0.f; // Initialise to 0 just incase

    /* Check plane/ray intersection - Find intersection point (if any) */
    Ray<float> objectSpaceRay = rayToObjectSpace(_ray);

    // Triangle edges
    const ngl::Vec3 e0 = m_v1 - m_v0;
    const ngl::Vec3 e1 = m_v2 - m_v0;
    const ngl::Vec3 e2 = m_v0 - m_v2;

    const ngl::Vec3 planeNormal = e0.cross(e1);

    const float rayDirectionDot = planeNormal.dot(objectSpaceRay.m_direction);
    if( rayDirectionDot == 0 || // Ray is parallel to triangle
        (rayDirectionDot < 0 && m_singleSided)) // Backface cull
    {

        return false;
    }

    // Distance from origin to plane's nearest point
    const float d = planeNormal.dot(m_v0);

    // Calculate length of intersecting ray
    const float t = ( planeNormal.dot(objectSpaceRay.m_origin) + d ) / rayDirectionDot;  /* Had to invert the value of this to get the right intersection point, unsure why, might be using the wrong handiness */
    if( t < 0)
    {
        // Ray is behind the triangle
        return false;
    }

    // Intersection point
    const ngl::Vec3 P = objectSpaceRay(t);

    /* Check triangle/ray intersection - Inside/Outside test
     *
     *       /|
     *      / |
     *     /  |
     *    /   |      P is the intersection point,
     *   /  P | P    if it is to the left of every edge,
     *  /C0->\|/     we assume it lies within the triangle
     * /______|
     *
     */

    // Inside edge vectors, from each vertex in the triangle to P
    const ngl::Vec3 C0 = P - m_v0;
    const ngl::Vec3 C1 = P - m_v1;
    const ngl::Vec3 C2 = P - m_v2;

    // Inside/Out test for edge 0
    const bool isPOnRightOfEdge0 = planeNormal.dot( e0.cross(C0) ) < 0;
    if(isPOnRightOfEdge0)
        return false;

    // Inside/Out test for edge 1
    const bool isPOnRightOfEdge1 = planeNormal.dot( e1.cross(C1) ) < 0;
    if(isPOnRightOfEdge1)
        return false;

    // Inside/Out test for edge 2
    const bool isPOnRightOfEdge2 = planeNormal.dot( e2.cross(C2) ) < 0;
    if(isPOnRightOfEdge2)
        return false;

    _t = t;

    // Ray hits the triangle
    return true;
}

/* These are handy for testing
 *
 * gnuplot> unset label; set label at 0.326836, -0.326836, 0.326836 "" point pointtype 6 pointsize 2 lc rgb "white" front
 * gnuplot> splot 'tri.dat' w pm3d notitle, 'line.dat' notitle with lines lt -1
 *
 */

void Triangle::draw(ngl::Mat4 &_globalMouseTx, const RenderContext &_context)
{

}

}


