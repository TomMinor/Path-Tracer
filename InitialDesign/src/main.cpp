/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include <QtGui/QGuiApplication>
#include <iostream>
#include "NGLScene.h"


//int main(int argc, char **argv)
//{
//    unsigned width = 640, height = 480;
//    unsigned char *buffer = new unsigned char [width * height * 3], *pix = buffer;
//    memset(buffer, 0x0, sizeof(unsigned char) * width * height);

//    float focal = 60;
//    float angle = tan(focal * 0.5 * M_PI / 180); // convert from degree to radian
//    float imageAspectRatio = float(width) / height;

//    // loop over all the pixels of the image
//    ngl::Vec3 rayOrigin(0.0f, 0.0f, 0.0f);
//    ngl::Camera cameraToWorld;

//    rayOrigin = cameraToWorld.getViewMatrix() * rayOrigin;

//    for (unsigned j = 0; j < height; ++j) {
//        for (unsigned i = 0; i < width; ++i, pix += 3) {
//            // remap from raster to screen space
//            float xx = (2 * (i + 0.5) / width - 1) * angle * imageAspectRatio;
//            float yy = (1 - 2 * (j + 0.5) / height) * angle;

//            // create the ray direction, looking down the z-axis
//            // and transform by the camera-to-world matrix
//            ngl::Vec3 rayDirection;
//            rayDirection = cameraToWorld.getViewMatrix() * ngl::Vec3(xx, yy, -1);

//            rayDirection.normalize();

//            Ray ray(rayOrigin, rayDirection, kCameraRay);

//            // convert the ray direction to a color
//            pix[0] = (unsigned char)(255 * (1 + rayDirection.m_x) * 0.5);
//            pix[1] = (unsigned char)(255 * (1 + rayDirection.m_y) * 0.5);
//            pix[2] = 0;
//        }
//    }
//    // save to PPM
//    std::ofstream ofs;
//    ofs.open("./untitled.ppm");
//    ofs << "P6\n" << width << " " << height << "\n255\n";
//    ofs.write((char*)buffer, sizeof(unsigned char) * width * height * 3);
//    ofs.close();
//    delete [] buffer;

//    return 0;
//}


int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
  // create an OpenGL format specifier
  QSurfaceFormat format;
  // set the number of samples for multisampling
  // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
  format.setSamples(4);
  #if defined( DARWIN)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(3);
    format.setMinorVersion(2);
  #else
    // with luck we have the latest GL version so set to this
    format.setMajorVersion(4);
    format.setMinorVersion(5);
  #endif
  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
  format.setProfile(QSurfaceFormat::CoreProfile);
  // now set the depth buffer to 24 bits
  format.setDepthBufferSize(24);
  // now we are going to create our scene window
  NGLScene window;
  // and set the OpenGL format
  window.setFormat(format);
  // we can now query the version to see if it worked
  std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";
  // set the window size
  window.resize(1024, 720);
  // and finally show
  window.show();

  return app.exec();
}



