#ifndef IMAGEPPM_H
#define IMAGEPPM_H


#include "raytracer/image.h"

///
/// \brief The ImagePPM class specialisation of the image class for writing to a .ppm file
///
class ImagePPM : public Image
{
public:
  ImagePPM(unsigned int _width, unsigned int _height) :
    Image(_width, _height)
  {;}

  virtual bool save(const std::string& _filename);
  virtual bool read(const std::string& _filename);

  virtual void tonemap(double _maxSceneLuminance, double _maxDisplayLuminance)
  {
    ///@unimplemented
    return;
  }
};

#endif // IMAGEPPM_H
