#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image
{
public:
  struct Pixel
  {
    float m_r;
    float m_g;
    float m_b;
  };

  Image(unsigned int _width, unsigned int _height);
  ~Image();

  virtual void setPixel(const Pixel& _pixel, unsigned int _x, unsigned int _y);
  virtual const Pixel& getPixel(unsigned int _x, unsigned int _y);

  virtual bool save(const std::string& _filename) = 0;
  virtual bool read(const std::string& _filename) = 0;

protected:
  unsigned int m_width;
  unsigned int m_height;

  ///@todo Maybe make this void and unique to each image type
  Pixel* m_image;

};

#endif // IMAGE_H
