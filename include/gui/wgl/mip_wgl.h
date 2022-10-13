#ifndef mip_wgl_included
#define mip_wgl_included
//----------------------------------------------------------------------

/*

see:

https://devblogs.microsoft.com/oldnewthing/20060601-06/?p=31003
https://stackoverflow.com/questions/48663815/getdc-releasedc-cs-owndc-with-opengl-and-gdi
https://community.khronos.org/t/cs-owndc-obligatory/50927/3
https://www.codeproject.com/Articles/66250/BeginPaint-EndPaint-or-GetDC-ReleaseDC

*/

//----------------------------------------------------------------------

/*

  GLX functions should not be called between glBegin and glEnd operations. If a
  GLX function is called within a glBegin/glEnd pair, then the result is undefined;
  however, no error is reported.

  https://stackoverflow.com/questions/47918078/creating-opengl-structures-in-a-multithreaded-program
  The requirement for OpenGL is that the context created for rendering should
  be owned by single thread at any given point and the thread that owns context
  should make it current and then call any gl related function. If you do that
  without owning and making context current then you get segmentation faults.
  By default the context will be current for the main thread.

  .. other option is to make one context per thread and make it current when
  thread starts.
*/

//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"

//#define MIP_OPENGL_MAJOR 3
//#define MIP_OPENGL_MINOR 2

//----------------------------------------------------------------------
//
// loader
//
//----------------------------------------------------------------------

// a) glloadgen

#include "extern/glloadgen/gl_core.3.2.h"
#include "extern/glloadgen/gl_core.3.2.c"

//----------

// b) simple-opengl-loader

// https://github.com/tsherif/simple-opengl-loader

//#define SOGL_MAJOR_VERSION 3
//#define SOGL_MINOR_VERSION 2
//#define SOGL_IMPLEMENTATION_WIN32
//
//#include "extern/simple-opengl-loader.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include <GL/gl.h>
//#include <GL/wgl.h>
#include <GL/wglext.h>

//----------------------------------------------------------------------
#endif
