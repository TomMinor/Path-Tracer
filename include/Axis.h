#ifndef AXIS_H__
#define AXIS_H__
#include <ngl/Camera.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>

/// @file Axis.h
/// @brief simple class to contain and draw an axis
/// @author Jonathan Macey
/// @version 1.0
/// @date 13/10/10
/// Revision History :
/// Initial Version 13/10/10
/// @class Axis
/// @brief Simple Axis drawing
class Axis
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor
  /// @param[in] _shaderName the name of the shader to invoke when drawing
  /// @parma[in] _scale uniform scale for the initial construction of the axis
  //----------------------------------------------------------------------------------------------------------------------
  Axis(std::string _shaderName, ngl::Real _scale );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor
  //----------------------------------------------------------------------------------------------------------------------
  ~Axis();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief draw method
  /// @brief pass in the TransformStack for the shader (should have all the tx needed)
  //----------------------------------------------------------------------------------------------------------------------
  void draw(const ngl::Mat4 &_globalTx);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the camera for drawing
  //----------------------------------------------------------------------------------------------------------------------
  inline void setCam(ngl::Camera *_cam){m_cam=_cam;}
private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the name of the shader
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_shaderName;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the scale of the axis
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Real m_scale;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief load matrices to the shader
  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to the camera
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Camera *m_cam;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief transform stack for drawing
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Transformation m_transform;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief global mouse rotation
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_globalMouseTx;



};

#endif // AXIS_H
