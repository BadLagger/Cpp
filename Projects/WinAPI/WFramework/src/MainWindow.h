#pragma once

#include <string>
#include "MainController.h"

class MainWindow{
  public:
	  MainWindow( std::string name, HINSTANCE hInst );
	  void setController( MainController &ctrl );
	 // void setStyle( unsigned long style );
	  void setIcon( unsigned long icon );
	  void setTitle( std::string name );
	  std::string getTitle();
	  void setStyle( unsigned long winStyle);
	  unsigned long getStyle();
	  void setPosition( int x, int y );
	  void setX( int x );
	  void setY( int y );
	  int  getX();
	  int  getY();
	  void setSize( int width, int height);
	  void setWidth( int width );
	  void setHeight( int width );
	  int  getWidth();
	  int  getHeight();
	  bool create( std::string title, unsigned long winStyle, int x, int y, int w, int h);
	  bool create();
	  void show( int cmd );
	  bool run();
	  WPARAM exit();
  protected:
	  HWND            g_hWnd;
	  MainController  Controller;
  private:
	  MSG         uMsg;
	  WNDCLASSEX  wClass;
	  std::string wTitle;
	  unsigned long wStyle;
	  int wX;
	  int wY;
	  int wWidth;
	  int wHeight;
	  bool created;
};
