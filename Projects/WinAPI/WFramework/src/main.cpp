#include <iostream>
#include "WFramework.h"

WF_MAIN{
	MainWindow app( "NEW_WIN_CLASS", hInstance );
	MainController ctrl;

	app.setController( ctrl );
	//app.setStyle( CS_HREDRAW | CS_VREDRAW | CS_OWNDC );

	if( !app.create() )
		return E_FAIL;

	app.show( nCmdShow );

	while( app.run() );

	return app.exit();
}

