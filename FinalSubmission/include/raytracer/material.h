#ifndef MATERIAL_H
#define MATERIAL_H

#include <ngl/Colour.h>

struct Material
{
public:
    enum SurfaceProperty { DIFFUSE, SPECULAR, EMISSIVE };

    Material()
    {
        m_diffuse = ngl::Colour(0,0,0);
    }

    Material(SurfaceProperty _type, const ngl::Colour& _diffuse)
        : m_diffuse(_diffuse), m_type(_type)
    {
        m_diffuse = _diffuse;
    }

    ngl::Colour m_diffuse;
    SurfaceProperty m_type;
};

#endif // MATERIAL_H

