#include "MainOpenGL.h"

MainOpenGL::MainOpenGL( std::string name, HINSTANCE hInst ) 
	: MainWindow( name , hInst ),
	  g_hDC( NULL ),
	  g_hRC( NULL ),
	  pxFormatDesc{0},
	  g_bRenderInWireFrame( false ),
	  g_currentPrimitive( GL_POLYGON ),
	  autoControl( false )
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
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0f, 1280.0f/768.0f, 0.1f, 100.0f);
	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );
}

bool MainOpenGL::run()
{
	checkController();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	drawPrimitive();
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

void MainOpenGL::checkController()
{
	switch( Controller.getLastKeyDown() )
	{
	  case VK_F1:
		  switchPrimitives();
		  break;
	  case VK_F2:
		  g_bRenderInWireFrame = !g_bRenderInWireFrame;
		  if( g_bRenderInWireFrame )
			  glPolygonMode( GL_FRONT, GL_LINE );
		  else
			  glPolygonMode( GL_FRONT, GL_FILL );
		  break;
	  case VK_F3:
		  autoControl = !autoControl;
		  break;
    }
}

void MainOpenGL::drawPrimitive()
{
	struct Vertex
	{
		unsigned char r, g, b, a;
		float x, y, z;
	} 
	g_polygon[] = {
		{255, 0,   0,   255, -0.3f, -1.5f, 0.0f},
		{0,   255, 0,   255,  0.3f, -1.5f, 0.0f},
		{0,   0,   255, 255,  0.5f,  0.5f, 0.0f},
		{255, 255, 0,   255,  0.0f,  1.5f, 0.0f},
		{255, 0,   255, 255, -0.5f,  0.5f, 0.0f},
	},
	g_quadStrip[] = {
		{255, 0,   0,   255, -0.5f, -1.5f, 0.0f},
		{0,   255, 0,   255,  0.5f, -1.5f, 0.0f},
		{0,   0,   255, 255, -0.2f, -0.5f, 0.0f},
		{255, 255, 0,   255,  0.2f, -0.5f, 0.0f},
		{255, 0,   255, 255, -0.5f,  0.5f, 0.0f},
		{0,   255, 255, 255,  0.5f,  0.5f, 0.0f},
		{255, 0,   0,   255, -0.4f,  1.5f, 0.0f},
		{0,   255, 0,   255,  0.4f,  1.5f, 0.0f},
	},
	g_quads[] = {
		{255, 0,   0,   255, -0.5f, -0.5f, 0.0f},
		{0,   255, 0,   255,  0.5f, -0.5f, 0.0f},
		{0,   0,   255, 255,  0.5f,  0.5f, 0.0f},
		{255, 255, 0,   255, -0.5f,  0.5f, 0.0f},

		{255, 0,   255, 255, -1.5f, -1.0f, 0.0f},
		{0,   255, 255, 255, -1.0f, -1.0f, 0.0f},
		{255, 0,   0,   255, -1.0f,  1.5f, 0.0f},
		{0,   255, 0,   255, -1.5f,  1.5f, 0.0f},

		{0,   0,   255, 255,  1.0f, -0.2f, 0.0f},
		{255, 255, 0,   255,  2.0f, -0.2f, 0.0f},
		{0,   255, 0,   255,  2.0f,  0.2f, 0.0f},
		{255, 0,   255, 255,  1.0f,  0.2f, 0.0f},
	},
	g_points[] = {
		{255, 0,   0,   255,   0.0f,   0.0f, 0.0f},
		{0,   255, 0,   255,   0.5f,   0.0f, 0.0f},
		{0,   0,   255, 255,  -0.5f,   0.0f, 0.0f},
		{255, 255, 0,   255,   0.0f,  -0.5f, 0.0f},
		{255, 0,   255, 255,   0.0f,   0.5f, 0.0f},
	},
	g_lines[] = {
		{255, 0,   0,   255,   -1.0f,   0.0f, 0.0f},
		{255, 0,   0,   255,    0.0f,   1.0f, 0.0f},

		{0, 255,   0,   255,   0.5f,    1.0f, 0.0f},
		{0, 255,   0,   255,   0.5f,   -1.0f, 0.0f},

		{0, 0,   255,   255,    1.0f,   -0.5f, 0.0f},
		{0, 0,   255,   255,   -1.0f,   -0.5f, 0.0f},
	},
	g_lineStrip_and_lineLoop[] = {
		{255, 0,   0,   255,    0.5f,   0.5f, 0.0f},
		{0,   255, 0,   255,    1.0f,   0.0f, 0.0f},
		{0,   0,   255, 255,    0.0f,  -1.0f, 0.0f},
		{255, 255, 0,   255,   -1.0f,   0.0f, 0.0f},
		{255, 0,   0,   255,    0.0f,   0.0f, 0.0f},
		{255, 0,   255, 255,    0.0f,   1.0f, 0.0f},
	},
	g_triangles[] = {
		{ 255,   0,   0, 255, -1.0f, 0.0f, 0.0f },
	    {   0,   0, 255, 255,  1.0f, 0.0f, 0.0f },
	    {   0, 255,   0, 255,  0.0f, 1.0f, 0.0f },


	    { 255, 255,   0, 255, -0.5f,-1.0f, 0.0f },
	    { 255,   0,   0, 255,  0.5f,-1.0f, 0.0f },
	    {   0, 255, 255, 255,  0.0f,-0.5f, 0.0f }
	},
	g_triangleStrip[] = {
		{ 255,   0,   0, 255, -2.0f, 0.0f, 0.0f },
	    {   0,   0, 255, 255, -1.0f, 0.0f, 0.0f },
	    {   0, 255,   0, 255, -1.0f, 1.0f, 0.0f },
	    { 255,   0, 255, 255,  0.0f, 0.0f, 0.0f },
	    { 255, 255,   0, 255,  0.0f, 1.0f, 0.0f },
	    { 255,   0,   0, 255,  1.0f, 0.0f, 0.0f },
	    {   0, 255, 255, 255,  1.0f, 1.0f, 0.0f },
	    {   0, 255,   0, 255,  2.0f, 1.0f, 0.0f }
	},
	g_trianleFan[] = {
		{ 255,   0,   0, 255,  0.0f,-1.0f, 0.0f },
	    {   0, 255, 255, 255,  1.0f, 0.0f, 0.0f },
	    { 255,   0, 255, 255,  0.5f, 0.5f, 0.0f },
	    { 255, 255,   0, 255,  0.0f, 1.0f, 0.0f },
	    {   0,   0, 255, 255, -0.5f, 0.5f, 0.0f },
	    {   0, 255,   0, 255, -1.0f, 0.0f, 0.0f }
	};

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -5.0f );

	switch( g_currentPrimitive )
	{
	  case GL_POLYGON:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_polygon );
		  glDrawArrays( GL_POLYGON, 0, 5);
		  break;
	  case GL_QUAD_STRIP:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_quadStrip );
		  glDrawArrays( GL_QUAD_STRIP, 0, 8 );
		  break;
	  case GL_QUADS:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_quads );
		  glDrawArrays( GL_QUADS, 0, 12 );
		  break;
	  case GL_POINTS:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_points );
		  glDrawArrays( GL_POINTS, 0, 5 );
		  break;
	  case GL_LINES:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_lines );
		  glDrawArrays( GL_LINES, 0, 6 );
		  break;
	  case GL_LINE_STRIP:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_lineStrip_and_lineLoop );
		  glDrawArrays( GL_LINE_STRIP, 0, 6 );
		  break;
	  case GL_LINE_LOOP:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_lineStrip_and_lineLoop );
		  glDrawArrays( GL_LINE_LOOP, 0, 6 );
		  break;
	  case GL_TRIANGLES:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_triangles );
		  glDrawArrays( GL_TRIANGLES, 0, 6 );
		  break;
	  case GL_TRIANGLE_STRIP:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_triangleStrip );
		  glDrawArrays( GL_TRIANGLE_STRIP, 0, 8 );
		  break;
	  case GL_TRIANGLE_FAN:
		  glInterleavedArrays( GL_C4UB_V3F, 0, g_trianleFan );
		  glDrawArrays( GL_TRIANGLE_FAN, 0, 6 );
		  break;
	}
}

void MainOpenGL::switchPrimitives()
{
	if( g_currentPrimitive == GL_POLYGON )
		g_currentPrimitive = GL_QUAD_STRIP;
	else if( g_currentPrimitive == GL_QUAD_STRIP )
		g_currentPrimitive = GL_QUADS;
	else if( g_currentPrimitive == GL_QUADS )
		g_currentPrimitive = GL_POINTS;
	else if( g_currentPrimitive == GL_POINTS )
		g_currentPrimitive = GL_LINES;
	else if( g_currentPrimitive == GL_LINES )
		g_currentPrimitive = GL_LINE_STRIP;
	else if( g_currentPrimitive == GL_LINE_STRIP )
		g_currentPrimitive = GL_LINE_LOOP;
	else if( g_currentPrimitive == GL_LINE_LOOP )
		g_currentPrimitive = GL_TRIANGLES;
	else if( g_currentPrimitive == GL_TRIANGLES )
		g_currentPrimitive = GL_TRIANGLE_STRIP;
	else if( g_currentPrimitive == GL_TRIANGLE_STRIP )
		g_currentPrimitive = GL_TRIANGLE_FAN;
	else if( g_currentPrimitive == GL_TRIANGLE_FAN )
		g_currentPrimitive = GL_POLYGON;
}

bool MainOpenGL::getAutoControl()
{
  return autoControl;
}
