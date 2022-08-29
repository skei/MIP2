#ifndef mip_scroll_bar_widget_included
#define mip_scroll_bar_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/mip_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ScrollBarWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

//  MIP_Color  MBackgroundColor  = MIP_COLOR_DARK_GRAY;

  MIP_Color  MThumbColor       = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;
  MIP_Color  MThumbBorderColor = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
  MIP_Color  MInteractiveColor = MIP_Color(1);//MIP_COLOR_WHITE;

  float       MThumbPos         = 0.0f;
  float       MPrevThumbPos     = 0.0f;
  float       MThumbSize        = 0.2f;
  float       MPageSize         = 0.2f;

  MIP_DRect   MThumbRect        = MIP_DRect(0,0);
  bool        MIsDragging       = false;
  float       MClickedX         = 0.0f;
  float       MClickedY         = 0.0f;
  float       MClickedPos       = 0.0f;

  uint32_t    MDirection        = MIP_HORIZONTAL;

//------------------------------
public:
//------------------------------

  MIP_ScrollBarWidget(MIP_DRect ARect)
  : MIP_PanelWidget(ARect) {
    //if (AVertical) setFlag(MIP_WIDGET_VERTICAL);
  }

  //----------

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
    if (ARedraw) do_widget_redraw(this,0);
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
    if (ARedraw) do_widget_redraw(this,0);
  }

  //----------

  virtual void setPageSize(float ASize) {
    MPageSize = ASize;
  }

  //----------

  virtual void recalcThumbRect() {
    MIP_DRect mrect = MRect;
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

//  void draw_background(MIP_Painter* APainter, MIP_DRect ARect) {
//    APainter->roundedRectangle(ARect,6,MIP_CORNER_ALL,MIP_EDGE_ALL);
//    APainter->setColor(MBackgroundColor);
//    APainter->fillPath();
//  }

  //----------

      /*
      if (MDrawRoundedCorners) painter->roundedRectVarying(MRect.x,MRect.y,MRect.w,MRect.h, MULCornerRadius, MURCornerRadius, MBRCornerRadius, MBLCornerRadius);
      else painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
      if (MFillGradient) {
        MIP_PaintSource paint = painter->linearGradient(MRect.x,MRect.y,MRect.x,MRect.y + MRect.y2(), MGradientColor1, MGradientColor2);
        painter->fillPaint(paint);
      }
      else {
        painter->fillColor(MBackgroundColor);
      }
      painter->fill();
      */

  void draw_thumb(MIP_Painter* APainter, MIP_DRect ARect, bool AVertical=true) {
    APainter->beginPath();
    //APainter->roundedRectangle(rect,5,MIP_CORNER_ALL,MIP_EDGE_ALL);
    APainter->roundedRectVarying(ARect.x,ARect.y,ARect.w,ARect.h, 5,5,5,5);
//    MIP_Color c1 = MThumbColor;
//    MIP_Color c2 = MThumbColor;
//    c1.blend(MIP_COLOR_WHITE,0.2);
//    c2.blend(MIP_COLOR_BLACK,0.2);
//    APainter->fillPathGradient(rect.x,rect.y,rect.x2(),rect.y2(),c1,c2,AVertical);
    APainter->fillColor(MThumbColor);
    APainter->fill();

    // border

    APainter->beginPath();
    //APainter->roundedRectangle(rect,5,MIP_CORNER_ALL,MIP_EDGE_ALL);
    APainter->roundedRectVarying(ARect.x,ARect.y,ARect.w,ARect.h, 5,5,5,5);
    APainter->strokeWidth(1);
    APainter->strokeColor(MThumbBorderColor);
    APainter->stroke();

  }

//------------------------------
public: // parent to child
//------------------------------

//  void on_widget_move(double AXpos, double AYpos)  override {}
//  void on_widget_resize(double AWidth, double AHeight) override {}
//  void on_widget_align(bool ARecursive=true) override {}

  void on_widget_paint(MIP_PaintContext* AContext) override {
    //paintChildWidgets(AContext);
    MIP_Painter* painter = AContext->painter;
    recalcThumbRect();
    //draw_background(painter,MRect);
    fillBackground(AContext);
    bool vgrad = false;
    if (MDirection == MIP_HORIZONTAL) vgrad = true;
    draw_thumb(painter,MThumbRect,vgrad);
  }

//  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    //MIP_DRect mrect = MRect;
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
            do_widget_redraw(this,0);
          }
          else if (AYpos > MThumbRect.y2()) {
            MPrevThumbPos = MThumbPos;
            MThumbPos += MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this,0);
          }
        }
        else {
          if (AXpos < MThumbRect.x) {
            MPrevThumbPos = MThumbPos;
            MThumbPos -= MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this,0);
          }
          else if (AXpos > MThumbRect.x2()) {
            MPrevThumbPos = MThumbPos;
            MThumbPos += MPageSize;
            //recalcThumbRect();
            do_widget_update(this);
            do_widget_redraw(this,0);
          }
        } // !vertical
      } // !contains
    } // left button
  }

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    //MIP_Widget::on_mouseRelease(AXpos,AYpos,AButton,AState);
    if (AButton == MIP_BUTTON_LEFT) {
      if (MIsDragging) {
        MIsDragging = false;
        //MIsInteractive = false;
        do_widget_redraw(this,0);
      }
    }
  }

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_DRect mrect = MRect;
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
        do_widget_redraw(this,0);
      }
    }
  }

//  void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

//  virtual void on_widget_modal(bool AModal) {}


//------------------------------
public: // child to parent
//------------------------------

//  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_redraw(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_modal(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_cursor(MIP_Widget* ASender, uint32_t ACursor) override {}
//  void do_widget_hint(MIP_Widget* ASender, const char* AHint) override {}
//  void do_widget_notify(MIP_Widget* ASender, uint32_t AMode, int32_t AValue) override {}

};

//----------------------------------------------------------------------
#endif
