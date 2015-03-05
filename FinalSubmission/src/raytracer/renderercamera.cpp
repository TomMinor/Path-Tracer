#include "raytracer/renderercamera.h"

RendererCamera::RendererCamera(const ngl::Mat4 &_toWorldSpace, float _fov, float _nearClip, float _farClip)
{
    setFOV(_fov);
    setClippingPlanes(_nearClip, _farClip);
    setCameraWorldSpace(_toWorldSpace);
}
