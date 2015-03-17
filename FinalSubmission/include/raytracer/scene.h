#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <ngl/Colour.h>

#include "raytracer/rendercontext.h"
#include "raytracer/camera.h"
#include "raytracer/light.h"

namespace Renderer
{

class Light;
class Primitive;
struct RenderContext;

class Scene
{
public:
    Scene(const std::vector<Primitive*> &_primitives,
          const ngl::Mat4 &_cameraTransform,
          const ngl::Mat4 &_cameraView,
          ngl::Colour _background = ngl::Colour(0.f, 0.f, 0.f) // Default to black
          );

    typedef std::vector<Primitive*> objectList;
    typedef objectList::const_iterator objectListIterator;

    // Allow iteration over private objects
    objectListIterator objectBegin() const { return m_primitives.begin(); }
    objectListIterator objectEnd() const { return m_primitives.end(); }

    inline const ngl::Colour& getBackgroundColour() const { return m_background; }

    inline Camera* getRenderCamera() const { return m_camera; }

//    inline void addPrimitive(const Primitive* _object) { m_primitives.push_back(_object); }
//    inline void addLight(const Light* _light) { m_lights.push_back(_light); }

private:
    Camera *m_camera;
    ngl::Colour m_background;
    ngl::Mat4 m_cameraView;

    //------------------------------------------
    // Objects
    //------------------------------------------
    objectList m_primitives;
};

}

#endif // SCENE_H

