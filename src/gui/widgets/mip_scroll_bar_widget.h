#ifndef kode_scroll_bar_widget_included
#define kode_scroll_bar_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

class KODE_ScrollBarWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  KODE_Color  MBackgroundColor  = KODE_COLOR_DARK_GRAY;
  KODE_Color  MThumbColor       = KODE_COLOR_LIGHT_GRAY;
  KODE_Color  MInteractiveColor = KODE_COLOR_WHITE;
  float       MThumbPos         = 0.0f;
  float       MPrevThumbPos     = 0.0f;
  float       MThumbSize        = 0.2f;
  float       MPageSize         = 0.2f;

  KODE_FRect  MThumbRect        = KODE_FRect(0,0);
  bool        MIsDragging       = false;
  float       MClickedX         = 0.0f;
  float       MClickedY         = 0.0f;
  float       MClickedPos       = 0.0f;

  uint32_t    MDirection        = KODE_HORIZONTAL;

//------------------------------
public:
//------------------------------

  KODE_ScrollBarWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_ScrollBarWidget");
    setHint("scrollbar");
    //if (AVertical) setFlag(KODE_WIDGET_VERTICAL);
  }

  virtual ~KODE_ScrollBarWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDirection(uint32_t ADir) {
    MDirection = ADir;
  }

  //----------

  virtual float getThumbPos() {
    return MThumbPos;
  }

  virtual float getPrevThumbPos() {
    return MPrevThumbPos;
  }

  virtual float getThumbSize() {
    return MThumbSize;
  }

  virtual void setBackgroundColor(KODE_Color AColor) {
    MBackgroundColor = AColor;
  }

  //----------

  virtual void setThumbColor(KODE_Color AColor) {
    MThumbColor = AColor;
  }

  //----------

  virtual void setInteractiveColor(KODE_Color AColor) {
    MInteractiveColor = AColor;
  }

  //----------

  virtual void setThumbPos(float APos, bool ARedraw=true) {
    MPrevThumbPos = MThumbPos;
    MThumbPos = APos;
    MThumbPos = KODE_Clamp(MThumbPos,0.0f,1.0f);
    //recalcThumbRect();
    if (ARedraw) do_widget_redraw(this,getRect(),0);
  }

  //----------

  virtual void setThumbSize(float ASize, bool ARedraw=true) {
    MThumbSize = ASize;
    if (MThumbSize >= 1.0f) {
      MPrevThumbPos = MThumbPos;
      MThumbPos = 0.0f;
    }
    MThumbSize = KODE_Clamp(MThumbSize,0.0f,1.0f);
    //recalcThumbRect();
    if (ARedraw) do_widget_redraw(this,getRect(),0);
  }

  //----------

  virtual void setPageSize(float ASize) {
    MPageSize = ASize;
  }

  //----------

  virtual void recalcThumbRect() {
    KODE_FRect mrect = getRect();
    if (MDirection == KODE_VERTICAL) {
      float thumb = mrect.h * MThumbSize;
      float available = mrect.h - thumb;
      MThumbRect.x = mrect.x;
      MThumbRect.y = mrect.y + (MThumbPos * available);
      MThumbRect.w = mrect.w;
      MThumbRect.h = thumb;
    }
    else {
      float thumb = mrect.w * MThumbSize;
      float available = mrect.w - thumb;
      MThumbRect.x = mrect.x + (MThumbPos * available);
      MThumbRect.y = mrect.y;
      MThumbRect.w = thumb;
      MThumbRect.h = mrect.h;
    }
    //KODE_Trace("ThumbRect %.2f %.2f %.2f %.2f\n",MThumbRect.x,MThumbRect.y,MThumbRect.w,MThumbRect.h);
  }

//------------------------------
public:
//------------------------------

  //void on_widget_setPos(float AXpos, float AYpos) final {
  //}

  //void on_widget_setSize(float AWidth, float AHeight) final {
  //}

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    //KODE_Widget::on_widget_paint(APainter,ARect,+);
    recalcThumbRect();
    //APainter->setFillColor(MBackgroundColor);
    //APainter->fillRectangle(MRect);
    APainter->fillRectangle(getRect(),MBackgroundColor);
    //if (MIsInteractive) APainter->setFillColor(MInteractiveColor);
    //else APainter->setFillColor(MThumbColor);
    //APainter->fillRectangle(MThumbRect);
    APainter->fillRectangle(MThumbRect,MThumbColor);
  }

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    KODE_FRect mrect = getRect();
    //KODE_Widget::on_widget_mouseClick(AXpos,AYpos,AButton,AState);
    if (AButton == KODE_BUTTON_LEFT) {
      if (MThumbRect.contains(AXpos,AYpos)) {
        MClickedX = AXpos;
        MClickedY = AYpos;
        MClickedPos = MThumbPos;
        MIsDragging = true;
        //MIsInteractive  = true;
        //do_widget_redraw(this,mrect,0);
      }
      else {
        if (MDirection == KODE_VERTICAL) {
          if (AYpos < MThumbRect.y) {
            MPrevThumbPos = MThumbPos;
            MThumbPos -= MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this,mrect,0);
          }
          else if (AYpos > MThumbRect.y2()) {
            MPrevThumbPos = MThumbPos;
            MThumbPos += MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this,mrect,0);
          }
        }
        else {
          if (AXpos < MThumbRect.x) {
            MPrevThumbPos = MThumbPos;
            MThumbPos -= MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this,mrect,0);
          }
          else if (AXpos > MThumbRect.x2()) {
            MPrevThumbPos = MThumbPos;
            MThumbPos += MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this,mrect,0);
          }
        } // !vertical
      } // !contains
    } // left button
  }

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    //KODE_Widget::on_mouseRelease(AXpos,AYpos,AButton,AState);
    if (AButton == KODE_BUTTON_LEFT) {
      if (MIsDragging) {
        MIsDragging = false;
        //MIsInteractive = false;
        do_widget_redraw(this,getRect(),0);
      }
    }
  }

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
    KODE_FRect mrect = getRect();
    //KODE_Widget::on_mouseMove(AXpos,AYpos,AState);
    if (MIsDragging) {

      float available = 0.0f;
      float dist = 0.0f;
      float thumb_size = 0.0f;
      if (MDirection == KODE_VERTICAL) {
        dist = AYpos - MClickedY;
        thumb_size = mrect.h * MThumbSize;
        available = mrect.h - thumb_size;
      }
      else {
        dist = AXpos - MClickedX;
        thumb_size = mrect.w * MThumbSize;
        available = mrect.w - thumb_size;
      }
      if (available > 0.0f) {
        float v = dist / available;
        MPrevThumbPos = MThumbPos;
        MThumbPos = MClickedPos + v;
        MThumbPos = KODE_Clamp(MThumbPos,0.0f,1.0f);
        //recalcThumbRect();
        do_widget_update(this);
        do_widget_redraw(this,mrect,0);
      }
    }
  }

//  void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_connect(KODE_Parameter* AParameter, uint32_t ASubIndex) final {
//  }

};

//----------------------------------------------------------------------
#endif
