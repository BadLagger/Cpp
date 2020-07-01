#pragma once

#include <vector>

class CoordXYZ{
  public:
	  CoordXYZ();
	  CoordXYZ( float x, float y, float z );
	  void set( float x, float y, float z );
	  void setX( float x );
	  void setY( float y );
	  void setZ( float z );
	  std::vector<float> get();
	  float getX();
	  float getY();
	  float getZ();
	  CoordXYZ operator=( CoordXYZ cxyz);
	  CoordXYZ operator=( std::vector<float> vcxyz );
  private:
	  float X;
	  float Y;
	  float Z;
};
