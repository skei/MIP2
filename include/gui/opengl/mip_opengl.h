#ifndef mip_opengl_included
#define mip_opengl_included
//----------------------------------------------------------------------

#include "mip.h"

//#include <GL/gl.h>
//#include <GL/glx.h>

#define MIP_OPENGL_MAJOR 4
#define MIP_OPENGL_MINOR 4

//

// https://github.com/tsherif/simple-opengl-loader

#define SOGL_MAJOR_VERSION MIP_OPENGL_MAJOR
#define SOGL_MINOR_VERSION MIP_OPENGL_MINOR
//#define SOGL_IMPLEMENTATION_WIN32
#define SOGL_IMPLEMENTATION_X11
#include "extern/simple-opengl-loader.h"

#include <GL/gl.h>
#include <GL/glx.h>


//----------------------------------------------------------------------
#endif
