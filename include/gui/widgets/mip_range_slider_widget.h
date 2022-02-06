#ifndef mip_range_slider_widget_included
#define mip_range_slider_widget_included
//----------------------------------------------------------------------

/*
  todo:

  * drag middle section to modify both values..

  * use drag_value instead of mouse x pos ?

  * base on slider
    - drawing
    - grab/hide cursor
    - shift-drag
*/

/*
*/


#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------

class MIP_RangeSliderWidget
: public MIP_PanelWidget {
//: public MIP_DragValueWidget {

//------------------------------
protected:
//------------------------------

  float       MValue2               = 0.0f;
  bool        MDrawValueText        = true;

  MIP_Color  MBackgroundColor      = MIP_COLOR_GRAY;
  MIP_Color  MBarColor             = MIP_COLOR_LIGHT_GRAY;

  MIP_Color  MEdgeColor            = MIP_COLOR_WHITE;
  uint32_t    MEdgeWidth            = 3;
  int32_t     MHoverEdge            = -1;
  bool        MIsDragging1          = false;
  bool        MIsDragging2          = false;
  int32_t     MEdgeDistance         = 3;

  char        MValue1Text[32]       = {0};
  char        MValue2Text[32]       = {0};

  MIP_Color  MValue1TextColor      = MIP_Color(0.0f);
  uint32_t    MValue1TextAlignment  = MIP_TEXT_ALIGN_LEFT;
  MIP_FRect  MValue1TextOffset     = MIP_FRect(2,2,2,2);

  MIP_Color  MValue2TextColor      = MIP_Color(0.0f);
  uint32_t    MValue2TextAlignment  = MIP_TEXT_ALIGN_RIGHT;
  MIP_FRect  MValue2TextOffset     = MIP_FRect(2,2,2,2);

  bool        MDrawLabel            = true;
  const char* MLabel                = "";
  MIP_Color  MLabelColor           = MIP_COLOR_BLACK;

//------------------------------
public:
//------------------------------

  MIP_RangeSliderWidget(MIP_FRect ARect)
  : MIP_PanelWidget(ARect) {
    setName("MIP_RangeSliderWidget");
    setHint("rangeslider");
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
    MIP_FRect mrect = getRect();
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
      do_widget_redraw(this,mrect,0);
      if (MHoverEdge >= 0) {
        do_widget_setMouseCursor(this,MIP_CURSOR_ARROWLEFTRIGHT);
      }
      else {
        do_widget_setMouseCursor(this,MIP_CURSOR_DEFAULT);
      }
    }
  }

  //----------

  void drawValues(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
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
      MIP_FRect value1_rect = getRect();
      MIP_FRect value2_rect = getRect();
      MIP_FRect label_rect = getRect();
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
//        APainter->setFontSize(14);
        APainter->drawText(value1_rect,MValue1Text,MValue1TextAlignment,MValue1TextColor);
        APainter->drawText(value2_rect,MValue2Text,MValue2TextAlignment,MValue2TextColor);
      }
      if (MDrawLabel) {
        APainter->drawText(label_rect,label,MIP_TEXT_ALIGN_CENTER,MLabelColor);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) final {
    MIP_FRect mrect = getRect();
    //APainter->fillRectangle( mrect, MBackgroundColor );
    fillBackground(APainter,ARect,AMode);
    float x1 = mrect.x + (mrect.w * getValue());
    float x2 = mrect.x + (mrect.w * getValue2()) - 1;
    float w  = (x2 - x1 + 1);
    //if (w > 0) {
      if (w < 3) {
        APainter->fillRectangle( MIP_FRect(x1-1,mrect.y,2,mrect.h), MBarColor );
      }
      else {
        APainter->fillRectangle( MIP_FRect(x1,mrect.y,w,mrect.h), MBarColor );
      }
    //}
    if (MHoverEdge == 0) {
      APainter->fillRectangle( MIP_FRect(x1,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
    }
    else if (MHoverEdge == 1) {
      APainter->fillRectangle( MIP_FRect(x2-MEdgeWidth,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
    }
    //drawLabel(APainter,ARect,AMode);
    drawValues(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == MIP_BUTTON_LEFT) {
      if (MHoverEdge==0) MIsDragging1 = true;
      else if (MHoverEdge==1) MIsDragging2 = true;
      //MIsInteractive = true;
      //if (hasFlag(MIP_WIDGET_HIDECURSOR)) do_setCursor(this,MIP_CURSOR_HIDE);
      do_widget_redraw(this,getRect(),0);
    }
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == MIP_BUTTON_LEFT) {
      MIsDragging1 = false;
      MIsDragging2 = false;
      if (getRect().contains(AXpos,AYpos)) findHoverEdge(AXpos);
      else {
        MHoverEdge = -1;
        do_widget_setMouseCursor(this,MIP_CURSOR_DEFAULT);
      }
      //MIsInteractive = false;
      //if (hasFlag(MIP_WIDGET_HIDECURSOR)) do_setCursor(this,MIP_CURSOR_SHOW);
      do_widget_redraw(this,getRect(),0);
    }
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
    MIP_FRect mrect = getRect();
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
      do_widget_redraw(this,mrect,0);
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
      do_widget_redraw(this,mrect,0);
    }
    else {
      findHoverEdge(AXpos);
    }
  }

  //----------

  void on_widget_mouseEnter(float AXpos, float AYpos, MIP_Widget* AFrom/*, uint32_t ATimeStamp=0*/) final {
    findHoverEdge(AXpos);
    if (flags.autoHint) do_widget_setHint(this,getHint(),0);
  }

  //----------

  void on_widget_mouseLeave(float AXpos, float AYpos, MIP_Widget* ATo/*, uint32_t ATimeStamp=0*/) final {
    if (MHoverEdge >= 0) {
      MHoverEdge = -1;
      do_widget_redraw(this,getRect(),0);
      do_widget_setMouseCursor(this,MIP_CURSOR_DEFAULT);
    }
    if (flags.autoHint) do_widget_setHint(this,"",0);
  }

};

//----------------------------------------------------------------------
#endif
