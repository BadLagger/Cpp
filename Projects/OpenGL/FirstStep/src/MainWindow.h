#pragma once

#include <windows.h>
#include <string>
#include "MainController.h"

class MainWindow{
  public:
	  MainWindow( std::string name, HINSTANCE hInst );
	  void setController(MainController &ctrl);
	  void setStyle( unsigned long style );
	  void setIcon( unsigned long icon );
	  bool create( std::string title, unsigned long winStyle, int x, int y, int w, int h);
	  void show( int cmd );
	  bool run();
	  WPARAM exit();
  protected:
	  HWND            g_hWnd;
	  MainController  Controller;
  private:
	  MSG        uMsg;
	  WNDCLASSEX wClass;
};
