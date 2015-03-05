#ifndef RENDERERCAMERA
#define RENDERERCAMERA

#include <ngl/Mat4.h>
#include <cmath>

class RendererCamera
{
public:
    RendererCamera( const ngl::Mat4 &_toWorldSpace = ngl::Mat4(),
                    float _fov = 90.f,
                    float _nearClip = 0.1f,
                    float _farClip = 1000.f );

    //----------------------------------------
    // Setters
    //----------------------------------------

    inline void setFOV(float _fov)
    {
        m_fov = _fov;
        m_angle = atan( (m_fov * 0.5) * (M_PI / 180.0f) ); // @todo What's this do again?
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
    float m_angle;

    float m_nearClippingPlane;
    float m_farClippingPlane;
    float m_fov;
};

#endif // RENDERERCAMERA

