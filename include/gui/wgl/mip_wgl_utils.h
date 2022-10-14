#ifndef mip_wgl_utils_included
#define mip_wgl_utils_included
//----------------------------------------------------------------------

/*
  https://community.khronos.org/t/cant-get-wglgetprocaddress-to-work-in-opengl-3-1/58281

  If you want to retrieve the function pointers for core 1.1 functionality,
  wglGetProcAddress only returns 0. But for these functions GetProcAddress()
  works. Unfortunately GetProcAddress does not work for non-core-1.1-functions
*/

/*
  typedef void (APIENTRY *PFNGLBEGINPROC)(GLenum);
  PFNGLBEGINPROC glBegin;
  HMODULE gl_module = LoadLibrary(TEXT("opengl32.dll"));
  glBegin = (PFNGLBEGINPROC)GetProcAddress("glBegin");
  FreeLibrary(gl_module);
*/

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

// PFNWGLSWAPINTERVALEXTPROC swap_interval;
//typedef void (*glXSwapIntervalEXT_t)(Display *dpy, GLXDrawable drawable, int interval);
//glXSwapIntervalEXT_t glXSwapIntervalEXT = nullptr;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// https://github.com/wrl/rutabaga/blob/c25215c0a3a502d7f5f3a6dfd5d069b55780f622/src/platform/win/window.c#L281-L390

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  /*
    PFD_DRAW_TO_WINDOW
    PFD_DRAW_TO_BITMAP
    PFD_SUPPORT_OPENGL
    PFD_GENERIC_ACCELERATED
    PFD_DOUBLEBUFFER
    PFD_SWAP_LAYER_BUFFERS
  */

HGLRC MIP_WglInitContext(HDC hdc) {

  PIXELFORMATDESCRIPTOR pd = {
    sizeof(pd),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,               // bits
    0, 0, 0, 0, 0, 0,
    8,//0,            // alpha
    0,                // shift
    0,                // accumulation
    0, 0, 0, 0,
    24,               // depth
    8,//0,            // stencil
    0,                // aux
    PFD_MAIN_PLANE,
    0, 0, 0, 0
  };


  int pixel_format = ChoosePixelFormat(hdc, &pd);
  SetPixelFormat(hdc, pixel_format, &pd);

  //--------------------

  HGLRC temp_ctx = wglCreateContext(hdc);
  if (!temp_ctx) {
    MIP_Print("error! couldn't create temp context\n");
    return NULL;
  }
  else {
    MIP_Print("created temp context\n");
  }

  MIP_Print("making temp context current\n");
  wglMakeCurrent(hdc, temp_ctx);

  // not available until we have loaded the opengl functions
  //
  //MIP_Print("GL_VERSION: %s\n",     (char*)glGetString(GL_VERSION));    // 3.1 Mesa 21.2.6
  //MIP_Print("GL_VENDOR: %s\n",      (char*)glGetString(GL_VENDOR));     // Mesa/X.org
  //int maj, min;
  //glGetIntegerv(GL_MAJOR_VERSION, &maj);
  //glGetIntegerv(GL_MINOR_VERSION, &min);
  //MIP_Print("GL_MAJOR_VERSION: %i GL_MINOR_VERSION: %i\n",maj,min);   // GL_MAJOR_VERSION: 3 GL_MINOR_VERSION: 1

  //----------

  // wglGetExtensionsStringEXT

  PFNWGLGETEXTENSIONSSTRINGEXTPROC wgl_GetExtensionsStringEXT;
  wgl_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

  /*
    HMODULE gl_module = LoadLibrary(TEXT("opengl32.dll"));
    wgl_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)GetProcAddress(gl_module,"wglGetExtensionsStringEXT");
    FreeLibrary(gl_module);
  */

  if (!wgl_GetExtensionsStringEXT) {
    MIP_Print("error! wglGetExtensionsStringEXT not found\n");
    //return NULL;
  }
  else {
    MIP_Print("wglGetExtensionsStringEXT found\n");
    const char* extensions = wgl_GetExtensionsStringEXT();
    if (strstr(extensions, "WGL_ARB_create_context")) {
      //ext->create_context_attribs = 1;
    }
    if (strstr(extensions, "WGL_EXT_swap_control")) {
      //ext->swap_control = 1;
      if (strstr(extensions, "WGL_EXT_swap_control_tear")) {
        //ext->swap_control_tear = 1;
      }
    }
    if (strstr(extensions, "NV")) {
      //ext->appears_to_be_nvidia = 1;
    }
  }

  //MIP_Assert( wgl_GetExtensionsStringEXT );

  //--------------------

  // a) gl_core.3.2

  /*
  if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
    MIP_Print("error loading opengl functions\n");
    wglMakeCurrent(hdc, NULL);
    wglDeleteContext(temp_ctx);
    return NULL;
  }
  else {
    MIP_Print("loaded opengl functions (ogl_LoadFunctions)\n");
  }
  //MIP_Print("(major %i minor %i)\n",ogl_GetMajorVersion(),ogl_GetMinorVersion());
  */

  // b) simple-opengl-loader

  if (!sogl_loadOpenGL()) {
  //if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
    // print sogl errors:
    MIP_Print("error! sogl_loadOpenGL failures: \n");
    const char** failures = sogl_getFailures();
    while (*failures) {
      MIP_DPrint("%s ",*failures);
      failures++;
    }
    MIP_DPrint("\n");
    wglMakeCurrent(hdc, NULL);
    wglDeleteContext(temp_ctx);
    return NULL;
    //exit(1);
  }
  else {
    MIP_Print("loaded opengl functions (sogl_loadOpenGL)\n");
  }

  //--------------------

  int maj, min;
  glGetIntegerv(GL_MAJOR_VERSION, &maj);
  glGetIntegerv(GL_MINOR_VERSION, &min);
  MIP_Print("GL_MAJOR_VERSION: %i GL_MINOR_VERSION: %i\n",maj,min);   // GL_MAJOR_VERSION: 3 GL_MINOR_VERSION: 1

  MIP_Print("GL_VERSION: %s\n",     (char*)glGetString(GL_VERSION));    // 3.1 Mesa 21.2.6
  MIP_Print("GL_VENDOR: %s\n",      (char*)glGetString(GL_VENDOR));     // Mesa/X.org
  MIP_Print("GL_RENDERER: %s\n",    (char*)glGetString(GL_RENDERER));   // llvmpipe (LLVM 12.0.0, 256 bits)
  //MIP_Print("GL_EXTENSIONS: %s\n",  (char*)glGetString(GL_EXTENSIONS)); // crashes!

  //--------------------

  // wglCreateContextAttribsARB

  PFNWGLCREATECONTEXTATTRIBSARBPROC wgl_CreateContextAttribsARB;
  wgl_CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
  if (!wgl_CreateContextAttribsARB) {
    MIP_Print("error! wglCreateContextAttribsARB not found\n");
    //return NULL;
  }
  else {
    MIP_Print("wglCreateContextAttribsARBfound\n");
  }

  //--------------------

  MIP_Assert( wgl_CreateContextAttribsARB );

  const int ctx_attribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 2,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };

  HGLRC gl_ctx = wgl_CreateContextAttribsARB(hdc, NULL, ctx_attribs);
  if (!gl_ctx) {
    MIP_Print("error creating context\n");
    wglMakeCurrent(hdc, NULL);
    wglDeleteContext(temp_ctx);
    return NULL;
  }
  else {
    MIP_Print("context created\n");
  }

  //--------------------

  wglMakeCurrent(hdc,NULL);
  wglDeleteContext(temp_ctx);

  wglMakeCurrent(hdc, gl_ctx);
  return gl_ctx;

}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  /*

  const char *extensions;
  extensions = get_extensions_string();

  if (strstr(extensions, "WGL_ARB_create_context")) {
    MIP_Print("has WGL_ARB_create_context\n");
    //ext->create_context_attribs = 1;
  }
  if (strstr(extensions, "WGL_EXT_swap_control")) {
    MIP_Print("has WGL_EXT_swap_control\n");
    //ext->swap_control = 1;
    if (strstr(extensions, "WGL_EXT_swap_control_tear")) {
      MIP_Print("has WGL_EXT_swap_control_tear\n");
      //ext->swap_control_tear = 1;
    }
  }
  if (strstr(extensions, "NV")) {
    MIP_Print("appears to be nvidia\n");
    //ext->appears_to_be_nvidia = 1;
  }

  */

  //--------------------
  //--------------------

  /* we want at least an openGL 3.2 context. if we've already got one,
  * bail out here. otherwise, we'll need to use
  * wglCreateContextAttribsARB. */

  /*
  MIP_Print("calling sogl_loadOpenGL\n");
  if (!sogl_loadOpenGL()) {
    MIP_Print("Error: sogl_loadOpenGL returns false.. couldn't load:\n");
    MIP_Print("");
    const char** failures = sogl_getFailures();
    while (*failures) {
      MIP_DPrint("%s ",*failures);
      failures++;
    }
    MIP_DPrint("\n");
    //exit(1);
    goto err_ctx_version;
  }
  */

  //--------------------
  //--------------------

  //glGetIntegerv(GL_MAJOR_VERSION, &maj);
  //glGetIntegerv(GL_MINOR_VERSION, &min);
  //MIP_Print("trampoline opengl version: v%i.%i\n",maj,min);

  //glGetError();
  //if (glGetError() == GL_INVALID_ENUM) {
  //  //messageboxf(title,
  //  //L"need an openGL version of 3.2 or higher, "
  //  //L"context only supports %s", glGetString(GL_VERSION));
  //  MIP_Print("need an openGL version of 3.2 or higher. context only supports %s\n",glGetString(GL_VERSION));
  //  goto err_ctx_version;
  //}

  //glGetIntegerv(GL_MAJOR_VERSION, &maj);
  //glGetIntegerv(GL_MINOR_VERSION, &min);
  //MIP_Print("v%i.%i\n",maj,min);

  //--------------------
  //--------------------

  //if (!ext.create_context_attribs) {
  //  //messageboxf(title,
  //  //  L"wglCreateContextAttribsARB is unsupported – "
  //  //  L"can't create the necessary openGL context.",
  //  //  min, maj
  //  //);
  //  MIP_Print("wglCreateContextAttribsARB is unsupported – can't create the necessary openGL context. (maj:%i, min.%i)\n",maj,min);
  //  goto err_ctx_version;
  //}

  //--------------------
  //--------------------

  // swap interval
  //
  //if (ext.swap_control) {
  //  swap_interval = (PFNWGLSWAPINTERVALEXTPROC)
  //  wglGetProcAddress("wglSwapIntervalEXT");
  //  /* vsync + wgl + nvidia == stupid high CPU usage */
  //  if (ext.appears_to_be_nvidia) swap_interval(0);
  //  else swap_interval(1);
  //}

  //--------------------
  //--------------------

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// Helper to check for extension string presence. Adapted from:
// http://www.opengl.org/resources/features/OGLextensions/

/*
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
*/

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

/*
void MIP_GLXDisableVSync(Display* ADisplay, GLXDrawable ADrawable) {
  const char* glXExtensions = glXQueryExtensionsString(ADisplay,DefaultScreen(ADisplay));
  if (strstr(glXExtensions,"GLX_EXT_swap_control") != nullptr) {
    //MIP_PRINT;
    glXSwapIntervalEXT = (glXSwapIntervalEXT_t)glXGetProcAddress((GLubyte *)"glXSwapIntervalEXT");
    glXSwapIntervalEXT(ADisplay,ADrawable,0);
  }
}
*/

//----------

  /*
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
  */

//----------------------------------------------------------------------
#endif

