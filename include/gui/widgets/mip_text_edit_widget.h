#ifndef mip_text_edit_widget_included
#define mip_text_edit_widget_included
//----------------------------------------------------------------------

#include "base/utils/mip_strutils.h"
#include "gui/mip_widget.h"

//#define mip_xcb_key_enter     65293 // 13
//#define mip_xcb_key_esc       65307 // 27
//#define mip_xcb_key_home      65360 // 1
//#define mip_xcb_key_end       65367 // 2
//#define mip_xcb_key_left      65361 // 3
//#define mip_xcb_key_right     65363 // 4
//#define mip_xcb_key_delete    65535 // 5
//#define mip_xcb_key_backspace 65288 // 6

class MIP_TextEditWidget
: public MIP_TextWidget {

//------------------------------
private:
//------------------------------

//  char        MText[256]  = {0};
//  MIP_Color  MTextColor  = MIP_COLOR_BLACK;
//  MIP_Color  MBackColor  = MIP_COLOR_LIGHT_GRAY;

  MIP_Color  MCaretColor = MIP_COLOR_BRIGHT_RED;
  bool        MEditing    = false;
  int32_t     MCaretPos   = 0;

//------------------------------
public:
//------------------------------

  MIP_TextEditWidget(MIP_DRect ARect, const char* AText)
  : MIP_TextWidget(ARect,AText) {

    MName = "MIP_TextEditWidget";
    //MHint = "textedit";
    MMouseCursor = MIP_CURSOR_IBEAM;
    //strncpy(MText,AText,255);

//    MTextSize = 10;
    MTextColor = MIP_COLOR_BLACK;
  }

  //----------

  virtual ~MIP_TextEditWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setText(const char* AText) {
    memset(MText,0,sizeof(MText));
    strncpy(MText,AText,255);
  }

//------------------------------
private:
//------------------------------

  void start_edit() {
    MEditing = true;
    MCaretPos = strlen(MText);
    //do_widget_update(this);
    do_widget_want_keys(this);
    do_widget_modal(this);
    do_widget_redraw(this,0);
  }

  //----------

  void stop_edit() {
    MEditing = false;
    //do_widget_update(this);
    do_widget_want_keys(nullptr);
    do_widget_modal(nullptr);
    do_widget_redraw(this,0);
  }

//------------------------------
public:
//------------------------------

  //void on_widget_paint(MIP_Painter* APainter, MIP_DRect ARect, uint32_t AMode) override {
  void on_widget_paint(MIP_PaintContext* AContext) override {
//    MIP_Widget::on_widget_paint(AContext);
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();
    MIP_DRect r;

//    //painter->fillRectangle(mrect,MBackColor);
//    painter->beginPath();
//    painter->rect(mrect.x,mrect.y,mrect.w,mrect.h);
//    painter->fillColor(MBackColor);
//    painter->fill();

    fillBackground(AContext);

    if (MEditing) {
      //APainter->setTextColor(MTextColor);
      //APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-2,MRect.y2(),MText,MIP_TEXT_ALIGN_LEFT);
      r = mrect;
      r.shrink(2,0,2,0); // 2 = text border

      //APainter->setColor(MTextColor);
      //APainter->drawText(r,MText,MIP_TEXT_ALIGN_LEFT);

      painter->beginPath();
      painter->fontSize(MTextSize);
      painter->drawTextBox(r,MText,MIP_TEXT_ALIGN_LEFT,MTextColor);

      char c = MText[MCaretPos];
      MText[MCaretPos] = 0;

      //int32_t txtwidth = painter->getTextWidth(MText);
      float bounds[4];
      painter->textBounds(MRect.x,MRect.y,MText,nullptr,bounds);
      int32_t txtwidth  = bounds[2] - bounds[0];
      //height = bounds[3] - bounds[1];


      MText[MCaretPos] = c;
      int32_t x = getRect().x + 2 + txtwidth; // 2 = caret distance/width
      //APainter->setDrawColor(MCaretColor);
      //APainter->drawLine(x,MRect.y,x,MRect.y2());

      //APainter->moveTo(x,mrect.y);
      //APainter->lineTo(x,mrect.y2());
      //APainter->setColor(MCaretColor);
      //APainter->strokePath();

      painter->beginPath();
      painter->moveTo(x,mrect.y);
      painter->lineTo(x,mrect.y2());
      painter->strokeColor(MCaretColor);
      painter->stroke();



    } // editing
    else {
      //APainter->setTextColor(MTextColor);
      //APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-4,MRect.y2(),MText,MIP_TEXT_ALIGN_LEFT);
      r = mrect;
      r.shrink(2,0,4,0);

      //APainter->setColor(MTextColor);
      //APainter->drawText(r,MText,MIP_TEXT_ALIGN_LEFT);

      painter->beginPath();
      painter->fontSize(MTextSize);
      painter->drawTextBox(r,MText,MIP_TEXT_ALIGN_LEFT,MTextColor);

    } // not editing

    drawBorder(AContext);

  }

  //----------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime=0) override {
    //MIP_Widget::on_widget_mouseClick(AXpos,AYpos,AButton,AState);
    switch(AButton) {
      case MIP_BUTTON_LEFT:
        if (getRect().contains(AXpos,AYpos)) {
          if (!MEditing) start_edit();
        }
        else {
          stop_edit();
        }
        break;
      case MIP_BUTTON_RIGHT:
        if (MEditing) stop_edit();
        break;
    }
  }

  //----------

  // AKey = key code, not ascii..

  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp=0) override {
    //MIP_Print("AChar %i AKey %i AState %i\n",(int)AChar,AKey,AState);
    MIP_DRect mrect = getRect();
    int32_t len;
    char  c;
    //MIP_Widget::on_keyPress(AChar,AKey,AState);
    //MIP_Print("key: %i, skift: %i\n",AKey,AState);
    switch(AKey) {
      case MIP_KEY_ENTER:
        stop_edit();
        break;
      case MIP_KEY_ESC:
        stop_edit();
        break;
      case MIP_KEY_HOME:
        MCaretPos = 0;
        do_widget_update(this);
        do_widget_redraw(this,0);
        break;
      case MIP_KEY_END:
        len = strlen(MText);
        MCaretPos = len;
        do_widget_update(this);
        do_widget_redraw(this,0);
        break;
      case MIP_KEY_LEFT:
        MCaretPos -= 1;
        if (MCaretPos < 0) MCaretPos = 0;
        do_widget_update(this);
        do_widget_redraw(this,0);
        break;
      case MIP_KEY_RIGHT:
        len = strlen(MText);
        MCaretPos += 1;
        if (MCaretPos > len) MCaretPos = len;
        do_widget_update(this);
        do_widget_redraw(this,0);
        break;
      case MIP_KEY_DELETE:
        len = strlen(MText);
        if ((uint32_t)MCaretPos < strlen(MText)) {
          MIP_DeleteChar(MText,MCaretPos);
          do_widget_update(this);
          do_widget_redraw(this,0);
        }
        break;
      case MIP_KEY_BACKSPACE:
        if (MCaretPos > 0) {
          MCaretPos -= 1;
          MIP_DeleteChar(MText,MCaretPos);
          do_widget_update(this);
          do_widget_redraw(this,0);
        }
        break;
      default:
        if ((AKey >= 32) && (AKey <= 127)) {
          c = AKey & 0xff;
          if ((c >= 'a') && (c <= 'z') && (AState == MIP_KEY_SHIFT)) {
            c -= 32;
          }
          MIP_InsertChar(MText,MCaretPos,c);
          MCaretPos += 1;
          do_widget_update(this);
          do_widget_redraw(this,0);
        }
        break;
    } // switch
  }

};

#undef kkc_enter
#undef kkc_esc
#undef kkc_home
#undef kkc_end
#undef kkc_left
#undef kkc_right
#undef kkc_delete
#undef kkc_backspace

//----------------------------------------------------------------------
#endif
