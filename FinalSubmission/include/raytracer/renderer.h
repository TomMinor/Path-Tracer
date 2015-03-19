#ifndef RENDERER_H
#define RENDERER_H

#include "raytracer/ray.h"
#include "raytracer/rendercontext.h"
#include "raytracer/image.h"

namespace Renderer
{
  Image::Pixel shade(RenderContext *_context);

  Image::Pixel trace(const Ray& _ray, RenderContext *_context);

  void render(RenderContext *_context);
}

#endif // RENDERER_H
