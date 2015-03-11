#ifndef RENDERER_H
#define RENDERER_H

#include "raytracer/ray.h"
#include "raytracer/rendercontext.h"
#include "raytracer/image.h"

namespace Renderer
{
  void trace(const Ray<float>& _ray, const RenderContext *_context, Image::Pixel o_colour);

  void render(const RenderContext *_context);
}

#endif // RENDERER_H
