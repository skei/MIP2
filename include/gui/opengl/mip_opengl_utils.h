#ifndef mip_opengl_utils_included
#define mip_opengl_utils_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/opengl/mip_opengl.h"

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



void MIP_printFBConfigs(Display* ADisplay, GLXFBConfig* fbc, int num_fbc) {
  MIP_Print("%i matching fb configs\n",num_fbc);
    for (int i=0; i<num_fbc; i++) {

      int fbc_fbconfig_id = -1;
      int fbc_buffer_size = -1;
      int fbc_level = -1;
      int fbc_doublebuffer = -1;
      int fbc_stereo = -1;
      int fbc_aux_buffers = -1;
      int fbc_red_size = -1;
      int fbc_green_size = -1;
      int fbc_blue_size = -1;
      int fbc_alpha_size = -1;
      int fbc_depth_size = -1;
      int fbc_stencil_size = -1;
      int fbc_accum_red_size = -1;
      int fbc_accum_green_size = -1;
      int fbc_accum_blue_size = -1;
      int fbc_accum_alpha_size = -1;
      int fbc_sample_buffers = -1;
      int fbc_samples = -1;
      int fbc_render_type = -1;
      int fbc_drawable_type = -1;
      int fbc_x_renderable = -1;
      int fbc_x_visual_type = -1;
      int fbc_config_caveat = -1;
      int fbc_transparent_type = -1;
      int fbc_transparent_index_value = -1;
      int fbc_transparent_red_value = -1;
      int fbc_transparent_green_value = -1;
      int fbc_transparent_blue_value = -1;
      int fbc_transparent_alpha_value = -1;
      int fbc_max_pbuffer_width = -1;
      int fbc_max_pbuffer_height = -1;
      int fbc_max_pbuffer_pixels = -1;
      int fbc_visual_id = -1;

      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_FBCONFIG_ID             , &fbc_fbconfig_id ); //XID       XID of GLXFBConfig
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_BUFFER_SIZE             , &fbc_buffer_size ); //integer   depth of the color buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_LEVEL                   , &fbc_level ); //integer   frame buffer level
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_DOUBLEBUFFER            , &fbc_doublebuffer ); //boolean   True if color buffers have front/back pairs
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_STEREO                  , &fbc_stereo ); //boolean   True if color buffers have left/right pairs
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_AUX_BUFFERS             , &fbc_aux_buffers ); //integer   no. of auxiliary color buffers
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_RED_SIZE                , &fbc_red_size ); //integer   no. of bits of Red in the color buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_GREEN_SIZE              , &fbc_green_size ); //integer   no. of bits of Green in the color buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_BLUE_SIZE               , &fbc_blue_size ); //integer   no. of bits of Blue in the color buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ALPHA_SIZE              , &fbc_alpha_size ); //integer   no. of bits of Alpha in the color buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_DEPTH_SIZE              , &fbc_depth_size ); //integer   no. of bits in the depth buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_STENCIL_SIZE            , &fbc_stencil_size ); //integer   no. of bits in the stencil buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_RED_SIZE          , &fbc_accum_red_size ); //integer   no. Red bits in the accum. buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_GREEN_SIZE        , &fbc_accum_green_size ); //integer   no. Green bits in the accum. buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_BLUE_SIZE         , &fbc_accum_blue_size ); //integer   no. Blue bits in the accum. buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_ALPHA_SIZE        , &fbc_accum_alpha_size ); //integer   no. of Alpha bits in the accum. buffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_SAMPLE_BUFFERS          , &fbc_sample_buffers ); //integer   number of multisample buffers
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_SAMPLES                 , &fbc_samples ); //integer   number of samples per pixel
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_RENDER_TYPE             , &fbc_render_type ); //bitmask   which rendering modes are supported.
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_DRAWABLE_TYPE           , &fbc_drawable_type ); //bitmask   which GLX drawables are supported.
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_X_RENDERABLE            , &fbc_x_renderable ); //boolean   True if X can render to drawable
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_X_VISUAL_TYPE           , &fbc_x_visual_type ); //integer   X visual type of the associated visual
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_CONFIG_CAVEAT           , &fbc_config_caveat ); //enum      any caveats for the configuration
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_TYPE        , &fbc_transparent_type ); //enum      type of transparency supported
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_INDEX_VALUE , &fbc_transparent_index_value ); //integer   transparent index value
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_RED_VALUE   , &fbc_transparent_red_value ); //integer   transparent red value
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_GREEN_VALUE , &fbc_transparent_green_value ); //integer   transparent green value
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_BLUE_VALUE  , &fbc_transparent_blue_value ); //integer   transparent blue value
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_ALPHA_VALUE , &fbc_transparent_alpha_value ); //integer   transparent alpha value
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_MAX_PBUFFER_WIDTH       , &fbc_max_pbuffer_width ); //integer   maximum width of GLXPbuffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_MAX_PBUFFER_HEIGHT      , &fbc_max_pbuffer_height ); //integer   maximum height of GLXPbuffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_MAX_PBUFFER_PIXELS      , &fbc_max_pbuffer_pixels ); //integer   maximum size of GLXPbuffer
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_VISUAL_ID               , &fbc_visual_id );                 //integer   XID of corresponding Visual

      /*
      typedef struct {
        Visual *visual;
        VisualID visualid;
        int screen;
        int depth;
        int class;
        unsigned long red_mask;
        unsigned long green_mask;
        unsigned long blue_mask;
        int colormap_size;
        int bits_per_rgb;
      } XVisualInfo;
      */

      //XVisualInfo* vi = glXGetVisualFromFBConfig(ADisplay,fbc[i]);
      //  MIP_Print("visualinfo.visualid: %i\n",vi->visualid);
      //XFree(vi);

      MIP_DPrint("FBConfig: %i\n",i);
      MIP_DPrint("  xcb_fbconfig_id:              %i  XID of GLXFBConfig\n", fbc_fbconfig_id  );
      MIP_DPrint("  xcb_buffer_size:              %i  depth of the color buffer\n", fbc_buffer_size  );
      MIP_DPrint("  xcb_level:                    %i  frame buffer level\n", fbc_level  );
      MIP_DPrint("  xcb_doublebuffer:             %i  True if color buffers have front/back pairs\n", fbc_doublebuffer  );
      MIP_DPrint("  xcb_stereo:                   %i  True if color buffers have left/right pairs\n", fbc_stereo  );
      MIP_DPrint("  xcb_aux_buffers:              %i  no. of auxiliary color buffers\n", fbc_aux_buffers  );
      MIP_DPrint("  xcb_red_size:                 %i  no. of bits of Red in the color buffer\n", fbc_red_size  );
      MIP_DPrint("  xcb_green_size:               %i  no. of bits of Green in the color buffer\n", fbc_green_size  );
      MIP_DPrint("  xcb_blue_size:                %i  no. of bits of Blue in the color buffer\n", fbc_blue_size  );
      MIP_DPrint("  xcb_alpha_size:               %i  no. of bits of Alpha in the color buffer\n", fbc_alpha_size  );
      MIP_DPrint("  xcb_depth_size:               %i  no. of bits in the depth buffer\n", fbc_depth_size  );
      MIP_DPrint("  xcb_stencil_size:             %i  no. of bits in the stencil buffer\n", fbc_stencil_size  );
      MIP_DPrint("  xcb_accum_red_size:           %i  no. Red bits in the accum. buffer\n", fbc_accum_red_size  );
      MIP_DPrint("  xcb_accum_green_size:         %i  no. Green bits in the accum. buffer\n", fbc_accum_green_size  );
      MIP_DPrint("  xcb_accum_blue_size:          %i  no. Blue bits in the accum. buffer\n", fbc_accum_blue_size  );
      MIP_DPrint("  xcb_accum_alpha_size:         %i  no. of Alpha bits in the accum. buffer\n", fbc_accum_alpha_size  );
      MIP_DPrint("  xcb_sample_buffers:           %i  number of multisample buffers\n", fbc_sample_buffers  );
      MIP_DPrint("  xcb_samples:                  %i  number of samples per pixel\n", fbc_samples  );
      MIP_DPrint("  xcb_render_type:              %i  which rendering modes are supported.\n", fbc_render_type  );
      MIP_DPrint("  xcb_drawable_type:            %i  which GLX drawables are supported.\n", fbc_drawable_type );
      MIP_DPrint("  xcb_x_renderable:             %i  True if X can render to drawable\n", fbc_x_renderable  );
      MIP_DPrint("  xcb_x_visual_type:            %4x  X visual type of the associated visual\n", fbc_x_visual_type  );
      MIP_DPrint("  xcb_config_caveat:            %i  any caveats for the configuration\n", fbc_config_caveat  );
      MIP_DPrint("  xcb_transparent_type:         %i  type of transparency supported\n", fbc_transparent_type  );
      MIP_DPrint("  xcb_transparent_index_value:  %i  transparent index value\n", fbc_transparent_index_value  );
      MIP_DPrint("  xcb_transparent_red_value:    %i  transparent red value\n", fbc_transparent_red_value  );
      MIP_DPrint("  xcb_transparent_green_value:  %i  transparent green value\n", fbc_transparent_green_value  );
      MIP_DPrint("  xcb_transparent_blue_value:   %i  transparent blue value\n", fbc_transparent_blue_value  );
      MIP_DPrint("  xcb_transparent_alpha_value:  %i  transparent alpha value\n", fbc_transparent_alpha_value  );
      MIP_DPrint("  xcb_max_pbuffer_width:        %i  maximum width of GLXPbuffer\n", fbc_max_pbuffer_width  );
      MIP_DPrint("  xcb_max_pbuffer_height:       %i  maximum height of GLXPbuffer\n", fbc_max_pbuffer_height  );
      MIP_DPrint("  xcb_max_pbuffer_pixels:       %i  maximum size of GLXPbuffer\n", fbc_max_pbuffer_pixels  );
      MIP_DPrint("  xcb_visual_id:                %i  XID of corresponding Visual\n", fbc_visual_id  );
    }

}

//----------------------------------------------------------------------
#endif

