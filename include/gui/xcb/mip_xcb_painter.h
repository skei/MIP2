#ifndef mip_xcb_painter_included
#define mip_xcb_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/base/mip_base_painter.h"
#include "gui/xcb/mip_xcb.h"
#include "gui/xcb/mip_xcb_utils.h"
#include "gui/mip_drawable.h"

#ifdef MIP_XCB_USE_STB_TRUETYPE
  #define STB_TRUETYPE_IMPLEMENTATION
  #include "extern/stb/stb_truetype.h"
  #include "../data/fonts/liberation_ttf.h"
#endif



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
//
//
//
//----------------------------------------------------------------------

class MIP_XcbPainter
: public MIP_BasePainter {

//------------------------------
protected:
//------------------------------

  MIP_Drawable*     MSurface                  = nullptr;
  MIP_Drawable*     MTarget                   = nullptr;

  xcb_connection_t* MConnection               = nullptr;
  xcb_visualid_t    MVisual                   = XCB_NONE;
  xcb_drawable_t    MDrawable                 = XCB_NONE;
  xcb_gcontext_t    MGC                       = XCB_NONE;
//  xcb_font_t        MFont                     = XCB_NONE;

  uint32_t          MWidth                    = 0;
  uint32_t          MHeight                   = 0;

  int32_t           MFontAscent               = 0;
  int32_t           MFontDescent              = 0;
  int32_t           MFontWidth                = 0;
  int32_t           MFontHeight               = 0;
  int32_t           MFontOrigin               = 0;
  int32_t           MFontLeft                 = 0;
  int32_t           MFontRight                = 0;

  int               MShapeAntiAlias;
  float             MMiterLimit;
  int               MLineCap;
  int               MLineJoin;
  float             MGlobalAlpha;

  MIP_Color         MStrokeColor                = MIP_Color(0);//MIP_COLOR_BLACK;
  MIP_PaintSource   MStrokePaint                = {};
  float             MStrokeWidth                = 1.0;
  MIP_Color         MFillColor                  = MIP_Color(1);//MIP_COLOR_WHITE;
  MIP_PaintSource   MFillPaint                  = {};
  int               MFont                       = -1;
  MIP_Color         MFontColor                  = MIP_Color(0);//MIP_COLOR_BLACK;
  float             MFontSize                   = 13.0;

  //MIP_PathItem      MPath[MIP_PATH_MAX_LENGTH]  = {};
  MIP_PathItemArray  MPath;
  uint32_t          MPathLength                 = 0;
  float             MPathXpos                   = 0.0f;
  float             MPathYpos                   = 0.0f;

  //MIP_PaintSource   MImages[MIP_XCB_PAINTER_MAX_IMAGES] = {0};
  MIP_Bitmap*         MImages[MIP_XCB_PAINTER_MAX_IMAGES] = {0};

  #ifdef MIP_XCB_USE_STB_TRUETYPE
  stbtt_fontinfo*     MFonts[MIP_XCB_PAINTER_MAX_FONTS]   = {0};
  #endif

//------------------------------
public:
//------------------------------

  // ASurface = surface to paint to
  // ATarget  = target window/visual

  MIP_XcbPainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget)
  : MIP_BasePainter(ASurface,ATarget) {
    if (ATarget->drawable_isDrawable()) {
      MTarget     = ATarget;
      MConnection = ATarget->drawable_getXcbConnection();
      MVisual     = ATarget->drawable_getXcbVisual();
      MDrawable   = ATarget->drawable_getXcbDrawable();
      MWidth      = ATarget->drawable_getWidth();
      MHeight     = ATarget->drawable_getHeight();
      MGC         = xcb_generate_id(MConnection);
      uint32_t mask =
        //XCB_GC_FUNCTION
        //XCB_GC_PLANE_MASK
        //XCB_GC_FOREGROUND
        //XCB_GC_BACKGROUND
        //XCB_GC_LINE_WIDTH
        //XCB_GC_LINE_STYLE
        //XCB_GC_CAP_STYLE
        //XCB_GC_JOIN_STYLE
        //XCB_GC_FILL_STYLE
        //XCB_GC_FILL_RULE
        //XCB_GC_TILE
        //XCB_GC_STIPPLE
        //XCB_GC_TILE_STIPPLE_ORIGIN_X
        //XCB_GC_TILE_STIPPLE_ORIGIN_Y
        //XCB_GC_FONT |
        //XCB_GC_SUBWINDOW_MODE
        XCB_GC_GRAPHICS_EXPOSURES;
        //XCB_GC_CLIP_ORIGIN_X
        //XCB_GC_CLIP_ORIGIN_Y
        //XCB_GC_CLIP_MASK
        //XCB_GC_DASH_OFFSET
        //XCB_GC_DASH_LIST
        //XCB_GC_ARC_MODE
      uint32_t values[] = {
        0
      };
      xcb_create_gc(MConnection,MGC,MDrawable,mask,values);
      xcb_flush(MConnection);
    }

    #ifdef MIP_XCB_USE_STB_TRUETYPE
      int font = createFontMem("font1",(unsigned char*)liberation_ttf,liberation_ttf_size,0);
      fontFaceId(font);
    #endif

  }

  //----------

  virtual ~MIP_XcbPainter() {
    xcb_free_gc(MConnection,MGC);
    for (uint32_t i=0; i<MIP_XCB_PAINTER_MAX_IMAGES; i++) {
      if (MImages[i]) delete MImages[i];
    }
    #ifdef MIP_XCB_USE_STB_TRUETYPE
      for (uint32_t i=0; i<MIP_XCB_PAINTER_MAX_FONTS; i++) {
        if (MFonts[i]) free(MFonts[i]);
      }
    #endif
  }

//------------------------------
public:
//------------------------------

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //MIP_Print("beginPaint: %i,%i,%i,%i\n",AXpos,AYpos,AWidth,AHeight);
  }

  //----------

  void endPaint() override {
  }

  //----------

  /*
    The XSetClipRectangles() function changes the clip-mask in the specified GC
    to the specified list of rectangles and sets the clip origin. The output is
    clipped to remain contained within the rectangles. The clip-origin is
    interpreted relative to the origin of whatever destination drawable is
    specified in a graphics request. The rectangle coordinates are interpreted
    relative to the clip-origin. The rectangles should be nonintersecting, or
    the graphics results will be undefined. Note that the list of rectangles
    can be empty, which effectively disables output. This is the opposite of
    passing None as the clip-mask in XCreateGC(), XChangeGC(), and
    XSetClipMask().
  */

  void setClip(MIP_DRect ARect) override {
    //MIP_Print("ARect: %.0f,%.0f,%.0f,%.0f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    //resetClip();
    xcb_rectangle_t rectangles[] = {{
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)(ARect.w + 1),
      (uint16_t)(ARect.h + 1),
    }};
    xcb_set_clip_rectangles(
      MConnection,
      XCB_CLIP_ORDERING_UNSORTED, // ordering,
      MGC,
      0, // clip_x_origin
      0, // clip_y_origin
      1, // rectangles_len
      rectangles
    );
    xcb_flush(MConnection);
  }

  //----------

  void resetClip() override {
    uint32_t mask = XCB_GC_CLIP_MASK;
    uint32_t values[1];
    values[0] = XCB_NONE;
    xcb_change_gc(MConnection, MGC, mask, values);
    xcb_flush(MConnection);

  }

  //--------------------
  // frame
  //--------------------

  void beginFrame(float windowWidth, float windowHeight, float devicePixelRatio) override {
    //MIP_Print("%.f,%.f\n",windowWidth,windowHeight);
  }

  //----------

  void cancelFrame() override {
  }

  //----------

  void endFrame() override {
    //xcb_flush(MConnection);
  }

  //--------------------
  // Composite operation
  //--------------------

  void globalCompositeOperation(int op) override {
  }

  //----------

  void globalCompositeBlendFunc(int sfactor, int dfactor) override {
  }

  //----------

  void globalCompositeBlendFuncSeparate(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha) override {
  }

  //--------------------
  // State Handling
  //--------------------

  void save() override {
  }

  //----------

  void restore() override {
  }

  //----------

  void reset() override {
  }

  //--------------------
  // Render styles
  //--------------------

  void shapeAntiAlias(int enabled) override {
    MShapeAntiAlias = enabled;
  }

  //----------

  void strokeColor(MIP_Color color) override {
    MStrokeColor = color;
    _set_color(MStrokeColor);
  }

  //----------

  void strokePaint(MIP_PaintSource paint) override {
  //void strokePaint(int paint) override {
    MStrokePaint = paint;
  }

  //----------

  void fillColor(MIP_Color color) override {
    MFillColor = color;
    _set_color(MFillColor);
    //_set_background_color(MFillColor);
  }

  //----------

  void fillPaint(MIP_PaintSource paint) override {
    MFillPaint = paint;
  }

  //----------

  void miterLimit(float limit) override {
    MMiterLimit = limit;
  }

  //----------

  void strokeWidth(float size) override {
    if (size < 1.0) size = 1.0;
    MStrokeWidth = size;
    _set_line_width(MStrokeWidth);
  }

  //----------

  void lineCap(int cap) override {
    MLineCap = cap;
  }

  //----------

  void lineJoin(int join) override {
    MLineJoin = join;
  }

  //----------

  void globalAlpha(float alpha) override {
    MGlobalAlpha = alpha;
  }

  //--------------------
  // Transforms
  //--------------------

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

  //--------------------
  // Images
  //--------------------

  int createImage(const char* filename, int imageFlags) override {
    return 0;
  }

  //----------

  int createImageMem(int imageFlags, unsigned char* data, int ndata) override {
    return 0;
  }

  //----------

  int createImageRGBA(int w, int h, int imageFlags, const unsigned char* data) override {
    for (int i=0; i<MIP_XCB_PAINTER_MAX_IMAGES; i++) {
      if (MImages[i] == nullptr) {
        //MIP_Print("setup image %i\n",i);
        MImages[i] = new MIP_Bitmap((uint32_t)w,(uint32_t)h,(uint32_t*)data);
        return i;
      }
    }
    return -1;
  }

  //----------

  void updateImage(int image, const unsigned char* data) override {
  }

  //----------

  void imageSize(int image, int* w, int* h) override {
    *w = MImages[image]->getWidth();
    *h = MImages[image]->getHeight();
  }

  //----------

  void deleteImage(int image) override {
    delete MImages[image];
  }

  //--------------------
  // Paints
  //--------------------

  MIP_PaintSource linearGradient(float sx, float sy, float ex, float ey, MIP_Color icol, MIP_Color ocol) override {
    return MIP_PaintSource();
  }

  //----------

  MIP_PaintSource boxGradient(float x, float y, float w, float h, float r, float f, MIP_Color icol, MIP_Color ocol) override {
    return MIP_PaintSource();
  }

  //----------

  MIP_PaintSource radialGradient(float cx, float cy, float inr, float outr, MIP_Color icol, MIP_Color ocol) override {
    return MIP_PaintSource();
  }

  //----------

  MIP_PaintSource imagePattern(float ox, float oy, float ex, float ey, float angle, int image, float alpha) override {
    //MIP_PRINT;
    MIP_PaintSource ps;
    ps.image = image;
    return ps;
  }

  //--------------------
  // Scissoring
  //--------------------

  void scissor(float x, float y, float w, float h) override {
  }

  //----------

  void intersectScissor(float x, float y, float w, float h) override {
  }

  //----------

  void resetScissor() override {
  }

  //--------------------
  // Paths
  //--------------------

  void beginPath() override {
    MPathLength = 0;
    MFillPaint = {};//
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
          xcb_point_t polyline[] =  {
            (int16_t)MPathXpos, (int16_t)MPathYpos,
            (int16_t)MPath[i].data[0], (int16_t)MPath[i].data[1],
          };
          xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,2,polyline);
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
          float x = MPath[i].data[0];
          float y = MPath[i].data[1];
          float w = MPath[i].data[2];
          float h = MPath[i].data[3];

          if (MFillPaint.image >= 0) {
            //MIP_PRINT;
            MIP_Bitmap* bitmap  = MImages[MFillPaint.image];
            _draw_buffer(bitmap->getBuffer(),x,y,w,h);

//----------

            uint32_t width      = bitmap->getWidth();
            uint32_t height     = bitmap->getHeight();
            uint32_t buffersize = bitmap->getBufferSize();
            uint32_t* buffer    = bitmap->getBuffer();

            xcb_image_t* image = xcb_image_create(
              width,                          // width      width in pixels.
              height,                         // height     height in pixels.
              XCB_IMAGE_FORMAT_Z_PIXMAP,      // format
              32,                             // xpad       scanline pad (8,16,32)
              24,                             // depth      (1,4,8,16,24 zpixmap),    (1 xybitmap), (anything xypixmap)
              32,                             // bpp        (1,4,8,16,24,32 zpixmap,  (1 xybitmap), (anything xypixmap)
              32,                             // unit       unit of image representation, in bits (8,16,32)
              XCB_IMAGE_ORDER_LSB_FIRST,      // byte_order
              XCB_IMAGE_ORDER_LSB_FIRST,      // bit_order
              buffer,                         // base       The base address of malloced image data
              buffersize,                     // bytes      The size in bytes of the storage pointed to by base.
                                              //            If base == 0 and bytes == ~0 and data == 0, no storage will be auto-allocated.
              (uint8_t*)buffer                // data       The image data. If data is null and bytes != ~0, then an attempt will be made
                                              //            to fill in data; from base if it is non-null (and bytes is large enough), else
                                              //            by mallocing sufficient storage and filling in base.
            );
            xcb_image_put(
              MConnection,            // xcb_connection_t*  conn,
              MDrawable,              // xcb_drawable_t     draw,
              MGC,                    // xcb_gcontext_t     gc,
              image,                  // xcb_image_t*       image,
              x,                      // int16_t            x,
              y,                      // int16_t            y,
              0                       // uint8_t            left_pad
            );

//----------

            image->base = nullptr;
            xcb_image_destroy(image);
            xcb_flush(MConnection);
          }
          else {
            //if ((ARect.w <= 0) || (ARect.h <= 0)) return;
            xcb_rectangle_t rectangles[] = {{
              (int16_t)x,//MPath[i].data[0],
              (int16_t)y,//MPath[i].data[1],
              (uint16_t)w,//MPath[i].data[2],
              (uint16_t)h,//MPath[i].data[3],
            }};
            xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
          }
          break;
        }

        //    MPath[MPathLength].data[0]  = x;
        //    MPath[MPathLength].data[1]  = y;
        //    MPath[MPathLength].data[2]  = w;
        //    MPath[MPathLength].data[3]  = h;
        //    MPath[MPathLength].data[4]  = r;

        case MIP_PATH_ROUNDED_RECT: {
          //TODO:
          xcb_rectangle_t rectangles[] = {{
            (int16_t)MPath[i].data[0],
            (int16_t)MPath[i].data[1],
            (uint16_t)MPath[i].data[2],
            (uint16_t)MPath[i].data[3],
          }};
          xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
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
          xcb_rectangle_t rectangles[] = {{
            (int16_t)MPath[i].data[0],
            (int16_t)MPath[i].data[1],
            (uint16_t)MPath[i].data[2],
            (uint16_t)MPath[i].data[3],
          }};
          xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
          break;
        }

        case MIP_PATH_ELLIPSE: {
          //    set_color(AColor);
          //    xcb_arc_t arcs[] = {
          //      (int16_t)ARect.x,
          //      (int16_t)ARect.y,
          //      (uint16_t)ARect.w, // +1,
          //      (uint16_t)ARect.h, // +1,
          //      (int16_t)(0),
          //      (int16_t)(360 * 64)
          //    };
          //    xcb_poly_fill_arc(MConnection, MDrawable, MGC, 1, arcs );
          break;
        }

        case MIP_PATH_CIRCLE: {
          break;
        }

      }
    }
    //MPathLength = 0;
    MPath.clear(false);
  }

  //----------

  void stroke() override {
    //MIP_Print("MPathLength: %i\n",MPathLength);

    for (uint32_t i=0; i<MPath.size(); i++) {
      switch (MPath[i].type) {

        case MIP_PATH_MOVE_TO: {
          MPathXpos = MPath[i].data[0];
          MPathYpos = MPath[i].data[1];
          break;
        }

        case MIP_PATH_LINE_TO: {
          xcb_point_t polyline[] =  {
            (int16_t)MPathXpos, (int16_t)MPathYpos,
            (int16_t)MPath[i].data[0], (int16_t)MPath[i].data[1],
          };
          xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,2,polyline);
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
          float a1 = MIP_PI2 - MPath[i].data[3];
          float a2 = - MPath[i].data[4];
          float x  = MPath[i].data[0] - MPath[i].data[2];
          float y  = MPath[i].data[1] - MPath[i].data[2];
          float w  = MPath[i].data[2] * 2.0;
          float h  = MPath[i].data[2] * 2.0;
          xcb_arc_t arcs[] = {
            (int16_t)x,
            (int16_t)y,
            (uint16_t)w,
            (uint16_t)h,
            (int16_t)(a1 * 360.0f * 64.0f * MIP_INVPI2),
            (int16_t)(a2 * 360.0f * 64.0f * MIP_INVPI2)
          };
          xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
          break;
        }

        case MIP_PATH_RECT: {
          xcb_rectangle_t rectangles[] = {{
            (int16_t)MPath[i].data[0],
            (int16_t)MPath[i].data[1],
            (uint16_t)MPath[i].data[2],
            (uint16_t)MPath[i].data[3],
          }};
          xcb_poly_rectangle(MConnection,MDrawable,MGC,1,rectangles);
          break;
        }

        case MIP_PATH_ROUNDED_RECT: {
          //TODO
          xcb_rectangle_t rectangles[] = {{
            (int16_t)MPath[i].data[0],
            (int16_t)MPath[i].data[1],
            (uint16_t)MPath[i].data[2],
            (uint16_t)MPath[i].data[3],
          }};
          xcb_poly_rectangle(MConnection,MDrawable,MGC,1,rectangles);
          //    float r  = ARadius;// - 1;
          //    float r2 = r*2;
          //    float AX1 = ARect.x;
          //    float AY1 = ARect.y;
          //    float AX2 = ARect.x2();
          //    float AY2 = ARect.y2();
          //    drawArc(  MIP_FRect(AX1,      AY1,      AX1+r2-2, AY1+r2-3), 0.75, 0.25, AColor, AWidth ); // upper left
          //    drawArc(  MIP_FRect(AX2-r2+1, AY1,      AX2-1,    AY1+r2-2), 0.00, 0.25, AColor, AWidth ); // upper right
          //    drawArc(  MIP_FRect(AX1,      AY2-r2+1, AX1+r2-2, AY2-1),    0.50, 0.25, AColor, AWidth ); // lower left
          //    drawArc(  MIP_FRect(AX2-r2+1, AY2-r2+2, AX2-1,    AY2-1),    0.25, 0.25, AColor, AWidth ); // lower right
          //    drawLine( AX1+r,    AY1,      AX2-r,    AY1,   AColor, AWidth );  // top
          //    drawLine( AX1+r,    AY2,      AX2-r,    AY2,   AColor, AWidth );  // bottom
          //    drawLine( AX1,      AY1+r,    AX1,      AY2-r, AColor, AWidth );  // left
          //    drawLine( AX2,      AY1+r,    AX2,      AY2-r, AColor, AWidth );  // right
          break;
        }

        case MIP_PATH_ROUNDED_RECT_VARYING: {
          //TODO
          xcb_rectangle_t rectangles[] = {{
            (int16_t)MPath[i].data[0],
            (int16_t)MPath[i].data[1],
            (uint16_t)MPath[i].data[2],
            (uint16_t)MPath[i].data[3],
          }};
          xcb_poly_rectangle(MConnection,MDrawable,MGC,1,rectangles);
          break;
        }

        case MIP_PATH_ELLIPSE: {
          //    set_color(AColor);
          //    set_line_width(AWidth);
          //    xcb_arc_t arcs[] = {
          //      (int16_t)ARect.x,
          //      (int16_t)ARect.y,
          //      (uint16_t)ARect.w, // +1
          //      (uint16_t)ARect.h, // +1
          //      0,
          //      360 * 64
          //    };
          //    xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
          break;
        }

        case MIP_PATH_CIRCLE: {
          break;
        }

      }
    }
    //MPathLength = 0;
    MPath.clear(false);
  }

  //--------------------
  // Text
  //--------------------

  int createFont(const char* name, const char* filename) override {
    MIP_PRINT;
    // load font
    long size;
    unsigned char* fontBuffer;
    FILE* fontFile = fopen(filename, "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */
    fontBuffer = (unsigned char*)malloc(size);
    long num_read = fread(fontBuffer, size, 1, fontFile);
    if (num_read != size) {
      MIP_Print("error reading font\n");
    }
    fclose(fontFile);
    int font = createFontMem("font1",fontBuffer,size,0);
//    free(fontBuffer);
    return font;
  }

  //----------

  int createFontAtIndex(const char* name, const char* filename, const int fontIndex) override {
    return -1;
  }

  //----------

  int createFontMem(const char* name, unsigned char* data, int ndata, int freeData) override {
    MIP_PRINT;
    #ifdef MIP_XCB_USE_STB_TRUETYPE
    for (int i=0; i<MIP_XCB_PAINTER_MAX_FONTS; i++) {
      if (MFonts[i] == nullptr) {
        MFonts[i] = (stbtt_fontinfo*)malloc(sizeof(stbtt_fontinfo));
        if (!stbtt_InitFont(MFonts[i],data,0)) {
          printf("Error loading font\n");
        }
        return i;
      }
    }
    #endif
    return -1;
  }

  //----------

  int createFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) override {
    return 0;
  }

  //----------

  int findFont(const char* name) override {
    MIP_PRINT;
    return 0;
  }

  //----------

  int addFallbackFontId(int baseFont, int fallbackFont) override {
    return 0;
  }

  //----------

  int addFallbackFont(const char* baseFont, const char* fallbackFont) override {
    return 0;
  }

  //----------

  void resetFallbackFontsId(int baseFont) override {
  }

  //----------

  void resetFallbackFonts(const char* baseFont) override {
  }

  //----------

  void fontSize(float size) override {
    MFontSize = size;
  }

  //----------

  void fontBlur(float blur) override {
  }

  //----------

  void textLetterSpacing(float spacing) override {
  }

  //----------

  void textLineHeight(float lineHeight) override {
  }

  //----------

  void textAlign(int align) override {
  }

  //----------

  void fontFaceId(int font) override {
    MFont = font;
  }

  //----------

  void fontFace(const char* font) override {
    //todo
    MFont = 0;
  }

  //----------

  float text(float x, float y, const char* string, const char* end) override {
    //MIP_PRINT;
    _measure_string(string);
    //
    #ifdef MIP_XCB_USE_STB_TRUETYPE
      //unsigned char* textbuffer = _render_string(string);
      //if (textbuffer) {
      //  _draw_buffer((uint32_t*)textbuffer,x,y,MFontWidth,MFontHeight);
      //  free (textbuffer);
      //}
    #else
      uint8_t buffer[512];
      MIP_XcbPolyText8 pt;
      pt.data = buffer;
      pt.used = 0;
      mip_xcb_add_string_text8(&pt,string);
      xcb_poly_text_8(MConnection,MDrawable,MGC,x,y + MFontAscent,pt.used,pt.data);
    #endif
    return 0.0;
  }

  //----------

  void textBox(float _x, float _y, float breakRowWidth, const char* string, const char* end) override {
  }

  //----------

  float textBounds(float x, float y, const char* string, const char* end, float* bounds) override {
    //MIP_PRINT;
    _measure_string(string);
    //MIP_Print("MFontHeight: %i\n",MFontHeight);
    bounds[0] = x;
    bounds[1] = y;
    bounds[2] = x + (float)MFontWidth;
    bounds[3] = y + (float)MFontHeight;//MFontHeight + MFontSize + MFontDescent;//Height;
    return 1.0;
  }

  //----------

  void textBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) override {
  }

  //----------

  int textGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) override {
    return 0;
  }

  //----------

  void textMetrics(float* ascender, float* descender, float* lineh) override {
  }

  //----------

  int textBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) override {
    return 0;
  }

//------------------------------
private:
//------------------------------

//  void resize(uint32_t AWidth, uint32_t AHeight) override {
//    //MIP_Print("w %i h %i\n",AWidth,AHeight);
//    MWidth = AWidth;
//    MHeight = AHeight;
//    // cairo: cairo_xcb_surface_set_size:
//  }

  float _get_text_width(const char* AText) {
    _measure_string(AText);
    return MFontWidth;
  }

  //----------

  float _get_text_height(const char* AText) {
    _measure_string(AText);
    return MFontHeight;
  }

  //----------

//  #ifdef MIP_USE_CAIRO
//
//  cairo_surface_t* createCairoSurface() {
//    cairo_surface_t* surface = cairo_xcb_surface_create(
//      MConnection,
//      MDrawable,
//      mip_xcb_find_visual(MConnection,MVisual),
//      MWidth,
//      MHeight
//    );
//    return surface;
//  }
//
//  #endif

  //----------

  void _set_color(MIP_Color AColor) {
    uint8_t r = AColor.r * 255.0f;
    uint8_t g = AColor.g * 255.0f;
    uint8_t b = AColor.b * 255.0f;
    uint8_t a = AColor.a * 255.0f;
    uint32_t color = (a << 24) + (r << 16) + (g << 8) + b;
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1];
    values[0] = color;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void _set_background_color(MIP_Color AColor) {
    uint8_t r = AColor.r * 255.0f;
    uint8_t g = AColor.g * 255.0f;
    uint8_t b = AColor.b * 255.0f;
    uint8_t a = AColor.a * 255.0f;
    uint32_t color = (a << 24) + (r << 16) + (g << 8) + b;
    uint32_t mask = XCB_GC_BACKGROUND;
    uint32_t values[1];
    values[0] = color;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void _set_line_width(uint32_t AWidth) {
    uint32_t mask = XCB_GC_LINE_WIDTH;
    uint32_t values[1];
    values[0] = AWidth;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

//  void _open_font(const char* AName) {
//    _close_font();
//    MFont = xcb_generate_id(MConnection);
//    xcb_open_font(
//      MConnection,
//      MFont, //font,
//      strlen(AName),
//      AName
//    );
//  }
//
//  //----------
//
//  void _close_font(void) {
//    if (MFont) {
//      xcb_close_font(MConnection,MFont);
//    }
//  }
//
//  //----------
//
//  void _select_font(const char* AName) {
//    _open_font(AName);
//    uint32_t mask = XCB_GC_FONT;
//    uint32_t values[1];
//    values[0] = MFont;
//    xcb_change_gc(MConnection, MGC, mask, values);
//  }

  //----------

  //cscale is from stbtt_ScaleForPixelHeight.

  #ifdef MIP_XCB_USE_STB_TRUETYPE

  float _calc_text_width(const char* text) {
    stbtt_fontinfo* font = MFonts[MFont];
    float length = 0;
    for (uint32_t i=0; i<strlen(text); i++) {
      int advancewidth;
      int leftsidebearing;
      stbtt_GetCodepointHMetrics(font,text[i], &advancewidth, &leftsidebearing);
      length += advancewidth;
    }
    return length * MFontSize;//cscale;
  }

  #endif

  //----------

  void _measure_string(const char *string) {

  #ifdef MIP_XCB_USE_STB_TRUETYPE

    stbtt_fontinfo* font = MFonts[MFont];
    float scale = stbtt_ScaleForPixelHeight(font,MFontSize);
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(font, &ascent, &descent, &lineGap);
    //ascent = roundf(ascent * scale);
    //descent = roundf(descent * scale);
    MFontAscent   = ascent * scale;
    MFontDescent  = descent * scale;
    MFontWidth    = _calc_text_width(string) * scale;
    MFontHeight   = MFontSize;//(y1 - y0 + 1);
    MFontOrigin   = 0;
    MFontLeft     = 0;
    MFontRight    = 0;
    MIP_Print("ascent %i descent %i\n",MFontAscent,MFontDescent);
    MIP_Print("width %i height %i\n",MFontWidth,MFontHeight);

  #else

    xcb_char2b_t xcb_str[256];
    for (uint32_t i = 0; i < strlen(string); i++) {
      xcb_str[i].byte1 = 0;
      xcb_str[i].byte2 = string[i];
    }
    xcb_query_text_extents_cookie_t cookie = xcb_query_text_extents(MConnection, MGC, strlen(string), xcb_str);
    xcb_query_text_extents_reply_t* reply = xcb_query_text_extents_reply(MConnection, cookie, NULL);
    MFontAscent   = reply->font_ascent;
    MFontDescent  = reply->font_descent;
    MFontWidth    = reply->overall_width;
    MFontHeight   = reply->font_ascent + reply->font_descent;
    MFontOrigin   = reply->font_ascent;
    MFontLeft     = reply->overall_left;
    MFontRight    = reply->overall_right;
    //MFontOverallAscent = reply->overall_ascent;
    //MFontOverallDescent = reply->overall_descent;
    //free(xcb_str); // ???
    free(reply);

  #endif

  }

  // called must free returned memory block

  #ifdef MIP_XCB_USE_STB_TRUETYPE

  unsigned char* _render_string(const char* string, unsigned char* bitmap=nullptr) {
    uint32_t string_length = strlen(string);
    if (string_length = 0) return nullptr;
    _measure_string(string);
    stbtt_fontinfo* font = MFonts[MFont];
    if (!bitmap) bitmap = (unsigned char*)calloc(MFontWidth * MFontHeight, sizeof(unsigned char));
    float scale = stbtt_ScaleForPixelHeight(font,MFontSize);
    int x = 0;
    for (uint32_t i = 0; i < string_length; ++i) {
      // how wide is this character
      int ax, lsb;
      stbtt_GetCodepointHMetrics(font, string[i], &ax, &lsb);
      // (Note that each Codepoint call has an alternative Glyph version which caches the work required to lookup the character word[i].)
      // get bounding box for character (may be offset to account for chars that dip above or below the line)
      int c_x1, c_y1, c_x2, c_y2;
      stbtt_GetCodepointBitmapBox(font, string[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
      //* compute y (different characters have different heights)
      int y = MFontAscent + c_y1;
      //* render character (stride and offset is important here)
      int byteOffset = x + roundf(lsb * scale) + (y * MFontWidth);
      stbtt_MakeCodepointBitmap(font, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, MFontWidth, scale, scale, string[i]);
      //* advance x
      x += roundf(ax * scale);
      //* add kerning
      int kern;
      kern = stbtt_GetCodepointKernAdvance(font, string[i], string[i + 1]);
      x += roundf(kern * scale);
    }
    //* save out a 1 channel image
    //stbi_write_png("out.png", b_w, b_h, 1, bitmap, b_w);
    //free(fontBuffer);
    //free(bitmap);
    return bitmap;
  }

  #endif

  //----------

  void _draw_buffer(uint32_t* buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    uint32_t buffersize = width * height * sizeof(uint32_t);
    xcb_image_t* image = xcb_image_create(
      width,                          // width      width in pixels.
      height,                         // height     height in pixels.
      XCB_IMAGE_FORMAT_Z_PIXMAP,      // format
      32,                             // xpad       scanline pad (8,16,32)
      24,                             // depth      (1,4,8,16,24 zpixmap),    (1 xybitmap), (anything xypixmap)
      32,                             // bpp        (1,4,8,16,24,32 zpixmap,  (1 xybitmap), (anything xypixmap)
      32,                             // unit       unit of image representation, in bits (8,16,32)
      XCB_IMAGE_ORDER_LSB_FIRST,      // byte_order
      XCB_IMAGE_ORDER_LSB_FIRST,      // bit_order
      buffer,                         // base       The base address of malloced image data
      buffersize,                     // bytes      The size in bytes of the storage pointed to by base.
                                      //            If base == 0 and bytes == ~0 and data == 0, no storage will be auto-allocated.
      (uint8_t*)buffer                // data       The image data. If data is null and bytes != ~0, then an attempt will be made
                                      //            to fill in data; from base if it is non-null (and bytes is large enough), else
                                      //            by mallocing sufficient storage and filling in base.
    );
    xcb_image_put(
      MConnection,            // xcb_connection_t*  conn,
      MDrawable,              // xcb_drawable_t     draw,
      MGC,                    // xcb_gcontext_t     gc,
      image,                  // xcb_image_t*       image,
      x,                      // int16_t            x,
      y,                      // int16_t            y,
      0                       // uint8_t            left_pad
    );
    image->base = nullptr;
    xcb_image_destroy(image);
    xcb_flush(MConnection);
  }

};

//----------------------------------------------------------------------

#undef MIP_PATH_MAX_LENGTH

//----------------------------------------------------------------------
#endif



















#if 0


  void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor, float AWidth=1) override {
    xcb_point_t polyline[] =  {
      (int16_t)AX1, (int16_t)AY1, (int16_t)AX2, (int16_t)AY2,
      (int16_t)AX2, (int16_t)AY2, (int16_t)AX3, (int16_t)AY3,
      (int16_t)AX3, (int16_t)AY3, (int16_t)AX1, (int16_t)AY1,
    };
    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,6,polyline);
  }

  void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor) override {
    set_color(AColor);
    xcb_point_t polyline[] =  {
      (int16_t)AX1, (int16_t)AY1, (int16_t)AX2, (int16_t)AY2,
      (int16_t)AX2, (int16_t)AY2, (int16_t)AX3, (int16_t)AY3,
      (int16_t)AX3, (int16_t)AY3, (int16_t)AX1, (int16_t)AY1,
    };
    xcb_fill_poly(MConnection,MDrawable,MGC,XCB_POLY_SHAPE_CONVEX,XCB_COORD_MODE_ORIGIN,6,polyline);
  }

//------------------------------
public:
//------------------------------

  void drawBitmap(float AXpos, float AYpos, MIP_Bitmap* ABitmap) override {
    uint32_t width      = ABitmap->getWidth();
    uint32_t height     = ABitmap->getHeight();
    uint32_t buffersize = ABitmap->getBufferSize();
    uint32_t* buffer    = ABitmap->getBuffer();
    xcb_image_t* image = xcb_image_create(
      width,                          // width      width in pixels.
      height,                         // height     height in pixels.
      XCB_IMAGE_FORMAT_Z_PIXMAP,      // format
      32,                             // xpad       scanline pad (8,16,32)
      24,                             // depth      (1,4,8,16,24 zpixmap),    (1 xybitmap), (anything xypixmap)
      32,                             // bpp        (1,4,8,16,24,32 zpixmap,  (1 xybitmap), (anything xypixmap)
      32,                             // unit       unit of image representation, in bits (8,16,32)
      XCB_IMAGE_ORDER_LSB_FIRST,      // byte_order
      XCB_IMAGE_ORDER_LSB_FIRST,      // bit_order
      buffer,                         // base       The base address of malloced image data
      buffersize,                     // bytes      The size in bytes of the storage pointed to by base.
                                      //            If base == 0 and bytes == ~0 and data == 0, no storage will be auto-allocated.
      (uint8_t*)buffer                // data       The image data. If data is null and bytes != ~0, then an attempt will be made
                                      //            to fill in data; from base if it is non-null (and bytes is large enough), else
                                      //            by mallocing sufficient storage and filling in base.
    );
    //xcb_flush(MTargetConnection);
    xcb_image_put(
      MConnection,            // xcb_connection_t*  conn,
      MDrawable,              // xcb_drawable_t     draw,
      MGC,                    // xcb_gcontext_t     gc,
      image,                  // xcb_image_t*       image,
      AXpos,                  // int16_t            x,
      AYpos,                  // int16_t            y,
      0                       // uint8_t            left_pad
    );
    //xcb_flush(MConnection);
    image->base = nullptr;
    xcb_image_destroy(image);
    xcb_flush(MConnection);
  }

//------------------------------
public:
//------------------------------

  void drawImage(float AXpos, float AYpos, MIP_Drawable* ASource) override {
    if (ASource->isImage()) {
      xcb_image_put(
        MConnection,            // xcb_connection_t*  conn,
        MDrawable,              // xcb_drawable_t     draw,
        MGC,                    // xcb_gcontext_t     gc,
        ASource->getXcbImage(), // xcb_image_t*       image,
        AXpos,                  // int16_t            x,
        AYpos,                  // int16_t            y,
        0                       // uint8_t            left_pad
      );
      xcb_flush(MConnection);
    }
    else if (ASource->isSurface()) {
      //#ifdef MIP_USE_CAIRO
      //cairo_surface_flush(MCairoSurface);
      //#endif
      xcb_copy_area(
        MConnection,                // Pointer to the xcb_connection_t structure
        ASource->getXcbDrawable(),  // The Drawable we want to paste
        MDrawable,                  // The Drawable on which we copy the previous Drawable
        MGC,                        // A Graphic Context
        0,                          // Top left x coordinate of the region we want to copy
        0,                          // Top left y coordinate of the region we want to copy
        AXpos,                      // Top left x coordinate of the region where we want to copy
        AYpos,                      // Top left y coordinate of the region where we want to copy
        ASource->getWidth(),        // Width                 of the region we want to copy
        ASource->getHeight()        // Height of the region we want to copy
      );
      xcb_flush(MConnection);
      //#ifdef MIP_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    //else {
    //  MIP_Print("unknown ADrawable for blit()\n");
    //}
  }

  //----------

  void drawImage(float AXpos, float AYpos, MIP_Drawable* ASource, MIP_FRect ASrc) override {
    if (ASource->isImage()) {
      MIP_Bitmap* bitmap = ASource->getBitmap();
      mip_xcb_put_image(
        MConnection,
        MDrawable,
        MGC,
        ASrc.w,
        ASrc.h,
        AXpos,
        AYpos,
        MTarget->getDepth(),  //ASource->getDepth(),
        bitmap->getStride(),
        bitmap->getPixelPtr(ASrc.x,ASrc.y)  //getBuffer()
      );
      xcb_flush(MConnection);
      //#ifdef MIP_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    else if (ASource->isSurface()) {
      //#ifdef MIP_USE_CAIRO
      //cairo_surface_flush(MCairoSurface);
      //#endif
      xcb_copy_area(
        MConnection,                // Pointer to the xcb_connection_t structure
        ASource->getXcbDrawable(),  // The Drawable we want to paste
        MDrawable,                    // The Drawable on which we copy the previous Drawable
        MGC,                  // A Graphic Context
        ASrc.x,                      // Top left x coordinate of the region we want to copy
        ASrc.y,                      // Top left y coordinate of the region we want to copy
        AXpos,                      // Top left x coordinate of the region where we want to copy
        AYpos,                      // Top left y coordinate of the region where we want to copy
        ASrc.w,                      // Width                 of the region we want to copy
        ASrc.h                       // Height of the region we want to copy
      );
      xcb_flush(MConnection);
      //#ifdef MIP_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    //else {
    //  MIP_Print("unknown ADrawable for blit()\n");
    //}
  }

};

#endif // 0
