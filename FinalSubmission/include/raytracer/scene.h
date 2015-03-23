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

///
/// \brief The Scene class represents the 3D world, mainly storing data about the object primitives
///
class Scene
{
public:
    ///
    /// \brief Scene initialise the scene
    /// \param _primitives is a std::vector of all the scene objects
    /// \param _cameraTransform is the initial (starting) position of the 3D camera
    /// \param _background is the colour of the background, used if a ray hits nothing in the scene
    ///
    Scene(const std::vector<Primitive*> &_primitives,
          const ngl::Mat4 &_cameraTransform,
          Image::Pixel _background = Image::Pixel(0.f, 0.f, 0.f) // Default to black
          );

    typedef std::vector<Primitive*> objectList;
    typedef objectList::iterator objectListIterator;

    ///
    /// \brief objectBegin allows iteration over private objects without making the containers themselves public
    /// \return
    ///
    objectListIterator objectBegin() { return m_primitives.begin(); }

    ///
    /// \brief objectEnd allows iteration over private objects without making the containers themselves public
    /// \return
    ///
    objectListIterator objectEnd() { return m_primitives.end(); }

    ///
    /// \brief getBackgroundColour
    /// \return the scene background colour
    ///
    inline const Image::Pixel& getBackgroundColour() const { return m_background; }
private:
    ///
    /// \brief m_initialCameraView
    ///
    const ngl::Mat4 m_initialCameraView;

    ///
    /// \brief m_background
    ///
    Image::Pixel m_background;

    ///
    /// \brief m_primitives is a std::vector containing any child class of Primitive*
    ///
    objectList m_primitives;
};

}

#endif // SCENE_H

