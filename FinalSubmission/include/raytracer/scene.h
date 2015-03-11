#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <ngl/Camera.h>
#include <ngl/Colour.h>

#include "raytracer/camera.h"

namespace Renderer
{

/// @todo Implement this class
class Light;
class Primitive;


class Scene
{
public:
    Scene(const ngl::Camera& _camera,
          const std::vector<Primitive*> &_primitives,
          const std::vector<Light*> &_lights,
          ngl::Colour _background = ngl::Colour(0.f, 0.f, 0.f) // Default to black
          );

    typedef std::vector<Primitive*> objectList;
    typedef objectList::const_iterator objectListIterator;

    typedef std::vector<Light*> lightList;
    typedef lightList::const_iterator lightListIterator;

    // Allow iteration over private objects
    objectListIterator objectBegin() const { return m_primitives.begin(); }
    objectListIterator objectEnd() const { return m_primitives.end(); }

    lightListIterator lightBegin() const { return m_lights.begin(); }
    lightListIterator lightEnd() const { return m_lights.end(); }

    inline const ngl::Colour& getBackgroundColour() const { return m_background; }
    inline Camera* getRenderCamera() const { return m_camera; }

//    inline void addPrimitive(const Primitive* _object) { m_primitives.push_back(_object); }
//    inline void addLight(const Light* _light) { m_lights.push_back(_light); }

private:
    //-------------------------------------------------
    // Scene Settings /// @todo Maybe this should be it's own Scene class?
    //-------------------------------------------------
    objectList m_primitives;
    lightList m_lights;
    Camera *m_camera;
    ngl::Camera m_drawCamera;

    ngl::Colour m_background;
};

}

#endif // SCENE_H

