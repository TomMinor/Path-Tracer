#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "raytracer/rendercontext.h"
#include "raytracer/camera.h"
#include "raytracer/image.h"

namespace Renderer
{

class Primitive;
struct RenderContext;

class Scene
{
public:
    Scene(const std::vector<Primitive*> &_primitives,
          const ngl::Mat4 &_cameraTransform,
          Image::Pixel _background = Image::Pixel(0.f, 0.f, 0.f) // Default to black
          );

    typedef std::vector<Primitive*> objectList;
    typedef objectList::iterator objectListIterator;

    // Allow iteration over private objects
    objectListIterator objectBegin() { return m_primitives.begin(); }
    objectListIterator objectEnd() { return m_primitives.end(); }

    inline const Image::Pixel& getBackgroundColour() const { return m_background; }
private:
    //Camera *m_camera;
    const ngl::Mat4 m_initialCameraView;
    Image::Pixel m_background;

    //------------------------------------------
    // Objects
    //------------------------------------------
    objectList m_primitives;
};

}

#endif // SCENE_H

