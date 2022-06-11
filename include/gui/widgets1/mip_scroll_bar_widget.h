#ifndef mip_scroll_bar_widget_included
#define mip_scroll_bar_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"

class MIP_ScrollBarWidget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  MIP_Color  MBackgroundColor  = MIP_COLOR_DARK_GRAY;
  MIP_Color  MThumbColor       = MIP_COLOR_LIGHT_GRAY;
  MIP_Color  MThumbBorderColor = MIP_COLOR_DARK_GRAY;
  MIP_Color  MInteractiveColor = MIP_COLOR_WHITE;
  float       MThumbPos         = 0.0f;
  float       MPrevThumbPos     = 0.0f;
  float       MThumbSize        = 0.2f;
  float       MPageSize         = 0.2f;

  MIP_FRect  MThumbRect        = MIP_FRect(0,0);
  bool        MIsDragging       = false;
  float       MClickedX         = 0.0f;
  float       MClickedY         = 0.0f;
  float       MClickedPos       = 0.0f;

  uint32_t    MDirection        = MIP_HORIZONTAL;

//------------------------------
public:
//------------------------------

  MIP_ScrollBarWidget(MIP_FRect ARect)
  : MIP_Widget(ARect) {
    setName("MIP_ScrollBarWidget");
    setHint("scrollbar");
    //if (AVertical) setFlag(MIP_WIDGET_VERTICAL);
  }

  virtual ~MIP_ScrollBarWidget() {
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

  virtual void setBackgroundColor(MIP_Color AColor) {
    MBackgroundColor = AColor;
  }

  //----------

  virtual void setThumbColor(MIP_Color AColor) {
    MThumbColor = AColor;
  }

  //----------

  virtual void setInteractiveColor(MIP_Color AColor) {
    MInteractiveColor = AColor;
  }

  //----------

  virtual void setThumbPos(float APos, bool ARedraw=true) {
    MPrevThumbPos = MThumbPos;
    MThumbPos = APos;
    MThumbPos = MIP_Clamp(MThumbPos,0.0f,1.0f);
    //recalcThumbRect();
    if (ARedraw) do_widget_redraw(this,getRect(),0);
  }

  //----------

  virtual void setThumbSize(float ASize, bool ARedraw=true) {
    //MIP_PRINT;
    MThumbSize = ASize;
    //MThumbSize = MIP_Max(0.05,ASize);
    if (MThumbSize >= 1.0f) {
      MPrevThumbPos = MThumbPos;
      MThumbPos = 0.0f;
    }
    MThumbSize = MIP_Clamp(MThumbSize,0.0f,1.0f);
    //recalcThumbRect();
    if (ARedraw) do_widget_redraw(this,getRect(),0);
  }

  //----------

  virtual void setPageSize(float ASize) {
    MPageSize = ASize;
  }

  //----------

  virtual void recalcThumbRect() {
    MIP_FRect mrect = getRect();
    if (MDirection == MIP_VERTICAL) {
      float thumb = mrect.h * MThumbSize;
      thumb = MIP_Max(20,thumb);
      float available = mrect.h - thumb;
      MThumbRect.x = mrect.x;
      MThumbRect.y = mrect.y + (MThumbPos * available);
      MThumbRect.w = mrect.w;
      MThumbRect.h = thumb;
    }
    else {
      float thumb = mrect.w * MThumbSize;
      thumb = MIP_Max(20,thumb);
      float available = mrect.w - thumb;
      MThumbRect.x = mrect.x + (MThumbPos * available);
      MThumbRect.y = mrect.y;
      MThumbRect.w = thumb;
      MThumbRect.h = mrect.h;
    }
    //MIP_Trace("ThumbRect %.2f %.2f %.2f %.2f\n",MThumbRect.x,MThumbRect.y,MThumbRect.w,MThumbRect.h);
  }

//------------------------------
private:
//------------------------------

  void draw_background(MIP_Painter* APainter, MIP_FRect ARect) {
    APainter->roundedRectangle(ARect,6,MIP_CORNER_ALL,MIP_EDGE_ALL);
    APainter->setColor(MBackgroundColor);
    APainter->fillPath();
  }

  //----------

  void draw_thumb(MIP_Painter* APainter, MIP_FRect ARect, bool AVertical=true) {

    //size
    MIP_FRect rect = ARect;

    // gradient
    APainter->roundedRectangle(rect,5,MIP_CORNER_ALL,MIP_EDGE_ALL);
    MIP_Color c1 = MThumbColor;
    MIP_Color c2 = MThumbColor;
    c1.blend(MIP_COLOR_WHITE,0.2);
    c2.blend(MIP_COLOR_BLACK,0.2);
    APainter->fillPathGradient(rect.x,rect.y,rect.x2(),rect.y2(),c1,c2,AVertical);
    // border
    APainter->setLineWidth(1);
    APainter->setColor(MThumbBorderColor);
    APainter->roundedRectangle(rect,5,MIP_CORNER_ALL,MIP_EDGE_ALL);
    APainter->strokePath();
  }


//------------------------------
public:
//------------------------------

  //void on_widget_setPos(float AXpos, float AYpos) final {
  //}

  //void on_widget_setSize(float AWidth, float AHeight) final {
  //}

  //----------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) final {
    recalcThumbRect();
    draw_background(APainter,MRect);
    bool vgrad = false;
    if (MDirection == MIP_HORIZONTAL) vgrad = true;
    draw_thumb(APainter,MThumbRect,vgrad);
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    MIP_FRect mrect = getRect();
    //MIP_Widget::on_widget_mouseClick(AXpos,AYpos,AButton,AState);
    if (AButton == MIP_BUTTON_LEFT) {
      if (MThumbRect.contains(AXpos,AYpos)) {
        MClickedX = AXpos;
        MClickedY = AYpos;
        MClickedPos = MThumbPos;
        MIsDragging = true;
        //MIsInteractive  = true;
        //do_widget_redraw(this,mrect,0);
      }
      else {
        if (MDirection == MIP_VERTICAL) {
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
    //MIP_Widget::on_mouseRelease(AXpos,AYpos,AButton,AState);
    if (AButton == MIP_BUTTON_LEFT) {
      if (MIsDragging) {
        MIsDragging = false;
        //MIsInteractive = false;
        do_widget_redraw(this,getRect(),0);
      }
    }
  }

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
    MIP_FRect mrect = getRect();
    //MIP_Widget::on_mouseMove(AXpos,AYpos,AState);
    if (MIsDragging) {

      float available = 0.0f;
      float dist = 0.0f;
      float thumb_size = 0.0f;
      if (MDirection == MIP_VERTICAL) {
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
        MThumbPos = MIP_Clamp(MThumbPos,0.0f,1.0f);
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
//  void on_widget_enter(float AXpos, float AYpos, MIP_Widget* AFrom, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_leave(float AXpos, float AYpos, MIP_Widget* ATo, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_connect(MIP_Parameter* AParameter, uint32_t ASubIndex) final {
//  }

};

//----------------------------------------------------------------------
#endif
