#pragma once

#include <windows.h>

class MainController{
  public:
	  MainController();
	  static LRESULT CALLBACK callback(HWND hWnd, UINT msg, WPARAM wPrm, LPARAM lPrm);
  private:
	  static void keyDownHndl(WPARAM key);
};
