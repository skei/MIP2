#ifndef mip_wgl_utils_included
#define mip_wgl_utils_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/wgl/mip_wgl.h"

//#include <GL/wglext.h>
//#include "gui/nanovg/mip_nanovg.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions

/*
void *GetAnyGLFuncAddress(const char *name) {
  void *p = (void *)wglGetProcAddress(name);
  if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1)) {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }
  return p;
}
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// https://github.com/wrl/rutabaga/blob/c25215c0a3a502d7f5f3a6dfd5d069b55780f622/src/platform/win/window.c#L281-L390

//----------

const int ctx_attribs[] = {
  WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
  WGL_CONTEXT_MINOR_VERSION_ARB, 2,
  WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
  0
};

//----------

PIXELFORMATDESCRIPTOR pd = {
  sizeof(pd),
  1,
  PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
  PFD_TYPE_RGBA,
  32,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  24,
  8,
  0,
  PFD_MAIN_PLANE,
  0, 0, 0, 0
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// PFNWGLSWAPINTERVALEXTPROC swap_interval;


HGLRC init_gl_ctx(HDC hdc) {

  //--------------------
  //--------------------

  SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pd), &pd);

  HGLRC trampoline_ctx = wglCreateContext(hdc);
  if (!trampoline_ctx) {
    MIP_Print("wglCreateContext returned null\n");
    return NULL;
  }

  //--------------------
  //--------------------

  MIP_PRINT;
  wglMakeCurrent(hdc, trampoline_ctx);

  // check
  HGLRC current_gl_context = wglGetCurrentContext();
  if (!current_gl_context) {
    MIP_Print("ouch! no gl context\n");
  }

  //--------------------
  //--------------------

  MIP_PRINT;

  if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
    MIP_Print("error loading opengl functions\n");
    wglMakeCurrent(hdc, NULL);
    wglDeleteContext(trampoline_ctx);
    return NULL;
  }

//  if (!sogl_loadOpenGL()) {
//  //if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
//    MIP_Print("error loading opengl functions\n");
//    // print sogl errors:
//    MIP_Print("Error: sogl_loadOpenGL returns false.. couldn't load:\n");
//    MIP_Print("sogl_getFailures() : ");
//    const char** failures = sogl_getFailures();
//    while (*failures) {
//      MIP_DPrint("%s ",*failures);
//      failures++;
//    }
//    MIP_DPrint("\n");
//    wglMakeCurrent(hdc, NULL);
//    wglDeleteContext(trampoline_ctx);
//    return NULL;
//    //exit(1);
//  }

  //--------------------
  //--------------------

  int maj, min;
  glGetIntegerv(GL_MAJOR_VERSION, &maj);
  glGetIntegerv(GL_MINOR_VERSION, &min);
  MIP_Print("GL_MAJOR_VERSION: %i GL_MINOR_VERSION: %i\n",maj,min);   // GL_MAJOR_VERSION: 3 GL_MINOR_VERSION: 1

  //--------------------
  //--------------------

  MIP_Print("GL_VERSION: %s\n",   (char*)glGetString(GL_VERSION));    // 3.1 Mesa 21.2.6
  MIP_Print("GL_VENDOR: %s\n",    (char*)glGetString(GL_VENDOR));     // Mesa/X.org
  MIP_Print("GL_RENDERER: %s\n",  (char*)glGetString(GL_RENDERER));   // llvmpipe (LLVM 12.0.0, 256 bits)

  // a modern OpenGL driver will return a very long string for the
  // glGetString(GL_EXTENSIONS) query and if the application naively copies the
  // string into a fixed-size buffer it can overflow the buffer and crash the
  // application.

//  char* gl_ext_str = (char*)glGetString(GL_EXTENSIONS);
//  MIP_Print("GL_EXTENSIONS: %s\n",gl_ext_str);

  //--------------------
  //--------------------

  PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT;
  wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

  if (!wglGetExtensionsStringEXT) {
    MIP_Print("error getting wglGetExtensionsStringEXT\n");
    return NULL;
  }

  //  const char *extensions;
  //  extensions = get_extensions_string();
  //
  //  if (strstr(extensions, "WGL_ARB_create_context")) {
  //    MIP_Print("has WGL_ARB_create_context\n");
  //    //ext->create_context_attribs = 1;
  //  }
  //  if (strstr(extensions, "WGL_EXT_swap_control")) {
  //    MIP_Print("has WGL_EXT_swap_control\n");
  //    //ext->swap_control = 1;
  //    if (strstr(extensions, "WGL_EXT_swap_control_tear")) {
  //      MIP_Print("has WGL_EXT_swap_control_tear\n");
  //      //ext->swap_control_tear = 1;
  //    }
  //  }
  //  if (strstr(extensions, "NV")) {
  //    MIP_Print("appears to be nvidia\n");
  //    //ext->appears_to_be_nvidia = 1;
  //  }

  //--------------------
  //--------------------

  /* we want at least an openGL 3.2 context. if we've already got one,
  * bail out here. otherwise, we'll need to use
  * wglCreateContextAttribsARB. */

  //  MIP_Print("calling sogl_loadOpenGL\n");
  //  if (!sogl_loadOpenGL()) {
  //    MIP_Print("Error: sogl_loadOpenGL returns false.. couldn't load:\n");
  //    MIP_Print("");
  //    const char** failures = sogl_getFailures();
  //    while (*failures) {
  //      MIP_DPrint("%s ",*failures);
  //      failures++;
  //    }
  //    MIP_DPrint("\n");
  //    //exit(1);
  //    goto err_ctx_version;
  //  }

  //  glGetIntegerv(GL_MAJOR_VERSION, &maj);
  //  glGetIntegerv(GL_MINOR_VERSION, &min);
  //  MIP_Print("trampoline opengl version: v%i.%i\n",maj,min);

  //  glGetError();
  //  if (glGetError() == GL_INVALID_ENUM) {
  //    //messageboxf(title,
  //    //L"need an openGL version of 3.2 or higher, "
  //    //L"context only supports %s", glGetString(GL_VERSION));
  //    MIP_Print("need an openGL version of 3.2 or higher. context only supports %s\n",glGetString(GL_VERSION));
  //    goto err_ctx_version;
  //  }

  //  glGetIntegerv(GL_MAJOR_VERSION, &maj);
  //  glGetIntegerv(GL_MINOR_VERSION, &min);
  //  MIP_Print("v%i.%i\n",maj,min);

  //--------------------
  //--------------------

  //  if (!ext.create_context_attribs) {
  //    //messageboxf(title,
  //    //  L"wglCreateContextAttribsARB is unsupported – "
  //    //  L"can't create the necessary openGL context.",
  //    //  min, maj
  //    //);
  //    MIP_Print("wglCreateContextAttribsARB is unsupported – can't create the necessary openGL context. (maj:%i, min.%i)\n",maj,min);
  //    goto err_ctx_version;
  //  }

  //--------------------
  //--------------------

  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
  wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
  if (!wglCreateContextAttribsARB) {
    MIP_Print("error getting wglCreateContextAttribsARB\n");
    return NULL;
  }

  HGLRC gl_ctx = wglCreateContextAttribsARB(hdc, NULL, ctx_attribs);
  if (!gl_ctx) {
    MIP_Print("error creating context\n");
    wglMakeCurrent(hdc, NULL);
    wglDeleteContext(trampoline_ctx);
    return NULL;
  }

  //--------------------
  //--------------------

  wglMakeCurrent(hdc,NULL);
  wglDeleteContext(trampoline_ctx);
  wglMakeCurrent(hdc, gl_ctx);

  //--------------------
  //--------------------

  // swap interval
  //
  //  if (ext.swap_control) {
  //    swap_interval = (PFNWGLSWAPINTERVALEXTPROC)
  //    wglGetProcAddress("wglSwapIntervalEXT");
  //    /* vsync + wgl + nvidia == stupid high CPU usage */
  //    if (ext.appears_to_be_nvidia) swap_interval(0);
  //    else swap_interval(1);
  //  }

  //--------------------
  //--------------------

  return gl_ctx;

}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//typedef void (*glXSwapIntervalEXT_t)(Display *dpy, GLXDrawable drawable, int interval);
//glXSwapIntervalEXT_t glXSwapIntervalEXT = nullptr;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// Helper to check for extension string presence. Adapted from:
// http://www.opengl.org/resources/features/OGLextensions/

bool MIP_GLExtensionSupported(const char *extList, const char *extension) {
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

// TODO:
//   - bool argument -> setVSync(bool AState)
//   - add the other variants..

//void MIP_GLXDisableVSync(Display* ADisplay, GLXDrawable ADrawable) {
//  const char* glXExtensions = glXQueryExtensionsString(ADisplay,DefaultScreen(ADisplay));
//  if (strstr(glXExtensions,"GLX_EXT_swap_control") != nullptr) {
//    //MIP_PRINT;
//    glXSwapIntervalEXT = (glXSwapIntervalEXT_t)glXGetProcAddress((GLubyte *)"glXSwapIntervalEXT");
//    glXSwapIntervalEXT(ADisplay,ADrawable,0);
//  }
//}

#if 0
  } else if (strstr(glXExtensions, "GLX_MESA_swap_control") != nullptr) {
    glXSwapIntervalMESA = reinterpret_cast<PFNGLXSWAPINTERVALMESAPROC>(glXGetProcAddress((GLubyte *)"glXSwapIntervalMESA"));
  } else {
    #ifdef DISPLAY_GLX_INFO
    puts("VSync not supported");
    #endif
    return;
  }
  if (glXSwapIntervalEXT) {
    if (strstr(glXExtensions, "GLX_EXT_swap_control_tear") != nullptr) {
      #ifdef DISPLAY_GLX_INFO
      puts("Enabling ADAPTIVE VSync");
      #endif
      glXSwapIntervalEXT(dpy, xWin, -1);
    } else {
      #ifdef DISPLAY_GLX_INFO
      puts("Enabling VSync");
      #endif
      glXSwapIntervalEXT(dpy, xWin, 1);
    }
  } else if (glXSwapIntervalMESA) {
      #ifdef DISPLAY_GLX_INFO
      puts("Enabling VSync");
      #endif
      glXSwapIntervalMESA(1);
    } else {
      #ifdef DISPLAY_GLX_INFO
      puts("Failed to load swap control function");
      #endif
    }
  }
#endif // 0

//----------------------------------------------------------------------

#define MIP_GL_ERROR_CHECK {    \
  GLint err = glGetError();     \
  if (err != GL_NO_ERROR) {     \
    MIP_Print("");              \
    MIP_PrintGLError(err);      \
  }                             \
}

//#define MIP_OPENGL_ERROR_CHECK { MIP_Print("gl error: %i\n",glGetError()); }

void MIP_PrintGLError(GLint err) {
//  switch (err) {
//    case GL_NO_ERROR:                       MIP_DPrint("OpenGL Error: No error has been recorded. The value of this symbolic constant is guaranteed to be 0.\n"); break;
//    case GL_INVALID_ENUM:                   MIP_DPrint("OpenGL Error: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
//    case GL_INVALID_VALUE:                  MIP_DPrint("OpenGL Error: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
//    case GL_INVALID_OPERATION:              MIP_DPrint("OpenGL Error: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
//    case GL_INVALID_FRAMEBUFFER_OPERATION:  MIP_DPrint("OpenGL Error: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
//    case GL_OUT_OF_MEMORY:                  MIP_DPrint("OpenGL Error: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n"); break;
//    case GL_STACK_UNDERFLOW:                MIP_DPrint("OpenGL Error: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n"); break;
//    case GL_STACK_OVERFLOW:                 MIP_DPrint("OpenGL Error: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n"); break;
//    default:                                MIP_DPrint("OpenGL Error: Unknown error %i\n",err); break;
//  }
}

//void MIP_PrintWGLError(GLint err) {
//  MIP_PRINT;
//}

//----------------------------------------------------------------------

#if 0

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

      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_FBCONFIG_ID             , &fbc_fbconfig_id );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_BUFFER_SIZE             , &fbc_buffer_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_LEVEL                   , &fbc_level );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_DOUBLEBUFFER            , &fbc_doublebuffer );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_STEREO                  , &fbc_stereo );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_AUX_BUFFERS             , &fbc_aux_buffers );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_RED_SIZE                , &fbc_red_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_GREEN_SIZE              , &fbc_green_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_BLUE_SIZE               , &fbc_blue_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ALPHA_SIZE              , &fbc_alpha_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_DEPTH_SIZE              , &fbc_depth_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_STENCIL_SIZE            , &fbc_stencil_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_RED_SIZE          , &fbc_accum_red_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_GREEN_SIZE        , &fbc_accum_green_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_BLUE_SIZE         , &fbc_accum_blue_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_ALPHA_SIZE        , &fbc_accum_alpha_size );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_SAMPLE_BUFFERS          , &fbc_sample_buffers );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_SAMPLES                 , &fbc_samples );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_RENDER_TYPE             , &fbc_render_type );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_DRAWABLE_TYPE           , &fbc_drawable_type );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_X_RENDERABLE            , &fbc_x_renderable );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_X_VISUAL_TYPE           , &fbc_x_visual_type );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_CONFIG_CAVEAT           , &fbc_config_caveat );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_TYPE        , &fbc_transparent_type );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_INDEX_VALUE , &fbc_transparent_index_value );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_RED_VALUE   , &fbc_transparent_red_value );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_GREEN_VALUE , &fbc_transparent_green_value );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_BLUE_VALUE  , &fbc_transparent_blue_value );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_ALPHA_VALUE , &fbc_transparent_alpha_value );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_MAX_PBUFFER_WIDTH       , &fbc_max_pbuffer_width );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_MAX_PBUFFER_HEIGHT      , &fbc_max_pbuffer_height );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_MAX_PBUFFER_PIXELS      , &fbc_max_pbuffer_pixels );
      glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_VISUAL_ID               , &fbc_visual_id );

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

#endif // 0

//----------------------------------------------------------------------
#endif

