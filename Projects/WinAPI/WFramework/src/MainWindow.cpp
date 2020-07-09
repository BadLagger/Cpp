#include "MainWindow.h"

MainWindow::MainWindow( std::string name, HINSTANCE hInst )
	: g_hWnd( 0 ),
	  uMsg{ 0 }, 
	  wClass{ 0 },
	  wTitle{ 0 },
	  wStyle( 0 ),
	  wX( 0 ), wY( 0 ),
	  wWidth( 0 ), wHeight( 0 ),
	  created( false )
{
	wClass.lpszClassName = name.c_str();
	wClass.cbSize = sizeof(WNDCLASSEX) ;
	wClass.hInstance = hInst;
	wClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wClass.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
}

void MainWindow::setController( MainController& ctrl )
{
	Controller = ctrl;
	wClass.lpfnWndProc = ctrl.callback;
}

/*void MainWindow::setStyle( unsigned long style )
{
	wClass.style = style;
}*/

void MainWindow::setIcon( unsigned long icon )
{
	wClass.hIcon   = LoadIcon( wClass.hInstance, (LPCTSTR) icon );
	wClass.hIconSm = LoadIcon( wClass.hInstance, ( LPCTSTR )icon );
}

void MainWindow::setTitle( std::string name )
{
  wTitle = name;
}

std::string MainWindow::getTitle() const 
{
	return wTitle;
}

void MainWindow::setStyle( unsigned long winStyle )
{
	if( !created )
		wStyle = winStyle;
}

unsigned long MainWindow::getStyle() const 
{
  return wStyle;
}

void MainWindow::setPosition( int x, int y )
{
	if( !created )
	{
		wX = x;
		wY = y;
	}
	else
	{
		if( setPos( x, y ) )
		{
			wX = x;
			wY = y;
		}
	}
}

void MainWindow::setX( int x )
{
	if( !created )
	  wX = x;
	else
	  if( setPos( x, wY ) )
		  wX = x;
}

void MainWindow::setY( int y )
{
	if( !created )
		wY = y;
	else
	    if( setPos( wX, y ) )
			wY = y;
}

int  MainWindow::getX() const
{
	return wX;
}

int  MainWindow::getY() const
{
    return wY;
}


void MainWindow::setSize( int width, int height )
{
	if( !created )
	{
		wWidth = width;
		wHeight = height;
	}
	else
	{
		if( setSi( width, height ) )
		{
			wWidth = width;
			wHeight = height;
		}
	}
}
void MainWindow::setWidth( int width )
{
	if( !created )
		wWidth = width;
	else
		if( setSi( width, wHeight ) )
			wWidth = width;
}
void MainWindow::setHeight( int height )
{
	if( !created )
		wHeight = height;
	else
		if( setSi( wWidth, height ) )
			wHeight = height;
}

int  MainWindow::getWidth() const
{
	return wWidth;
}
int  MainWindow::getHeight() const
{
	return wHeight;
}

bool MainWindow::create( std::string title, unsigned long winStyle, int x, int y, int w, int h )
{
	if( !created )
	{
		if( RegisterClassEx( &wClass ) )
		{
			if( (g_hWnd = CreateWindowEx( NULL,
				wClass.lpszClassName, title.c_str(),
				winStyle, x, y, w, h,
				NULL, NULL, wClass.hInstance, NULL )) != NULL )
			{
				wTitle = title;
				wStyle = winStyle;
				wX = x;
				wY = y;
				wWidth = w;
				wHeight = h;
				created = true;
			}
		}
	}
	return created;
}

bool MainWindow::create()
{
	if( !created )
	{
		if( RegisterClassEx( &wClass ) )
		{
			if( (g_hWnd = CreateWindowEx( NULL,
				wClass.lpszClassName, wTitle.c_str(),
				wStyle, wX, wY, wWidth, wHeight,
				NULL, NULL, wClass.hInstance, NULL )) != NULL )
			{
				created = true;
			}
		}
	}
	return created;
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

bool MainWindow::setPos( int x, int y )
{
  return SetWindowPos( g_hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE );
}

bool MainWindow::setSi( int w, int h )
{
	return SetWindowPos( g_hWnd, HWND_TOP, 0, 0, w, h, SWP_NOMOVE );
}