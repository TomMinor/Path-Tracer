#ifndef TESTLIGHT_H
#define TESTLIGHT_H

#include <ngl/Colour.h>
#include <ngl/Mat4.h>

/**
 * @brief The TestLight class is a really primitive light to test out shadow rays in the renderer
 */
class TestLight
{
public:
    TestLight(ngl::Mat4 _lightToWorld = ngl::Mat4(), ngl::Colour _colour = ngl::Colour(1.f, 1.f, 1.f));

private:
    ngl::Colour m_colour;
    ngl::Mat4 m_lightToWorld;
    ngl::Mat4 m_worldToLight;

};

#endif // TESTLIGHT_H
