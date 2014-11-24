#include <fstream>
#include <assert.h>
#include <tracer/Framebuffer.h>

Framebuffer::Framebuffer(unsigned int _width, unsigned int _height)
  : m_width(_width), m_height(_height)
{
  m_image = new ngl::Colour[_width * _height];
}

Framebuffer::~Framebuffer()
{
  delete [] m_image;
}

void Framebuffer::setPixel(unsigned int _x, unsigned int _y, ngl::Colour _colour)
{
  assert( _x <= m_width && _y <= m_height );

  m_image[ m_width*_y + _x ] = _colour;
}

ngl::Colour Framebuffer::getPixel(unsigned int _x, unsigned int _y) const
{
  assert( _x <= m_width && _y <= m_height );

  return m_image[ m_width*_y + _x ];
}

void Framebuffer::savePPM(const std::string &_path)
{
  std::ofstream ofs;
  ofs.open(_path.c_str());
  ofs << "P6\n" << m_width << " " << m_height << "\n255\n";

  for (uint32_t j = 0; j < m_height; ++j)
  {
    for (uint32_t i = 0; i < m_width; ++i)
    {
      unsigned char pixelChar[3];

      pixelChar[0] = (unsigned char)(std::min(getPixel(i,j).m_r * 255 + 0.5, 255.0));
      pixelChar[1] = (unsigned char)(std::min(getPixel(i,j).m_g * 255 + 0.5, 255.0));
      pixelChar[2] = (unsigned char)(std::min(getPixel(i,j).m_b * 255 + 0.5, 255.0));

      ofs.write((char*)&pixelChar[0], 3);
    }
  }
  ofs.close();
}
