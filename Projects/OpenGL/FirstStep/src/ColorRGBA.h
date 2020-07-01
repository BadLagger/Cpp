#pragma once

#include <vector>

class ColorRGBA{
  public:
      ColorRGBA();
      ColorRGBA( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );
      ColorRGBA( unsigned char red, unsigned char green, unsigned char blue );
      ColorRGBA( float red, float green, float blue);
      void set( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha );
      void set( unsigned char red, unsigned char green, unsigned char blue );
      void set( float red, float green, float blue, float alpha );
      void set( float red, float green, float blue );
      void setR( unsigned char red );
      void setR( float red );
      void setG( unsigned char green );
      void setG( float green );
      void setB( unsigned char blue );
      void setB( float blue );
      void setA( unsigned char alpha );
      void setA( float alpha );
      unsigned long getInUlong();
      std::vector<unsigned char> getInUcharVector();
      std::vector<float> getInFloatVector();
      unsigned char getR();
      float getRFloat();
      unsigned char getG();
      float getGFloat();
      unsigned char getB();
      float getBFloat();
      unsigned char getA();
      float getAFloat();

      ColorRGBA operator=( ColorRGBA crgba);
      ColorRGBA operator=( std::vector<unsigned char> vcrgb );
      ColorRGBA operator=( std::vector<float> vcrgb );
  private:
      unsigned char r;
      unsigned char g;
      unsigned char b;
      unsigned char a;

      unsigned char floatToUchar( float color );
      float         ucharToFloat( unsigned char color );
};