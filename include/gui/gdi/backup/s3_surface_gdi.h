#ifndef s3_surface_gdi_included
#define s3_surface_gdi_included
//----------------------------------------------------------------------

#include "gui/base/s3_surface_base.h"
#include "gui/s3_drawable.h"

//----------------------------------------------------------------------

class S3_Surface_Gdi
: public S3_Surface_Base
, public S3_Drawable {

  private:
    HBITMAP MHandle;
  private:
    int32   MWidth;
    int32   MHeight;
    int32   MDepth;

  //------------------------------
  //
  //------------------------------

  public:

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

    S3_Surface_Gdi(int32 AWidth, int32 AHeight)
    : S3_Surface_Base(AWidth,AHeight) {
      HDC tempdc = GetDC(S3_NULL); // dc for the entire screen
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = GetDeviceCaps(tempdc,BITSPIXEL) * GetDeviceCaps(tempdc,PLANES);
      MHandle = CreateCompatibleBitmap(tempdc,MWidth,MHeight);
      ReleaseDC(0,tempdc);
    }

    //----------

    /*
      ADepth
        1   monochrome
        8   greyscale
        16
        24
        32  rgba
    */

    S3_Surface_Gdi(int32 AWidth, int32 AHeight, int32 ADepth)
    : S3_Surface_Base(AWidth,AHeight,ADepth) {
      HDC tempdc = GetDC(S3_NULL); // dc for the entire screen
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = ADepth;
      //MHandle = CreateCompatibleBitmap(tempdc,MWidth,MHeight);
      MHandle = CreateBitmap(
        MWidth,   // nWidth,
        MHeight,  // nHeight,
        1,        // cPlanes,
        MDepth,   // cBitsPerPel,
        S3_NULL   // VOID* lpvBits
      );
      ReleaseDC(0,tempdc);
    }

    //----------

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

    virtual ~S3_Surface_Gdi() {
      DeleteObject(MHandle);
    }

  //------------------------------
  // S3_Drawable
  //------------------------------

  public:

    virtual int32   width(void)   { return MWidth; }
    virtual int32   height(void)  { return MHeight; }
    virtual int32   depth(void)   { return MDepth; }
    //virtual void*   buffer(void)    { return S3_NULL; }
    virtual HBITMAP hbitmap(void) { return MHandle; }

  //------------------------------
  //
  //------------------------------

  public:


};

//----------------------------------------------------------------------
#endif
