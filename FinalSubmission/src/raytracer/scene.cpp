#include "raytracer/scene.h"

Scene::Scene(const ngl::Camera &_camera, const std::vector<Primitive *> &_primitives,
             const std::vector<TestLight *> &_lights,
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

    m_camera = new RendererCamera( ngl::Mat4(), 90.f );
}
