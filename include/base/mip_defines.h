#ifndef mip_defines_included
#define mip_defines_included
//----------------------------------------------------------------------

//#include "mip.h"

//----------------------------------------------------------------------
//
// os
//
//----------------------------------------------------------------------

#ifdef __gnu_linux__
  #define MIP_LINUX
#endif

//----------

#ifdef _WIN32
  #define  MIP_WIN32
#endif

//----------------------------------------------------------------------
//
// bits
//
//----------------------------------------------------------------------

#if defined  (__i386__) || defined (_X86_) || defined (__X86__)
  #define MIP_32BIT
#endif

//----------

#if defined (__x86_64) || (__LP64__)
  #define MIP_64BIT
#endif

//----------------------------------------------------------------------
//
// endianness
//
//----------------------------------------------------------------------

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define MIP_BIG_ENDIAN
#endif

//----------

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define MIP_LITTLE_ENDIAN
#endif

//----------------------------------------------------------------------
//
// c++11
//
//----------------------------------------------------------------------

#if (__cplusplus >= 201103L)
  #define MIP_CPP11
#endif

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

//#ifdef MIP_PLUGIN_ALL
//  #define MIP_PLUGIN_EXE
//  #define MIP_PLUGIN_CLAP
//  //#define MIP_PLUGIN_VST2
//  //#define MIP_PLUGIN_VST3
//  //#define MIP_PLUGIN_LV2
//#endif
//
//#ifdef MIP_EXE
//  #undef MIP_PLUGIN_CLAP
//  #undef MIP_PLUGIN_LV2
//  #undef MIP_PLUGIN_VST2
//  #undef MIP_PLUGIN_VST3
//#endif
//
//#ifdef MIP_LIB
//  #undef MIP_PLUGIN_EXE
//#endif

//----------

/*
#undef MIP_PLUGIN_DEFINED

  #ifdef MIP_NO_PLUGIN
    #define MIP_PLUGIN_DEFINED
  #endif

  #ifdef MIP_PLUGIN_CLAP
    #define MIP_PLUGIN_DEFINED
    //#ifndef MIP_LIB
    //  #define MIP_LIB
    //#endif
  #endif

  #ifdef MIP_PLUGIN_EXE
    #define MIP_PLUGIN_DEFINED
    //#ifndef MIP_EXE
    //  #define MIP_EXE
    //#endif
  #endif

  #ifdef MIP_PLUGIN_LV2
    #define MIP_PLUGIN_DEFINED
    //#ifndef MIP_LIB
    //  #define MIP_LIB
    //#endif
  #endif

  #ifdef MIP_PLUGIN_VST2
    #define MIP_PLUGIN_DEFINED
    //#ifndef MIP_LIB
    //  #define MIP_LIB
    //#endif
  #endif

  #ifdef MIP_PLUGIN_VST3
    #define MIP_PLUGIN_DEFINED
    //#ifndef MIP_LIB
    //  #define MIP_LIB
    //#endif
  #endif

#ifndef MIP_PLUGIN_DEFINED
  #warning No plugin format defined! (#define MIP_PLUGIN_***)
#endif
*/

//----------------------------------------------------------------------
//
// gui
//
//----------------------------------------------------------------------

#undef MIP_GUI_DEFINED

  #ifdef MIP_NO_GUI
    #define MIP_GUI_DEFINED
  #endif

  #ifdef MIP_GUI_XCB
    #define MIP_GUI_DEFINED
    #ifndef MIP_USE_XCB
      #define MIP_USE_XCB
    #endif
  #endif

  //#ifdef MIP_GUI_CAIRO
  //  #define MIP_GUI_DEFINED
  //  #undef  MIP_GUI_XCB
  //  #ifndef MIP_USE_XCB
  //    #define MIP_USE_XCB
  //  #endif
  //  #ifndef MIP_USE_CAIRO
  //    #define MIP_USE_CAIRO
  //  #endif
  //#endif

#ifndef MIP_GUI_DEFINED
  #warning No gui type defined! (#define MIP_GUI_***)
#endif

//----------------------------------------------------------------------
//
// painter
//
//----------------------------------------------------------------------


#undef MIP_PAINTER_DEFINED

  #ifdef MIP_NO_PAINTER
    #define MIP_PAINTER_DEFINED
  #endif

  #ifdef MIP_PAINTER_XCB
    #define MIP_PAINTER_DEFINED
    #ifndef MIP_USE_XCB
      #define MIP_USE_XCB
    #endif
  #endif

  #ifdef MIP_PAINTER_CAIRO
    #define MIP_PAINTER_DEFINED
    //#ifndef MIP_USE_XCB
    //  #define MIP_USE_XCB
    //#endif
    #ifndef MIP_USE_CAIRO
      #define MIP_USE_CAIRO
    #endif
  #endif

#ifndef MIP_PAINTER_DEFINED
  #warning No painter type defined! (#define MIP_PAINTER_***)
#endif

//----------------------------------------------------------------------
#endif
