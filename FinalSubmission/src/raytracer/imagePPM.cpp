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
    file << "P6\n" << m_width << " " << m_height << "\n255\n";
    for(unsigned int j = 0; j < m_height; ++j)
    {
      for(unsigned int i = 0; i < m_width; ++i)
      {
        Pixel pixel = getPixel(i,j);
        unsigned char pixelChar[3];

        pixelChar[0] = (unsigned char)(std::min(pixel.m_r * 255 + 0.5, 255.0));
        pixelChar[1] = (unsigned char)(std::min(pixel.m_g * 255 + 0.5, 255.0));
        pixelChar[2] = (unsigned char)(std::min(pixel.m_b * 255 + 0.5, 255.0));

        file.write((char*)&pixelChar[0], 3);
      }
    }

    return true;
  }
}

bool ImagePPM::read(const std::string& _filename)
{
  // This is pretty much untested

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
