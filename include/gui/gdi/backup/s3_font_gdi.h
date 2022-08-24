#ifndef s3_font_gdi_included
#define s3_font_gdi_included
//----------------------------------------------------------------------

// AddFontMemResourceEx

#include "gui/base/s3_font_base.h"

//class S3_Font_Win32;
//typedef S3_Font_Win32 S3_Font_Implementation;

//----------

class S3_Font_Gdi
: public S3_Font_Base {

  private:
    HFONT     MHandle;
    COLORREF  MTextColor;
    S3_Color  MColor;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Font_Gdi()
    : S3_Font_Base() {
      MColor = S3_Black;
      MTextColor = S3_RGB(MColor);
      LOGFONT lfont;
      S3_Memset(&lfont,0,sizeof(lfont));
      S3_Strcpy(lfont.lfFaceName,"Arial");
      HDC tempdc = GetDC(0);
      lfont.lfHeight = -MulDiv(8,GetDeviceCaps(tempdc,LOGPIXELSY),72);
      ReleaseDC(0,tempdc);
      MHandle = CreateFontIndirect(&lfont);
    }

    //----------

    S3_Font_Gdi(const char* AName, int32 ASize, S3_Color AColor)
    : S3_Font_Base(AName,ASize,AColor) {
      MColor = AColor;
      MTextColor = S3_RGB(MColor);
      LOGFONT lfont;
      S3_Memset(&lfont,0,sizeof(lfont));
      S3_Strcpy(lfont.lfFaceName,AName);
      HDC tempdc = GetDC(0);
      lfont.lfHeight = -MulDiv(ASize,GetDeviceCaps(tempdc,LOGPIXELSY),72);
      ReleaseDC(0,tempdc);
      MHandle = CreateFontIndirect(&lfont);
    }

    //----------

    virtual ~S3_Font_Gdi() {
      DeleteObject(MHandle);
    }

  //------------------------------
  //
  //------------------------------

  public:

    HFONT     hfont(void)             { return MHandle; }
    void      color(S3_Color AColor)  { MColor = AColor; }
    S3_Color  color(void)             { return MColor; }
    COLORREF  textColor(void)         { return MTextColor; }

  //------------------------------
  //
  //------------------------------

  public:


};

//----------------------------------------------------------------------
#endif
