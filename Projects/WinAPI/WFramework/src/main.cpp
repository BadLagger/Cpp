#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include "WFramework.h"

static void repos( bool &quit, void *prm );
static void move( bool& quit, void* prm );

WF_MAIN{
	bool quit = false;
	MainWindow app( "NEW_WIN_CLASS", hInstance );
	MainController ctrl;

	std::srand( std::time( nullptr ) );

	app.setController( ctrl );
	
	app.setTitle( "WFramework" );
	app.setStyle( WS_CAPTION | WS_SYSMENU );
	app.setSize( 640, 480 );

	if( !app.create( ) )
		return E_FAIL;

	app.show( nCmdShow );

	std::thread ReposThread( move, std::ref(quit), &app );
	while( app.run() );

	quit = true;
	ReposThread.join();

	return app.exit();
}

static void repos( bool &quit, void *prm )
{
	MainWindow *app = (MainWindow* )prm;
	int rX = 0,
		rY = 0;

	while( !quit )
	{
		rX = std::rand() % ( 1920 - app->getWidth());
		rY = std::rand() % ( 1080 - app->getHeight());

		app->setPosition( rX, rY );
		std::this_thread::sleep_for( std::chrono::milliseconds(500) );
	}
}

static void move( bool& quit, void* prm )
{
	MainWindow* app = (MainWindow* )prm;

	int destX = 0,
		destY = 0;

	while( !quit )
	{
		if( destX == app->getX() && destY == app->getY() )
		{
			destX = std::rand() % (1920 - app->getWidth());
			destY = std::rand() % (1080 - app->getHeight());
		}

		if( app->getX() > destX )
			app->setX( app->getX() - 1 );
		else 
			if( app->getX() < destX )
				app->setX( app->getX() + 1 );

		if( app->getY() > destY )
			app->setY( app->getY() - 1 );
		else
			if( app->getY() < destY )
				app->setY( app->getY() + 1 );

		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}
}

