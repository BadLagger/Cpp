#include "ColorRGBA.h"

ColorRGBA::ColorRGBA()
	: r( 0 ), g( 0 ), b( 0 ), a( 0 )
{

}

ColorRGBA::ColorRGBA( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
	: r( red ), g( green ), b( blue ), a( alpha )
{

}

ColorRGBA::ColorRGBA( unsigned char red, unsigned char green, unsigned char blue )
	: r( red ), g( green ), b( blue ), a( 0 )
{

}

ColorRGBA::ColorRGBA( float red, float green, float blue ) 
	: a(0)
{
	r = floatToUchar( red );
	g = floatToUchar( green );
	b = floatToUchar( blue );
}

void ColorRGBA::set( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
	set( red , green, blue );
	a = alpha;
}

void ColorRGBA::set( unsigned char red, unsigned char green, unsigned char blue )
{
	r = red;
	g = green;
	b = blue;
}

void ColorRGBA::set( float red, float green, float blue, float alpha )
{
	set( red, green, blue );
	a = floatToUchar( alpha );
}

void ColorRGBA::set( float red, float green, float blue )
{
	r = floatToUchar( red );
	g = floatToUchar( green );
	b = floatToUchar( blue );
}

void ColorRGBA::setR( unsigned char red )
{
	r = red;
}

void ColorRGBA::setR( float red )
{
	r = floatToUchar( red );
}

void ColorRGBA::setG( unsigned char green )
{
	g = green;
}

void ColorRGBA::setG( float green )
{
	g = floatToUchar( green );
}

void ColorRGBA::setB( unsigned char blue )
{
	b = blue;
}

void ColorRGBA::setB( float blue )
{
	b = floatToUchar( blue );
}

void ColorRGBA::setA( unsigned char alpha )
{
	a = alpha;
}

void ColorRGBA::setA( float alpha )
{
	a = floatToUchar( alpha );
}

unsigned long ColorRGBA::getInUlong()
{
	unsigned long ret = r;

	ret |= ( g << 8 );
	ret |= ( b << 16 );
	ret |= ( a << 24 );

	return ret;
}

std::vector<unsigned char> ColorRGBA::getInUcharVector()
{
	std::vector<unsigned char> ret { r, g, b, a};
	return ret;
}

std::vector<float> ColorRGBA::getInFloatVector()
{
	std::vector<float> ret{ ucharToFloat( r ), ucharToFloat( g ), ucharToFloat( b ), ucharToFloat( a ) };
	return ret;
}

unsigned char ColorRGBA::getR()
{
	return r;
}

float ColorRGBA::getRFloat()
{
	return ucharToFloat( r );
}

unsigned char ColorRGBA::getG()
{
	return g;
}

float ColorRGBA::getGFloat()
{
	return ucharToFloat( g );
}

unsigned char ColorRGBA::getB()
{
	return b;
}

float ColorRGBA::getBFloat()
{
	return ucharToFloat( b );
}

unsigned char ColorRGBA::getA()
{
	return a;
}

float ColorRGBA::getAFloat()
{
	return ucharToFloat( a );
}

ColorRGBA ColorRGBA::operator=( ColorRGBA crgba )
{
	r = crgba.getR();
	g = crgba.getG();
	b = crgba.getB();
	a = crgba.getA();
	return ( *this );
}

ColorRGBA ColorRGBA::operator=( std::vector<unsigned char> vcrgb )
{
	if( vcrgb.size() == 4 )
	{
		r = vcrgb[ 0 ];
		g = vcrgb[ 1 ];
		b = vcrgb[ 2 ];
		a = vcrgb[ 3 ];
	}
	return ( *this );
}

ColorRGBA ColorRGBA::operator=( std::vector<float> vcrgb )
{
	if( vcrgb.size() == 4 )
	{
		r = floatToUchar( vcrgb[ 0 ] );
		g = floatToUchar( vcrgb[ 1 ] );
		b = floatToUchar( vcrgb[ 2 ] );
		a = floatToUchar( vcrgb[ 3 ] );
	}
	return ( *this );
}

unsigned char ColorRGBA::floatToUchar( float color )
{
	if( color > 1. )
		return 255;
	else
	{
		int Color = static_cast< int >( color * 255 );
		if( Color < 0 )
			return 0;
		else
			return static_cast< unsigned char >( Color );
	}
}

float  ColorRGBA::ucharToFloat( unsigned char color )
{
  return ( color / 255. );
}