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