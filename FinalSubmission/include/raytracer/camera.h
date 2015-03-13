#ifndef RENDERERCAMERA
#define RENDERERCAMERA

#include <ngl/Mat4.h>

#include "raytracer/tracemath.h"

namespace Renderer
{

class Camera
{
public:
    Camera( const ngl::Mat4 &_toWorldSpace = ngl::Mat4(),
                    float _fov = 45.f,
                    float _nearClip = 0.1f,
                    float _farClip = 1000.f );

    //----------------------------------------
    // Setters
    //----------------------------------------

    inline void setFOV(float _fov)
    {
        m_fov = _fov;

        /*                C
         *
         *               /|
         *              / |
         *             /  |
         *            /   |
         *           /    | 1 (screenheight is between 1 and -1)
         *          /     |
         *         /\     |
         *      A /α_\____| B
         *            1
         *
         * m_angle = α = atan(opp/adj) = atan(1/1)
         */

        m_angle = atan( degreesToRadians(m_fov * 0.5) ); // Necessary to remap the [1:-1] screenspace ratio onto the aspect ratio, into camera space
    }

    inline void setClippingPlanes(float _near, float _far)
    {
        m_nearClippingPlane = _near;
        m_farClippingPlane = _far;
    }

    inline void setCameraWorldSpace(const ngl::Mat4 &_toWorldSpace)
    {
        ngl::Mat4 inverseWorldSpace = _toWorldSpace; // Make a non-const copy
        m_toWorldSpace = _toWorldSpace;
        m_toObjectSpace = inverseWorldSpace.inverse();
    }

    //----------------------------------------
    // Getters
    //----------------------------------------

    inline const ngl::Mat4& getObjectSpaceMatrix() const { return m_toObjectSpace; }

    inline const ngl::Mat4& getWorldSpaceMatrix() const { return m_toWorldSpace; }

    inline float getFOV() const    { return m_fov; }
    inline float getAngle() const  { return m_angle; }

    inline float getNearClipPlane() const { return m_nearClippingPlane; }
    inline float getfarClipPlane() const  { return m_farClippingPlane; }

private:
    ngl::Mat4 m_toObjectSpace;
    ngl::Mat4 m_toWorldSpace;

    float m_nearClippingPlane;
    float m_farClippingPlane;
    float m_fov;

    /**
     * @brief m_angle is used to compute cameraspace rays
     */
    float m_angle;
};

}


#endif // RENDERERCAMERA

