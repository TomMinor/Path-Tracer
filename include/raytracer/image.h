#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <ngl/Colour.h>

///
/// \brief The Image is the base class of a structure that stores an image, child classes are expected to
/// implemented the read/write functions to write to specific locations (framebuffers, files of various types etc)
///
class Image
{
public:

    ///
    /// \brief The Pixel struct simply represents an RGB colour, overloads the +,-,/ and * operators
    ///
  struct Pixel
  {
    float m_r;
    float m_g;
    float m_b;

    Pixel(float _r=0.0f, float _g=0.0f, float _b=0.0f)
        : m_r(_r), m_g(_g), m_b(_b) {;}

    Pixel(const ngl::Colour& _colour)
      : m_r(_colour.m_r), m_g(_colour.m_g), m_b(_colour.m_b) {;}

    Pixel operator +(const Pixel& _t)   {   return Pixel( m_r + _t.m_r, m_g + _t.m_g, m_b + _t.m_b );   }
    Pixel operator -(const Pixel& _t)   {   return Pixel( m_r - _t.m_r, m_g - _t.m_g, m_b - _t.m_b );   }
    Pixel operator /(const Pixel& _t)   {   return Pixel( m_r / _t.m_r, m_g / _t.m_g, m_b / _t.m_b );   }
    Pixel operator *(const Pixel& _t)   {   return Pixel( m_r * _t.m_r, m_g * _t.m_g, m_b * _t.m_b );   }
    Pixel operator /(float _t)          {   return Pixel( m_r / _t, m_g / _t, m_b / _t );   }
    Pixel operator *(float _t)          {   return Pixel( m_r * _t, m_g * _t, m_b * _t );   }

    void operator +=(const Pixel& _t)   {   *this = *this + _t;  }
    void operator -=(const Pixel& _t)   {   *this = *this - _t;  }
    void operator *=(const Pixel& _t)   {   *this = *this * _t;  }
    void operator /=(const Pixel& _t)   {   *this = *this / _t;  }
    void operator *=(float _t)          {   *this = *this * _t;  }
    void operator /=(float _t)          {   *this = *this / _t;  }
  };

  Image(unsigned int _width, unsigned int _height);
  ~Image();

  virtual void setPixel(const Pixel& _pixel, unsigned int _x, unsigned int _y);
  virtual Pixel &getPixel(unsigned int _x, unsigned int _y);

  ///
  /// \brief save Saves the image to the _filename path
  /// \param _filename system path
  /// \return true if success, else false
  ///
  virtual bool save(const std::string& _filename) = 0;

  ///
  /// \brief read Read an image from _filename into the pixel buffer
  /// \param _filename system path
  /// \return true if success, else false
  ///
  virtual bool read(const std::string& _filename) = 0;

  ///
  /// \brief tonemap Gamma correct the scene
  /// \param _maxSceneLuminance
  /// \param _maxDisplayLuminance
  ///
  virtual void tonemap(double _maxSceneLuminance, double _maxDisplayLuminance) = 0;

protected:
  unsigned int m_width;
  unsigned int m_height;

  ///@todo Maybe make this void and unique to each image type
  Pixel* m_image;

};

#endif // IMAGE_H
