#include <stdexcept>
#include <sstream>
#include <iostream>

#include "raytracer/scenefile.h"
#include "raytracer/StringUtils.h"
#include "raytracer/scene.h"

namespace Renderer
{

template <typename T>
T popFirstItem(std::deque<T>& _items)
{
    T item = _items.front();
    _items.pop_front();
    return item;
}


SceneFile::SceneFile(const std::string &_path)
  : m_sceneFile(NULL), m_counter(0)
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

Renderer::Scene* SceneFile::read()
{
  Scene::objectList primitives;
  Camera* renderCamera = NULL; // Only allow 1 render camera, maybe extend to support many cameras in scenes
  Image::Pixel background;

  std::string line;
  while ( std::getline(*m_sceneFile, line) )
  {
    std::vector<std::string> tokenVector;
    tokenize(line, tokenVector, " ");

    std::deque<std::string> tokens;
    std::copy( tokenVector.begin(), tokenVector.end(), std::inserter( tokens, tokens.end() ) );

    if(tokens.size() > 0)
    {
      float xres = 640, yres = 480; // Render resolution

      std::string lineType = popFirstItem<std::string>(tokens);
      if(lineType[0] != '#' && lineType[0] == '$') // Ignore lines not starting with $ and are not comments #
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
        else if(lineType == "tri")
        {
          Triangle* primitive = parseTriangle(tokens);
          if(primitive)
            primitives.push_back(primitive);
        }
        else if(lineType == "plane")
        {
          Plane* primitive = parsePlane(tokens);
          if(primitive)
            primitives.push_back(primitive);
        }
        else if(lineType == "res")
        {
          try
          {
            xres = parseFloat(popFirstItem<std::string>(tokens));
            yres = parseFloat(popFirstItem<std::string>(tokens));
          }
          catch(std::runtime_error& e)
          {
            std::cerr << "Error parsing resolution data : " << e.what() << std::endl;
          }
        }
        else
        {
            //std::cerr << "Unknown data type : " << lineType << std::endl;
        }
      } // End $ check
    } // End token size check
    m_counter++;
  } // End line loop

  if(!renderCamera)
      renderCamera = new Camera;

  return new Scene(primitives, renderCamera->getWorldSpaceMatrix(), background);
}


ngl::Mat4 SceneFile::parseTransform(std::deque<std::string> &_tokens)
{
  ngl::Vec3 position, rotation, scale;

  if(_tokens.size() < 9)        { throw std::runtime_error("Could not read rotation"); }
  else if(_tokens.size() < 6)   { throw std::runtime_error("Could not read scale"); }
  else if(_tokens.size() < 3)   { throw std::runtime_error("Could not read position"); }

    position = ngl::Vec3( tokenToFloat(popFirstItem<std::string>(_tokens)),
                      tokenToFloat(popFirstItem<std::string>(_tokens)),
                      tokenToFloat(popFirstItem<std::string>(_tokens)) );

    scale = ngl::Vec3( tokenToFloat(popFirstItem<std::string>(_tokens)),
                     tokenToFloat(popFirstItem<std::string>(_tokens)),
                     tokenToFloat(popFirstItem<std::string>(_tokens)) );

    rotation = ngl::Vec3( tokenToFloat(popFirstItem<std::string>(_tokens)),
                        tokenToFloat(popFirstItem<std::string>(_tokens)),
                        tokenToFloat(popFirstItem<std::string>(_tokens)) );

  ngl::Mat4 translateXYZ, scaleXYZ, rotX, rotY, rotZ;

  rotX.rotateX(rotation.m_x);
  rotY.rotateY(rotation.m_y);
  rotZ.rotateZ(rotation.m_z);

  translateXYZ.translate(position.m_x, position.m_y, position.m_z);
  scaleXYZ.scale(scale.m_x, scale.m_y, scale.m_z);

  ngl::Mat4 result;
  result = translateXYZ * rotZ * rotY * rotX * scaleXYZ;

  return result;
}

ngl::Colour SceneFile::parseColour(std::deque<std::string> &_tokens)
{
  ngl::Colour colour;

  if(_tokens.size() < 3)
  {
      throw std::runtime_error("Could not read colour");
  }

  colour = ngl::Colour( tokenToFloat(popFirstItem<std::string>(_tokens)),
                        tokenToFloat(popFirstItem<std::string>(_tokens)),
                        tokenToFloat(popFirstItem<std::string>(_tokens)) );

  return colour;
}

ngl::Vec3 SceneFile::parseVertex(std::deque<std::string> &_tokens)
{
  ngl::Vec3 vtx;

  if(_tokens.size() < 3)
  {
      throw std::runtime_error("Could not read vertex");
  }

  vtx = ngl::Vec3( tokenToFloat(popFirstItem<std::string>(_tokens)),
                   tokenToFloat(popFirstItem<std::string>(_tokens)),
                   tokenToFloat(popFirstItem<std::string>(_tokens)) );

  return vtx;
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

Material::SurfaceProperty SceneFile::parseMaterialType(const std::string& _token)
{
    if(_token == "DIFFUSE")         return Material::DIFFUSE;
    else if (_token == "SPECULAR")  return Material::SPECULAR;
    else if (_token == "EMISSIVE")  return Material::EMISSIVE;
    else
        throw std::runtime_error("Unknown material ");

    return Material::DIFFUSE;
}


Sphere* SceneFile::parseSphere(std::deque<std::string>& _tokens)
{
  /*
   * $sphere tx ty tz sx sy sz rx ry rz cr cg cb
   */

  ngl::Mat4 transform;
  ngl::Colour colour;
  Material::SurfaceProperty type;


  try
  {
    transform = parseTransform(_tokens);
    colour = parseColour(_tokens);
    type = parseMaterialType( _tokens[0] );
  }
  catch(std::runtime_error& e)
  {
    std::cerr << " : Error parsing sphere data : " << e.what() << std::endl;
    return NULL;
  }

  Material mat(type, colour);

  return new Sphere(transform, mat, 1.0f);
}

Triangle* SceneFile::parseTriangle(std::deque<std::string> &_tokens)
{
    /*
    * $tri v0x v0y v0z v1x v1y v1z v2x v2y v2z tx ty tz sx sy sz rx ry rz cr cg cb
    */

   ngl::Mat4 transform;
   ngl::Colour colour;
   ngl::Vec3 vertex[3];
   Material::SurfaceProperty type;

   try
   {
     vertex[0] = parseVertex(_tokens);
     vertex[1] = parseVertex(_tokens);
     vertex[2] = parseVertex(_tokens);

     transform = parseTransform(_tokens);
     colour = parseColour(_tokens);
     type = parseMaterialType( _tokens[0] );

   }
   catch(std::runtime_error& e)
   {
     std::cerr << "Line " << m_counter << " : Error parsing triangle data : " << e.what() << std::endl;
     return NULL;
   }

   Material mat(type, colour);
   mat.m_diffuse.m_r = colour.m_r;
   mat.m_diffuse.m_g = colour.m_g;
   mat.m_diffuse.m_b = colour.m_b;
//   mat.m_type = type;

   return new Triangle(vertex[0], vertex[1], vertex[2], transform, mat);
}

Plane* SceneFile::parsePlane(std::deque<std::string> &_tokens)
{
    /*
    * $tri v0x v0y v0z v1x v1y v1z v2x v2y v2z tx ty tz sx sy sz rx ry rz cr cg cb
    */

   ngl::Mat4 transform;
   ngl::Colour colour;
   ngl::Vec3 vertex[4];
   Material::SurfaceProperty type;

   try
   {
     vertex[0] = parseVertex(_tokens);
     vertex[1] = parseVertex(_tokens);
     vertex[2] = parseVertex(_tokens);
     vertex[3] = parseVertex(_tokens);

     transform = parseTransform(_tokens);
     colour = parseColour(_tokens);
     type = parseMaterialType( _tokens[0] );
   }
   catch(std::runtime_error& e)
   {
     std::cerr  << "Line " << m_counter << " : Error parsing plane data : " << e.what() << std::endl;
     return NULL;
   }

   Material mat(type, colour);

   return new Plane(vertex[0], vertex[1], vertex[2], vertex[3], transform, mat);
}

Camera* SceneFile::parseCamera(std::deque<std::string> &_tokens)
{
  ngl::Mat4 transform;
  float FOV;
  float nearZ = 0.0f, farZ = 1e9;

  try
  {
    transform = parseTransform(_tokens);
    FOV = parseFloat( popFirstItem<std::string>(_tokens) );
    nearZ = parseFloat( popFirstItem<std::string>(_tokens) );
    farZ = parseFloat( popFirstItem<std::string>(_tokens) );
  }
  catch(std::runtime_error& e)
  {
    std::cerr  << "Line " << m_counter << " : Error parsing camera data : " << e.what() << std::endl;
    return NULL;
  }

  return new Camera(transform, FOV, nearZ, farZ);
}

bool SceneFile::write(Renderer::Scene* _scene)
{

}
}
