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

//----------------------------------------------------------------------

// Helper to check for extension string presence. Adapted from:
// http://www.opengl.org/resources/features/OGLextensions/

//static
bool isExtensionSupported(const char *extList, const char *extension) {
  const char *start;
  const char *where, *terminator;
  // Extension names should not have spaces.
  where = strchr(extension, ' ');
  if (where || *extension == '\0') return false;
  // It takes a bit of care to be fool-proof about parsing the OpenGL
  // extensions string. Don't be fooled by sub-strings, etc.
  for (start=extList;;) {
    where = strstr(start, extension);
    if (!where) break;
    terminator = where + strlen(extension);
    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;
    start = terminator;
  }
  return false;
}

//----------------------------------------------------------------------

#define MIP_OPENGL_ERROR_CHECK {                      \
  GLint err = glGetError();                           \
  if (err != GL_NO_ERROR) MIP_printOpenGLError(err);  \
}

//#define MIP_OPENGL_ERROR_CHECK { MIP_Print("gl error: %i\n",glGetError()); }

void MIP_printOpenGLError(GLint err) {
  switch (err) {
    case GL_NO_ERROR:                       MIP_Print("OpenGL Error: No error has been recorded. The value of this symbolic constant is guaranteed to be 0.\n"); break;
    case GL_INVALID_ENUM:                   MIP_Print("OpenGL Error: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
    case GL_INVALID_VALUE:                  MIP_Print("OpenGL Error: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
    case GL_INVALID_OPERATION:              MIP_Print("OpenGL Error: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:  MIP_Print("OpenGL Error: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
    case GL_OUT_OF_MEMORY:                  MIP_Print("OpenGL Error: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n"); break;
    case GL_STACK_UNDERFLOW:                MIP_Print("OpenGL Error: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n"); break;
    case GL_STACK_OVERFLOW:                 MIP_Print("OpenGL Error: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n"); break;
    default:                                MIP_Print("OpenGL Error: Unknown error %i\n",err); break;
  }
}

//----------------------------------------------------------------------
#endif
