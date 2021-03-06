#ifndef GL_WINDOW_H__
#define GL_WINDOW_H__


#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Transformation.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <Axis.h>

//#include <QtGui/QWindow>
#include <QGLWidget>
#include <QMouseEvent>
#include <QColorDialog>
#include <QtConcurrent/QtConcurrent>

//#include <QGLFunctions>
//#include <QtOpenGL>

#include "raytracer/scene.h"
#include "raytracer/rendercontext.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file GLWindow.h
/// @brief a basic Qt GL window class for ngl demos
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/10/10
/// Revision History :
/// Initial Version 10/10/10 (Binary day ;-0 )
/// @class GLWindow
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------
class GLWindow : public QGLWidget
{
Q_OBJECT        // must include this if you use Qt signals/slots
public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for GLWindow
  /// @param [in] _parent the parent window to create the GL context in
  //----------------------------------------------------------------------------------------------------------------------
  GLWindow(const QGLFormat _format, QWidget *_parent );
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------

  ~GLWindow()
  {
//    renderThread.cancel();
//    renderThread.waitForFinished();
  }

  void resetMouse();

  bool loadScene(const std::string& _path);

  void renderScene(Renderer::RenderContext *_context);

  Renderer::Scene* getScene() { return m_scene; }

  void createFramebufferObject();

  void createTextureObject();

  std::vector <ngl::Vec3> m_verts;

  std::string m_scenePath;

  inline void setScenePath(const std::string& _path) { m_scenePath = _path; }

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Our Camera
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Camera *m_cam;

  ngl::VertexArrayObject *test_vao;

  QFuture<void> renderThread;



private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our texture id used by the FBO
  //----------------------------------------------------------------------------------------------------------------------
  GLuint m_textureID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our FBO id used by the FBO
  //----------------------------------------------------------------------------------------------------------------------
  GLuint m_fboID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our render buffer id used by the FBO
  //----------------------------------------------------------------------------------------------------------------------
  GLuint m_rboID;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the x rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_spinXFace;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the y rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_spinYFace;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if the Left mouse button is pressed when dragging
  //----------------------------------------------------------------------------------------------------------------------
  bool m_rotateActive;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if the Right mouse button is pressed when dragging
  //----------------------------------------------------------------------------------------------------------------------
  bool m_translateActive;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous x mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_origX;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous y mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_origY;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous x mouse value for Position changes
  //----------------------------------------------------------------------------------------------------------------------
  int m_origXPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous y mouse value for Position changes
  //----------------------------------------------------------------------------------------------------------------------
  int m_origYPos;
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_modelPos;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the global mouse transforms
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_mouseGlobalTX;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the index into the VBO name array for the drawing
  //----------------------------------------------------------------------------------------------------------------------
  int m_drawIndex;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if we draw the normals
  //----------------------------------------------------------------------------------------------------------------------
  bool m_drawNormals;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the final transform matrix
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_transform;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a matrix to hold the rotation
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_rotate;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a matrix to demonstrate gimal locs
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_gimbal;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the translation matrix
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_translate;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the scale matrix
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_scale;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the Euler rotation matrix
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_euler;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the light for the shading
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Light *m_light;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the colour for the object material
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Colour m_colour;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the size to draw the normals
  //----------------------------------------------------------------------------------------------------------------------
  int m_normalSize;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the axis
  //----------------------------------------------------------------------------------------------------------------------
  Axis *m_axis;
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------

  ngl::Material m_material;

  Renderer::Scene* m_scene;



  //----------------------------------------------------------------------------------------------------------------------
  /// @enum for the Matrix order Rotate Trans Scale, Trans Rotate Scale or Euler
  //----------------------------------------------------------------------------------------------------------------------
  enum MATRIXORDER{
                    RTS, ///<Rotate Translate Scale
                    TRS, ///<Translate Rotate Scale
                    GIMBALLOCK,
                    EULERTS, //< Use Axis Angle Euler Trans Scale
                    TEULERS //<  Use Translate Euler Scale

                  };
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the order of multiplication for the transform matrix
  //----------------------------------------------------------------------------------------------------------------------
  MATRIXORDER m_matrixOrder;

public slots :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief slot to indicate the vbo selection combo box has changed
  /// called from MainWindow
  /// @param[in] _index the new value of the combo box index
  //----------------------------------------------------------------------------------------------------------------------
  void vboChanged(int _index);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief slot to indicate the normal tick box has been activated
  /// called from MainWindow
  /// @param[in] _value the new value of the tick box
  //----------------------------------------------------------------------------------------------------------------------
  void toggleNormals(bool _value );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief slot to indicate the wireframe tick box has been activated
  /// called from MainWindow
  /// @param[in] _value the new value of the tick box
  //----------------------------------------------------------------------------------------------------------------------
  void toggleWireframe(bool _value );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief slot to indicate the normal length slider had changed
  /// called from MainWindow
  /// @param[in] _value the new value of the tick box
  //----------------------------------------------------------------------------------------------------------------------
  void setNormalSize(int _value );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief called when any of the scale elements are modified sets the
  /// new m_scale matrix value and forces a re-calcuation and re-draw
  /// called from MainWindow
  /// @param[in] _x the value of scale in the x
  /// @param[in] _y the value of scale in the y
  /// @param[in] _z the value of scale in the z
  //----------------------------------------------------------------------------------------------------------------------
  void setScale(float _x,float _y, float _z );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief called when any of the translate elements are modified sets the
  /// new m_translate matrix value and forces a re-calcuation and re-draw
  /// called from MainWindow
  /// @param[in] _x the value of translate in the x
  /// @param[in] _y the value of translate in the y
  /// @param[in] _z the value of translate in the z
  //----------------------------------------------------------------------------------------------------------------------

  void setTranslate(float _x,float _y,float _z);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief called when any of the rotate elements are modified sets the
  /// new m_rotate matrix value and forces a re-calcuation and re-draw
  /// called from MainWindow
  /// @param[in] _x the value of rotation in the x
  /// @param[in] _y the value of rotation in the y
  /// @param[in] _z the value of rotation in the z
  //----------------------------------------------------------------------------------------------------------------------
  void setRotate(float _x,float _y, float _z );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief called when the colour button is pressed and a new colour selected
  /// sets a new current material and forces update
  /// called from MainWindow
  /// @param[in] _r the value of red
  /// @param[in] _g the value of green
  /// @param[in] _b the value of blue
  //----------------------------------------------------------------------------------------------------------------------
  void setColour(float _r,float _g,float _b );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief called when the matrix order combobox is changed sets the new
  /// matrix order for m_matrixOrder and forces re-draw
  /// called from MainWindow
  /// @param[in] _index the index of the combobox
  //---------------------------------------------------------------------------------------------------------------------
  void setMatrixOrder( int _index);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief called when any of the euler rotation elements are modified sets the
  /// new m_euler matrix value and forces a re-calcuation and re-draw
  /// called from MainWindow
  /// @param[in] _angle the angle in degrees
  /// @param[in] _x the value of rotation axis [-1 , 1]
  /// @param[in] _y the value of rotation axis [-1 , 1]
  /// @param[in] _z the value of rotation axis [-1 , 1]
  //----------------------------------------------------------------------------------------------------------------------
  void setEuler(float _angle,float _x,float _y,float _z );

 signals :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief signal emitted when the transform matrix is modified
  /// it is recived by the main window which will set the main matrix values
  /// @param _m the new transformation values used in the display
  //----------------------------------------------------------------------------------------------------------------------
  void matrixDirty(ngl::Mat4 _m);
protected:

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  //----------------------------------------------------------------------------------------------------------------------
  void initializeGL();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  //----------------------------------------------------------------------------------------------------------------------
  void resizeGL(const int _w, const int _h  );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is the main gl drawing routine which is called whenever the window needs to
  /// be re-drawn
  //----------------------------------------------------------------------------------------------------------------------
  void paintGL();

private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseMoveEvent ( QMouseEvent * _event );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------

  void mousePressEvent (QMouseEvent *_event );

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseReleaseEvent (QMouseEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse wheel is moved
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void wheelEvent(QWheelEvent *_event );

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief static array of names used in the vbo drawing
  /// initialised in GLWindow.cpp
  //----------------------------------------------------------------------------------------------------------------------
  const static std::string s_vboNames[17];
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader();



};

#endif
