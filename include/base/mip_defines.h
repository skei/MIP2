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

#ifdef _WIN32
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

#undef MIP_GUI_DEFINED
#undef MIP_PAINTER_DEFINED

//----------

#ifdef MIP_NO_GUI
  #undef MIP_GUI_XCB
  #undef MIP_USE_XCB
  #undef MIP_USE_CAIRO
  #undef MIP_USE_GLX
  #undef MIP_USE_NANOVG
  #define MIP_GUI_DEFINED
  #define MIP_NO_PAINTER
#endif

#ifdef MIP_GUI_XCB
  #define MIP_USE_XCB
  #define MIP_GUI_DEFINED
#endif

//#ifdef MIP_GUI_WIN32
//#endif

//

#ifdef MIP_NO_PAINTER
  #define MIP_PAINTER_DEFINED
#endif


#ifdef MIP_PAINTER_XCB
  #define MIP_USE_XCB
  #define MIP_PAINTER_DEFINED
#endif

#ifdef MIP_PAINTER_CAIRO
  //#define MIP_USE_XCB
  #define MIP_USE_CAIRO
  #define MIP_PAINTER_DEFINED
#endif

#ifdef MIP_PAINTER_GLX
  #define MIP_USE_GLX
  #define MIP_PAINTER_DEFINED
#endif

#ifdef MIP_PAINTER_NANOVG
  #define MIP_USE_GLX
  #define MIP_USE_NANOVG
  #define MIP_PAINTER_DEFINED
#endif

//#ifdef MIP_PAINTER_WIN32
//#endif

//----------

#ifndef MIP_GUI_DEFINED
  #warning No Gui defined (MIP_GUI_*)
  #define MIP_NO_GUI
#endif

#ifndef MIP_PAINTER_DEFINED
  #warning No Painter defined (MIP_PAINTER_*)
  #define MIP_NO_GUI
#endif

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
#endif
