#ifndef s3_image_gdi_included
#define s3_image_gdi_included
//----------------------------------------------------------------------

// HBITMAP = CreateDIBSection
// DIB : Device Independent Bitmap (ram/client)

/*
  COLORREF = 0xaabbggrr = rgba (little endian)
  RGBA macro
*/

//----------------------------------------------------------------------

#include "common/s3_memory_stdlib.h"
#include "gui/base/s3_image_base.h"
#include "gfx/s3_bitmap.h"
#include "gui/s3_drawable.h"

//class S3_Image_Win32;
//typedef S3_Image_Win32 S3_Image_Implementation;

//----------

class S3_Image_Gdi
: public S3_Image_Base
, public S3_Drawable {

  private:
    HBITMAP     MHandle;
    //S3_Bitmap*  MBitmap;
  private:
    int32     MWidth;
    int32     MHeight;
    int32     MDepth;
    //int32     MStride;
    void*     MBuffer;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Image_Gdi(int32 AWidth, int32 AHeight, int32 ADepth=32)
    : S3_Image_Base(AWidth,AHeight,ADepth) {
      MWidth = AWidth;
      MHeight = AHeight;
      MDepth = ADepth;
      //switch (MDepth) {
      //  case 1:   MStride = MWidth >> 3; break;
      //  case 8:   MStride = MWidth;      break;
      //  case 16:  MStride = MWidth * 2;  break;
      //  case 24:  MStride = MWidth * 3;  break;
      //  case 32:  MStride = MWidth * 4;  break;
      //}
      BITMAPINFO bmi;
      S3_Memset(&bmi,0,sizeof(BITMAPINFO));
      bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth       = MWidth;     // width of the bitmap, in pixels.
      bmi.bmiHeader.biHeight      = -MHeight;   // height (negative)
      bmi.bmiHeader.biPlanes      = 1;          // number of planes for target device. must be set to 1.
      bmi.bmiHeader.biBitCount    = MDepth;     // bits per pixel
      bmi.bmiHeader.biCompression = BI_RGB;     // uncompressed
      bmi.bmiHeader.biSizeImage   = 0;          // size, in bytes, of the image. may be set to zero for BI_RGB bitmaps.
      HDC tempdc = GetDC(0);
      MHandle = CreateDIBSection(tempdc,&bmi,DIB_RGB_COLORS,&MBuffer,NULL,0);
      ReleaseDC(0,tempdc);
    }

    //----------

    /*
    */

    //----------

    virtual ~S3_Image_Gdi() {
      DeleteObject(MHandle);
    }

  //------------------------------
  //
  //------------------------------

  public:

  //------------------------------
  // S3_Drawable
  //------------------------------

  public:

    int32   width(void)   { return MWidth; }
    int32   height(void)  { return MHeight; }
    int32   depth(void)   { return MDepth; }
    int32   stride(void)   { return MDepth; }
    void*   buffer(void)  { return MBuffer; }
    HBITMAP hbitmap(void) { return MHandle; }

};

//----------------------------------------------------------------------
#endif
