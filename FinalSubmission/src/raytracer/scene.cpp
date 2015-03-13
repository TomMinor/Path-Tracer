#include "raytracer/scene.h"
#include <ngl/Camera.h>

namespace Renderer
{

Scene::Scene(const std::vector<Primitive *> &_primitives,
             const std::vector<Light *> &_lights,
             const ngl::Mat4 &_cameraTransform,
             const ngl::Mat4 &_cameraView,
             ngl::Colour _background)
    : m_cameraView(_cameraView),
      m_primitives(_primitives),
      m_lights(_lights),
      m_background(_background)
{
  ngl::Mat4 a;
  //a.translate(0, 0, -5);

  m_camera = new Camera(_cameraTransform, 90);
}

}
