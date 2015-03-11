#include "raytracer/image.h"
#include <assert.h>

Image::Image(unsigned int _width, unsigned int _height)
  : m_width(_width), m_height(_height)
{
  m_image = new Pixel[_width * _height];
}

Image::~Image()
{
  delete [] m_image;
}

void Image::setPixel(const Pixel &_pixel, unsigned int _x, unsigned int _y)
{
  assert( (_x >= 0  && _x <= m_width) && (_y >= 0  && _y <= m_height) );

  unsigned int index = (_y * m_width) + _x;
  m_image[index].m_r = _pixel.m_r;
  m_image[index].m_g = _pixel.m_g;
  m_image[index].m_b = _pixel.m_b;
}

const Image::Pixel& Image::getPixel(unsigned int _x, unsigned int _y)
{
  assert( (_x >= 0  && _x <= m_width) && (_y >= 0  && _y <= m_height) );

  return m_image[ (_y * m_width) + _x ];
}
