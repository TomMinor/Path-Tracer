#ifndef SCENEFILE_H
#define SCENEFILE_H

#include <string>
#include <fstream>

#include "raytracer/scene.h"
#include "raytracer/sphere.h"

namespace Renderer
{

class SceneFile
{
public:
  SceneFile(const std::string& _path);
  ~SceneFile();

  Renderer::Scene* read() const;
  bool write(Renderer::Scene *_scene) const;

private:
  std::ifstream* m_sceneFile;

  enum TokenOffsets { LineTypeSize = 1, TransformSize = 9, ColourSize = 3 };

  ///
  /// \brief parseTransform Parses a stream of tokens containing position, scale & rotation and constructing a transform matrix from it.
  /// \param _tokens Expects 9 strings from start index of token stream, of the form px, py, pz, sx, sy, sz, rx, ry, rz
  /// \param _tokenOffset is the start index of the token stream
  /// \return a transform matrix, M = Translation * Rotation * Scale
  /// \exceptions std::runtime_error is returned if the token stream is too short
  ///
  static ngl::Mat4 parseTransform(const std::vector<std::string> &_tokens, unsigned int _tokenOffset = 1);

  ///
  /// \brief parseColour Parses a stream of tokens and extracts the r,g,b colour values
  /// \param _tokens Expects 3 strings from start index of token stream, of the form r,g,b
  /// \param _tokenOffset is the start index of the token stream
  /// \return an rgb colour
  /// \exceptions std::runtime_error is returned if the token stream is too short
  ///
  static ngl::Colour parseColour(const std::vector<std::string> &_tokens, unsigned int _tokenOffset = 1);

  ///
  /// \brief parseFloat Parses a token and extracts the float value
  /// \param _tokens Expects 1 string token
  /// \return a float value
  /// \exceptions std::runtime_error is returned if the token could not be read
  ///
  static float parseFloat(const std::string &_token);

  static Sphere* parseSphere(const std::vector<std::string> &_tokens);
  static Camera* parseCamera(const std::vector<std::string> &_tokens);
};

}

#endif // SCENEFILE_H
