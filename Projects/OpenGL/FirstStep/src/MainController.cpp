#include "MainController.h"
#include <gl/GL.h>
#include <gl/GLU.h>

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
		 /* int nWidth = LOWORD( lPrm );
		  int nHeigth = HIWORD( lPrm );
		  glViewport( 0, 0, nWidth , nHeigth );
		  glMatrixMode( GL_PROJECTION );
		  glLoadIdentity();
		  gluPerspective( 45.0, (GLdouble) nWidth / ( GLdouble )nHeigth, 0.1, 100.0 );*/
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