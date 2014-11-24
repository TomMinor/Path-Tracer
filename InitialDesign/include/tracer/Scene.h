#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <ngl/Colour.h>
#include <tracer/Ray.h>
#include <tracer/Primitive.h>
#include <tracer/Framebuffer.h>
#include <tracer/RendererContext.h>

/**
 * @brief The Scene class manages the entire raytracing pipeline, starting at render()
 */
class Scene
{
    // Given how I use it for this prototype this may as well be static, RendererContext handles all the variables
    // It may make more sense to combine them
public:
    Scene() {;}

    /**
     * @brief shade
     * @param _ray
     * @param _primitive
     * @return Colour of the primitive the ray hits
     */
    ngl::Colour shade(const Ray& _ray, const Primitive& _primitive);

    /**
     * @brief trace a ray through the scene, setting _spectrum to object colour if there is a hit
     * or the background colour if there is not.
     * @param _ray
     * @param _context
     * @param _spectrum
     */
    void trace(const Ray& _ray, ngl::Colour &_spectrum);

    /**
     * @brief render sets up a Framebuffer and fires traces through the centers of each pixel.
     * @param _context
     * @return Framebuffer containing the render result.
     */
    Framebuffer render(const RendererContext* _context);

private:
    const RendererContext* m_context;
};

#endif // RENDERER_H
