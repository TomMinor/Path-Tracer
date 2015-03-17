#include <stdexcept>
#include <sstream>
#include <iostream>

#include "raytracer/scenefile.h"

#include "raytracer/StringUtils.h"

namespace Renderer
{

SceneFile::SceneFile(const std::string &_path)
  : m_sceneFile(NULL)
{
  m_sceneFile = new std::ifstream(_path.c_str(), std::ifstream::in | std::ifstream::out);

  if(!m_sceneFile->is_open())
  {
    throw std::invalid_argument("Scenefile does not exist : " + _path);
  }
}

SceneFile::~SceneFile()
{
  m_sceneFile->close();
  delete m_sceneFile;
}

Renderer::Scene* SceneFile::read() const
{
  Scene::objectList primitives;
  Camera* renderCamera = NULL; // Only allow 1 render camera, maybe extend to support many cameras in scenes

  std::string line;
  int counter = 0;
  while ( std::getline(*m_sceneFile, line) )
  {
    std::vector<std::string> tokens;
    tokenize(line, tokens, " ");

    if(tokens.size() > 0)
    {
      float xres = 640, yres = 480; // Render resolution

      std::string lineType = tokens[0];
      if(lineType[0] == '$') // Ignore lines not starting with $
      {
        lineType.erase(0, 1);
        if(lineType == "camera")
        {
          if(renderCamera)
          {
            std::cerr << "More than one camera found, using first camera\n";
          }
          else
          {
            renderCamera = parseCamera(tokens);
          }
        }
        else if(lineType == "sphere")
        {
          Sphere* primitive = parseSphere(tokens);
          if(primitive)
            primitives.push_back(primitive);
        }
        else if(lineType == "res")
        {
          try
          {
            xres = parseFloat(tokens[1]);
            yres = parseFloat(tokens[2]);
          }
          catch(std::runtime_error& e)
          {
            std::cerr << "Error parsing resolution data : " << e.what() << std::endl;
          }
        }
      } // End $ check
    } // End token size check
    counter++;
  } // End line loop

  ///@todo Move camera projection info out of Scene constructor, construct it at render time from ngl::Camera,
  /// but don't make ngl a dependency for the renderer to work
  //return Scene(primitives, )


}


ngl::Mat4 SceneFile::parseTransform(const std::vector<std::string> &_tokens, unsigned int _tokenOffset)
{
  ngl::Vec3 position, rotation, scale;

  try
  {
    position = ngl::Vec3( tokenToFloat(_tokens[_tokenOffset + 0]),
                          tokenToFloat(_tokens[_tokenOffset + 1]),
                          tokenToFloat(_tokens[_tokenOffset + 2]) );
  }
  catch(std::runtime_error)
  {
    throw std::runtime_error("Could not read position");
  }

  try
  {
    scale = ngl::Vec3( tokenToFloat(_tokens[_tokenOffset + 3]),
                       tokenToFloat(_tokens[_tokenOffset + 4]),
                       tokenToFloat(_tokens[_tokenOffset + 5]) );
  }
  catch(std::runtime_error)
  {
    throw std::runtime_error("Could not read scale");
  }

  try
  {
    rotation = ngl::Vec3( tokenToFloat(_tokens[_tokenOffset + 6]),
                          tokenToFloat(_tokens[_tokenOffset + 7]),
                          tokenToFloat(_tokens[_tokenOffset + 8]) );
  }
  catch(std::runtime_error)
  {
    throw std::runtime_error("Could not read rotation");
  }

  ngl::Mat4 translateXYZ, scaleXYZ, rotX, rotY, rotZ;

  rotX.rotateX(rotation.m_x);
  rotY.rotateY(rotation.m_x);
  rotZ.rotateZ(rotation.m_x);

  translateXYZ.translate(position.m_x, position.m_y, position.m_z);
  scaleXYZ.scale(scale.m_x, scale.m_y, scale.m_z);

  return ngl::Mat4(translateXYZ * rotZ * rotY * rotX * scaleXYZ);
}

ngl::Colour SceneFile::parseColour(const std::vector<std::string> &_tokens, unsigned int _tokenOffset)
{
  ngl::Colour colour;

  try
  {
    colour = ngl::Colour( tokenToFloat(_tokens[_tokenOffset + 0]),
                          tokenToFloat(_tokens[_tokenOffset + 1]),
                          tokenToFloat(_tokens[_tokenOffset + 2]) );
  }
  catch(std::runtime_error)
  {
    throw std::runtime_error("Could not read colour");
  }

  return colour;
}

float SceneFile::parseFloat(const std::string &_token)
{
  float val;

  try
  {
    val = tokenToFloat(_token);
  }
  catch(std::runtime_error)
  {
    throw std::runtime_error("Could not read float value");
  }

  return val;
}


Sphere* SceneFile::parseSphere(const std::vector<std::string>& _tokens)
{
  /*
   * $sphere tx ty tz sx sy sz rx ry rz cr cg cb
   */

  ngl::Mat4 transform;
  ngl::Colour colour;

  try
  {
    transform = parseTransform(_tokens, LineTypeSize);
    colour = parseColour(_tokens, LineTypeSize + TransformSize);
  }
  catch(std::runtime_error& e)
  {
    std::cerr << "Error parsing sphere data : " << e.what() << std::endl;
    return NULL;
  }

  return new Sphere(transform, colour);
}

Camera* SceneFile::parseCamera(const std::vector<std::string> &_tokens)
{
  /*
   * $sphere tx ty tz sx sy sz rx ry rz cr cg cb
   */

  ngl::Mat4 transform;
  float FOV;
  float nearZ = 0.0f, farZ = 1e9;

  try
  {
    transform = parseTransform(_tokens, LineTypeSize);
    FOV = parseFloat(_tokens[LineTypeSize + 1]);
    nearZ = parseFloat(_tokens[LineTypeSize + 2]);
    farZ = parseFloat(_tokens[LineTypeSize + 3]);
  }
  catch(std::runtime_error& e)
  {
    std::cerr << "Error parsing camera data : " << e.what() << std::endl;
    return NULL;
  }

  return new Camera(transform, FOV, nearZ, farZ);
}

bool SceneFile::write(Renderer::Scene* _scene) const
{

}


}
