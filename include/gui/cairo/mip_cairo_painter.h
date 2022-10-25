#ifndef mip_cairo_painter_included
#define mip_cairo_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/base/mip_base_painter.h"
#include "gui/base/mip_base_painter.h"
#include "gui/mip_drawable.h"

//----------------------------------------------------------------------

//#define MIP_PATH_MAX_LENGTH           256
//
//#define MIP_PATH_MOVE_TO              1
//#define MIP_PATH_LINE_TO              2
//#define MIP_PATH_BEZIER_TO            3
//#define MIP_PATH_QUAD_TO              4
//#define MIP_PATH_ARC_TO               5
//
//#define MIP_PATH_ARC                  6
//#define MIP_PATH_RECT                 7
//#define MIP_PATH_ROUNDED_RECT         8
//#define MIP_PATH_ROUNDED_RECT_VARYING 9
//#define MIP_PATH_ELLIPSE              10
//#define MIP_PATH_CIRCLE               11
//
//struct MIP_PathItem {
//  uint32_t  type    = 0;
//  float     data[8] = {0};
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_CairoPainter
: public MIP_BasePainter {

//------------------------------
protected:
//------------------------------

  MIP_Drawable*   MSurface                    = nullptr;
  MIP_Drawable*   MTarget                     = nullptr;

//MIP_PathItem    MPath[MIP_PATH_MAX_LENGTH]  = {};
//uint32_t        MPathLength                 = 0;

  MIP_Color       MStrokeColor                = MIP_COLOR_BLACK;
  float           MStrokeWidth                = 1.0;

  MIP_Color       MFillColor                  = MIP_COLOR_WHITE;
  MIP_PaintSource MFillPaint                  = {};

  MIP_Color       MFontColor                  = MIP_COLOR_BLACK;
  float           MFontSize                   = 13.0;

//------------------------------
public:
//------------------------------

  // ASurface = surface to paint to
  // ATarget = target window/visual

  MIP_CairoPainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget)
  : MIP_BasePainter(ASurface,ATarget) {
    if (ASurface->drawable_isWindow()) {
    }
    else if (ASurface->drawable_isSurface()) {
    }
    else if (ASurface->drawable_isBitmap()) {
    }
  }

  //----------

  virtual ~MIP_CairoPainter() {
  }

//------------------------------
public:
//------------------------------

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
  }

  void endPaint() override {
  }

  void setClip(MIP_DRect ARect) override {
  }

  void resetClip() override {
  }

  //--------------------
  // frame
  //--------------------

  void beginFrame(float windowWidth, float windowHeight, float devicePixelRatio) override {
  }

  void cancelFrame() override {
  }

  void endFrame() override {
  }

  //--------------------
  // Composite operation
  //--------------------

  void globalCompositeOperation(int op) override {
  }

  void globalCompositeBlendFunc(int sfactor, int dfactor) override {
  }

  void globalCompositeBlendFuncSeparate(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha) override {
  }

  //--------------------
  // State Handling
  //--------------------

  void save() override {
  }

  void restore() override {
  }

  void reset() override {
  }

  //--------------------
  // Render styles
  //--------------------

  void shapeAntiAlias(int enabled) override {}

  void strokeColor(MIP_Color color) override {
    MStrokeColor = color;
  }

  void strokePaint(MIP_PaintSource paint) override {
  }

  void fillColor(MIP_Color color) override {
    MFillColor = color;
  }

  void fillPaint(MIP_PaintSource paint) override {
    MFillPaint = paint;
  }

  void miterLimit(float limit) override {
  }

  void strokeWidth(float size) override {
    MStrokeWidth = size;
  }

  void lineCap(int cap) override {
  }

  void lineJoin(int join) override {
  }

  void globalAlpha(float alpha) override {
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

  int createImageMem(int imageFlags, unsigned char* data, int ndata) override {
    return 0;
  }

  int createImageRGBA(int w, int h, int imageFlags, const unsigned char* data) override {
    return 0;
  }

  void updateImage(int image, const unsigned char* data) override {
  }

  void imageSize(int image, int* w, int* h) override {
  }

  void deleteImage(int image) override {
  }

  //--------------------
  // Paints
  //--------------------

  MIP_PaintSource linearGradient(float sx, float sy, float ex, float ey, MIP_Color icol, MIP_Color ocol) override {
    return MIP_PaintSource();
  }

  MIP_PaintSource boxGradient(float x, float y, float w, float h, float r, float f, MIP_Color icol, MIP_Color ocol) override {
    return MIP_PaintSource();
  }

  MIP_PaintSource radialGradient(float cx, float cy, float inr, float outr, MIP_Color icol, MIP_Color ocol) override {
    return MIP_PaintSource();
  }

  MIP_PaintSource imagePattern(float ox, float oy, float ex, float ey, float angle, int image, float alpha) override {
    return MIP_PaintSource();
  }

  //--------------------
  // Scissoring
  //--------------------

  void scissor(float x, float y, float w, float h) override {
  }

  void intersectScissor(float x, float y, float w, float h) override {
  }

  void resetScissor() override {
  }

  //--------------------
  // Paths
  //--------------------

  void beginPath() override {
  }

  void moveTo(float x, float y) override {
  }

  void lineTo(float x, float y) override {
  }

  void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) override {
  }

  void quadTo(float cx, float cy, float x, float y) override {
  }

  void arcTo(float x1, float y1, float x2, float y2, float radius) override {
  }

  void closePath() override {
  }

  void pathWinding(int dir) override {
  }

  void arc(float cx, float cy, float r, float a0, float a1, int dir) override {
  }

  void rect(float x, float y, float w, float h) override {
  }

  void roundedRect(float x, float y, float w, float h, float r) override {
  }

  void roundedRectVarying(float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft) override {
  }

  void ellipse(float cx, float cy, float rx, float ry) override {
  }

  void circle(float cx, float cy, float r) override {
  }

  void fill() override {
  }

  void stroke() override {
  }

  //--------------------
  // Text
  //--------------------

  int createFont(const char* name, const char* filename) override {
    return 0;
  }

  int createFontAtIndex(const char* name, const char* filename, const int fontIndex) override {
    return 0;
  }

  int createFontMem(const char* name, unsigned char* data, int ndata, int freeData) override {
    return 0;
  }

  int createFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) override {
    return 0;
  }

  int findFont(const char* name) override {
    return 0;
  }

  int addFallbackFontId(int baseFont, int fallbackFont) override {
    return 0;
  }

  int addFallbackFont(const char* baseFont, const char* fallbackFont) override {
    return 0;
  }

  void resetFallbackFontsId(int baseFont) override {
  }

  void resetFallbackFonts(const char* baseFont) override {
  }

  void fontSize(float size) override {
    MFontSize = size;
  }

  void fontBlur(float blur) override {
  }

  void textLetterSpacing(float spacing) override {
  }

  void textLineHeight(float lineHeight) override {
  }

  void textAlign(int align) override {
  }

  void fontFaceId(int font) override {
  }

  void fontFace(const char* font) override {
  }

  float text(float x, float y, const char* string, const char* end) override {
    return 0.0;
  }

  void textBox(float x, float y, float breakRowWidth, const char* string, const char* end) override {
  }

  float textBounds(float x, float y, const char* string, const char* end, float* bounds) override {
    return 0.0;
  }

  void textBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) override {
  }

  int textGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) override {
    return 0;
  }

  void textMetrics(float* ascender, float* descender, float* lineh) override {
  }

  int textBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) override {
    return 0;
  }

//----------



};

//----------------------------------------------------------------------

//#undef MIP_PATH_MAX_LENGTH

//----------------------------------------------------------------------
#endif


