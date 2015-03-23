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

  ///
  /// \brief save Saves the image buffer to a PPM file
  /// \param _filename
  /// \return
  ///
  virtual bool save(const std::string& _filename);

  ///
  /// \brief read Reads a PPM file into the image buffer
  /// \param _filename
  /// \return
  ///
  virtual bool read(const std::string& _filename);

  virtual void tonemap(double _maxSceneLuminance, double _maxDisplayLuminance)
  {
    ///@unimplemented
    return;
  }
};

#endif // IMAGEPPM_H
