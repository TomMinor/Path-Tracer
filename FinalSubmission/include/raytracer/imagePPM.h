#ifndef IMAGEPPM_H
#define IMAGEPPM_H


#include "raytracer/image.h"

class ImagePPM : public Image
{
public:
  ImagePPM(unsigned int _width, unsigned int _height) :
    Image(_width, _height)
  {;}

  virtual bool save(const std::string& _filename) override;
  virtual bool read(const std::string& _filename) override;
};

#endif // IMAGEPPM_H
