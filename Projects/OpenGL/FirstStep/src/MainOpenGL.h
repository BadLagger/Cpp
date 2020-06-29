#pragma once

#include "MainWindow.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class MainOpenGL : public MainWindow {
  public:
	  MainOpenGL( std::string name, HINSTANCE hInst );
	  bool setPixel(unsigned long flags, unsigned long type, unsigned long bits);
	  void setBackground(double r, double g, double b, double a );
	  bool run();
	  WPARAM exit();
	  void switchPrimitives();
	  bool getAutoControl();
  private:
	  HDC   g_hDC;
	  HGLRC g_hRC;
	  PIXELFORMATDESCRIPTOR pxFormatDesc;
	  bool g_bRenderInWireFrame;
	  GLenum g_currentPrimitive;
	  bool autoControl;

	  void checkController();
	  void drawPrimitive();
};
