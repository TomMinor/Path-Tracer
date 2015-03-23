#ifndef RENDERER_H
#define RENDERER_H

#include "raytracer/ray.h"
#include "raytracer/rendercontext.h"
#include "raytracer/image.h"

namespace Renderer
{
  ///
  /// \brief render starts the tracing process, setting up canonical camera space rays and firing them into the scene
  /// \param _context stores scene information that needs to be passed between all parts of the pipeline
  ///
  void render(RenderContext *_context);

  ///
  /// \brief trace checks if _ray intersects with anything in the scene
  /// \param _ray Is expected to be a camera ray in world space
  /// \param _context stores scene information that needs to be passed between all parts of the pipeline
  /// \return the background colour if there was no hit, or the result of the shade function otherwise
  ///
  Image::Pixel trace(const Ray& _ray, RenderContext *_context);

  ///
  /// \brief shade checks if the last ray intersection is shaded by another object or has a clear path to a light source
  /// \param _context stores scene information that needs to be passed between all parts of the pipeline
  /// \return the sum of all the light flux affecting this pixel
  ///
  Image::Pixel shade(RenderContext *_context);
}

#endif // RENDERER_H
