#ifndef mip_gdi_painter_included
#define mip_gdi_painter_included
//----------------------------------------------------------------------

/*
  https://msdn.microsoft.com/en-us/library/windows/desktop/ff729480%28v=vs.85%29.aspx
  "..text rendering, BitBlts, AlphaBlend, TransparentBlt, and StretchBlt are all accelerated.."
*/

//      GdiFlush();


//----------------------------------------------------------------------

#include "mip.h"
#include "gui/base/mip_base_painter.h"
#include "gui/gdi/mip_gdi.h"

//----------------------------------------------------------------------

// 10k * 68 = 680k
#define MIP_PATH_MAX_LENGTH           1000

#define MIP_PATH_MOVE_TO              1
#define MIP_PATH_LINE_TO              2
#define MIP_PATH_BEZIER_TO            3
#define MIP_PATH_QUAD_TO              4
#define MIP_PATH_ARC_TO               5

#define MIP_PATH_ARC                  6
#define MIP_PATH_RECT                 7
#define MIP_PATH_ROUNDED_RECT         8
#define MIP_PATH_ROUNDED_RECT_VARYING 9
#define MIP_PATH_ELLIPSE              10
#define MIP_PATH_CIRCLE               11

// 4 + 8*8 = 68
// 1000 = 68k

struct MIP_PathItem {
  uint32_t  type    = 0;
  float     data[8] = {0};
};

typedef MIP_Array<MIP_PathItem> MIP_PathItemArray;

//----------

#define MIP_XCB_PAINTER_MAX_IMAGES  64
#define MIP_XCB_PAINTER_MAX_FONTS   16

//----------------------------------------------------------------------

class MIP_GdiPainter
: public MIP_BasePainter {

//------------------------------
private:
//------------------------------

  bool need_cleanup = false;

  //MIP_Drawable*   MSurface        = nullptr;
  //MIP_Drawable*   MTarget         = nullptr;

  HDC             MHandle         = nullptr;

  HPEN            MPenHandle      = nullptr;
  HBRUSH          MBrushHandle    = nullptr;
  HFONT           MFontHandle     = nullptr;
  HBITMAP         MBitmapHandle   = nullptr;

  HGDIOBJ         MDefaultPen     = nullptr;
  HGDIOBJ         MDefaultBrush   = nullptr;
  HGDIOBJ         MDefaultFont    = nullptr;
  HGDIOBJ         MDefaultBitmap  = nullptr;

//  HGDIOBJ         MOldPen         = nullptr;
//  HGDIOBJ         MOldBrush       = nullptr;
//  HGDIOBJ         MOldFont        = nullptr;
//  HGDIOBJ         MOldBitmap      = nullptr;

  HPEN            MNullPen = nullptr;
  HBRUSH          MNullBrush    = nullptr;

  uint32_t        MSrcRasterOp = SRCCOPY;
  uint32_t        MDstRasterOp = DSTCOPY;

  BLENDFUNCTION   MBlendFunc    = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
  BLENDFUNCTION   MStretchFunc  = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

//------------------------------
private:
//------------------------------

  MIP_PathItemArray MPath     = {};
  float             MPathXpos = 0.0f;
  float             MPathYpos = 0.0f;

  //MIP_Bitmap* MImages[MIP_XCB_PAINTER_MAX_IMAGES] = {0};

  //#ifdef MIP_XCB_USE_STB_TRUETYPE
  //stbtt_fontinfo* MFonts[MIP_XCB_PAINTER_MAX_FONTS] = {0};
  //#endif

//------------------------------
protected:
//------------------------------

  int               MShapeAntiAlias = 0;
  float             MMiterLimit     = 0;
  int               MLineCap        = 0;
  int               MLineJoin       = 0;
  float             MGlobalAlpha    = 0;

  MIP_Color         MStrokeColor    = MIP_Color(0.2);
  MIP_PaintSource   MStrokePaint    = {};
  float             MStrokeWidth    = 1.0;
  MIP_Color         MFillColor      = MIP_Color(0.8);
  MIP_PaintSource   MFillPaint      = {};
  int               MFont           = -1;
  MIP_Color         MFontColor      = MIP_Color(0);
  float             MFontSize       = 13.0;

  //

//------------------------------
public:
//------------------------------

  MIP_GdiPainter(MIP_Drawable* ATarget, MIP_Drawable* ASource)
  : MIP_BasePainter(ATarget,ASource) {

    HDC tempdc = GetDC(0);
    HDC handle = CreateCompatibleDC(tempdc); // (nullptr);
    setup(handle);
    ReleaseDC(0,tempdc);

  }

  //----------

  virtual ~MIP_GdiPainter() {
    cleanup();
  }

//------------------------------
public:
//------------------------------

  HDC getHandle(void) {
    return MHandle;
  }

  //----------

  void setup(HDC AHandle) {
    if (need_cleanup) cleanup();
    MHandle = AHandle;
    //MPen = (HPEN)GetStockObject(DC_PEN);
    MPenHandle = CreatePen(PS_SOLID,1,0xff0000);
    MDefaultPen = SelectObject(AHandle,MPenHandle);

    //MBrush = (HBRUSH)GetStockObject(DC_BRUSH);
    MBrushHandle = CreateSolidBrush(0x00ff00);
    MDefaultBrush = SelectObject(AHandle,MBrushHandle);

    MNullPen = CreatePen(PS_NULL,0,0);

    LOGBRUSH lbrush;
    lbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ..
    lbrush.lbColor = 0;       // ignored if null
    lbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
    MNullBrush = CreateBrushIndirect(&lbrush);

    //MFont = (HFONT)GetStockObject(DC_FONT);
    LOGFONT lfont;
    memset(&lfont,0,sizeof(lfont));
    strcpy(lfont.lfFaceName,"Arial");
    lfont.lfHeight = -MulDiv(8,GetDeviceCaps(AHandle,LOGPIXELSY),72);
    MFontHandle = CreateFontIndirect(&lfont);
    MDefaultFont = SelectObject(MHandle,MFontHandle);

    //TODO: MBitmap..
    //MDefaultBitmap = SelectObject(AHandle,MBitmapHandle);

    need_cleanup = true;
  }

  //----------

  void cleanup() {
    if (need_cleanup) {
      SelectObject(MHandle,MDefaultPen);
      SelectObject(MHandle,MDefaultBrush);
      SelectObject(MHandle,MDefaultFont);
      //SelectObject(MHandle,MDefaultBitmap);
      // It is not necessary (but it is not harmful) to delete stock objects
      // by scalling DeleteObject.
      DeleteObject(MPenHandle);
      DeleteObject(MBrushHandle);
      DeleteObject(MFontHandle);
      //DeleteObject(MBitmapHandle);
      DeleteObject(MNullPen);
      DeleteObject(MNullBrush);
      DeleteDC(MHandle);
    }
    need_cleanup = false;
  }

//------------------------------
public:
//------------------------------

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
  }

  //----------

  void endPaint() override {
  }

//------------------------------
public: // clipping
//------------------------------

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

  //  virtual // S3_Painter_Base
  //  void clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  //    //self.noClip;
  //    HRGN rgn = CreateRectRgn( AX1, AY1, AX2+1, AY2+1 );
  //    SelectClipRgn(MHandle,rgn);
  //    DeleteObject(rgn);
  //    //  IntersectClipRect(FDC,ARect.x-0{1}, ARect.y-0{1}, ARect.x2+1, ARect.y2+1 );
  //  }

  void setClip(MIP_DRect ARect) override {
    HRGN rgn = CreateRectRgn(
      ARect.x,//AX1,
      ARect.y,//AY1,
      ARect.x2() + 1,
      ARect.y2() + 1
    );
    SelectClipRgn(MHandle,rgn);
    DeleteObject(rgn);
  }

  //----------

  // To remove a device-context's clipping region, specify a NULL region handle.

  void resetClip() override {
    SelectClipRgn(MHandle,nullptr);
  }

  //----------

  //  void      pushClip(MIP_DRect ARect) override {}
  //  void      pushOverlapClip(MIP_DRect ARect) override {}
  //  MIP_DRect popClip() override { return MIP_DRect(); }
  //  void      resetClipStack() override {}
  //  void      setClipRect(MIP_DRect ARect) override {}
  //  MIP_DRect getClipRect() override { return MIP_DRect(); }

    //void resize(uint32_t AWidth, uint32_t AHeight) override {}

  //  void drawTextBox(MIP_DRect ARect, const char* AText, uint32_t AAlignment, MIP_Color AColor) override {}
  //  void triangle( double x1, double y1, double x2, double y2, double x3, double y3) override {}
  //  void rectangle(MIP_DRect r) override {}
  //  void ellipse(MIP_DRect r) override {}

//------------------------------
public: // frame
//------------------------------

  void beginFrame(float windowWidth, float windowHeight, float devicePixelRatio) override {
  }

  //----------

  void cancelFrame() override {
  }

  //----------

  void endFrame() override {
  }

//------------------------------
public: // Composite operation
//------------------------------

  void globalCompositeOperation(int op) override {
  }

  //----------

  void globalCompositeBlendFunc(int sfactor, int dfactor) override {
  }

  //----------

  void globalCompositeBlendFuncSeparate(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha) override {
  }

//------------------------------
public: // state handling
//------------------------------

  void save() override {
  }

  //----------

  void restore() override {
  }

  //----------

  void reset() override {
  }

//------------------------------
public: // render styles
//------------------------------

  void shapeAntiAlias(int enabled) override {
    MShapeAntiAlias = enabled;
  }

  //----------

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

  void strokeColor(MIP_Color color) override {
    MStrokeColor = color;
    SetDCPenColor(MHandle,MIP_RGBA(color));
  }

  //----------

  void strokePaint(MIP_PaintSource paint) override {
    MStrokePaint = paint;
  }

  //----------

  void fillColor(MIP_Color color) override {
    MFillColor = color;
    SetDCBrushColor(MHandle,MIP_RGBA(color));
  }

  //----------

  void fillPaint(MIP_PaintSource paint) override {
    MFillPaint = paint;
  }

  void miterLimit(float limit) override {
    MMiterLimit = limit;
  }

  void strokeWidth(float size) override {
    MStrokeWidth = size;
  }

  void lineCap(int cap) override {
    MLineCap = cap;
  }

  void lineJoin(int join) override {
    MLineJoin = join;
  }

  void globalAlpha(float alpha) override {
    MGlobalAlpha = alpha;
  }

//------------------------------
public: // transforms
//------------------------------

  void resetTransform() override {}
  void transform(float a, float b, float c, float d, float e, float f) override {}
  void translate(float x, float y) override {}
  void rotate(float angle) override {}
  void skewX(float angle) override {}
  void skewY(float angle) override {}
  void scale(float x, float y) override {}
  void currentTransform(float* xform) override {}
  void transformIdentity(float* dst) override {}
  void transformTranslate(float* dst, float tx, float ty) override {}
  void transformScale(float* dst, float sx, float sy) override {}
  void transformRotate(float* dst, float a) override {}
  void transformSkewX(float* dst, float a) override {}
  void transformSkewY(float* dst, float a) override {}
  void transformMultiply(float* dst, const float* src) override {}
  void transformPremultiply(float* dst, const float* src) override {}
  int transformInverse(float* dst, const float* src) override { return 0; }
  void transformPoint(float* dstx, float* dsty, const float* xform, float srcx, float srcy) override {}
  float degToRad(float deg) override { return 0.0; }
  float radToDeg(float rad) override { return 0.0; }

//------------------------------
public: // images
//------------------------------

  int createImage(const char* filename, int imageFlags) override { return 0; }

  //----------

  int createImageMem(int imageFlags, unsigned char* data, int ndata) override { return 0; }

  //----------

  int createImageRGBA(int w, int h, int imageFlags, const unsigned char* data) override { return 0; }

  //----------

  void updateImage(int image, const unsigned char* data) override {}

  //----------

  void imageSize(int image, int* w, int* h) override {}

  //----------

  void deleteImage(int image) override {}

//------------------------------
public: // paints
//------------------------------

  MIP_PaintSource linearGradient(float sx, float sy, float ex, float ey, MIP_Color icol, MIP_Color ocol) override { return MIP_PaintSource(); }

  //----------

  MIP_PaintSource boxGradient(float x, float y, float w, float h, float r, float f, MIP_Color icol, MIP_Color ocol) override { return MIP_PaintSource(); }

  //----------

  MIP_PaintSource radialGradient(float cx, float cy, float inr, float outr, MIP_Color icol, MIP_Color ocol) override { return MIP_PaintSource(); }

  //----------

  MIP_PaintSource imagePattern(float ox, float oy, float ex, float ey, float angle, int image, float alpha) override { return MIP_PaintSource(); }

//------------------------------
public: // scissoring
//------------------------------

  // Scissoring

  void scissor(float x, float y, float w, float h) override {}

  //----------

  void intersectScissor(float x, float y, float w, float h) override {}

  //----------

  void resetScissor() override {}

//------------------------------
public: // paths
//------------------------------

  void beginPath() override {
    MPath.clear(false);
  }

  //----------

  void moveTo(float x, float y) override {
    MIP_PathItem item;
    item.type = MIP_PATH_MOVE_TO;
    item.data[0]  = x;
    item.data[1]  = y;
    MPath.append(item);
  }

  //----------

  void lineTo(float x, float y) override {
    MIP_PathItem item;
    item.type = MIP_PATH_LINE_TO;
    item.data[0]  = x;
    item.data[1]  = y;
    MPath.append(item);
  }

  //----------

  void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) override {
    MIP_PathItem item;
    item.type = MIP_PATH_BEZIER_TO;
    item.data[0]  = c1x;
    item.data[1]  = c1y;
    item.data[2]  = c2y;
    item.data[3]  = c2y;
    item.data[4]  = x;
    item.data[5]  = y;
    MPath.append(item);
  }

  //----------

  void quadTo(float cx, float cy, float x, float y) override {
    MIP_PathItem item;
    item.type = MIP_PATH_QUAD_TO;
    item.data[0]  = cx;
    item.data[1]  = cy;
    item.data[2]  = x;
    item.data[3]  = y;
    MPath.append(item);
  }

  //----------

  void arcTo(float x1, float y1, float x2, float y2, float radius) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ARC_TO;
    item.data[0]  = x1;
    item.data[1]  = y1;
    item.data[2]  = x2;
    item.data[3]  = y2;
    item.data[4]  = radius;
    MPath.append(item);
  }

  //----------

  void closePath() override {
  }

  //----------

  void pathWinding(int dir) override {
  }

  //----------

  void arc(float cx, float cy, float r, float a0, float a1, int dir) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ARC;
    item.data[0]  = cx;
    item.data[1]  = cy;
    item.data[2]  = r;
    item.data[3]  = a0;
    item.data[4]  = a1;
    item.data[5]  = dir;
    MPath.append(item);
  }

  //----------

  void rect(float x, float y, float w, float h) override {
    MIP_PathItem item;
    item.type = MIP_PATH_RECT;
    item.data[0]  = x;
    item.data[1]  = y;
    item.data[2]  = w;
    item.data[3]  = h;
    MPath.append(item);
  }

  //----------

  void roundedRect(float x, float y, float w, float h, float r) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ROUNDED_RECT;
    item.data[0]  = x;
    item.data[1]  = y;
    item.data[2]  = w;
    item.data[3]  = h;
    item.data[4]  = r;
    MPath.append(item);

  }

  //----------

  void roundedRectVarying(float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ROUNDED_RECT_VARYING;
    item.data[0]  = x;
    item.data[1]  = y;
    item.data[2]  = w;
    item.data[3]  = h;
    item.data[4]  = radTopLeft;
    item.data[5]  = radTopRight;
    item.data[6]  = radBottomRight;
    item.data[7]  = radBottomLeft;
    MPath.append(item);
  }

  //----------

  void ellipse(float cx, float cy, float rx, float ry) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ELLIPSE;
    item.data[0]  = cx;
    item.data[1]  = cy;
    item.data[2]  = rx;
    item.data[3]  = ry;
    MPath.append(item);
  }

  //----------

  void circle(float cx, float cy, float r) override {
    MIP_PathItem item;
    item.type = MIP_PATH_CIRCLE;
    item.data[0]  = cx;
    item.data[1]  = cy;
    item.data[2]  = r;
    MPath.append(item);
  }

  //----------

  void fill() override {
    for (uint32_t i=0; i<MPath.size(); i++) {
      switch (MPath[i].type) {

        case MIP_PATH_MOVE_TO: {
          MPathXpos = MPath[i].data[0];
          MPathYpos = MPath[i].data[1];
          break;
        }

        case MIP_PATH_LINE_TO: {
          //..
          MPathXpos = MPath[i].data[0];
          MPathYpos = MPath[i].data[1];
          break;
        }

        case MIP_PATH_BEZIER_TO: {
          break;
        }

        case MIP_PATH_QUAD_TO: {
          break;
        }

        case MIP_PATH_ARC_TO: {
          break;
        }

        case MIP_PATH_ARC: {
          break;
        }

        /*
          The FillRect function fills a rectangle by using the specified brush.
          This function includes the left and top borders, but excludes the
          right and bottom borders of the rectangle.
        */

        case MIP_PATH_RECT: {
          //if (MFillPaint.image >= 0) {
          //}
          //else {
          RECT R;
          R.left    = MPath[i].data[0];
          R.top     = MPath[i].data[1];
          R.right   = MPath[i].data[0] + MPath[i].data[2] + 1;
          R.bottom  = MPath[i].data[1] + MPath[i].data[3] + 1;
          //HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
          //FillRect(MHandle,&R,brush);
          FillRect(MHandle,&R,MBrushHandle);
          //Rectangle(mDC,aX1,aY1,aX2,aY2);

          break;
        }

        case MIP_PATH_ROUNDED_RECT: {
          //TODO:
          RECT R;
          R.left    = MPath[i].data[0];
          R.top     = MPath[i].data[1];
          R.right   = MPath[i].data[0] + MPath[i].data[2] + 1;
          R.bottom  = MPath[i].data[1] + MPath[i].data[3] + 1;
          //HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
          //FillRect(MHandle,&R,brush);
          FillRect(MHandle,&R,MBrushHandle);
          //Rectangle(mDC,aX1,aY1,aX2,aY2);
          //float r   = MPath[i].data[4];// - 1;
          //float r2  = r * 2;
          //float AX1 = MPath[MPathLength].data[0];
          //float AY1 = MPath[MPathLength].data[0];
          //float AX2 = MPath[MPathLength].data[0] + MPath[MPathLength].data[2];
          //float AY2 = MPath[MPathLength].data[0] + MPath[MPathLength].data[2];
          //fillArc(       MIP_FRect(AX1-1,  AY1-1,   AX1+r2,   AY1+r2),   0.75, 0.25, AColor ); // upper left
          //fillArc(       MIP_FRect(AX2-r2, AY1-1,   AX2,      AY1+r2-1), 0.00, 0.25, AColor ); // upper right
          //fillArc(       MIP_FRect(AX1-1,  AY2-r2,  AX1+r2-1, AY2),      0.50, 0.25, AColor ); // lower left
          //fillArc(       MIP_FRect(AX2-r2, AY2-r2,  AX2,      AY2),      0.25, 0.25, AColor ); // lower right
          //fillRectangle( MIP_FRect(AX1+r,  AY1,     AX2-r,    AY1+r-1), AColor );  // top
          //fillRectangle( MIP_FRect(AX1,    AY1+r,   AX2,      AY2-r),   AColor );  // mid
          //fillRectangle( MIP_FRect(AX1+r,  AY2-r+1, AX2-r,    AY2),     AColor );  // bot
          break;
        }

        case MIP_PATH_ROUNDED_RECT_VARYING: {
          //TODO:
          RECT R;
          R.left    = MPath[i].data[0];
          R.top     = MPath[i].data[1];
          R.right   = MPath[i].data[0] + MPath[i].data[2] + 1;
          R.bottom  = MPath[i].data[1] + MPath[i].data[3] + 1;
          //HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
          //FillRect(MHandle,&R,brush);
          FillRect(MHandle,&R,MBrushHandle);
          //Rectangle(mDC,aX1,aY1,aX2,aY2);
        }

        case MIP_PATH_ELLIPSE: {
          break;
        }

        case MIP_PATH_CIRCLE: {
          break;
        }

      }
    }
    MPath.clear(false);
    MFillPaint = {};
  }

  //----------

  void stroke() override {
    for (uint32_t i=0; i<MPath.size(); i++) {
      switch (MPath[i].type) {

        case MIP_PATH_MOVE_TO: {
          MPathXpos = MPath[i].data[0];
          MPathYpos = MPath[i].data[1];
          break;
        }

        case MIP_PATH_LINE_TO: {
          MPathXpos = MPath[i].data[0];
          MPathYpos = MPath[i].data[1];
          break;
        }

        case MIP_PATH_BEZIER_TO: {
          break;
        }

        case MIP_PATH_QUAD_TO: {
          break;
        }

        case MIP_PATH_ARC_TO: {
          break;
        }

        case MIP_PATH_ARC: {
          break;
        }

        case MIP_PATH_RECT: {
          //TODO
          float x = MPath[i].data[0];
          float y = MPath[i].data[1];
          float w = MPath[i].data[2];
          float h = MPath[i].data[3];
          Rectangle(MHandle,x,y,x+w+1,y+h+1);
          break;
        }

        case MIP_PATH_ROUNDED_RECT: {
          //TODO
          float x = MPath[i].data[0];
          float y = MPath[i].data[1];
          float w = MPath[i].data[2];
          float h = MPath[i].data[3];
          Rectangle(MHandle,x,y,x+w+1,y+h+1);
          break;
        }

        case MIP_PATH_ROUNDED_RECT_VARYING: {
          //TODO
          float x = MPath[i].data[0];
          float y = MPath[i].data[1];
          float w = MPath[i].data[2];
          float h = MPath[i].data[3];
          Rectangle(MHandle,x,y,x+w+1,y+h+1);
          break;
        }

        case MIP_PATH_ELLIPSE: {
          break;
        }

        case MIP_PATH_CIRCLE: {
          break;
        }

      }
    }
    MPath.clear(false);
    MStrokePaint = {};
  }

//------------------------------
public: // text
//------------------------------

  int createFont(const char* name, const char* filename) override { return 0; }
  int createFontAtIndex(const char* name, const char* filename, const int fontIndex) override { return 0; }
  int createFontMem(const char* name, unsigned char* data, int ndata, int freeData) override { return 0; }
  int createFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) override { return 0; }
  int findFont(const char* name) override { return 0; }
  int addFallbackFontId(int baseFont, int fallbackFont) override { return 0; }
  int addFallbackFont(const char* baseFont, const char* fallbackFont) override { return 0; }
  void resetFallbackFontsId(int baseFont) override {}
  void resetFallbackFonts(const char* baseFont) override {}
  void fontSize(float size) override {}
  void fontBlur(float blur) override {}
  void textLetterSpacing(float spacing) override {}
  void textLineHeight(float lineHeight) override {}
  void textAlign(int align) override {}
  void fontFaceId(int font) override {}
  void fontFace(const char* font) override {}
  float text(float x, float y, const char* string, const char* end) override { return 0.0; }
  void textBox(float x, float y, float breakRowWidth, const char* string, const char* end) override {}
  float textBounds(float x, float y, const char* string, const char* end, float* bounds) override { return 0.0; }
  void textBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) override {}
  //int textGlyphPositions(float x, float y, const char* string, const char* end, NVGglyphPosition* positions, int maxPositions) override { return 0; }
  int textGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) override { return 0; }
  void textMetrics(float* ascender, float* descender, float* lineh) override {}
  int textBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) override { return 0; }

//------------------------------
private:
//------------------------------

  int32_t _get_text_width(const char* AText) {
    SIZE S;
    GetTextExtentPoint32(MHandle,AText,strlen(AText),&S);
    return S.cx;
  }

  //----------

  int32_t _get_text_height(const char* AText) {
    SIZE S;
    GetTextExtentPoint32(MHandle,AText,strlen(AText),&S);
    return S.cy;
  }

  //----------

  /*

  // APen = nullptr -> invisible pen
  void _select_pen(S3_Pen* APen, bool ARemember=true) {
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

  void _reset_pen(void) {
    SelectObject(MHandle,MOldPen);
  }

  //----------

  // ABrush = S3_NULL -> invisible brush
  void _select_brush(S3_Brush* ABrush, bool ARemember=true) {
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

  void _reset_brush(void) {
    SelectObject(MHandle,MOldBrush);
  }

  //----------

  void _select_font(S3_Font* AFont, bool ARemember=true) {
    MTextColor = AFont->textColor();
    if (ARemember) {
      MOldFont = SelectObject(MHandle,AFont->hfont());
    }
    else {
      SelectObject(MHandle,AFont->hfont());
    }
  }

  //----------

  void _reset_font(void) {
    SelectObject(MHandle,MOldFont);
  }

  //----------

  // https://msdn.microsoft.com/en-us/library/windows/desktop/dd162957%28v=vs.85%29.aspx
  // Bitmaps can only be selected into memory DC's. A single bitmap cannot be
  // selected into more than one DC at the same time.

  void _select_drawable(S3_Drawable* ADrawable, bool ARemember=true) {
    HBITMAP hbitmap = ADrawable->hbitmap();
    if (hbitmap) {
      if (ARemember) MOldBitmap = SelectObject(MHandle,hbitmap);
      else SelectObject(MHandle,hbitmap);
    }
  }

  //----------

  void _reset_drawable(void) {
    SelectObject(MHandle,MOldBitmap);
  }

  */

  //----------

  //    void setDrawColor(S3_Color AColor) {
  //      MDrawColor = AColor;
  //      SetDCPenColor(MHandle,S3_RGB(MDrawColor));
  //    }
  //
  //    //----------
  //
  //    void setFillColor(S3_Color AColor) {
  //      MFillColor = AColor;
  //      SetDCBrushColor(MHandle,S3_RGB(MFillColor));
  //    }
  //
  //    //----------
  //
  //    void setTextColor(S3_Color AColor) {
  //      MTextColor = AColor;
  //      SetTextColor(MHandle,S3_RGB(MTextColor));
  //    }

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

};

//----------------------------------------------------------------------
#endif












#if 0

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



































// qwe










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


};


#endif // 0
