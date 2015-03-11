#include <QDebug>

#include "raytracer/camera.h"

namespace Renderer
{

Camera::Camera(const ngl::Mat4 &_toWorldSpace, float _fov, float _nearClip, float _farClip)
{
    setFOV(_fov);
    setClippingPlanes(_nearClip, _farClip);
    setCameraWorldSpace(_toWorldSpace);

    if( _toWorldSpace.m_00 != 1.0f
     || _toWorldSpace.m_11 != 1.0f
     || _toWorldSpace.m_22 != 1.0f)
    {
        qWarning() << "Render camera scale is not 1";
    }
}

}
