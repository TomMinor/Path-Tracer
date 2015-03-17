#include "raytracer/scene.h"
#include <ngl/Camera.h>

namespace Renderer
{

Scene::Scene(const std::vector<Primitive *> &_primitives,
             const ngl::Mat4 &_cameraTransform,
             ngl::Colour _background)
    : m_initialCameraView(_cameraTransform),
      m_primitives(_primitives),
      m_background(_background)
{
}

}
