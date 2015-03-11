#include <fstream>
#include <QDebug>
#include <cmath>
#include <stdint.h>

#include "raytracer/imagePPM.h"

bool ImagePPM::save(const std::string& _filename)
{
  std::ofstream file;
  file.open(_filename.c_str());

  if( !file )
  {
    qWarning() << "Unable to save file " << _filename.c_str() << "\n";
    return false;
  }
  else
  {
    Pixel* pixel = &m_image[0];

    file << "P6\n" << m_width << " " << m_height << "\n255\n";
    for(unsigned int j = 0; j < m_height; ++j)
    {
      for(unsigned int i = 0; i < m_width; ++i)
      {
        unsigned char r = (unsigned char)(std::max(0.f, std::min(255.f, powf(pixel->m_r, 1/2.2) * 255 + 0.5f)));
        unsigned char g = (unsigned char)(std::max(0.f, std::min(255.f, powf(pixel->m_g, 1/2.2) * 255 + 0.5f)));
        unsigned char b = (unsigned char)(std::max(0.f, std::min(255.f, powf(pixel->m_b, 1/2.2) * 255 + 0.5f)));
        file << r << g << b;
        pixel++;
      }
    }

    return true;
  }
}

bool ImagePPM::read(const std::string& _filename)
{
  unsigned int width, height;
  std::ifstream file(_filename.c_str());

  if(!file)
  {
    return false;
  }

  std::string header;
  file >> header; // P6
  if(header != "P6")
  {
    qWarning() << "Malformed header in file " << _filename.c_str() << "\n";
    return false;
  }

  uint32_t maximumColours;
  file >> width >> height >> maximumColours;
  unsigned char *screenPixels = new unsigned char[ 3 * width * height];
  file.ignore();
  file.read((char*)screenPixels, 3 * width * height);

  m_image = (Pixel*)screenPixels;

  file.close();

  return true;
}
