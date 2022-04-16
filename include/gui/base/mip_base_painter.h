#ifndef mip_base_painter_included
#define mip_base_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_color.h"
#include "base/types/mip_rect.h"
#include "gui/mip_surface.h"
#include "gui/mip_drawable.h"

//----------------------------------------------------------------------

class MIP_BasePainter {

//------------------------------
public:
//------------------------------

  MIP_BasePainter() {
  }

  virtual ~MIP_BasePainter() {
  }

//------------------------------
public:
//------------------------------

  //virtual MIP_Drawable* getTarget() { return nullptr; }

  virtual void setScale(float AXscale, float AYscale) {}
  virtual void resetScale() {}

  virtual void resize(uint32_t AWidth, uint32_t AHeight) {
  }

  //-----

  virtual void flush() {}
  virtual void dirty() {}
  virtual void dirty(MIP_FRect ARect) {}
  virtual void finish() {}

  virtual void setClip(MIP_FRect ARect) {}
  virtual void resetClip() {}

  //virtual void pushClip(MIP_FRect ARect) {}
  //virtual MIP_FRect popClip() {}
  //virtual void resetClipStack() {}
  //virtual void setClipRect(MIP_FRect ARect) {}
  //virtual MIP_FRect getClipRect() {}

  virtual float getTextWidth(const char* AText) { return 0.0; }
  virtual float getTextHeight(const char* AText) { return 0.0; }
  //virtual void  getTextSize(const char* AText, float* AWidth, float* AHeight) { *AWidth = 0.0; *AHeight = 0.0; }


  //----------

  virtual void setColor(MIP_Color AColor) {}
  virtual void setLineWidth(float ASize) {}
  virtual void setLineDash(double* ADashes, uint32_t ANumDashes, float AOffset) {}
  virtual void setLineCap(uint32_t ALineCap) {}
  virtual void setLineJoin(uint32_t ALineJoin) {}
  virtual void setFontSize(float ASize) {}
  virtual void setFontFace(const char* AName, uint32_t ASlant, uint32_t AWeight) {}

  //-----
  virtual void strokePath(bool APreserve=false) {}
  virtual void fillPath(bool APreserve=false) {}
  virtual void fillPathGradient(float AX1, float AY1, float AX2, float AY2, MIP_Color AColor1, MIP_Color AColor2, bool AVertical, bool APreserve=false) {}

  //-----

  virtual void moveTo(float AX, float AY) {}
  virtual void lineTo(float AX, float AY) {}
  virtual void curveTo(float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) {}

  virtual void horizLine(float AX1, float AY1, float AX2) {}
  virtual void vertLine(float AX1, float AY1, float AY2) {}
  virtual void rectangle(MIP_FRect ARect) {}
  virtual void roundedRectangle(MIP_FRect ARect, float ARadius, uint32_t ACorners, uint32_t AEdges) {}
  virtual void ellipse(MIP_FRect ARect) {}
  virtual void arc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) {}
  virtual void triangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) {}

  //-----

  virtual void text(float AXpos, float AYpos, const char* AText) {}

  //-----

  virtual void drawPoint(float AX, float AY, MIP_Color AColor) {}
  virtual void drawLine(float AXpos1, float AYpos1, float AXpos2, float AYpos2, MIP_Color AColor, float AWidth=1) {}
  virtual void drawRectangle(MIP_FRect ARect, MIP_Color AColor, float AWidth=1) {}
  virtual void drawRoundedRectangle(MIP_FRect ARect, float ARadius, uint32_t ACorners, uint32_t AEdges, MIP_Color AColor, float AWidth=1) {}
  virtual void drawArc(MIP_FRect ARect, float AAngle1, float AAngle2, MIP_Color AColor, float AWidth=1) {}
  virtual void drawEllipse(MIP_FRect ARect, MIP_Color AColor, float AWidth=1) {}
  virtual void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor, float AWidth=1) {}

  //-----

  virtual void fillRectangle(MIP_FRect ARect, MIP_Color AColor) {}
  virtual void fillRoundedRectangle(MIP_FRect ARect, float ARadius, uint32_t ACorners, uint32_t AEdges, MIP_Color AColor) {}
  virtual void fillArc(MIP_FRect ARect, float AAngle1, float AAngle2, MIP_Color AColor) {}
  virtual void fillEllipse(MIP_FRect ARect, MIP_Color AColor) {}
  virtual void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor) {}

  //-----

  virtual void drawText(float AXpos, float AYpos, const char* AText/*, MIP_Color AColor*/) {}
  virtual void drawText(MIP_FRect ARect, const char* AText, uint32_t AAlignment/*, MIP_Color AColor*/) {}

  //-----

  virtual void uploadBitmap(float AXpos, float AYpos, MIP_Bitmap* ABitmap) {}
  virtual void drawImage(float AXpos, float AYpos, MIP_Drawable* ASource) {}
  virtual void drawImage(float AXpos, float AYpos, MIP_Drawable* ASource, MIP_FRect ASrc) {}
  virtual void drawImage(MIP_FRect ADst, MIP_Drawable* ASource, MIP_FRect ASrc) {}

};

//----------------------------------------------------------------------
#endif
