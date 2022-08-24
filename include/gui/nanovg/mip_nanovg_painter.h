#ifndef mip_nanovg_painter_included
#define mip_nanovg_painter_included
//----------------------------------------------------------------------

/*
  TODO: find liberal licensed font to include
*/


#include "mip.h"
#include "gui/glx/mip_glx_painter.h"
#include "gui/nanovg/mip_nanovg.h"
#include "gui/nanovg/mip_nanovg_utils.h"

#include "../data/fonts/liberation_ttf.h"

//#include "extern/blendish/blendish.h"
//#include "extern/blendish/blendish.c"

// so we can make the mip_painter layer cross-backend..
//typedef NVGpaint MIP_PaintSource;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_NanoVGPainter
: public MIP_GlxPainter {

//------------------------------
private:
//------------------------------

  NVGcontext* MContext    = nullptr;
  int         MFont       = 0;
  //int         MIconImage  = 0;

//------------------------------
public:
//------------------------------

  MIP_NanoVGPainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget)
  : MIP_GlxPainter(ASurface,ATarget) {
    MIP_GlxPainter::makeCurrent();
    MContext = nvgCreateGL3(NVG_ANTIALIAS);// | NVG_STENCIL_STROKES); // NVG_DEBUG
    //MFont = nvgCreateFont(MContext,"font1","/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
    MFont = nvgCreateFontMem(MContext,"font1",(unsigned char*)liberation_ttf,liberation_ttf_size,0);
    nvgFontFaceId(MContext,MFont);
    //nvgFontSize(MContext,10);
    #ifdef BLENDISH_H
    bndSetFont(MFont);
    #endif
    //MIconImage  = nvgCreateImage(MContext,"/DISKS/sda2/code/git/MIP2/include/extern/oui-blendish/blender_icons16.png",0); // NVG_IMAGE_PREMULTIPLIED
    //bndSetFont(MFont);
    //bndSetIconImage(MIconImage);
    MIP_GlxPainter::resetCurrent();
  }

  //----------

  virtual ~MIP_NanoVGPainter() {
    nvgDeleteGL3(MContext);
    //nvgDeleteImage(MIconImage);
  }

//------------------------------
public:
//------------------------------

  NVGcontext* getNvgContext() {
    //MIP_PRINT;
    return MContext;
  }

  int getNvgFont(int index=0) {
    return MFont;
  }

  //int getNvgIconImage(int index=0) {
  //  return MIconImage;
  //}

//------------------------------
public:
//------------------------------

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MIP_GlxPainter::beginPaint(AXpos,AYpos,AWidth,AHeight);
    nvgBeginFrame(MContext,AWidth,AHeight,1.0);
  }

  //----------

  void endPaint() override {
    nvgEndFrame(MContext);
    MIP_GlxPainter::endPaint();
  }

  //----------

  void setClip(MIP_DRect ARect) override {
    scissor(ARect.x,ARect.y,ARect.w,ARect.h);
  }

  void resetClip() override {
    resetScissor();
  }

//------------------------------
public:
//------------------------------

  // frame

  void beginFrame(float windowWidth, float windowHeight, float devicePixelRatio) override {
    nvgBeginFrame(MContext,windowWidth,windowHeight,devicePixelRatio);
  }

  void cancelFrame() override {
    nvgCancelFrame(MContext);
  }

  void endFrame() override {
    nvgEndFrame(MContext);
  }

  // Composite operation

  void globalCompositeOperation(int op) override {
    nvgGlobalCompositeOperation(MContext,op);
  }

  void globalCompositeBlendFunc(int sfactor, int dfactor) override {
    nvgGlobalCompositeBlendFunc(MContext,sfactor,dfactor);
  }

  void globalCompositeBlendFuncSeparate(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha) override {
    nvgGlobalCompositeBlendFuncSeparate(MContext,srcRGB,dstRGB,srcAlpha,dstAlpha);
  }

  // Color utils

//  NVGcolor RGB(unsigned char r, unsigned char g, unsigned char b) {
//    return nvgRGB(r, g, b);
//  }
//
//  NVGcolor RGBf(float r, float g, float b) {
//    return nvgRGBf(r, g, b);
//  }
//
//  NVGcolor RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
//    return nvgRGBA(r, g, b, a);
//  }
//
//  NVGcolor RGBAf(float r, float g, float b, float a) {
//    return nvgRGBAf(r, g, b, a);
//  }
//
//  NVGcolor LerpRGBA(NVGcolor c0, NVGcolor c1, float u) {
//    return nvgLerpRGBA(c0,c1,u);
//  }
//
//  NVGcolor TransRGBA(NVGcolor c0, unsigned char a) {
//    return nvgTransRGBA(c0,a);
//  }
//
//  NVGcolor TransRGBAf(NVGcolor c0, float a) {
//    return nvgTransRGBAf(c0,a);
//  }
//
//  NVGcolor HSL(float h, float s, float l) {
//    return nvgHSL(h, s, l);
//  }
//
//  NVGcolor HSLA(float h, float s, float l, unsigned char a) {
//    return nvgHSLA(h, s, l, a);
//  }

  // State Handling

  void save() override {
    nvgSave(MContext);
  }

  void restore() override {
    nvgRestore(MContext);
  }

  void reset() override {
    nvgReset(MContext);
  }

  // Render styles

  void shapeAntiAlias(int enabled) override {
    nvgShapeAntiAlias(MContext,enabled);
  }

  //void strokeColor(NVGcolor color) {
  void strokeColor(MIP_Color color) override {
    nvgStrokeColor(MContext,nvg_color(color));
  }

  //void strokePaint(NVGpaint paint) {
  void strokePaint(MIP_PaintSource paint) override {
    nvgStrokePaint(MContext,paint.nvg);
  }

  //void fillColor(NVGcolor color) {
  void fillColor(MIP_Color color) override {
    nvgFillColor(MContext,nvg_color(color));
  }

  void fillPaint(MIP_PaintSource paint) override {
    nvgFillPaint(MContext,paint.nvg);
  }

  void miterLimit(float limit) override {
    nvgMiterLimit(MContext,limit);
  }

  void strokeWidth(float size) override {
    nvgStrokeWidth(MContext,size);
  }

  void lineCap(int cap) override {
    nvgLineCap(MContext,cap);
  }

  void lineJoin(int join) override {
    nvgLineJoin(MContext,join);
  }

  void globalAlpha(float alpha) override {
    nvgGlobalAlpha(MContext,alpha);
  }

  // Transforms

  void resetTransform() override {
    nvgResetTransform(MContext);
  }

  void transform(float a, float b, float c, float d, float e, float f) override {
    nvgTransform(MContext,a,b,c,d,e,f);
  }

  void translate(float x, float y) override {
    nvgTranslate(MContext,x,y);
  }

  void rotate(float angle) override {
    nvgRotate(MContext,angle);
  }

  void skewX(float angle) override {
    nvgSkewX(MContext,angle);
  }

  void skewY(float angle) override {
    nvgSkewY(MContext,angle);
  }

  void scale(float x, float y) override {
    nvgScale(MContext,x,y);
  }

  void currentTransform(float* xform) override {
    nvgCurrentTransform(MContext,xform);
  }

  void transformIdentity(float* dst) override {
    nvgTransformIdentity(dst);
  }

  void transformTranslate(float* dst, float tx, float ty) override {
    nvgTransformTranslate(dst,tx,ty);
  }

  void transformScale(float* dst, float sx, float sy) override {
    nvgTransformScale(dst,sx,sy);
  }

  void transformRotate(float* dst, float a) override {
    nvgTransformRotate(dst,a);
  }

  void transformSkewX(float* dst, float a) override {
    nvgTransformSkewX(dst,a);
  }

  void transformSkewY(float* dst, float a) override {
    nvgTransformSkewY(dst,a);
  }

  void transformMultiply(float* dst, const float* src) override {
    nvgTransformMultiply(dst,src);
  }

  void transformPremultiply(float* dst, const float* src) override {
    nvgTransformPremultiply(dst,src);
  }

  int transformInverse(float* dst, const float* src) override {
    return nvgTransformInverse(dst,src);
  }

  void transformPoint(float* dstx, float* dsty, const float* xform, float srcx, float srcy) override {
    nvgTransformPoint(dstx,dsty,xform,srcx,srcy);
  }

  float degToRad(float deg) override {
    return nvgDegToRad(deg);
  }

  float radToDeg(float rad) override {
    return nvgRadToDeg(rad);
  }

  // Images

  int createImage(const char* filename, int imageFlags) override {
    return nvgCreateImage(MContext,filename,imageFlags);
  }

  int createImageMem(int imageFlags, unsigned char* data, int ndata) override {
    return nvgCreateImageMem(MContext,imageFlags,data,ndata);
  }

  int createImageRGBA(int w, int h, int imageFlags, const unsigned char* data) override {
    return nvgCreateImageRGBA(MContext,w,h,imageFlags,data);
  }

  void updateImage(int image, const unsigned char* data) override {
    nvgUpdateImage(MContext,image,data);
  }

  void imageSize(int image, int* w, int* h) override {
    nvgImageSize(MContext,image,w,h);
  }

  void deleteImage(int image) override {
    nvgDeleteImage(MContext,image);
  }

  // Paints
  //NVGpaint linearGradient(float sx, float sy, float ex, float ey, NVGcolor icol, NVGcolor ocol) override {
  MIP_PaintSource linearGradient(float sx, float sy, float ex, float ey, MIP_Color icol, MIP_Color ocol) override {
    NVGcolor ic = nvg_color(icol);
    NVGcolor oc = nvg_color(ocol);
    MIP_PaintSource paint;
    paint.nvg = nvgLinearGradient(MContext,sx,sy,ex,ey,ic,oc);
    return paint;
  }

  //NVGpaint boxGradient(float x, float y, float w, float h, float r, float f, NVGcolor icol, NVGcolor ocol) override {
  MIP_PaintSource boxGradient(float x, float y, float w, float h, float r, float f, MIP_Color icol, MIP_Color ocol) override {
    NVGcolor ic = nvg_color(icol);
    NVGcolor oc = nvg_color(ocol);
    MIP_PaintSource paint;
    paint.nvg = nvgBoxGradient(MContext,x,y,w,h,r,f,ic,oc);
    return paint;
  }

  //NVGpaint radialGradient(float cx, float cy, float inr, float outr, NVGcolor icol, NVGcolor ocol) override {
  MIP_PaintSource radialGradient(float cx, float cy, float inr, float outr, MIP_Color icol, MIP_Color ocol) override {
    NVGcolor ic = nvg_color(icol);
    NVGcolor oc = nvg_color(ocol);
    MIP_PaintSource paint;
    paint.nvg = nvgRadialGradient(MContext,cx,cy,inr,outr,ic,oc);
    return paint;
  }

  MIP_PaintSource imagePattern(float ox, float oy, float ex, float ey, float angle, int image, float alpha) override {
    MIP_PaintSource paint;
    paint.nvg = nvgImagePattern(MContext,ox,oy,ex,ey,angle,image,alpha);
    return paint;
  }

  // Scissoring

  void scissor(float x, float y, float w, float h) override {
    nvgScissor(MContext,x,y,w,h);
  }

  void intersectScissor(float x, float y, float w, float h) override {
    nvgIntersectScissor(MContext,x,y,w,h);
  }

  void resetScissor() override {
    nvgResetScissor(MContext);
  }

  // Paths

  void beginPath() override {
    nvgBeginPath(MContext);
  }

  void moveTo(float x, float y) override {
    nvgMoveTo(MContext,x,y);
  }

  void lineTo(float x, float y) override {
    nvgLineTo(MContext,x,y);
  }

  void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) override {
    nvgBezierTo(MContext,c1x,c1y,c2x,c2y,x,y);
  }

  void quadTo(float cx, float cy, float x, float y) override {
    nvgQuadTo(MContext,cx,cy,x,y);
  }

  void arcTo(float x1, float y1, float x2, float y2, float radius) override {
    nvgArcTo(MContext,x1,y1,x2,y2,radius);
  }

  void closePath() override {
    nvgClosePath(MContext);
  }

  void pathWinding(int dir) override {
    nvgPathWinding(MContext,dir);
  }

  void arc(float cx, float cy, float r, float a0, float a1, int dir) override {
    nvgArc(MContext,cx,cy,r,a0,a1,dir);
  }

  void rect(float x, float y, float w, float h) override {
    nvgRect(MContext,x,y,w,h);
  }

  void roundedRect(float x, float y, float w, float h, float r) override {
    nvgRoundedRect(MContext,x,y,w,h,r);
  }

  void roundedRectVarying(float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft) override {
    nvgRoundedRectVarying(MContext,x,y,w,h,radTopLeft,radTopRight,radBottomRight,radBottomLeft);
  }

  void ellipse(float cx, float cy, float rx, float ry) override {
    nvgEllipse(MContext,cx,cy,rx,ry);
  }

  void circle(float cx, float cy, float r) override {
    nvgCircle(MContext,cx,cy,r);
  }

  void fill() override {
    nvgFill(MContext);
  }

  void stroke() override {
    nvgStroke(MContext);
  }

  // Text

  int createFont(const char* name, const char* filename) override {
    return nvgCreateFont(MContext,name,filename);
  }

  int createFontAtIndex(const char* name, const char* filename, const int fontIndex) override {
    return nvgCreateFontAtIndex(MContext,name,filename,fontIndex);
  }

  int createFontMem(const char* name, unsigned char* data, int ndata, int freeData) override {
    return nvgCreateFontMem(MContext,name,data,ndata,freeData);
  }

  int createFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) override {
    return nvgCreateFontMemAtIndex(MContext,name,data,ndata,freeData,fontIndex);
  }

  int findFont(const char* name) override {
    return nvgFindFont(MContext,name);
  }

  int addFallbackFontId(int baseFont, int fallbackFont) override {
    return nvgAddFallbackFontId(MContext,baseFont,fallbackFont);
  }

  int addFallbackFont(const char* baseFont, const char* fallbackFont) override {
    return nvgAddFallbackFont(MContext,baseFont,fallbackFont);
  }

  void resetFallbackFontsId(int baseFont) override {
    nvgResetFallbackFontsId(MContext,baseFont);
  }

  void resetFallbackFonts(const char* baseFont) override {
    nvgResetFallbackFonts(MContext,baseFont);
  }

  void fontSize(float size) override {
    nvgFontSize(MContext,size);
  }

  void fontBlur(float blur) override {
    nvgFontBlur(MContext,blur);
  }

  void textLetterSpacing(float spacing) override {
    nvgTextLetterSpacing(MContext,spacing);
  }

  void textLineHeight(float lineHeight) override {
    nvgTextLineHeight(MContext,lineHeight);
  }

  void textAlign(int align) override {
    nvgTextAlign(MContext,align);
  }

  void fontFaceId(int font) override {
    nvgFontFaceId(MContext,font);
  }

  void fontFace(const char* font) override {
    nvgFontFace(MContext,font);
  }

  float text(float x, float y, const char* string, const char* end) override {
    return nvgText(MContext,x,y,string,end);
  }

  void textBox(float x, float y, float breakRowWidth, const char* string, const char* end) override {
    nvgTextBox(MContext,x,y,breakRowWidth,string,end);
  }

  float textBounds(float x, float y, const char* string, const char* end, float* bounds) override {
    return nvgTextBounds(MContext,x,y,string,end,bounds);
  }

  void textBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) override {
    nvgTextBoxBounds(MContext,x,y,breakRowWidth,string,end,bounds);
  }

  int textGlyphPositions(float x, float y, const char* string, const char* end, NVGglyphPosition* positions, int maxPositions) override {
    return nvgTextGlyphPositions(MContext,x,y,string,end,positions,maxPositions);
  }

  void textMetrics(float* ascender, float* descender, float* lineh) override {
    nvgTextMetrics(MContext,ascender,descender,lineh);
  }

  int textBreakLines(const char* string, const char* end, float breakRowWidth, NVGtextRow* rows, int maxRows) override {
    return nvgTextBreakLines(MContext,string,end,breakRowWidth,rows,maxRows);
  }

};

//----------------------------------------------------------------------
#endif
