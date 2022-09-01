#ifndef mip_win32_surface_included
#define mip_win32_surface_included
//----------------------------------------------------------------------

/*
  we need the hinstance to register our window class ..
  .. do we only need this if we have a gui/interface?
*/

//----------------------------------------------------------------------

//
// windows.h
//

#undef  _WIN32_WINNT
#define _WIN32_WINNT 0x0501

/*
  exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets..
  If using gdiplus you cannot define WIN32_LEAN_AND_MEAN. Gdiplus needs some
  type definitions that are excluded by this macro.
*/

//#ifndef MIP_WIN32_GDIPLUS
#define  WIN32_LEAN_AND_MEAN
//#endif

#include <windows.h>

//----------

//
// gdi
//

// see also:
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd145130%28v=vs.85%29.aspx

// missing raster ops
#define DSTCOPY   0x00AA0029
#define DSTERASE  0x00220326 // dest = dest & (~ src) :  DSna
//#define FORE_ROP3(ROP4) (0x00FFFFFF & (ROP4))
//#define BACK_ROP3(ROP4) (ROP3FromIndex (SwapROP3_SrcDst (BYTE ((ROP4) >> 24))))

//----------

#include "mip.h"

//----------------------------------------------------------------------
//
// DllMain
//
//----------------------------------------------------------------------

/*
  if multiple DLLs all register a window class with the same name,
  the instance handle is used to tell them apart.
*/

//----------

//___MIP_UNUSED static
HINSTANCE MIP_GLOBAL_WIN32_INSTANCE = 0;

/*
  http://msdn.microsoft.com/en-us/library/ms682583%28v=vs.85%29.aspx
  http://stackoverflow.com/questions/4496233/which-is-called-first-dllmain-or-global-static-object-constructor
  Which is called first, DllMain() or global static object constructor?
  MSDN's DllMain documentation says:
  If your DLL is linked with the C run-time library (CRT), the entry point
  provided by the CRT calls the constructors and destructors for global and
  static C++ objects. Therefore, these restrictions for DllMain also apply to
  constructors and destructors and any code that is called from them.
  Since the code within DllMain may use the static objects, the static objects
  must be constructed before DllMain is run for DLL_PROCESS_ATTACH, and
  destroyed after it is run for DLL_PROCESS_DETACH.
*/

//----------

#ifdef MIP_PLUGIN

  __MIP_EXTERNC
  BOOL APIENTRY
  DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved) {
    switch(reason) {

      case DLL_PROCESS_ATTACH:
        MIP_Print("*** MIP_GLOBAL_WIN32_INSTANCE = hModule;\n");
        MIP_GLOBAL_WIN32_INSTANCE = hModule;
        //MIP_RegisterWindowClass(&s3_global_WinInstance);
        break;

      //case DLL_PROCESS_DETACH:
      //  //MIP_UnregisterWindowClass();
      //  break;

      //case DLL_THREAD_ATTACH:
      //  break;
      //case DLL_THREAD_DETACH:
      //  break;

      //default:
      //  break;

    }
    return TRUE;
  }

#endif // MIP_PLUGIN

//----------------------------------------------------------------------
#endif
