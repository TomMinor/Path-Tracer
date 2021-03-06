#ifndef MATERIAL_H
#define MATERIAL_H

#include <ngl/Colour.h>

///
/// \brief The Material struct represents the properi
///
struct Material
{
public:

    ///
    /// \brief The SurfaceProperty enum Stores the material surface type, used when figuring out how light behaves with the surface
    ///
    enum SurfaceProperty { DIFFUSE, SPECULAR, EMISSIVE };

    Material()
      : m_diffuse(0,0,0), m_specularCoefficient(1.0), m_type(DIFFUSE)
    {;}

    Material(SurfaceProperty _type, const ngl::Colour& _diffuse)
        : m_diffuse(_diffuse), m_specularCoefficient(1.0), m_type(_type)
    {
        m_diffuse = _diffuse;
    }

    ngl::Colour m_diffuse;
    float m_specularCoefficient; //unused
    SurfaceProperty m_type;
};

#endif // MATERIAL_H

