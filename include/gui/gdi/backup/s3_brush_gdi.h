#ifndef s3_brush_gdi_included
#define s3_brush_gdi_included
//----------------------------------------------------------------------

#include "gui/base/s3_brush_base.h"
//#include "gui/s3_image.h"
//#include "gui/s3_surface.h"

//

//class S3_Brush_Win32;
//typedef S3_Brush_Win32 S3_Brush_Implementation;



class S3_Brush_Gdi
: public S3_Brush_Base{

  private:
		HBRUSH    MHandle;
    COLORREF  MBrushColor;
    S3_Color  MColor;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Brush_Gdi()
    : S3_Brush_Base() {
      MColor = S3_Black;
      MBrushColor = S3_RGB(MColor);
      //LOGBRUSH lbrush;
      //lbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ++
      //lbrush.lbColor = 0;       // ignored if null
      //lbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
      //MNullBrush = CreateBrushIndirect(&lbrush);
      //createNullBrush();
      MHandle = (HBRUSH)GetStockObject(DC_BRUSH);
    }

    //----------

    S3_Brush_Gdi(S3_Color AColor)
    : S3_Brush_Base(AColor) {
      // http://msdn.microsoft.com/en-us/library/dd145035%28v=VS.85%29.aspx
      MColor = AColor;
      MBrushColor = S3_RGB(MColor);
      //LOGBRUSH lbrush;
      //lbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ++
      //lbrush.lbColor = 0;       // ignored if null
      //lbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
      //MNullBrush = CreateBrushIndirect(&lbrush);
      //createNullBrush();
      MHandle = CreateSolidBrush(MBrushColor);
    }

    //----------

    S3_Brush_Gdi(S3_Surface* ASurface)
    : S3_Brush_Base(ASurface) {
      //createNullBrush();
      MHandle = CreatePatternBrush(ASurface->hbitmap());
    }

    //----------

    //S3_Brush_Gdi(S3_Image* AImage) {
    //  createNullBrush();
    //  MHandle = CreateDIBPatternBrushPt(,DIB_RGB_COLORS);
    //}

    //----------

    /*
      GetStockObject docs..
      It is not necessary (but it is not harmful) to delete stock objects by
      calling DeleteObject.
    */

    virtual ~S3_Brush_Gdi() {
      //DeleteObject(MNullBrush);
      DeleteObject(MHandle);
    }

  //------------------------------
  //
  //------------------------------

  public:

    HBRUSH    hbrush(void)            { return MHandle; }
    //HBRUSH    null(void)              { return MNullBrush; }
    void      color(S3_Color AColor)  { MColor = AColor; }
    S3_Color  color(void)             { return MColor; }
    COLORREF  brushColor(void)        { return MBrushColor; }

  //------------------------------
  //
  //------------------------------

  public:

    //void createNullBrush(void) {
    //  LOGBRUSH lbrush;
    //  lbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ..
    //  lbrush.lbColor = 0;       // ignored if null
    //  lbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
    //  MNullBrush = CreateBrushIndirect(&lbrush);
    //}

};

//----------------------------------------------------------------------
#endif
