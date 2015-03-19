#ifndef MATERIAL_H
#define MATERIAL_H

#include <ngl/Colour.h>

///@todo Is the material the same as the BSDF? and is the BSDF/BRDF the same as the shader?
struct Material
{
public:
    enum SurfaceProperty { DIFFUSE, SPECULAR, EMISSIVE }; ///@todo What does Ian suggest

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

