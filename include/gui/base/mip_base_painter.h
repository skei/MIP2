#ifndef mip_base_painter_included
#define mip_base_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_color.h"
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

  virtual MIP_Drawable* getTarget() { return nullptr; }
  virtual void resize(uint32_t AWidth, uint32_t AHeight) {}

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

  virtual void drawLine(float AXpos1, float AYpos1, float AXpos2, float AYpos2, MIP_Color AColor, uint32_t AWidth=1) {}
  virtual void drawRectangle(MIP_FRect ARect, MIP_Color AColor, uint32_t AWidth=1) {}
  virtual void drawRoundedRectangle(MIP_FRect ARect, float ARadius, uint32_t ACorners, MIP_Color AColor, uint32_t AWidth=1) {}
  virtual void drawArc(MIP_FRect ARect, float AAngle1, float AAngle2, MIP_Color AColor, uint32_t AWidth=1) {}
  virtual void drawEllipse(MIP_FRect ARect, MIP_Color AColor, uint32_t AWidth=1) {}
  virtual void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor, uint32_t AWidth=1) {}

  virtual void fillRectangle(MIP_FRect ARect, MIP_Color AColor) {}
  virtual void fillRoundedRectangle(MIP_FRect ARect, float ARadius, uint32_t ACorners, MIP_Color AColor) {}
  virtual void fillArc(MIP_FRect ARect, float AAngle1, float AAngle2, MIP_Color AColor) {}
  virtual void fillEllipse(MIP_FRect ARect, MIP_Color AColor) {}
  virtual void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor) {}

  virtual void drawText(float AXpos, float AYpos, const char* AText, MIP_Color AColor) {}
  virtual void drawText(MIP_FRect ARect, const char* AText, uint32_t AAlignment, MIP_Color AColor) {}

  virtual void uploadBitmap(float AXpos, float AYpos, MIP_Bitmap* ABitmap) {}
  virtual void drawImage(float AXpos, float AYpos, MIP_Drawable* ASource) {}
  virtual void drawImage(float AXpos, float AYpos, MIP_Drawable* ASource, MIP_FRect ASrc) {}
  virtual void drawImage(MIP_FRect ADst, MIP_Drawable* ASource, MIP_FRect ASrc) {}

};

//----------------------------------------------------------------------
#endif
