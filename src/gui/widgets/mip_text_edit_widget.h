#ifndef kode_text_edit_widget_included
#define kode_text_edit_widget_included
//----------------------------------------------------------------------

#include "base/utils/kode_strutils.h"
#include "gui/kode_widget.h"

#define kkc_enter     65293 // 13
#define kkc_esc       65307 // 27
#define kkc_home      65360 // 1
#define kkc_end       65367 // 2
#define kkc_left      65361 // 3
#define kkc_right     65363 // 4
#define kkc_delete    65535 // 5
#define kkc_backspace 65288 // 6

class KODE_TextEditWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

    char        MText[256]  = {0};
    KODE_Color  MTextColor  = KODE_COLOR_BLACK;
    KODE_Color  MBackColor  = KODE_COLOR_LIGHT_GRAY;
    KODE_Color  MCaretColor = KODE_COLOR_BRIGHT_RED;
    bool        MEditing    = false;
    int32_t     MCaretPos   = 0;


//------------------------------
public:
//------------------------------

  KODE_TextEditWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_TextEditWidget");
    setHint("textedit");
    setCursor(KODE_CURSOR_IBEAM);
    //strncpy(MText,AText,255);
  }

  //----------

  virtual ~KODE_TextEditWidget() {
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
    do_widget_grabKeyboard(this);
//    do_widget_grabModal(this);
    do_widget_redraw(this,getRect(),0);
  }

  //----------

  void stop_edit() {
    MEditing = false;
    //do_widget_update(this);
    do_widget_grabKeyboard(KODE_NULL);
//    do_widget_grabModal(KODE_NULL);
    do_widget_redraw(this,getRect(),0);
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    KODE_Widget::on_widget_paint(APainter,ARect,AMode);
    KODE_FRect mrect = getRect();
    KODE_FRect r;
    APainter->fillRectangle(mrect,MBackColor);
    if (MEditing) {
      //APainter->setTextColor(MTextColor);
      //APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-2,MRect.y2(),MText,KODE_TEXT_ALIGN_LEFT);
      r = mrect;
      r.shrink(2,0,2,0); // 2 = text border
      APainter->drawText(r,MText,KODE_TEXT_ALIGN_LEFT,MTextColor);
      char c = MText[MCaretPos];
      MText[MCaretPos] = 0;
      int32_t txtwidth = APainter->getTextWidth(MText);
      MText[MCaretPos] = c;
      int32_t x = getRect().x + 2 + txtwidth; // 2 = caret distance/width
      //APainter->setDrawColor(MCaretColor);
      //APainter->drawLine(x,MRect.y,x,MRect.y2());
      APainter->drawLine(x,mrect.y,x,mrect.y2(),MCaretColor);
    } // editing
    else {
      //APainter->setTextColor(MTextColor);
      //APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-4,MRect.y2(),MText,KODE_TEXT_ALIGN_LEFT);
      r = mrect;
      r.shrink(2,0,4,0);
      APainter->drawText(r,MText,KODE_TEXT_ALIGN_LEFT,MTextColor);
    } // not editing
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
    //KODE_Widget::on_widget_mouseClick(AXpos,AYpos,AButton,AState);
    switch(AButton) {
      case KODE_BUTTON_LEFT:
        if (getRect().contains(AXpos,AYpos)) {
          if (!MEditing) start_edit();
        }
        else {
          stop_edit();
        }
        break;
      case KODE_BUTTON_RIGHT:
        if (MEditing) stop_edit();
        break;
    }
  }

  //----------

  // AKey = key code, not ascii..

  void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
    //KODE_Print("AChar %i AKey %i AState %i\n",(int)AChar,AKey,AState);
    KODE_FRect mrect = getRect();
    int32_t len;
    char  c;
    //KODE_Widget::on_keyPress(AChar,AKey,AState);
    //STrace("key: %i, skift: %i\n",AKey,AState);
    switch(AKey) {
      case kkc_enter:
        stop_edit();
        break;
      case kkc_esc:
        stop_edit();
        break;
      case kkc_home:
        MCaretPos = 0;
        do_widget_update(this);
        do_widget_redraw(this,mrect,0);
        break;
      case kkc_end:
        len = strlen(MText);
        MCaretPos = len;
        do_widget_update(this);
        do_widget_redraw(this,mrect,0);
        break;
      case kkc_left:
        MCaretPos -= 1;
        if (MCaretPos < 0) MCaretPos = 0;
        do_widget_update(this);
        do_widget_redraw(this,mrect,0);
        break;
      case kkc_right:
        len = strlen(MText);
        MCaretPos += 1;
        if (MCaretPos > len) MCaretPos = len;
        do_widget_update(this);
        do_widget_redraw(this,mrect,0);
        break;
      case kkc_delete:
        len = strlen(MText);
        if ((uint32_t)MCaretPos < strlen(MText)) {
          KODE_DeleteChar(MText,MCaretPos);
          do_widget_update(this);
          do_widget_redraw(this,mrect,0);
        }
        break;
      case kkc_backspace:
        if (MCaretPos > 0) {
          MCaretPos -= 1;
          KODE_DeleteChar(MText,MCaretPos);
          do_widget_update(this);
          do_widget_redraw(this,mrect,0);
        }
        break;
      default:
        if ((AKey >= 32) && (AKey <= 127)) {
          c = AKey & 0xff;
          KODE_InsertChar(MText,MCaretPos,c);
          MCaretPos += 1;
          do_widget_update(this);
          do_widget_redraw(this,mrect,0);
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
