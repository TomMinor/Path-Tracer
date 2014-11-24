#ifndef CAMERA_H
#define CAMERA_H

#include <ngl/Mat4.h>
#include <cmath>

/**
 * @brief The Camera class stores information about the render camera (not the ngl camera)
 */
class Camera
{
public:
    Camera();

    void set( const ngl::Mat4 &_cameraToWorld = ngl::Mat4(),
              const ngl::Real _fov = 90,
              const ngl::Real _nearClippingPlane = 0.1f,
              const ngl::Real _farClippingPlane = 1000.f);

    ngl::Mat4 m_cameraToWorld;
    ngl::Mat4 m_worldToCamera;
    ngl::Real m_angle;

    ngl::Real m_nearClippingPlane;
    ngl::Real m_farClippingPlane;
    ngl::Real m_fov;
};

#endif // CAMERA_H
