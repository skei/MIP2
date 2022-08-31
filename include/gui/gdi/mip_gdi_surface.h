#ifndef mip_xcb_surface_included
#define mip_xcb_surface_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"
#include "gui/win32/mip_win32.h"
#include "gui/gdi/mip_gdi.h"

//#include "gui/base/s3_surface_base.h"
//#include "gui/s3_drawable.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_GdiSurface
: public MIP_Drawable {

  private:
    HBITMAP MHandle;

  private:
    int32_t   MWidth;
    int32_t   MHeight;
    int32_t   MDepth;

//------------------------------
public:
//------------------------------

    /*
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd183488%28v=vs.85%29.aspx
      CreateCompatibleBitmap
      This bitmap can be selected into any memory device context that is
      compatible with the original device.
      Note: When a memory device context is created, it initially has a 1-by-1
      monochrome bitmap selected into it. If this memory device context is used
      in CreateCompatibleBitmap, the bitmap that is created is a monochrome
      bitmap. To create a color bitmap, use the HDC that was used to create the
      memory device context, as shown in the following code:
        HDC memDC = CreateCompatibleDC ( hDC );
        HBITMAP memBM = CreateCompatibleBitmap ( hDC, nWidth, nHeight );
        SelectObject ( memDC, memBM );
    */

    /*
      ADepth
        1   monochrome
        8   greyscale
        16
        24
        32  rgba
    */

  MIP_GdiSurface(MIP_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0) {
    if (ADepth == 0) {
      HDC tempdc = GetDC(nullptr); // dc for the entire screen
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = GetDeviceCaps(tempdc,BITSPIXEL) * GetDeviceCaps(tempdc,PLANES);
      MHandle = CreateCompatibleBitmap(tempdc,MWidth,MHeight);
      ReleaseDC(0,tempdc);
    }
    else {
      HDC tempdc = GetDC(nullptr); // dc for the entire screen
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = ADepth;
      //MHandle = CreateCompatibleBitmap(tempdc,MWidth,MHeight);
      MHandle = CreateBitmap(
        MWidth,   // nWidth,
        MHeight,  // nHeight,
        1,        // cPlanes,
        MDepth,   // cBitsPerPel,
        nullptr   // VOID* lpvBits
      );
      ReleaseDC(0,tempdc);
    }
  }

  /*
    see also:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd183485%28v=vs.85%29.aspx
    MHandle = CreateBitmap(
      MWidth, MHeight,
      1,      // planes
      32,     // bits per pixel
      S3_NULL // data
    );
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd183486%28v=vs.85%29.aspx
    MHandle = CreateBitmapIndirect( ABitmap->hbitmap() );
  */

  //----------

  virtual ~MIP_GdiSurface() {
  }

//------------------------------
public: // drawable
//------------------------------

//  int32   width(void)   override { return MWidth; }
//  int32   height(void)  override { return MHeight; }
//  int32   depth(void)   override { return MDepth; }
//  //void*   buffer(void)  override   { return S3_NULL; }
//  HBITMAP hbitmap(void) override { return MHandle; }

};

//----------------------------------------------------------------------
#endif
