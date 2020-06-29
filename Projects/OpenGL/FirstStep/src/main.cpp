#include "MainOpenGL.h"
#include "MainController.h"
#include "resource.h"


int WINAPI WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	MainOpenGL app( "NEW_WIN_CLASS", hInstance );

	MainController ctrl;

    app.setController( ctrl );
	app.setStyle( CS_HREDRAW | CS_VREDRAW | CS_OWNDC );
	app.setIcon( IDI_OPENGL_ICON );

	if( !app.create( "FirstStep WinAPI + OpenGL", 
		             WS_OVERLAPPED | WS_VISIBLE, 
		             1920/2 - 1280/2, 1080/2 - 768/2, 1280, 768) )
		return E_FAIL;

	app.show( nCmdShow );

	if( !app.setPixel( (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER),
		          PFD_TYPE_RGBA,
		          16 ) )
		return app.exit();

	app.setBackground(1.0, 0.8, 0.5, 0.0);

	while( app.run() );



	return app.exit();
}
