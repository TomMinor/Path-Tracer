#include "raytracer/scene.h"

namespace Renderer
{

Scene::Scene(ngl::Camera *_camera,
             const std::vector<Primitive *> &_primitives,
             const std::vector<Light *> &_lights,
             ngl::Colour _background)
    :   m_primitives(_primitives),
        m_lights(_lights),
        m_drawCamera(_camera),
        m_background(_background)
{
//    ngl::Vec4 eye = _camera.getEye();
//    ngl::Vec4 lookAt = _camera.getLook();
//    ngl::Mat4 transform;

  ngl::Mat4 a;
  //a.translate(_camera.getEye().m_x, _camera.getEye().m_y, _camera.getEye().m_z);
  a.translate(0, 0, 5);
  //a.inverse();

  m_camera = new Camera( a, 45.f );
}

void Scene::draw(ngl::Mat4 &_globalMouseTx)
{
  for(objectListIterator object = objectBegin();
      object != objectEnd();
      object++)
  {
    (*object)->draw(_globalMouseTx,
                    m_drawCamera->getViewMatrix(),
                    m_drawCamera->getProjectionMatrix());
  }
}

}
