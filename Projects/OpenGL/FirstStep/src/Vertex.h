#pragma once

#include "ColorRGBA.h"
#include "CoordXYZ.h"

class Vertex{
  public:
	  Vertex();
	  Vertex( Vertex *vertex);
	  Vertex( ColorRGBA clr );
	  Vertex( CoordXYZ xyz );
	  Vertex( ColorRGBA clr, CoordXYZ xyz );
	  Vertex( ColorRGBA *clr, CoordXYZ *xyz );
	  Vertex( ColorRGBA &clr, CoordXYZ* xyz );
	  void      set( ColorRGBA clr, CoordXYZ  xyz );
	  void      setColor( ColorRGBA clr );
	  void      setCoord( CoordXYZ  xyz );
	  ColorRGBA getColor();
	  CoordXYZ  getCoord();
	  unsigned long copyToArray( unsigned long* arr, unsigned long arrLen );
	  void copyToVector( std::vector<unsigned long> &out );
      Vertex    operator=( Vertex vertex);

  private:
	  ColorRGBA color;
	  CoordXYZ  xyz;
};