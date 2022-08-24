#ifndef s3_pen_gdi_included
#define s3_pen_gdi_included
//----------------------------------------------------------------------

#include "gui/base/s3_pen_base.h"

//----------

//class S3_Pen_Win32;
//typedef S3_Pen_Win32 S3_Pen_Implementation;

//----------

class S3_Pen_Gdi
: public S3_Pen_Base {

  private:
		HPEN      MHandle;
    COLORREF  MPenColor;
    S3_Color  MColor;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Pen_Gdi()
    : S3_Pen_Base() {
      MColor    = S3_Black;
      MPenColor = S3_RGB(MColor);
      MHandle   = (HPEN)GetStockObject(DC_PEN);
    }

    //----------

    S3_Pen_Gdi(S3_Color AColor, int32 AWidth=S3_PAINTER_DEFAULT_PEN_WIDTH)
    : S3_Pen_Base(AColor,AWidth) {
      MColor    = AColor;
      MPenColor = S3_RGB(MColor);
      MHandle   = CreatePen(PS_SOLID,AWidth,MPenColor);
    }

    //----------

    /*
      GetStockObject docs..
      It is not necessary (but it is not harmful) to delete stock objects by
      calling DeleteObject.
    */

    virtual ~S3_Pen_Gdi() {
      DeleteObject(MHandle);
    }

  //------------------------------
  //
  //------------------------------

  public:

    HPEN      hpen(void)              { return MHandle; }
    //HPEN      null(void)              { return MNullPen; }
    void      color(S3_Color AColor)  { MColor = AColor; }
    S3_Color  color(void)             { return MColor; }
    COLORREF  penColor(void)          { return MPenColor; }

  //------------------------------
  //
  //------------------------------

  public:

};

//----------------------------------------------------------------------
#endif
