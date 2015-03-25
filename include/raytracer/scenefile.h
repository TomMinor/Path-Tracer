#ifndef SCENEFILE_H
#define SCENEFILE_H

#include <string>
#include <fstream>
#include <deque>

#include "raytracer/sphere.h"
#include "raytracer/triangle.h"
#include "raytracer/plane.h"


namespace Renderer
{

class Scene;
class Camera;

///
/// \brief The SceneFile class parses a text file for scene information and constructs a new scene from it
///
class SceneFile
{
public:

  ///
  /// \brief SceneFile opens the file in _path, ready for reading
  /// \param _path system path
  /// \exception std::invalid_argument is _path is invalid
  ///
  SceneFile(const std::string& _path);

  ~SceneFile();

  ///
  /// \brief reads the system path stored in m_sceneFile
  /// \return constructs a scene and returns it based on the file data
  ///
  Scene* read();

private:
  std::ifstream* m_sceneFile;

  ///
  /// \brief parseTransform Parses a stream of tokens containing position, scale & rotation and constructing a transform matrix from it.
  /// \param _tokens Expects 9 strings from start index of token stream, of the form px, py, pz, sx, sy, sz, rx, ry, rz
  /// \param _tokenOffset is the start index of the token stream
  /// \return a transform matrix, M = Translation * Rotation * Scale
  /// \exceptions std::runtime_error is returned if the token stream is too short
  ///
  ngl::Mat4 parseTransform(std::deque<std::string> &_tokens);

  ///
  /// \brief parseColour Parses a stream of tokens and extracts the r,g,b colour values
  /// \param _tokens Expects 3 strings from start index of token stream, of the form r,g,b
  /// \param _tokenOffset is the start index of the token stream
  /// \return an rgb colour
  /// \exceptions std::runtime_error is returned if the token stream is too short
  ///
  ngl::Colour parseColour(std::deque<std::string> &_tokens);

  ///
  /// \brief parseVertex Parses a stream of tokens and extracts the x,y,z values
  /// \param _tokens Expects 3 strings from start index of token stream, of the form x,y,z
  /// \param _tokenOffset is the start index of the token stream
  /// \return a vector
  /// \exceptions std::runtime_error is returned if the token stream is too short
  ///
  ngl::Vec3 parseVertex(std::deque<std::string> &_tokens);

  ///
  /// \brief parseFloat Parses a token and extracts the float value
  /// \param _tokens Expects 1 string token
  /// \return a float value
  /// \exceptions std::runtime_error is returned if the token could not be read
  ///
  float parseFloat(const std::string &_token);

  ///
  /// \brief parseMaterialType Reads a token to find out the material type
  /// \param _token A string value
  /// \return The parsed material type
  ///
  Material::SurfaceProperty parseMaterialType(const std::string& _token);

  ////
  /// \brief parseSphere Reads a series of tokens to find out spherical data
  /// \param _tokens Expects 13 tokens
  /// \return Constructs a new sphere and returns it
  ///
  Sphere* parseSphere(std::deque<std::string> &_tokens);

  ////
  /// \brief parseTriangle Reads a series of tokens to find out triangle data
  /// \param _tokens Expects 21 tokens
  /// \return Constructs a new triangle and returns it
  ///
  Triangle* parseTriangle(std::deque<std::string> &_tokens);

//  ////
//  /// \brief parsePlane Reads a series of tokens to find out planar data
//  /// \param _tokens Expects 13 tokens
//  /// \return Constructs a new sphere and returns it
//  ///
//  Plane* parsePlane(std::deque<std::string> &_tokens);
  ////
  /// \brief parseCamera Reads a series of tokens to find out camera data
  /// \param _tokens Expects 11 tokens
  /// \return Constructs a new camera and returns it
  ///
  Camera* parseCamera(std::deque<std::string> &_tokens);

  ///
  /// \brief m_counter stores the current line number
  ///
  unsigned int m_counter;
};

}

#endif // SCENEFILE_H
