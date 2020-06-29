#include "MainWindow.h"

MainWindow::MainWindow( std::string name, HINSTANCE hInst )
{
	wClass.lpszClassName = name.c_str();
	wClass.cbSize = sizeof(WNDCLASSEX) ;
	wClass.hInstance = hInst;
	wClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wClass.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
	wClass.lpszMenuName = NULL;
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;
}

void MainWindow::setController( MainController& ctrl )
{
	wClass.lpfnWndProc = ctrl.callback;
}

void MainWindow::setStyle( unsigned long style )
{
	wClass.style = style;
}

void MainWindow::setIcon( unsigned long icon )
{
	wClass.hIcon   = LoadIcon( wClass.hInstance, (LPCTSTR) icon );
	wClass.hIconSm = LoadIcon( wClass.hInstance, ( LPCTSTR )icon );
}

bool MainWindow::create( std::string title, unsigned long winStyle, int x, int y, int w, int h )
{
	if( RegisterClassEx( &wClass ) )
	{
		if( ( g_hWnd = CreateWindowEx( NULL, 
			                           wClass.lpszClassName, title.c_str(), 
			                           winStyle, x, y, w, h, 
			                           NULL, NULL, wClass.hInstance, NULL) ) != NULL )
			return true;
	}
	return false;
}

void MainWindow::show( int cmd )
{
	ShowWindow( g_hWnd, cmd );
	UpdateWindow( g_hWnd );
}

bool MainWindow::run()
{
	if( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ))
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		return true;
	}
	return false;
}

WPARAM MainWindow::exit()
{
	UnregisterClass( wClass.lpszClassName, wClass.hInstance );
	return uMsg.wParam;
}