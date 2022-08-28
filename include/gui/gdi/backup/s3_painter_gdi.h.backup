#ifndef s3_painter_gdi_included
#define s3_painter_gdi_included
//----------------------------------------------------------------------

/*
  https://msdn.microsoft.com/en-us/library/windows/desktop/ff729480%28v=vs.85%29.aspx
  "..text rendering, BitBlts, AlphaBlend, TransparentBlt, and StretchBlt are all accelerated.."
*/

#include "common/s3_stack.h"
#include "common/s3_rect.h"
#include "gui/base/s3_painter_base.h"
#include "gui/s3_drawable.h"
#include "gui/s3_pen.h"
#include "gui/s3_brush.h"
#include "gui/s3_font.h"
#include "gui/s3_surface.h"
#include "gui/s3_image.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  wingdi.h

  BLACKNESS       Fills the destination rectangle using the color associated
                  with index 0 in the physical palette. (This color is black for
                  the default physical palette.)
  CAPTUREBLT      Includes any windows that are layered on top of your window in
                  the resulting image. By default, the image only contains your
                  window. Note that this generally cannot be used for printing
                  device contexts.
  DSTINVERT       Inverts the destination rectangle.
  MERGECOPY       Merges the colors of the source rectangle with the brush
                  currently selected in hdcDest, by using the Boolean AND
                  operator.
  MERGEPAINT      Merges the colors of the inverted source rectangle with the
                  colors of the destination rectangle by using the Boolean OR
                  operator.
  NOMIRRORBITMAP  Prevents the bitmap from being mirrored.
  NOTSRCCOPY      Copies the inverted source rectangle to the destination.
  NOTSRCERASE     Combines the colors of the source and destination rectangles
                  by using the Boolean OR operator and then inverts the
                  resultant color.
  PATCOPY         Copies the brush currently selected in hdcDest, into the
                  destination bitmap.
  PATINVERT       Combines the colors of the brush currently selected in
                  hdcDest, with the colors of the destination rectangle by using
                  the Boolean XOR operator.
  PATPAINT        Combines the colors of the brush currently selected in
                  hdcDest, with the colors of the inverted source rectangle by
                  using the Boolean OR operator. The result of this operation is
                  combined with the colors of the destination rectangle by using
                  the Boolean OR operator.
  SRCAND          Combines the colors of the source and destination rectangles
                  by using the Boolean AND operator.
  SRCCOPY         Copies the source rectangle directly to the destination
                  rectangle.
  SRCERASE        Combines the inverted colors of the destination rectangle with
                  the colors of the source rectangle by using the Boolean AND
                  operator.
  SRCINVERT       Combines the colors of the source and destination rectangles
                  by using the Boolean XOR operator.
  SRCPAINT        Combines the colors of the source and destination rectangles
                  by using the Boolean OR operator.
  WHITENESS       Fills the destination rectangle using the color associated
                  with index 1 in the physical palette. (This color is white for
                  the default physical palette.)
*/

//----------

/*
  DWORD dwRop;
  switch (rop)
  case wxXOR:          dwRop = SRCINVERT;
  case wxINVERT:       dwRop = DSTINVERT;
  case wxOR_REVERSE:   dwRop = 0x00DD0228;
  case wxAND_REVERSE:  dwRop = SRCERASE;
  case wxCLEAR:        dwRop = BLACKNESS;
  case wxSET:          dwRop = WHITENESS;
  case wxOR_INVERT:    dwRop = MERGEPAINT;
  case wxAND:          dwRop = SRCAND;
  case wxOR:           dwRop = SRCPAINT;
  case wxEQUIV:        dwRop = 0x00990066;
  case wxNAND:         dwRop = 0x007700E6;
  case wxAND_INVERT:   dwRop = 0x00220326;
  case wxCOPY:         dwRop = SRCCOPY;
  case wxNO_OP:        dwRop = DSTCOPY;
  case wxSRC_INVERT:   dwRop = NOTSRCCOPY;
  case wxNOR:          dwRop = NOTSRCCOPY;
  if ( dwRop == BLACKNESS || dwRop == WHITENESS || dwRop == DSTINVERT || dwRop == DSTCOPY ) {
    hdcSrc = NULL;
  }

*/

//----------

/*
// see also:
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd145130%28v=vs.85%29.aspx
#define DSTCOPY          0x00AA0029
#define DSTERASE         0x00220326 // dest = dest & (~ src) :  DSna
#define FORE_ROP3(ROP4) (0x00FFFFFF & (ROP4))
#define BACK_ROP3(ROP4) (ROP3FromIndex (SwapROP3_SrcDst (BYTE ((ROP4) >> 24))))
*/

// DWORD   dwRop
// FORE_ROP3(dwRop)

const uint32 s3_win32_rasterops[] = {
  BLACKNESS,    // s3_ro_black
  PATCOPY,      // s3_ro_brush
  PATPAINT,     // s3_ro_brush_or_invsrcor_or_dst
  PATINVERT,    // s3_ro_brush_xor_dst
  DSTCOPY,      // s3_ro_dst
  DSTERASE,     // s3_ro_dst_and_invsrc
  DSTINVERT,    // s3_ro_invdst
  SRCERASE,     // s3_ro_invdst_and_src
  NOTSRCCOPY,   // s3_ro_invsrc
  MERGEPAINT,   // s3_ro_invsrc_or_dst
  SRCCOPY,      // s3_ro_src
  SRCAND,       // s3_ro_src_and_dst
  MERGECOPY,    // s3_ro_src_and_brush
  SRCPAINT,     // s3_ro_src_or_dst
  NOTSRCERASE,  // s3_ro_src_or_dst_inv
  SRCINVERT,    // s3_ro_src_xor_dst
  WHITENESS     //s3_ro_white                     =
};

//----------------------------------------------------------------------

class S3_Painter_Gdi
: public S3_Painter_Base {

  private:
    HDC             MHandle;
    HGDIOBJ         MOldPen;
    HGDIOBJ         MOldBrush;
    HGDIOBJ         MOldFont;
    HGDIOBJ         MOldBitmap;
    HGDIOBJ         MDefaultPen;
    HGDIOBJ         MDefaultBrush;
    HGDIOBJ         MDefaultFont;
    HGDIOBJ         MDefaultBitmap;
    BLENDFUNCTION   MBlendFunc;
    BLENDFUNCTION   MStretchFunc;
		HPEN            MNullPen;
		HBRUSH          MNullBrush;
  private:
    S3_Pen*         MPen;
    S3_Brush*       MBrush;
    S3_Font*        MFont;
    S3_Color        MDrawColor; // COLORREF
    S3_Color        MFillColor; // COLORREF
    S3_Color        MTextColor; // COLORREF
  private:
    uint32          MSrcRasterOp;
    uint32          MDstRasterOp;

  //------------------------------
  //
  //------------------------------

  public:

    /*
      ADrawable
      - where we want to draw to (HBITMAP, aka S3_Surface)
      - uses width(), height() to set up initial clip rectangle
    */

    S3_Painter_Gdi(S3_Drawable* ADrawable)
    : S3_Painter_Base(ADrawable) {
      HDC tempdc      = GetDC(0);
      MHandle         = CreateCompatibleDC(tempdc); // CreateCompatibleDC(S3_NULL);
      ReleaseDC(0,tempdc);
      MDrawColor      = S3_White;
      MFillColor      = S3_Grey;
      MTextColor      = S3_Black;
      MPen            = new S3_Pen();   // (MDrawColor);
      MBrush          = new S3_Brush(); // (MFillColor);
      MFont           = new S3_Font();  // ("Arial",8,MTextColor);
      MDefaultPen     = SelectObject(MHandle,MPen->hpen());
      MDefaultBrush   = SelectObject(MHandle,MBrush->hbrush());
      MDefaultFont    = SelectObject(MHandle,MFont->hfont());
      MDefaultBitmap  = SelectObject(MHandle,ADrawable->hbitmap());
      MBlendFunc.BlendOp                = AC_SRC_OVER;
      MBlendFunc.BlendFlags             = 0;
      MBlendFunc.SourceConstantAlpha    = 255;          // 128;//0x7f;
      MBlendFunc.AlphaFormat            = AC_SRC_ALPHA; // 0 =  ignore source alpha channel
      MStretchFunc.BlendOp              = AC_SRC_OVER;
      MStretchFunc.BlendFlags           = 0;
      MStretchFunc.SourceConstantAlpha  = 255;          // 128;//0x7f;
      MStretchFunc.AlphaFormat          = AC_SRC_ALPHA; // 0 =  ignore source alpha channel
      MNullPen  = CreatePen(PS_NULL,0,0);
      LOGBRUSH lbrush;
      lbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ..
      lbrush.lbColor = 0;       // ignored if null
      lbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
      MNullBrush = CreateBrushIndirect(&lbrush);
      MSrcRasterOp = s3_ro_src;
      MDstRasterOp = s3_ro_dst;
    }

    //----------

    virtual ~S3_Painter_Gdi() {
      SelectObject(MHandle,MDefaultPen);
      SelectObject(MHandle,MDefaultBrush);
      SelectObject(MHandle,MDefaultFont);
      SelectObject(MHandle,MDefaultBitmap);
      DeleteObject(MNullPen);
      DeleteObject(MNullBrush);
      delete MPen;
      delete MBrush;
      delete MFont;
      DeleteDC(MHandle);
    }

  public:

  //------------------------------
  //
  //------------------------------

  public:

    HDC hdc(void) { return MHandle; }

  //------------------------------
  //
  //------------------------------

  public:

  //------------------------------
  // low level
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void lock(void) {
    }

    virtual // S3_Painter_Base
    void unlock(void) {
    }

    virtual // S3_Painter_Base
    void sync(void) {
    }

    virtual // S3_Painter_Base
    void flush(void) {
      GdiFlush();
    }

  //------------------------------
  // select/reset
  //------------------------------

  public:

    // APen = S3_NULL -> invisible pen

    virtual // S3_Painter_Base
    void selectPen(S3_Pen* APen, bool ARemember=true) {
      HGDIOBJ old;
      if (APen) {
        MDrawColor = APen->penColor();
        old = SelectObject(MHandle,APen->hpen());
      }
      else {
        old = SelectObject(MHandle,MNullPen);
      }
      if (ARemember) MOldPen = old;
    }

    //----------

    virtual // S3_Painter_Base
    void resetPen(void) {
      SelectObject(MHandle,MOldPen);
    }

    //----------

    // ABrush = S3_NULL -> invisible brush

    virtual // S3_Painter_Base
    void selectBrush(S3_Brush* ABrush, bool ARemember=true) {
      HGDIOBJ old;
      if (ABrush) {
        MFillColor = ABrush->brushColor();
        old = SelectObject(MHandle,ABrush->hbrush());
      }
      else {
        old = SelectObject(MHandle,MNullBrush);
      }
      if (ARemember) MOldBrush = old;
    }

    //----------

    virtual // S3_Painter_Base
    void resetBrush(void) {
      SelectObject(MHandle,MOldBrush);
    }

    //----------

    virtual // S3_Painter_Base
    void selectFont(S3_Font* AFont, bool ARemember=true) {
      MTextColor = AFont->textColor();
      if (ARemember) {
        MOldFont = SelectObject(MHandle,AFont->hfont());
      }
      else {
        SelectObject(MHandle,AFont->hfont());
      }
    }

    //----------

    virtual // S3_Painter_Base
    void resetFont(void) {
      SelectObject(MHandle,MOldFont);
    }

    //----------

    /*
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd162957%28v=vs.85%29.aspx
      Bitmaps can only be selected into memory DC's. A single bitmap cannot be
      selected into more than one DC at the same time.
    */

    virtual // S3_Painter_Base
    void selectDrawable(S3_Drawable* ADrawable, bool ARemember=true) {
      HBITMAP hbitmap = ADrawable->hbitmap();
      if (hbitmap) {
        if (ARemember) MOldBitmap = SelectObject(MHandle,hbitmap);
        else SelectObject(MHandle,hbitmap);
      }
    }

    //----------

    virtual // S3_Painter_Base
    void resetDrawable(void) {
      SelectObject(MHandle,MOldBitmap);
    }

  //------------------------------
  // get/set
  //------------------------------

  public:

    /*
      SetDCPenColor function sets the current device context (DC) pen color to
      the specified color value.
      If the function succeeds, the return value specifies the previous DC pen
      color as a COLORREF value. If the function fails, the return value is
      CLR_INVALID.
      The function returns the previous DC_PEN color, even if the stock pen
      DC_PEN is not selected in the DC; however, this will not be used in
      drawing operations until the stock DC_PEN is selected in the DC.
      The GetStockObject function with an argument of DC_BRUSH or DC_PEN can be
      used interchangeably with the SetDCPenColor and SetDCBrushColor
      functions.
    */

    virtual // S3_Painter_Base
    void setDrawColor(S3_Color AColor) {
      MDrawColor = AColor;
      SetDCPenColor(MHandle,S3_RGB(MDrawColor));
    }

    //----------

    virtual // S3_Painter_Base
    void setFillColor(S3_Color AColor) {
      MFillColor = AColor;
      SetDCBrushColor(MHandle,S3_RGB(MFillColor));
    }

    //----------

    virtual // S3_Painter_Base
    void setTextColor(S3_Color AColor) {
      MTextColor = AColor;
      SetTextColor(MHandle,S3_RGB(MTextColor));
    }

    //----------

    //void setTextSize(int32 ASize) {
    //  LOGFONT lfont;
    //  HFONT font = (HFONT)GetCurrentObject(MHandle,OBJ_FONT);
    //  GetObject(font,sizeof(LOGFONT),&lfont);
    //  lfont.lfHeight = -MulDiv(ASize,GetDeviceCaps(MHandle,LOGPIXELSY),72);
    //  if (MFont) DeleteObject(MFont);
    //  MFont = CreateFontIndirect(&lfont);
    //  SelectObject(MHandle,MFont);
    //}

    //----------

    virtual // S3_Painter_Base
    int32 getTextWidth(const char* AText) {
      SIZE S;
      GetTextExtentPoint32(MHandle,AText,S3_Strlen(AText),&S);
      return S.cx;
    }

    //----------

    virtual // S3_Painter_Base
    int32 getTextHeight(const char* AText) {
      SIZE S;
      GetTextExtentPoint32(MHandle,AText,S3_Strlen(AText),&S);
      return S.cy;
    }

  //------------------------------
  // shapes
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void drawPoint(int32 AX, int32 AY) {
      SetPixel(MHandle,AX,AY,S3_RGB(MDrawColor));
    }

    //----------

    virtual // S3_Painter_Base
    void drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      MoveToEx(MHandle,AX1,AY1,0);
      LineTo(MHandle,AX2,AY2);
      SetPixel(MHandle,AX2,AY2,S3_RGB(MDrawColor));
    }

    //----------

    virtual // S3_Painter_Base
    void drawRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      MoveToEx(MHandle,AX1,AY1,0);
      LineTo(MHandle,AX2,AY1);
      LineTo(MHandle,AX2,AY2);
      LineTo(MHandle,AX1,AY2);
      LineTo(MHandle,AX1,AY1);
      //drawPoint(aX2,aY2);         // !!!
    }

    //----------

    virtual // S3_Painter_Base
    void drawRoundedRectangle(void) {
    }

    //----------

    virtual // S3_Painter_Base
    void drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      //Ellipse(mDC, aX1,aY1,aX2,aY2 );
      Arc( MHandle,AX1,AY1,AX2+1,AY2+1,0,0,0,0);
    }

    //----------

    /*
      angle 1 = start angle, relative to 3 o'clock
      angle 2 = 'distance' 0..1, counter-clockwise
    */

    virtual // S3_Painter_Base
    void drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
      AX2 += 1;
      AY2 += 1;
      if (fabsf(AAngle2) >= 0.01) {
        float a1 = AAngle1 -= 0.25;
        float a2 = a1 + AAngle2;
        if (AAngle2>0) {
          float temp = a1;
          a1 = a2;
          a2 = temp;
        }
        float w = AX2-AX1+1;
        float h = AY2-AY1+1;
        float s = fmaxf(w,h);
        float x = AX1 + w*0.5;
        float y = AY1 + h*0.5;
        float x1 = x + cosf(a1*S3_PI2) * s;
        float y1 = y + sinf(a1*S3_PI2) * s;
        float x2 = x + cosf(a2*S3_PI2) * s;
        float y2 = y + sinf(a2*S3_PI2) * s;
        Arc(MHandle,AX1,AY1,AX2,AY2,(int)x1,(int)y1,(int)x2,(int)y2);
      }
    }

    //----------

    virtual // S3_Painter_Base
    void drawTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
      drawLine(AX1,AY1,AX2,AY2);
      drawLine(AX2,AY2,AX3,AY3);
      drawLine(AX3,AY3,AX1,AY1);
    }

  //------------------------------
  // fill shapes
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void fillRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      RECT R;
      R.left    = AX1;
      R.top     = AY1;
      R.right   = AX2+1;           // !!!
      R.bottom  = AY2+1;           // !!!
      HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
      FillRect(MHandle,&R,brush);
      //Rectangle(mDC,aX1,aY1,aX2,aY2);
    }

    //----------

    virtual // S3_Painter_Base
    void fillRoundedRectangle(void) {
    }

    //----------

    /*
      The Ellipse function draws an ellipse. The center of the ellipse is the
      center of the specified bounding rectangle. The ellipse is outlined by
      using the current pen and is filled by using the current brush.
    */

    virtual // S3_Painter_Base
    void fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      AX2 += 2;
      AY2 += 2;
      selectPen(S3_NULL);
      Ellipse( MHandle,AX1,AY1,AX2,AY2);
      resetPen();
    }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise

    virtual // S3_Painter_Base
    void fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
      AX2 += 2;
      AY2 += 2;
      if( fabsf(AAngle2) >= 0.01 ) {
        float a1 = AAngle1 -= 0.25;
        float a2 = a1 + AAngle2;
        if ( AAngle2>0 ) {
          float temp = a1;
          a1 = a2;
          a2 = temp;
        }
        float w = AX2-AX1+1;
        float h = AY2-AY1+1;
        float s = fmaxf(w,h);
        float x = AX1 + w*0.5;
        float y = AY1 + h*0.5;
        float x1 = x + cosf(a1*S3_PI2) * s;
        float y1 = y + sinf(a1*S3_PI2) * s;
        float x2 = x + cosf(a2*S3_PI2) * s;
        float y2 = y + sinf(a2*S3_PI2) * s;
        selectPen(S3_NULL);
        Pie(MHandle,AX1,AY1,AX2,AY2,(int)x1,(int)y1,(int)x2,(int)y2);
        resetPen();
      }
    }

    //----------

    virtual // S3_Painter_Base
    void fillTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
      POINT points[3];
      points[0].x = AX1;
      points[0].y = AY1;
      points[1].x = AX2;
      points[1].y = AY2;
      points[2].x = AX3;
      points[2].y = AY3;
      selectPen(S3_NULL);
      Polygon(MHandle,points,3);
      resetPen();
    }

  //------------------------------
  // gradients
  //------------------------------

  /*
    gradients are not gamma correct..
  */

  public:

    virtual // S3_Painter_Base
    void gradientRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, S3_Color AUpperLeft, S3_Color ALowerRight, bool AVertical=true) {
      TRIVERTEX vertices[2];
      vertices[0].x     = AX1;
      vertices[0].y     = AY1;
      vertices[0].Red   = (uint32)(AUpperLeft.r * 255) << 8;
      vertices[0].Green = (uint32)(AUpperLeft.g * 255) << 8;
      vertices[0].Blue  = (uint32)(AUpperLeft.b * 255) << 8;
      vertices[0].Alpha = (uint32)(AUpperLeft.a * 255) << 8;
      vertices[1].x     = AX2;
      vertices[1].y     = AY2;
      vertices[1].Red   = (uint32)(ALowerRight.r * 255) << 8;
      vertices[1].Green = (uint32)(ALowerRight.g * 255) << 8;
      vertices[1].Blue  = (uint32)(ALowerRight.b * 255) << 8;
      vertices[1].Alpha = (uint32)(ALowerRight.a * 255) << 8;
      GRADIENT_RECT R;
      R.UpperLeft  = 0;
      R.LowerRight = 1;
      uint32 mode = GRADIENT_FILL_RECT_H;
      if (AVertical) mode = GRADIENT_FILL_RECT_V;
      GdiGradientFill(MHandle,vertices,2,&R,1,mode);
    }

    //----------

    virtual // S3_Painter_Base
    void gradientTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3, S3_Color AColor1, S3_Color AColor2, S3_Color AColor3) {
      TRIVERTEX vertices[3];
      vertices[0].x     = AX1;
      vertices[0].y     = AY1;
      vertices[0].Red   = (uint32)(AColor1.r * 255) << 8;
      vertices[0].Green = (uint32)(AColor1.g * 255) << 8;
      vertices[0].Blue  = (uint32)(AColor1.b * 255) << 8;
      vertices[0].Alpha = (uint32)(AColor1.a * 255) << 8;
      vertices[1].x     = AX2;
      vertices[1].y     = AY2;
      vertices[1].Red   = (uint32)(AColor2.r * 255) << 8;
      vertices[1].Green = (uint32)(AColor2.g * 255) << 8;
      vertices[1].Blue  = (uint32)(AColor2.b * 255) << 8;
      vertices[1].Alpha = (uint32)(AColor2.a * 255) << 8;
      vertices[2].x     = AX3;
      vertices[2].y     = AY3;
      vertices[2].Red   = (uint32)(AColor3.r * 255) << 8;
      vertices[2].Green = (uint32)(AColor3.g * 255) << 8;
      vertices[2].Blue  = (uint32)(AColor3.b * 255) << 8;
      vertices[2].Alpha = (uint32)(AColor3.a * 255) << 8;
      GRADIENT_TRIANGLE T;
      T.Vertex1 = 0;
      T.Vertex2 = 1;
      T.Vertex3 = 2;
      uint32 mode = GRADIENT_FILL_TRIANGLE;
      GdiGradientFill(MHandle,vertices,3,&T,1,mode);
    }

  //------------------------------
  // blend shapes
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void blendRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
    }

  //------------------------------
  // text
  //------------------------------

  public:

    /*
      hack alert!
      draw to a rect size 1000x1000, and align to upper left
    */

    //----------

    virtual // S3_Painter_Base
    void drawText(int32 AX, int32 AY, const char* AText) {
      //SetBkMode(mDC,TRANSPARENT);
      //TextOut(mDC,aX,aY,aText.ptr(), aText.length() );
      this->drawText( AX,AY, AX+1000,AY+1000, AText, s3_ta_left|s3_ta_top);
    }

    //----------

    virtual // S3_Painter_Base
    void drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) {
      SetBkMode(MHandle,TRANSPARENT);
      RECT R;
      R.left    = AX1;
      R.top     = AY1;
      R.right   = AX2;
      R.bottom  = AY2;
      int flags = 0;
      if (AAlign & s3_ta_left) flags = flags | DT_LEFT;
      else if (AAlign & s3_ta_right) flags = flags | DT_RIGHT;
      else flags = flags | DT_CENTER;
      if (AAlign & s3_ta_top) flags = flags | DT_TOP;
      else if (AAlign & s3_ta_bottom) flags = flags | DT_BOTTOM;
      else flags = flags | DT_VCENTER;
      //HFONT oldfont = (HFONT)SelectObject(MHandle,MFont);
      DrawText(MHandle,AText,-1,&R,flags|DT_NOCLIP|DT_SINGLELINE|DT_NOPREFIX);
      //SelectObject(MHandle, oldfont);
    }

  //------------------------------
  // bitmaps
  //------------------------------

  /*
    https://msdn.microsoft.com/en-us/library/windows/desktop/dd183385%28v=vs.85%29.aspx
    BitBlt          draw
    AlphaBlend      blend
    StretchBlt      stretch
    MaskBlt         alphaMask
    TransparentBlt  bitMask
    GradientFill    gradient
    ExtFloodFill
    PlgBlt          parallellogram
  */

  public:

    /*
      see also:
      StretchDIBits
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd145121%28v=vs.85%29.aspx
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd183370%28v=vs.85%29.aspx
    */

    virtual // S3_Painter_Base
    void drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
      HDC tempdc = CreateCompatibleDC(MHandle);
      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
      uint32 rop = s3_win32_rasterops[MSrcRasterOp];  // SRCCOPY;
      BitBlt(MHandle,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,rop);
      SelectObject(tempdc,oldbitmap);
      DeleteDC(tempdc);
    }

    virtual // S3_Painter_Base
    void maskBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH, S3_Drawable* AMask, int32 AMaskX, int32 AMaskY) {
      HDC tempdc = CreateCompatibleDC(MHandle);
      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
      uint32 fore = s3_win32_rasterops[MSrcRasterOp]; // SRCCOPY;
      uint32 back = s3_win32_rasterops[MDstRasterOp]; // DSTCOPY;
      uint32 rop = MAKEROP4(fore,back);
      MaskBlt(MHandle,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,AMask->hbitmap(),AMaskX,AMaskY,rop);
      SelectObject(tempdc,oldbitmap);
      DeleteDC(tempdc);
    }

    virtual // S3_Painter_Base
    void tileBitmap(void) {
      // https://msdn.microsoft.com/en-us/library/windows/desktop/dd162778%28v=vs.85%29.aspx
      // PatBlt
    }

    //----------

    virtual // S3_Painter_Base
    void transparentBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH, S3_Color ATransparentColor) {
      HDC tempdc = CreateCompatibleDC(MHandle);
      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
      //GdiAlphaBlend(MHandle,ADstX,ADstY,ADstW,ADstH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,MStretchFunc);
      UINT color = S3_RGB(ATransparentColor);
      GdiTransparentBlt(MHandle,ADstX,ADstY,ADstW,ADstH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,color);
      SelectObject(tempdc,oldbitmap);
      DeleteDC(tempdc);
    }

    //----------

    // link with: libmsimg32, or..
    // are also in gdi32 with Gdi* prefix

    //----------

    virtual // S3_Painter_Base
    void blendBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
      HDC tempdc = CreateCompatibleDC(MHandle);
      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
      GdiAlphaBlend(MHandle,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,MBlendFunc);
      SelectObject(tempdc,oldbitmap);
      DeleteDC(tempdc);
    }

    //----------

    virtual // S3_Painter_Base
    void stretchBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
      HDC tempdc = CreateCompatibleDC(MHandle);
      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());

      GdiAlphaBlend(MHandle,ADstX,ADstY,ADstW,ADstH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,MStretchFunc);

//      uint32 rop = s3_win32_rasterops[MSrcRasterOp]; // SRCCOPY;
//      StretchBlt(MHandle,ADstX,ADstY,ADstW,ADstH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,rop);
      SelectObject(tempdc,oldbitmap);
      DeleteDC(tempdc);
    }

  //------------------------------
  // clipping
  //------------------------------

  public:

    // http://www.codeproject.com/Articles/2095/A-Guide-to-WIN32-Clipping-Regions

    /*
      CreateRectRgn
      http://msdn.microsoft.com/en-us/library/windows/desktop/dd183514%28v=vs.85%29.aspx

      Regions created by the Create<shape>Rgn methods (such as CreateRectRgn and
      CreatePolygonRgn) only include the interior of the shape; the shape's outline
      is excluded from the region. This means that any point on a line between two
      sequential vertices is not included in the region. If you were to call
      PtInRegion for such a point, it would return zero as the result.
      When you no longer need the HRGN object, call the DeleteObject function to
      delete it.

      SelectClipRgn
      http://msdn.microsoft.com/en-us/library/windows/desktop/dd162955%28v=vs.85%29.aspx

      The SelectClipRgn function selects a region as the current clipping region for
      the specified device context.
      To remove a device-context's clipping region, specify a NULL region handle.
      Only a copy of the selected region is used. The region itself can be selected
      for any number of other device contexts or it can be deleted.
      "When you call SelectClipRgn() within a BeginPaint()/EndPaint() block in an
      application's WM_PAINT case, the maximum size to which you can set your
      clipping region is the size of the update region of your paint structure. In
      other words you can use SelectClipRgn() to shrink your update region but not
      to grow it".

      The BeginPaint function automatically sets the clipping region of the device
      context to exclude any area outside the update region.

      http://books.google.no/books?id=-O92IIF1Bj4C&pg=PA427&lpg=PA427&dq=SelectClipRgn&source=bl&ots=Sx2GC35hp7&sig=NFtTCTcuzrcBMoHaTJYY998CJww&hl=no&sa=X&ei=3ZvrUpOgJ4bhywOH74DAAg&redir_esc=y#v=onepage&q=SelectClipRgn&f=false

      For a device context returned by BeginPaint, GetDC, or CreateDC, the clipping
      region is NULL

      --------------------------------------------------

      http://msdn.microsoft.com/en-us/library/windows/desktop/dd183439%28v=vs.85%29.aspx

      if you obtain a device context handle from the BeginPaint function, the DC
      contains a predefined rectangular clipping region that corresponds to the
      invalid rectangle that requires repainting. However, when you obtain a device
      context handle by calling the GetDC function with a NULLhWnd parameter, or by
      calling the CreateDC function, the DC does not contain a default clipping
      region.

      ExcludeClipRect
      IntersectClipRect

      invalidate -> GetUpdateRect

    */

    virtual // S3_Painter_Base
    void clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      //self.noClip;
      HRGN rgn = CreateRectRgn( AX1, AY1, AX2+1, AY2+1 );
      SelectClipRgn(MHandle,rgn);
      DeleteObject(rgn);
      //  IntersectClipRect(FDC,ARect.x-0{1}, ARect.y-0{1}, ARect.x2+1, ARect.y2+1 );
    }

    //----------

    // To remove a device-context's clipping region, specify a NULL region handle.

    virtual // S3_Painter_Base
    void noClip(void) {
      SelectClipRgn(MHandle,S3_NULL);
    }

  //------------------------------
  // helpers
  //------------------------------

  public:

    void drawPoint(S3_Point APoint) {
      this->drawPoint(APoint.x,APoint.y);
    }

    void drawRectangle(S3_Rect ARect) {
      this->drawRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void drawEllipse(S3_Rect ARect) {
      this->drawEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void drawArc(S3_Rect ARect, float AAngle1, float AAngle2) {
      this->drawArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    }

    void fillRectangle(S3_Rect ARect) {
      this->fillRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void fillEllipse(S3_Rect ARect) {
      this->fillEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void fillArc(S3_Rect ARect, float AAngle1, float AAngle2) {
      this->fillArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    }

    void gradientRectangle(S3_Rect ARect, S3_Color AUpperLeft, S3_Color ALowerRight, bool AVertical=false) {
      this->gradientRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AUpperLeft,ALowerRight,AVertical);
    }

    void gradientTriangle(S3_Point APoint1, S3_Point APoint2, S3_Point APoint3, S3_Color AColor1, S3_Color AColor2, S3_Color AColor3) {
      this->gradientTriangle(APoint1.x,APoint1.y,APoint2.x,APoint2.y,APoint3.x,APoint3.y,AColor1,AColor2,AColor3);
    }

    void blendRectangle(S3_Rect ARect) {
      this->blendRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void drawText(S3_Point APoint, const char* AText) {
      this->drawText(APoint.x,APoint.y,AText);
    }

    void drawText(S3_Rect ARect, const char* AText, uint32 AAlign) {
      this->drawText(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AText,AAlign);
    }

    void drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, S3_Rect ASrc) {
      this->drawBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void drawBitmap(S3_Point ADst, S3_Drawable* ASource, S3_Rect ASrc) {
      this->drawBitmap(ADst.x,ADst.y,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void maskBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, S3_Rect ASrc, S3_Drawable* AMask, int32 AMaskX, int32 AMaskY) {
      this->maskBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h,AMask,AMaskX,AMaskY);
    }

    void blendBitmap(S3_Point ADst, S3_Drawable* ASource, S3_Rect ASrc) {
      this->blendBitmap(ADst.x,ADst.y,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void stretchBitmap(S3_Rect ADst, S3_Drawable* ASource, S3_Rect ASrc) {
      this->stretchBitmap(ADst.x,ADst.y,ADst.w,ADst.h,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void clip(S3_Rect ARect) {
      this->clip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

};

//----------------------------------------------------------------------
#endif

/*
  don't call virtual functions in constructor..
  ADrawable->hbitmap(), used in selectDrawable is virtual..

  The reason is that C++ objects are constructed like onions, from the inside
  out. Super-classes are constructed before derived classes. So, before a B can
  be made, an A must be made. When A's constructor is called, it's not a B yet,
  so the virtual function table still has the entry for A's copy of fn().

  During base class construction, virtual functions never go down into derived
  classes. Instead, the object behaves as if it were of the base type.
  Informally speaking, during base class construction, virtual functions
  aren't.

*/

