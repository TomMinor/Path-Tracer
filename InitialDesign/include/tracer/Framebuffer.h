#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <string>
#include <ngl/Colour.h>


/**
 * @brief The Framebuffer class represents what would be an FBO in the final version of the raytracer, for simplicity it simply writes to a .PPM file right now
 */
class Framebuffer
{
public:
  Framebuffer(unsigned int _width, unsigned int _height);
  ~Framebuffer();

  void setPixel(unsigned int _x, unsigned int _y, ngl::Colour _colour);
  ngl::Colour getPixel(unsigned int _x, unsigned int _y) const;

  void savePPM(const std::string &_path);

private:
  ngl::Colour *m_image;
  unsigned int m_width;
  unsigned int m_height;
};

#endif // FRAMEBUFFER_H
