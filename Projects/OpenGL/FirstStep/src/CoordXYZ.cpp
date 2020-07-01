#include "CoordXYZ.h"

CoordXYZ::CoordXYZ()
	: X( 0. ), Y( 0. ), Z( 0. )
{

}

CoordXYZ::CoordXYZ( float x, float y, float z )
	: X( x ), Y( y ), Z( z )
{

}

void CoordXYZ::set( float x, float y, float z )
{
    X = x;
	Y = y;
	Z = z;
}

void CoordXYZ::setX( float x )
{
	X = x;
}

void CoordXYZ::setY( float y )
{
	Y = y;
}

void CoordXYZ::setZ( float z )
{
	Z = z;
}

std::vector<float> CoordXYZ::get()
{
	std::vector<float> ret{ X, Y, Z};
	return ret;
}

float CoordXYZ::getX()
{
	return X;
}

float CoordXYZ::getY()
{
	return Y;
}

float CoordXYZ::getZ()
{
	return Z;
}

CoordXYZ CoordXYZ::operator=( CoordXYZ cxyz )
{
	X = cxyz.getX();
	Y = cxyz.getY();
	Z = cxyz.getZ();

	return ( *this );
}

CoordXYZ CoordXYZ::operator=( std::vector<float> vcxyz )
{
	if( vcxyz.size() == 3 )
	{
		X = vcxyz[ 1 ];
		Y = vcxyz[ 2 ];
		Z = vcxyz[ 3 ];
	}

	return ( *this );
}