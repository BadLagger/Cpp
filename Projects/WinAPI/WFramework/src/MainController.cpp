#include "MainController.h"

WPARAM MainController::LastKeyDown = 0;

MainController::MainController()
{}

LRESULT CALLBACK MainController::callback( HWND hWnd, UINT msg, WPARAM wPrm, LPARAM lPrm )
{
	switch( msg )
	{
	  case WM_KEYDOWN:
	  {
		  keyDownHndl( wPrm );
	  }
	  break;
	  case WM_SIZE:
	  {
	  }
	  break;
	  case WM_CLOSE:
	  {
		  PostQuitMessage( 0 );
	  }
	  break;
	  case WM_DESTROY:
	  {
		  PostQuitMessage( 0 );
	  }
	  break;
	  default: 
		  return DefWindowProc( hWnd, msg, wPrm, lPrm );
	}
	return 0;
}

void MainController::keyDownHndl( WPARAM key )
{
	LastKeyDown = key;
	switch( key )
	{
	  case VK_ESCAPE:
		  PostQuitMessage(0);
	  break;
	}

}

WPARAM  MainController::getLastKeyDown()
{
  WPARAM ret = LastKeyDown;
  LastKeyDown = 0;
  return ret;
}