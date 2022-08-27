#ifndef mip_dual_slider_widget_included
#define mip_dual_slider_widget_included
//----------------------------------------------------------------------

  //TODO
  //calcValueFromMouse


#include "mip.h"
#include "gui/mip_widget.h"
#include "gui/mip_paint_context.h"

#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_DualSliderWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  float       MValue2               = 0.0f;
  bool        MDrawValueText        = true;

  MIP_Color   MBackgroundColor      = MIP_COLOR_GRAY;
  MIP_Color   MBarColor             = MIP_COLOR_LIGHT_GRAY;

  MIP_Color   MEdgeColor            = MIP_COLOR_WHITE;
  uint32_t    MEdgeWidth            = 3;
  int32_t     MHoverEdge            = -1;
  bool        MIsDragging1          = false;
  bool        MIsDragging2          = false;
  int32_t     MEdgeDistance         = 3;

  char        MValue1Text[32]       = {0};
  char        MValue2Text[32]       = {0};

  MIP_Color   MValue1TextColor      = MIP_Color(0.0f);
  uint32_t    MValue1TextAlignment  = MIP_TEXT_ALIGN_LEFT;
  MIP_DRect   MValue1TextOffset     = MIP_DRect(2,2,2,2);

  MIP_Color   MValue2TextColor      = MIP_Color(0.0f);
  uint32_t    MValue2TextAlignment  = MIP_TEXT_ALIGN_RIGHT;
  MIP_DRect   MValue2TextOffset     = MIP_DRect(2,2,2,2);

  bool        MDrawLabel            = true;
  const char* MLabel                = "";
  MIP_Color   MLabelColor           = MIP_COLOR_BLACK;

//------------------------------
public:
//------------------------------

  MIP_DualSliderWidget(MIP_DRect ARect)
  : MIP_PanelWidget(ARect) {
    MName = "MIP_RangeSliderWidget";
    Options.wantHoverEvents = true;
  }

//------------------------------
public:
//------------------------------

  virtual void setBackgroundColor(MIP_Color AColor)  { MBackgroundColor = AColor; }
  virtual void setBarColor(MIP_Color AColor)         { MBarColor = AColor; }
  virtual void setEdgeColor(MIP_Color AColor)        { MEdgeColor = AColor; }

  virtual float getValue2(void) {
    return MValue2;
  }

  virtual void  setValue2(float AValue) {
    MValue2 = AValue;
  }

//------------------------------
public:
//------------------------------

  virtual void findHoverEdge(float AXpos) {
    MIP_DRect mrect = getRect();
    float v = (AXpos - mrect.x) / mrect.w;
    float d1 = v - getValue();
    float d2 = v - getValue2();
    int32_t d = 0;
    int32_t prev = MHoverEdge;
    MHoverEdge = -1;
    //const char* hint = "";
    if (fabs(d1) <= fabs(d2)) {
      d = d1 * mrect.w;
      if (d <= -MEdgeDistance) {
        //hint = "PgDn";
      }
      else if (d >=  MEdgeDistance) {
        //hint = "left thumb";
      }
      else {
        MHoverEdge = 0;
        //hint = "left";
      }
    }
    else {
      d = d2 * mrect.w;
      if (d <= -MEdgeDistance) {
        //hint = "right thumb";
      }
      else if (d >=  MEdgeDistance) {
        //hint = "PgUp";
      }
      else {
        MHoverEdge = 1;
        //hint = "right";
      }
    }

    if (MHoverEdge != prev) {
      //do_widget_setHint(this,hint);
      do_widget_redraw(this,0);
      if (MHoverEdge >= 0) {
        do_widget_cursor(this,MIP_CURSOR_ARROW_LEFT_RIGHT);
      }
      else {
        do_widget_cursor(this,MIP_CURSOR_DEFAULT);
      }
    }
  }

  //----------

  void drawValues(MIP_PaintContext* AContext) {
    MIP_Painter* painter = AContext->painter;
    if (MDrawValueText) {
      float value1 = getValue();
      float value2 = getValue2();
      const char* label = "";
//      MIP_Parameter* param = getParameter();
//      if (param) {
//        label = param->getLabel();
//        param->displayText(MValue1Text,value1);
//        param->displayText(MValue2Text,value2);
//      }
//      else {
        label = MLabel;
        MIP_FloatToString(MValue1Text,value1);
        MIP_FloatToString(MValue2Text,value2);
//      }
      MIP_DRect value1_rect = getRect();
      MIP_DRect value2_rect = getRect();
      MIP_DRect label_rect = getRect();
      value1_rect.shrink(MValue1TextOffset);
      value2_rect.shrink(MValue2TextOffset);
      //label_rect.shrink(MValueTextOffset);
      //if (MDrawLabel) {
      //  float width = APainter->getTextWidth(label);
      //  label_rect.x = value_rect.x2() - width;
      //  label_rect.w = width;
      //  value_rect.w -= (width + MIP_VALUE_WIDGET_LABEL_SPACE);
      //}
      if (MDrawValueText) {

        //APainter->setColor(MValue1TextColor);
        //APainter->drawText(value1_rect,MValue1Text,MValue1TextAlignment);
        //APainter->setColor(MValue2TextColor);
        //APainter->drawText(value2_rect,MValue2Text,MValue2TextAlignment);

        painter->fontSize(12);
        painter->drawTextBox(value1_rect,MValue1Text,MValue1TextAlignment,MValue1TextColor);
        painter->drawTextBox(value2_rect,MValue2Text,MValue2TextAlignment,MValue2TextColor);


      }
      if (MDrawLabel) {

        //APainter->setColor(MLabelColor);
        //APainter->drawText(label_rect,label,MIP_TEXT_ALIGN_CENTER);

        painter->drawTextBox(label_rect,label,MIP_TEXT_ALIGN_CENTER,MLabelColor);

      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();
    //APainter->fillRectangle( mrect, MBackgroundColor );
    fillBackground(AContext);
    float x1 = mrect.x + (mrect.w * getValue());
    float x2 = mrect.x + (mrect.w * getValue2()) - 1;
    float w  = (x2 - x1 + 1);
    //if (w > 0) {
      if (w < 3) {

        //APainter->fillRectangle( MIP_DRect(x1-1,mrect.y,2,mrect.h), MBarColor );
        painter->beginPath();
        painter->rectangle(MIP_DRect(x1-1,mrect.y,2,mrect.h));
        painter->fillColor(MBarColor);
        painter->fill();

      }
      else {

        //APainter->fillRectangle( MIP_DRect(x1,mrect.y,w,mrect.h), MBarColor );
        painter->beginPath();
        painter->rectangle(MIP_DRect(x1,mrect.y,w,mrect.h));
        painter->fillColor(MBarColor);
        painter->fill();

      }
    //}
    if (MHoverEdge == 0) {

      //APainter->fillRectangle( MIP_DRect(x1,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
      painter->beginPath();
      painter->rectangle(MIP_DRect(x1,mrect.y,MEdgeWidth,mrect.h));
      painter->fillColor(MEdgeColor);
      painter->fill();

    }
    else if (MHoverEdge == 1) {

      //APainter->fillRectangle( MIP_DRect(x2-MEdgeWidth,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
      painter->beginPath();
      painter->rectangle(MIP_DRect(x2-MEdgeWidth,mrect.y,MEdgeWidth,mrect.h));
      painter->fillColor(MEdgeColor);
      painter->fill();

    }
    //drawLabel(APainter,ARect,AMode);
    drawValues(AContext);
    drawBorder(AContext);
  }

  //----------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    if (AButton == MIP_BUTTON_LEFT) {
      if (MHoverEdge==0) MIsDragging1 = true;
      else if (MHoverEdge==1) MIsDragging2 = true;
      //MIsInteractive = true;
      //if (hasFlag(MIP_WIDGET_HIDECURSOR)) do_setCursor(this,MIP_CURSOR_HIDE);
      do_widget_redraw(this,0);
    }
  }

  //----------

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    if (AButton == MIP_BUTTON_LEFT) {
      MIsDragging1 = false;
      MIsDragging2 = false;
      if (getRect().contains(AXpos,AYpos)) findHoverEdge(AXpos);
      else {
        MHoverEdge = -1;
        do_widget_cursor(this,MIP_CURSOR_DEFAULT);
      }
      //MIsInteractive = false;
      //if (hasFlag(MIP_WIDGET_HIDECURSOR)) do_setCursor(this,MIP_CURSOR_SHOW);
      do_widget_redraw(this,0);
    }
  }

  //----------

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_DRect mrect = getRect();
    float v = (AXpos - mrect.x) / mrect.w;
    v = MIP_Clamp(v,0,1);
    float value1 = getValue();
    float value2 = getValue2();
    if (MIsDragging1) {
      value1 = v;
      if (value1 > value2) {
        setValue(value2);
        setValue2(value1);
        //MIP_Swap(MIsDragging1,MIsDragging2);
        {
          bool temp = MIsDragging1;
          MIsDragging1 = MIsDragging2;
          MIsDragging2 = temp;
        }
        MHoverEdge = 1;
      }
      else {
        setValue(value1);
        setValue2(value2);
      }
      do_widget_redraw(this,0);
    }
    else if (MIsDragging2) {
      value2 = v;
      if (value1 > value2) {
        setValue(value2);
        setValue2(value1);
        //MIP_Swap(MIsDragging1,MIsDragging2);
        {
          bool temp = MIsDragging1;
          MIsDragging1 = MIsDragging2;
          MIsDragging2 = temp;
        }
        MHoverEdge = 0;
      }
      else {
        setValue(value1);
        setValue2(value2);
      }
      do_widget_update(this);
      do_widget_redraw(this,0);
    }
    else {
      findHoverEdge(AXpos);
    }
  }

  //----------

  void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {
    //MIP_PanelWidget::on_widget_enter(AFrom,AXpos,AYpos,ATime);
    findHoverEdge(AXpos);
  }

  //----------

  void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {
    //MIP_PanelWidget::on_widget_leave(ATo,AXpos,AYpos,ATime);
    do_widget_cursor(this,MIP_CURSOR_DEFAULT);
    if (MHoverEdge >= 0) {
      MHoverEdge = -1;
      do_widget_redraw(this,0);
    }
  }



};

//----------------------------------------------------------------------
#endif



















/*
  todo:

  * drag middle section to modify both values..

  * use drag_value instead of mouse x pos ?

  * base on slider
    - drawing
    - grab/hide cursor
    - shift-drag
*/


#if 0

class MIP_DualSliderWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  char      MValue1Text[32]       = {0};
  MIP_Color MValue1TextColor      = MIP_Color(0.0f);
  uint32_t  MValue1TextAlignment  = MIP_TEXT_ALIGN_LEFT;
  MIP_DRect MValue1TextOffset     = MIP_DRect(2,2,2,2);

  float     MValue2               = 0.5f;

  char      MValue2Text[32]       = {0};
  MIP_Color MValue2TextColor      = MIP_Color(0.0f);
  uint32_t  MValue2TextAlignment  = MIP_TEXT_ALIGN_RIGHT;
  MIP_DRect MValue2TextOffset     = MIP_DRect(2,2,2,2);

  int32_t   MHoverEdge            = -1;
  MIP_Color MEdgeColor            = MIP_COLOR_WHITE;
  uint32_t  MEdgeWidth            = 3;
  int32_t   MEdgeDistance         = 3;

  bool      MIsDragging1          = false;
  bool      MIsDragging2          = false;

//------------------------------
public:
//------------------------------

  MIP_DualSliderWidget(MIP_DRect ARect, const char* AText="", double AValue=0.0)
  : MIP_SliderWidget(ARect,AText,AValue) {
    MName = "MIP_DualSliderWidget";
    //MHint("dual_slider");
    State.autoSetCursor      = false;
    State.autoHideCursor     = true;
    //flags.autoMouseRedraw = true;
  }

  virtual ~MIP_DualSliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void  setValue2(float AValue)               { MValue2 = AValue; }
  virtual void  setBackgroundColor(MIP_Color AColor)  { MBackgroundColor = AColor; }
  //virtual void  setBarColor(MIP_Color AColor)         { MBarColor = AColor; }
  virtual void  setEdgeColor(MIP_Color AColor)        { MEdgeColor = AColor; }
  //----------

  virtual float getValue2(void) { return MValue2; }

//------------------------------
private:
//------------------------------

  virtual void findHoverEdge(float AXpos, bool ASetCursor=true) {
    MIP_DRect mrect = getRect();
    float v = (AXpos - mrect.x) / mrect.w;
    float d1 = v - getValue();
    float d2 = v - getValue2();
    int32_t d = 0;
    int32_t prev = MHoverEdge;
    MHoverEdge = -1;
    //const char* hint = "";
    if (fabs(d1) <= fabs(d2)) {
      d = d1 * mrect.w;
      if (d <= -MEdgeDistance) {
        //hint = "PgDn";
      }
      else if (d >=  MEdgeDistance) {
        //hint = "left thumb";
      }
      else {
        MHoverEdge = 0;
        //hint = "left";
      }
    }
    else {
      d = d2 * mrect.w;
      if (d <= -MEdgeDistance) {
        //hint = "right thumb";
      }
      else if (d >=  MEdgeDistance) {
        //hint = "PgUp";
      }
      else {
        MHoverEdge = 1;
        //hint = "right";
      }
    }

    if (MHoverEdge != prev) {
      //do_widget_setHint(this,hint);
      do_widget_redraw(this,0);
      if (ASetCursor) {
        if (MHoverEdge >= 0) {
          do_widget_cursor(this,MIP_CURSOR_ARROW_LEFT_RIGHT);
        }
        else {
          do_widget_cursor(this,MIP_CURSOR_DEFAULT);
        }
      }
    }
  }

  //----------

  void drawValues(MIP_PaintContext* AContext) {
    MIP_Painter* painter = AContext->painter;
    if (MDrawValue) {
      float value1 = getValue();
      float value2 = getValue2();
      const char* label = "";
//      MIP_Parameter* param = getParameter();
//      if (param) {
//        label = param->getLabel();
//        param->displayText(MValue1Text,value1);
//        param->displayText(MValue2Text,value2);
//      }
//      else {
        label = MText;
        MIP_FloatToString(MValue1Text,value1);
        MIP_FloatToString(MValue2Text,value2);
//      }
      MIP_DRect value1_rect = getRect();
      MIP_DRect value2_rect = getRect();
      MIP_DRect label_rect = getRect();
      value1_rect.shrink(MValue1TextOffset);
      value2_rect.shrink(MValue2TextOffset);
      //label_rect.shrink(MValueTextOffset);
      //if (MDrawLabel) {
      //  float width = APainter->getTextWidth(label);
      //  label_rect.x = value_rect.x2() - width;
      //  label_rect.w = width;
      //  value_rect.w -= (width + MIP_VALUE_WIDGET_LABEL_SPACE);
      //}
      if (MDrawValue) {
//        APainter->setFontSize(14);

        //APainter->setColor(MValue1TextColor);
        //APainter->drawText(value1_rect,MValue1Text,MValue1TextAlignment);
        //APainter->setColor(MValue2TextColor);
        //APainter->drawText(value2_rect,MValue2Text,MValue2TextAlignment);

        painter->drawTextBox(value1_rect,MValue1Text,MValue1TextAlignment,MValue1TextColor);
        painter->drawTextBox(value2_rect,MValue2Text,MValue2TextAlignment,MValue2TextColor);

      }
      if (MDrawText) {

        //APainter->setColor(MLabelColor);
        //APainter->drawText(label_rect,label,MIP_TEXT_ALIGN_CENTER);

        painter->drawTextBox(label_rect,label,MIP_TEXT_ALIGN_CENTER,MTextColor);

      }
    }
  }

  //----------

  virtual void drawValueBar2(MIP_PaintContext* AContext) {
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();
    float x1 = mrect.x + (mrect.w * getValue());
    float x2 = mrect.x + (mrect.w * getValue2()) - 1;
    float w  = (x2 - x1 + 1);
    //if (w > 0) {
      if (w < 3) {

        //APainter->fillRectangle( MIP_DRect(x1-1,mrect.y,2,mrect.h), MValueBarColor );
        painter->beginPath();
        painter->rectangle(MIP_DRect(x1-1,mrect.y,2,mrect.h));
        painter->fillColor(MSliderColor);
        painter->fill();

      }
      else {

        //APainter->fillRectangle( MIP_DRect(x1,mrect.y,w,mrect.h), MValueBarColor );
        painter->beginPath();
        painter->rectangle(MIP_DRect(x1,mrect.y,w,mrect.h));
        painter->fillColor(MSliderColor);
        painter->fill();

      }
    //}
    if (MHoverEdge == 0) {

      //APainter->fillRectangle( MIP_DRect(x1,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
      painter->beginPath();
      painter->rectangle(MIP_DRect(x1,mrect.y,MEdgeWidth,mrect.h));
      painter->fillColor(MEdgeColor);
      painter->fill();

    }
    else if (MHoverEdge == 1) {

      //APainter->fillRectangle( MIP_DRect(x2-MEdgeWidth,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
      painter->beginPath();
      painter->rectangle(MIP_DRect(x2-MEdgeWidth,mrect.y,MEdgeWidth,mrect.h));
      painter->fillColor(MEdgeColor);
      painter->fill();

    }
  }

//------------------------------
public:
//------------------------------

  //TODO

  float calcValueFromMouse(double AXpos, double AYpos, uint32_t AState) {
    return 0.0;
  }

//------------------------------
public: // parent to child
//------------------------------



  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    //paintChildren(APainter,getRect(),AMode);
    drawValueBar2(AContext);
//    drawText(APainter,ARect,AMode);
    drawValues(AContext);
    drawBorder(AContext);
  }

  //----------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_SliderWidget::on_widget_mouse_click(AButton,AState,AXpos,AYpos,ATime);
    if (AButton == MIP_BUTTON_LEFT) {
      if (MHoverEdge==0) MIsDragging1 = true;
      else if (MHoverEdge==1) MIsDragging2 = true;
      //MIsInteractive = true;
      //if (hasFlag(MIP_WIDGET_HIDECURSOR)) do_setCursor(this,MIP_CURSOR_HIDE);
//      do_widget_redraw(this,getRect(),0);
    }
  }

  //----------

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_SliderWidget::on_widget_mouse_release(AButton,AState,AXpos,AYpos,ATime);
    if (AButton == MIP_BUTTON_LEFT) {
      MIsDragging1 = false;
      MIsDragging2 = false;
      if (getRect().contains(AXpos,AYpos)) findHoverEdge(AXpos);
      else {
        MHoverEdge = -1;
        do_widget_cursor(this,MIP_CURSOR_DEFAULT);
      }
      //MIsInteractive = false;
      //if (hasFlag(MIP_WIDGET_HIDECURSOR)) do_setCursor(this,MIP_CURSOR_SHOW);
//      do_widget_redraw(this,getRect(),0);
    }
  }

  //----------

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {

    //float preValue = getValue();
    //float preValue2 = getValue2();

    //MIP_SliderWidget::on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);

    //float v = calcValueFromMouse(AXpos,AYpos,AState);
    //MIP_Print("v %f\n",v);
    float value1 = getValue();
    float value2 = getValue2();
    if (MIsDragging1) {

      value1 = calcValueFromMouse(AXpos,AYpos,AState);//v;

      if (value1 > value2) {
        setValue(value2);
        setValue2(value1);
        //MIP_Swap(MIsDragging1,MIsDragging2);
        {
          bool temp = MIsDragging1;
          MIsDragging1 = MIsDragging2;
          MIsDragging2 = temp;
        }
        MHoverEdge = 1;
      }
      else {
        setValue(value1);
        setValue2(value2);
      }
      do_widget_update(this);
      do_widget_redraw(this,0);
    }
    else if (MIsDragging2) {
      value2 = calcValueFromMouse(AXpos,AYpos,AState);//v;
      if (value1 > value2) {
        setValue(value2);
        setValue2(value1);
        //MIP_Swap(MIsDragging1,MIsDragging2);
        {
          bool temp = MIsDragging1;
          MIsDragging1 = MIsDragging2;
          MIsDragging2 = temp;
        }
        MHoverEdge = 0;
      }
      else {
        setValue(value1);
        setValue2(value2);
      }
      do_widget_update(this);
      do_widget_redraw(this,0);
    }
    //if (!(MIsDragging1 || MIsDragging2)) {
    else {
      findHoverEdge(AXpos,false);
    }
  }

  //----------

  void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {
    findHoverEdge(AXpos,false);
    //if (flags.autoHint) do_widget_setHint(this,getHint());
    //MIP_SliderWidget::on_widget_enter(AXpos,AYpos,AFrom,ATimeStamp);
  }

  //----------

  void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {
    if (MHoverEdge >= 0) {
      MHoverEdge = -1;
      do_widget_redraw(this,0);
      do_widget_cursor(this,MIP_CURSOR_DEFAULT);
    }
    do_widget_cursor(this,MIP_CURSOR_DEFAULT);
    //if (flags.autoHint) do_widget_setHint(this,"");
    //MIP_SliderWidget::on_widget_leave(AXpos,AYpos,ATo,ATimeStamp);
  }

};

#endif // 0

