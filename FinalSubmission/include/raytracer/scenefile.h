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

class SceneFile
{
public:
  SceneFile(const std::string& _path);
  ~SceneFile();

  Scene* read();
  bool write(Scene *_scene);

private:
  std::ifstream* m_sceneFile;

  enum TokenOffsets { LineTypeSize = 1, TransformSize = 9, ColourSize = 3, VertexSize = 3 };

  ///
  /// \brief parseTransform Parses a stream of tokens containing position, scale & rotation and constructing a transform matrix from it.
  /// \param _tokens Expects 9 strings from start index of token stream, of the form px, py, pz, sx, sy, sz, rx, ry, rz
  /// \param _tokenOffset is the start index of the token stream
  /// \return a transform matrix, M = Translation * Rotation * Scale
  /// \exceptions std::runtime_error is returned if the token stream is too short
  ///
  ngl::Mat4 parseTransform(std::deque<std::string> &_tokens, unsigned int _tokenOffset = 1);

  ///
  /// \brief parseColour Parses a stream of tokens and extracts the r,g,b colour values
  /// \param _tokens Expects 3 strings from start index of token stream, of the form r,g,b
  /// \param _tokenOffset is the start index of the token stream
  /// \return an rgb colour
  /// \exceptions std::runtime_error is returned if the token stream is too short
  ///
  ngl::Colour parseColour(std::deque<std::string> &_tokens, unsigned int _tokenOffset = 1);

  ///
  /// \brief parseVertex Parses a stream of tokens and extracts the x,y,z values
  /// \param _tokens Expects 3 strings from start index of token stream, of the form x,y,z
  /// \param _tokenOffset is the start index of the token stream
  /// \return a vector
  /// \exceptions std::runtime_error is returned if the token stream is too short
  ///
  ngl::Vec3 parseVertex(std::deque<std::string> &_tokens, unsigned int _tokenOffset = 1);

  ///
  /// \brief parseFloat Parses a token and extracts the float value
  /// \param _tokens Expects 1 string token
  /// \return a float value
  /// \exceptions std::runtime_error is returned if the token could not be read
  ///
  float parseFloat(const std::string &_token);

  Sphere* parseSphere(std::deque<std::string> &_tokens);
  Triangle* parseTriangle(std::deque<std::string> &_tokens);
  Plane* parsePlane(std::deque<std::string> &_tokens);
  Camera* parseCamera(std::deque<std::string> &_tokens);

  unsigned int m_counter;
};

}

#endif // SCENEFILE_H
