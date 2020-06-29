#include "MainOpenGL.h"
#include <gl/GL.h>
#include <gl/GLU.h>

MainOpenGL::MainOpenGL( std::string name, HINSTANCE hInst ) 
	: MainWindow( name , hInst ),
	  g_hDC( NULL ),
	  g_hRC( NULL ),
	  pxFormatDesc{0}
{
}

bool MainOpenGL::setPixel( unsigned long flags, unsigned long type, unsigned long bits )
{
	pxFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pxFormatDesc.nVersion = 1;
	pxFormatDesc.dwFlags = flags;
	pxFormatDesc.iPixelType = type;
	pxFormatDesc.cColorBits = bits;
	pxFormatDesc.cDepthBits = bits;
	g_hDC = GetDC( g_hWnd );

	GLuint iPixelFormat = ChoosePixelFormat( g_hDC, &pxFormatDesc );

	if( iPixelFormat != 0 )
	{
		PIXELFORMATDESCRIPTOR bestMatch_pxFormatDesc;
		DescribePixelFormat( g_hDC, iPixelFormat, sizeof( pxFormatDesc ), &bestMatch_pxFormatDesc );
		if( ( bestMatch_pxFormatDesc.cDepthBits < pxFormatDesc.cDepthBits )
		||  ( SetPixelFormat( g_hDC, iPixelFormat, &pxFormatDesc ) == FALSE ) )
			return false;
	}
	else
	  return false;

	g_hRC = wglCreateContext( g_hDC );
	wglMakeCurrent( g_hDC, g_hRC );

	return true;
}

void MainOpenGL::setBackground( double r, double g, double b, double a )
{
	glClearColor( r, g, b, a );
}

bool MainOpenGL::run()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	SwapBuffers( g_hDC );
	return MainWindow::run();
}

WPARAM MainOpenGL::exit()
{
	if( g_hRC != NULL )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( g_hRC );
		g_hRC = NULL;
	}

	if( g_hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_hDC );
		g_hDC = NULL;
	}

	return MainWindow::exit();
}