#include "raytracer/scene.h"

namespace Renderer
{

Scene::Scene(const ngl::Camera &_camera,
             const std::vector<Primitive *> &_primitives,
             const std::vector<Light *> &_lights,
             ngl::Colour _background)
    :   m_primitives(_primitives),
        m_lights(_lights),
        m_background(_background),
        m_drawCamera(_camera)
{
//    ngl::Vec4 eye = _camera.getEye();
//    ngl::Vec4 lookAt = _camera.getLook();
//    ngl::Mat4 transform;

//    transform.

  ngl::Mat4 a;
  a.translate(_camera.getEye().m_x, _camera.getEye().m_y, _camera.getEye().m_z);

  m_camera = new Camera( a, 90.f );
}

}
