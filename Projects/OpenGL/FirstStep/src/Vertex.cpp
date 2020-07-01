#include "Vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex( Vertex* vertex )
{
	*this = *vertex;
}

Vertex::Vertex( ColorRGBA clr )
{
	color = clr;
}

Vertex::Vertex( CoordXYZ xyz )
{
	this->xyz = xyz;
}

Vertex::Vertex( ColorRGBA clr, CoordXYZ xyz )
{
	color = clr;
	this->xyz = xyz;
}

Vertex::Vertex( ColorRGBA* clr, CoordXYZ* xyz )
{
	color = *clr;
	this->xyz = *xyz;
}

Vertex::Vertex( ColorRGBA& clr, CoordXYZ* xyz )
{
	color = clr;
	this->xyz = *xyz;
}

void Vertex::set( ColorRGBA clr, CoordXYZ  xyz )
{
	color = clr;
	this->xyz = xyz;
}

void      Vertex::setColor( ColorRGBA clr )
{
	color = clr;
}

void      Vertex::setCoord( CoordXYZ  xyz )
{
	this->xyz = xyz;
}

ColorRGBA Vertex::getColor()
{
	return color;
}

CoordXYZ  Vertex::getCoord()
{
	return xyz;
}

Vertex    Vertex::operator=( Vertex vertex )
{
	color = vertex.getColor();
	xyz = vertex.getCoord();

	return ( *this );
}

unsigned long Vertex::copyToArray( unsigned long* arr, unsigned long arrLen )
{
	if( arrLen >= 4 )
	{
		*(arr++) = color.getInUlong();
		float ftmp = xyz.getX();
		*( arr++ ) = *( reinterpret_cast < unsigned long* >( &ftmp ) );
		ftmp = xyz.getY();
		*( arr++ ) = *( reinterpret_cast< unsigned long* >( &ftmp ) );
		ftmp = xyz.getZ();
		*( arr++ ) = *( reinterpret_cast< unsigned long* >( &ftmp ) );
		return 4;
	}
	return 0;
}

void Vertex::copyToVector( std::vector<unsigned long> &out )
{
	out.push_back( color.getInUlong() );
	for( auto f : xyz.get() )
		out.push_back( *( reinterpret_cast < unsigned long* >( &f ) ) );
}