#ifndef mip_base_painter_included
#define mip_base_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gfx/mip_bitmap.h"
#include "gui/mip_drawable.h"

//----------------------------------------------------------------------

//typedef NVGpaint MIP_PaintSource;
//struct MIP_PaintSource;
//typedef NVGpaint MIP_PaintSource;

#ifdef MIP_PAINTER_NANOVG
  #include "gui/nanovg/mip_nanovg.h"
  //typedef NVGpaint MIP_PaintSource;
#endif

#ifdef MIP_PAINTER_XCB
  #include "gui/xcb/mip_xcb.h"
  //typedef MIP_Bitmap* MIP_PaintSource;
#endif

//----------

struct MIP_PaintSource {
  union {
    #ifdef MIP_PAINTER_CAIRO
    #endif
    #ifdef MIP_PAINTER_GDI
    #endif
    #ifdef MIP_PAINTER_GLX
    #endif
    #ifdef MIP_PAINTER_NANOVG
      NVGpaint nvg;
    #endif
    #ifdef MIP_PAINTER_WGL
    #endif
    #ifdef MIP_PAINTER_XCB
      int image = -1;
    #endif
  };
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_BasePainter {

//------------------------------
public:
//------------------------------

  MIP_BasePainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget) {
  }

  //----------

  virtual ~MIP_BasePainter() {
  }

//------------------------------
public:
//------------------------------

  virtual void      beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void      endPaint() {}

  virtual void      setClip(MIP_DRect ARect) {}
  virtual void      resetClip() {}

  //----------

  virtual void      pushClip(MIP_DRect ARect) {}
  virtual void      pushOverlapClip(MIP_DRect ARect) {}

  virtual MIP_DRect popClip() { return MIP_DRect(); }
  virtual void      resetClipStack() {}
  virtual void      setClipRect(MIP_DRect ARect) {}
  virtual MIP_DRect getClipRect() { return MIP_DRect(); }

//------------------------------
public:
//------------------------------

  //virtual void resize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void drawTextBox(MIP_DRect ARect, const char* AText, uint32_t AAlignment, MIP_Color AColor) {}
  virtual void triangle( double x1, double y1, double x2, double y2, double x3, double y3) {}
  virtual void rectangle(MIP_DRect r) {}
  virtual void ellipse(MIP_DRect r) {}

//------------------------------
public:
//------------------------------

  virtual void beginFrame(float windowWidth, float windowHeight, float devicePixelRatio) {}
  virtual void cancelFrame() {}
  virtual void endFrame() {}

  // Composite operation

  virtual void globalCompositeOperation(int op) {}
  virtual void globalCompositeBlendFunc(int sfactor, int dfactor) {}
  virtual void globalCompositeBlendFuncSeparate(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha) {}

  // State Handling

  virtual void save() {}
  virtual void restore() {}
  virtual void reset() {}

  // Render styles

  virtual void shapeAntiAlias(int enabled) {}
  virtual void strokeColor(MIP_Color color) {}
  virtual void strokePaint(MIP_PaintSource paint) {}
  //virtual void strokePaint(int paint) {}
  virtual void fillColor(MIP_Color color) {}
  virtual void fillPaint(MIP_PaintSource paint) {}
  //virtual void fillPaint(int paint) {}
  virtual void miterLimit(float limit) {}
  virtual void strokeWidth(float size) {}
  virtual void lineCap(int cap) {}
  virtual void lineJoin(int join) {}
  virtual void globalAlpha(float alpha) {}

  // Transforms

  virtual void resetTransform() {}
  virtual void transform(float a, float b, float c, float d, float e, float f) {}
  virtual void translate(float x, float y) {}
  virtual void rotate(float angle) {}
  virtual void skewX(float angle) {}
  virtual void skewY(float angle) {}
  virtual void scale(float x, float y) {}
  virtual void currentTransform(float* xform) {}
  virtual void transformIdentity(float* dst) {}
  virtual void transformTranslate(float* dst, float tx, float ty) {}
  virtual void transformScale(float* dst, float sx, float sy) {}
  virtual void transformRotate(float* dst, float a) {}
  virtual void transformSkewX(float* dst, float a) {}
  virtual void transformSkewY(float* dst, float a) {}
  virtual void transformMultiply(float* dst, const float* src) {}
  virtual void transformPremultiply(float* dst, const float* src) {}
  virtual int transformInverse(float* dst, const float* src) { return 0; }
  virtual void transformPoint(float* dstx, float* dsty, const float* xform, float srcx, float srcy) {}
  virtual float degToRad(float deg) { return 0.0; }
  virtual float radToDeg(float rad) { return 0.0; }

  // Images

  virtual int createImage(const char* filename, int imageFlags) { return 0; }
  virtual int createImageMem(int imageFlags, unsigned char* data, int ndata) { return 0; }
  virtual int createImageRGBA(int w, int h, int imageFlags, const unsigned char* data) { return 0; }
  virtual void updateImage(int image, const unsigned char* data) {}
  virtual void imageSize(int image, int* w, int* h) {}
  virtual void deleteImage(int image) {}

  // Paints

  virtual MIP_PaintSource linearGradient(float sx, float sy, float ex, float ey, MIP_Color icol, MIP_Color ocol) { return MIP_PaintSource(); }
  virtual MIP_PaintSource boxGradient(float x, float y, float w, float h, float r, float f, MIP_Color icol, MIP_Color ocol) { return MIP_PaintSource(); }
  virtual MIP_PaintSource radialGradient(float cx, float cy, float inr, float outr, MIP_Color icol, MIP_Color ocol) { return MIP_PaintSource(); }
  virtual MIP_PaintSource imagePattern(float ox, float oy, float ex, float ey, float angle, int image, float alpha) { return MIP_PaintSource(); }

  // Scissoring

  virtual void scissor(float x, float y, float w, float h) {}
  virtual void intersectScissor(float x, float y, float w, float h) {}
  virtual void resetScissor() {}

  // Paths

  virtual void beginPath() {}
  virtual void moveTo(float x, float y) {}
  virtual void lineTo(float x, float y) {}
  virtual void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {}
  virtual void quadTo(float cx, float cy, float x, float y) {}
  virtual void arcTo(float x1, float y1, float x2, float y2, float radius) {}
  virtual void closePath() {}
  virtual void pathWinding(int dir) {}
  virtual void arc(float cx, float cy, float r, float a0, float a1, int dir) {}
  virtual void rect(float x, float y, float w, float h) {}
  virtual void roundedRect(float x, float y, float w, float h, float r) {}
  virtual void roundedRectVarying(float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft) {}
  virtual void ellipse(float cx, float cy, float rx, float ry) {}
  virtual void circle(float cx, float cy, float r) {}
  virtual void fill() {}
  virtual void stroke() {}

  // Text

  virtual int createFont(const char* name, const char* filename) { return 0; }
  virtual int createFontAtIndex(const char* name, const char* filename, const int fontIndex) { return 0; }
  virtual int createFontMem(const char* name, unsigned char* data, int ndata, int freeData) { return 0; }
  virtual int createFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) { return 0; }
  virtual int findFont(const char* name) { return 0; }
  virtual int addFallbackFontId(int baseFont, int fallbackFont) { return 0; }
  virtual int addFallbackFont(const char* baseFont, const char* fallbackFont) { return 0; }
  virtual void resetFallbackFontsId(int baseFont) {}
  virtual void resetFallbackFonts(const char* baseFont) {}
  virtual void fontSize(float size) {}
  virtual void fontBlur(float blur) {}
  virtual void textLetterSpacing(float spacing) {}
  virtual void textLineHeight(float lineHeight) {}
  virtual void textAlign(int align) {}
  virtual void fontFaceId(int font) {}
  virtual void fontFace(const char* font) {}
  virtual float text(float x, float y, const char* string, const char* end) { return 0.0; }
  virtual void textBox(float x, float y, float breakRowWidth, const char* string, const char* end) {}
  virtual float textBounds(float x, float y, const char* string, const char* end, float* bounds) { return 0.0; }
  virtual void textBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) {}
  //virtual int textGlyphPositions(float x, float y, const char* string, const char* end, NVGglyphPosition* positions, int maxPositions) { return 0; }
  virtual int textGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) { return 0; }
  virtual void textMetrics(float* ascender, float* descender, float* lineh) {}
  virtual int textBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) { return 0; }

//----------


};

//----------------------------------------------------------------------
#endif
