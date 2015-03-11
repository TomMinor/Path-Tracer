#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "raytracer/primitive.h"
#include "raytracer/renderercamera.h"

/// @todo Implement this class
class TestLight;
class Primitive;

class Scene
{
public:
    Scene(const ngl::Camera& _camera,
          const std::vector<Primitive*> &_primitives,
          const std::vector<TestLight*> &_lights,
          ngl::Colour _background = ngl::Colour(0.f, 0.f, 0.f) // Default to black
          );


private:
    //-------------------------------------------------
    // Scene Settings /// @todo Maybe this should be it's own Scene class?
    //-------------------------------------------------
    const std::vector<Primitive*> m_primitives;
    const std::vector<TestLight*> m_lights;
    RendererCamera *m_camera;
    ngl::Camera m_drawCamera;

    ngl::Colour m_background;
    ngl::Colour *m_pixels; /// @todo This should have its own, more efficient type
};

#endif // SCENE_H

