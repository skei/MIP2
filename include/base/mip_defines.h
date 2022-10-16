#ifndef mip_defines_included
#define mip_defines_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef __gnu_linux__
  #define MIP_LINUX
#endif

#if defined _WIN32 || defined __CYGWIN__
//#ifdef _WIN32
  #define  MIP_WIN32
#endif

//----------

#if defined  (__i386__) || defined (_X86_) || defined (__X86__)
  #define MIP_32BIT
#endif

#if defined (__x86_64) || (__LP64__)
  #define MIP_64BIT
#endif

//----------

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define MIP_BIG_ENDIAN
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define MIP_LITTLE_ENDIAN
#endif

//----------

#if (__cplusplus >= 201103L)
  #define MIP_CPP11
#endif

//----------------------------------------------------------------------
//
// debug
//
//----------------------------------------------------------------------

// see /base/mip_debug.h

//----------------------------------------------------------------------
//
// gui
//
//----------------------------------------------------------------------

// use     = include code for..
// gui     = window/surface
// painter = paint

//

#undef MIP_GUI_DEFINED
#undef MIP_PAINTER_DEFINED

//----------

#ifdef MIP_NO_GUI
  #undef MIP_USE_CAIRO
  #undef MIP_USE_GDI
  #undef MIP_USE_GLX
  #undef MIP_USE_NANOVG
  #undef MIP_USE_WIN32
  #undef MIP_USE_WGL
  #undef MIP_USE_XCB
  #undef MIP_GUI_WIN32
  #undef MIP_GUI_XCB
  #define MIP_GUI_DEFINED
  #define MIP_NO_PAINTER
#endif

//-----

#ifdef MIP_GUI_XCB
  #define MIP_USE_XCB
  #define MIP_GUI_DEFINED
#endif

#ifdef MIP_GUI_WIN32
  #define MIP_USE_WIN32
  #define MIP_GUI_DEFINED
#endif

//-----

#ifdef MIP_NO_PAINTER
  #define MIP_PAINTER_DEFINED
#endif


#ifdef MIP_PAINTER_CAIRO
  //#define MIP_USE_XCB
  #define MIP_USE_CAIRO
  #define MIP_PAINTER_DEFINED
#endif

#ifdef MIP_PAINTER_GDI
  #define MIP_USE_WIN32
  #define MIP_USE_GDI
  #define MIP_PAINTER_DEFINED
#endif

#ifdef MIP_PAINTER_GLX
  //#define MIP_USE_XCB
  #define MIP_USE_GLX
  #define MIP_PAINTER_DEFINED
#endif

#ifdef MIP_PAINTER_NANOVG
  #ifdef MIP_LINUX
    //#define MIP_USE_XCB
    #define MIP_USE_GLX
  #endif
  #ifdef MIP_WIN32
    #define MIP_USE_WIN32
    #define MIP_USE_GDI
    #define MIP_USE_WGL
  #endif
  #define MIP_USE_NANOVG
  #define MIP_PAINTER_DEFINED
#endif

#ifdef MIP_PAINTER_WGL
  #define MIP_USE_WIN32
  #define MIP_USE_GDI
  #define MIP_USE_WGL
  #define MIP_PAINTER_DEFINED
#endif

#ifdef MIP_PAINTER_XCB
  #define MIP_USE_XCB
  #define MIP_PAINTER_DEFINED
#endif


//----------

#ifndef MIP_GUI_DEFINED
  #ifndef MIP_NO_WARNINGS
    #warning No GUI defined.
  #endif
  #define MIP_NO_GUI
#endif

#ifndef MIP_PAINTER_DEFINED
  #ifndef MIP_NO_WARNINGS
    #warning No PAINTER defined.
  #endif
  #define MIP_NO_GUI
  #define MIP_NO_PAINTER
#endif

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
#endif
