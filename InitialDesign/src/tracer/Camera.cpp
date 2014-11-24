#include <tracer/Camera.h>

Camera::Camera()
{
    set();
}

void Camera::set( const ngl::Mat4 &_cameraToWorld,
                  const ngl::Real _fov,
                  const ngl::Real _nearClippingPlane,
                  const ngl::Real _farClippingPlane)
{
    m_cameraToWorld = _cameraToWorld;

    ngl::Mat4 inverse = m_cameraToWorld;
    m_worldToCamera = inverse.inverse();

    m_nearClippingPlane = _nearClippingPlane;
    m_farClippingPlane = _farClippingPlane;
    // BUG : FOV calculations don't seem to match the viewport very well
    m_fov = _fov;
    m_angle = atan( (m_fov * 0.5) * (M_PI / 180.0f) );

}
