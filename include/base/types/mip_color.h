#ifndef mip_color_included
#define mip_color_included
//----------------------------------------------------------------------

#include "mip.h"

/*
struct NVGcolor {
	union {
		float rgba[4];
		struct {
			float r,g,b,a;
		};
	};
};

*/




class MIP_Color {

public:

  //float r = 0.0f;
  //float g = 0.0f;
  //float b = 0.0f;
  //float a = 1.0f;
  union {
    float rgba[4];
    struct { float r,g,b,a; };
  };

//------------------------------
public:
//------------------------------

  MIP_Color() {}

  //----------

  MIP_Color(float v) {
    r = v;
    g = v;
    b = v;
    a = 1.0f;
  }

  //----------

  MIP_Color(float ar, float ag, float ab, float aa=1.0f) {
    r = ar;
    g = ag;
    b = ab;
    a = aa;
  }

//------------------------------
public:
//------------------------------

  operator uint32_t () {
    uint8_t rr = r * 255.0f;
    uint8_t gg = g * 255.0f;
    uint8_t bb = b * 255.0f;
    uint8_t aa = a * 255.0f;
    uint32_t color = (aa << 24) + (rr << 16) + (gg << 8) + bb;
    return color;
  }

  MIP_Color& operator = (uint32_t color) {
    float aa = (color & 0xff000000) >> 24;
    float rr = (color & 0x00ff0000) >> 16;
    float gg = (color & 0x0000ff00) >> 8;
    float bb = (color & 0x000000ff);
    r = rr * MIP_INV255F;
    g = gg * MIP_INV255F;
    b = bb * MIP_INV255F;
    a = aa * MIP_INV255F;
    return *this;
  }

  MIP_Color& operator += (uint32_t color) {
    return *this;
  }

  //bool operator == (const char* str) {
  //  return (strcmp(MBuffer,str) == 0) ? true : false;
  //}

//------------------------------
public:
//------------------------------

  void add(MIP_Color color) {
    r += color.r;
    g += color.g;
    b += color.b;
    a += color.a;
  }

  void mul(MIP_Color color) {
    r *= color.r;
    g *= color.g;
    b *= color.b;
    a *= color.a;
  }

//  void blend(MIP_Color color1, MIP_Color color2, float blend) {
//    MIP_Color result;
//    result.r = (color1.r * blend) + (color2.r * (1.0 - blend));
//    result.g = (color1.g * blend) + (color2.g * (1.0 - blend));
//    result.b = (color1.b * blend) + (color2.b * (1.0 - blend));
//    result.a = (color1.a * blend) + (color2.a * (1.0 - blend));
//  }

  void blend(MIP_Color color, float alpha) {
    r = (color.r * alpha) + (r * (1.0 - alpha));
    g = (color.g * alpha) + (g * (1.0 - alpha));
    b = (color.b * alpha) + (b * (1.0 - alpha));
    a = 1;//(color.a * alpha) + (a * (1.0 - alpha));
  }

  void blend(MIP_Color color) {
    blend(color,color.a);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const MIP_Color MIP_COLOR_DARK_RED        = MIP_Color( 0.25, 0.00, 0.00 );
const MIP_Color MIP_COLOR_RED             = MIP_Color( 0.50, 0.00, 0.00 );
const MIP_Color MIP_COLOR_LIGHT_RED       = MIP_Color( 0.75, 0.00, 0.00 );
const MIP_Color MIP_COLOR_BRIGHT_RED      = MIP_Color( 1.00, 0.00, 0.00 );

const MIP_Color MIP_COLOR_DARK_ORANGE     = MIP_Color( 0.25, 0.12, 0.00 );
const MIP_Color MIP_COLOR_ORANGE          = MIP_Color( 0.50, 0.25, 0.00 );
const MIP_Color MIP_COLOR_LIGHT_ORANGE    = MIP_Color( 0.75, 0.37, 0.00 );
const MIP_Color MIP_COLOR_BRIGHT_ORANGE   = MIP_Color( 1.00, 0.50, 0.00 );

const MIP_Color MIP_COLOR_DARK_YELLOW     = MIP_Color( 0.25, 0.25, 0.00 );
const MIP_Color MIP_COLOR_YELLOW          = MIP_Color( 0.50, 0.50, 0.00 );
const MIP_Color MIP_COLOR_LIGHT_YELLOW    = MIP_Color( 0.75, 0.75, 0.00 );
const MIP_Color MIP_COLOR_BRIGHT_YELLOW   = MIP_Color( 1.00, 1.00, 0.00 );

const MIP_Color MIP_COLOR_DARK_LIME       = MIP_Color( 0.12, 0.25, 0.00 );
const MIP_Color MIP_COLOR_LIME            = MIP_Color( 0.25, 0.50, 0.00 );
const MIP_Color MIP_COLOR_LIGHT_LIME      = MIP_Color( 0.37, 0.75, 0.00 );
const MIP_Color MIP_COLOR_BRIGHT_LIME     = MIP_Color( 0.50, 1.00, 0.00 );


const MIP_Color MIP_COLOR_DARK_GREEN      = MIP_Color( 0.00, 0.25, 0.00 );
const MIP_Color MIP_COLOR_GREEN           = MIP_Color( 0.00, 0.50, 0.00 );
const MIP_Color MIP_COLOR_LIGHT_GREEN     = MIP_Color( 0.00, 0.75, 0.00 );
const MIP_Color MIP_COLOR_BRIGHT_GREEN    = MIP_Color( 0.00, 1.00, 0.00 );

const MIP_Color MIP_COLOR_DARK_GREEN2     = MIP_Color( 0.00, 0.25, 0.12 );
const MIP_Color MIP_COLOR_GREEN2          = MIP_Color( 0.00, 0.50, 0.35 );
const MIP_Color MIP_COLOR_LIGHT_GREEN2    = MIP_Color( 0.00, 0.75, 0.37 );
const MIP_Color MIP_COLOR_BRIGHT_GREEN2   = MIP_Color( 0.00, 1.00, 0.50 );

const MIP_Color MIP_COLOR_DARK_CYAN       = MIP_Color( 0.00, 0.25, 0.25 );
const MIP_Color MIP_COLOR_CYAN            = MIP_Color( 0.00, 0.50, 0.50 );
const MIP_Color MIP_COLOR_LIGHT_CYAN      = MIP_Color( 0.00, 0.75, 0.75 );
const MIP_Color MIP_COLOR_BRIGHT_CYAN     = MIP_Color( 0.00, 1.00, 1.00 );

const MIP_Color MIP_COLOR_DARK_CYAN2       = MIP_Color( 0.00, 0.18, 0.25 );
const MIP_Color MIP_COLOR_CYAN2            = MIP_Color( 0.00, 0.37, 0.50 );
const MIP_Color MIP_COLOR_LIGHT_CYAN2      = MIP_Color( 0.00, 0.50, 0.75 );
const MIP_Color MIP_COLOR_BRIGHT_CYAN2     = MIP_Color( 0.00, 0.75, 1.00 );

const MIP_Color MIP_COLOR_DARK_BLUE1      = MIP_Color( 0.00, 0.12, 0.25 );
const MIP_Color MIP_COLOR_BLUE1           = MIP_Color( 0.00, 0.25, 0.50 );
const MIP_Color MIP_COLOR_LIGHT_BLUE1     = MIP_Color( 0.00, 0.37, 0.75 );
const MIP_Color MIP_COLOR_BRIGHT_BLUE1    = MIP_Color( 0.00, 0.50, 1.00 );

const MIP_Color MIP_COLOR_DARK_BLUE       = MIP_Color( 0.00, 0.00, 0.25 );
const MIP_Color MIP_COLOR_BLUE            = MIP_Color( 0.00, 0.00, 0.50 );
const MIP_Color MIP_COLOR_LIGHT_BLUE      = MIP_Color( 0.00, 0.00, 0.75 );
const MIP_Color MIP_COLOR_BRIGHT_BLUE     = MIP_Color( 0.00, 0.00, 1.00 );

const MIP_Color MIP_COLOR_DARK_BLUE2      = MIP_Color( 0.12, 0.00, 0.25 );
const MIP_Color MIP_COLOR_BLUE2           = MIP_Color( 0.25, 0.00, 0.50 );
const MIP_Color MIP_COLOR_LIGHT_BLUE2     = MIP_Color( 0.37, 0.00, 0.75 );
const MIP_Color MIP_COLOR_BRIGHT_BLUE2    = MIP_Color( 0.50, 0.00, 1.00 );

const MIP_Color MIP_COLOR_DARK_MAGENTA    = MIP_Color( 0.25, 0.00, 0.25 );
const MIP_Color MIP_COLOR_MAGENTA         = MIP_Color( 0.50, 0.00, 0.50 );
const MIP_Color MIP_COLOR_LIGHT_MAGENTA   = MIP_Color( 0.75, 0.00, 0.75 );
const MIP_Color MIP_COLOR_BRIGHT_MAGENTA  = MIP_Color( 1.00, 0.00, 1.00 );

const MIP_Color MIP_COLOR_DARK_RED2       = MIP_Color( 0.25, 0.00, 0.12 );
const MIP_Color MIP_COLOR_RED2            = MIP_Color( 0.50, 0.00, 0.25 );
const MIP_Color MIP_COLOR_LIGHT_RED2      = MIP_Color( 0.75, 0.00, 0.37 );
const MIP_Color MIP_COLOR_BRIGHT_RED2     = MIP_Color( 1.00, 0.00, 0.50 );


/*
const MIP_Color MIP_COLOR_                = MIP_Color( 0.00, 0.00, 0.00 );
*/

const MIP_Color MIP_COLOR_BLACK           = MIP_Color( 0.00 );
const MIP_Color MIP_COLOR_DARK_GRAY       = MIP_Color( 0.25 );
const MIP_Color MIP_COLOR_GRAY            = MIP_Color( 0.50 );
const MIP_Color MIP_COLOR_LIGHT_GRAY      = MIP_Color( 0.75 );
const MIP_Color MIP_COLOR_WHITE           = MIP_Color( 1.00 );

//----------------------------------------------------------------------
#endif
