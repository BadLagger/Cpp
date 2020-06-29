#pragma once

#include "MainWindow.h"

class MainOpenGL : public MainWindow {
  public:
	  MainOpenGL( std::string name, HINSTANCE hInst );
	  bool setPixel(unsigned long flags, unsigned long type, unsigned long bits);
	  void setBackground(double r, double g, double b, double a );
	  bool run();
	  WPARAM exit();
  private:
	  HDC   g_hDC;
	  HGLRC g_hRC;
	  PIXELFORMATDESCRIPTOR pxFormatDesc;
};
