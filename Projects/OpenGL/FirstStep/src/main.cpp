#include "MainOpenGL.h"
#include "MainController.h"
#include "resource.h"
#include <thread>

static void loop_thread(bool *quit, void *ptr);

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

	app.setBackground(0.0, 0.0, 0.0, 0.0);

	bool quit = false;
	std::thread loop( loop_thread , &quit, (void*)&app);

	while( app.run() );
	quit = true;
	loop.join();

	return app.exit();
}

static void loop_thread( bool *quit, void* ptr )
{
	MainOpenGL *app = (MainOpenGL*)ptr;

	while( !(*quit) )
	{
		if( app->getAutoControl() )
		  app->switchPrimitives();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
