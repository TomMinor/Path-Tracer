#ifndef LIGHT_H
#define LIGHT_H

#include "raytracer/primitive.h"

namespace Renderer
{

class Light : public Primitive
{
public:
  Light(const ngl::Mat4 &_toWorldSpace)
    : Primitive(_toWorldSpace)
  {;}

private:

};

}

#endif // LIGHT_H
