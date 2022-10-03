#ifndef mip_button_row_widget_included
#define mip_button_row_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_grid_widget.h"

#define MIP_MAX_STATES 32

#define MIP_BUTTON_ROW_SINGLE  0
#define MIP_BUTTON_ROW_MULTI   1


class MIP_ButtonRowWidget
: public MIP_GridWidget {

//------------------------------
protected:
//------------------------------

  int32_t     MMode                   = 0;
  int32_t     MSelected               = 0;
  bool        MStates[MIP_MAX_STATES] = {0};
  const char* MLabels[MIP_MAX_STATES] = {0};

  MIP_Color   MTextColor              = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
  MIP_Color   MActiveTextColor        = MIP_Color(0);//MIP_COLOR_BLACK;

  double      MTextSize               = 12.0;

  MIP_Color   MBackgroundCellColor    = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;
  MIP_Color   MActiveCellColor        = MIP_Color(0.5);//MIP_COLOR_GRAY;

  bool        MValueIsBits            = false;
  uint32_t    MNumBits                = 0;

  bool        MDrawRoundedBottom      = true;
  float       MRounded                = 8;

//------------------------------
public:
//------------------------------

  MIP_ButtonRowWidget(MIP_DRect ARect, int32_t ANum, const char** ATxt=nullptr, int32_t AMode=MIP_BUTTON_ROW_SINGLE)
  : MIP_GridWidget(ARect,ANum,1) {

    MName = "MIP_ButtonRowWidget";
    //setHint("buttonrow");
    MMouseCursor = MIP_CURSOR_FINGER;
    MMode                 = AMode;
    MSelected             = 0;
    MDrawSelectedCells    = false;

    MDrawHorizontalLines  = false;
    MDrawVerticalLines    = false;

    if (ATxt) {
      for (int32_t i=0; i<ANum; i++) {
        MStates[i] = false;
        MLabels[i] = (char*)ATxt[i];
      }
    }

    setFillBackground(true);
    setBackgroundColor(0.6);
    setDrawBorder(false);
  }

  //----------

  virtual ~MIP_ButtonRowWidget() {
  }

//------------------------------
public:
//------------------------------

  int32_t getSelected(void) {
    return MSelected;
  }

  void setValueIsBits(bool AIsBits=true, uint32_t ANumBits=8) {
    MValueIsBits = AIsBits;
    MNumBits = ANumBits;
  }

  void setDrawRoundedBottom(bool AState) {
    MDrawRoundedBottom = AState;
  }

  void setTextSize(double ASize) { MTextSize = ASize; }

//------------------------------
public:
//------------------------------

  void setNumButtons(uint32_t ANum) {
    setNumColumns(ANum);
  }

  //----------

  bool getButtonState(int32_t i) {
    return MStates[i];
  }

  void setButtonState(int32_t i, bool s) {
    MStates[i] = s;
  }


  //----------

  uint32_t getButtonBits() {
    uint32_t bits = 0;
    for (uint32_t i=0; i<MNumBits; i++) {
      if (MStates[i] == true) bits |= (1 << i);
    }
    return bits;
  }

  void setButtonBits(uint32_t ABits) {
    for (uint32_t i=0; i<MNumBits; i++) {
      bool b = ( ABits & (1 << i) );
      MStates[i] = b;
    }
  }

  //----------

  const char* getButtonName(int32_t i) {
    return MLabels[i];
  }

  void setButtonName(int32_t i, const char* AName) {
    MLabels[i] = (char*)AName;
  }

  //----------

//  float getValue() override {
//    if (MValueIsBits) {
//      uint32_t bits = getButtonBits();
//      float v = (float)bits / 255.0;
//      MIP_GridWidget::setValue(v);
//      return v;
//    }
//    else {
//      return MIP_GridWidget::getValue();
//    }
//  }
//
//  void setValue(float AValue) override {
//    if (MValueIsBits) {
//      float f = AValue * 255.0;
//      int i = (int)f;
//      setButtonBits(i);
//      MIP_GridWidget::setValue(AValue);
//    }
//    else {
//      MIP_GridWidget::setValue(AValue);
//      selectValue(AValue);
//    }
//  }

  //----------

  void selectButton(int32_t index) {
    MSelected = index;
    if (MMode == MIP_BUTTON_ROW_SINGLE) {
      for (int32_t i=0; i<MNumColumns; i++) {
        if (i==MSelected) MStates[i] = true;
        else MStates[i] = false;
      }
//      float v = (float)MSelected / ((float)MNumColumns - 1.0f);
//      MIP_Widget::setValue(v);
    }
    else {
      MStates[MSelected] = MStates[MSelected] ? false : true;
    }
  }

  //----------

  void selectValue(float AValue) {
    float num = AValue * MNumColumns;
    num = MIP_Min(num,float(MNumColumns-1));
    selectButton( (int)num );
  }

//------------------------------
public:
//------------------------------

  void on_clickCell(int32_t AX, int32_t AY, int32_t AB) override {
    selectButton(AX);
    do_widget_update(this);
    do_widget_redraw(this);
  }

  //----------

  void on_paintCell(MIP_PaintContext* AContext, MIP_DRect ARect, int32_t AX, int32_t AY) override {
    char buf[256];
    MIP_Painter* painter= AContext->painter;
    MIP_Color c1,c2;
    if (MStates[AX]) c1 = MActiveCellColor;
    else c1 = MBackgroundCellColor;

    painter->beginPath();
    painter->rect(ARect.x,ARect.y,ARect.w,ARect.h);
    painter->fillColor(c1);
    painter->fill();

    painter->beginPath();
    painter->rect(ARect.x,ARect.y,ARect.w,ARect.h);
    painter->strokeColor( MIP_Color(0.25) /*MIP_COLOR_DARK_GRAY*/ );
    painter->strokeWidth(1);
    painter->stroke();

    const char* txt = MLabels[AX];
    MIP_Color color = MTextColor;
    if (MStates[AX]) color = MActiveTextColor;
    else color = MTextColor;
    if (txt) {
      double textsize = MTextSize;
      if (MTextSize < 0) { textsize = MRect.h * (- MTextSize); }
      painter->fontSize(textsize);
      painter->drawTextBox(ARect,txt,MIP_TEXT_ALIGN_CENTER,color);

    }
    else {
      sprintf(buf,"%i",AX);
      double textsize = MTextSize;
      if (MTextSize < 0) { textsize = MRect.h * (- MTextSize); }
      painter->fontSize(textsize);
      painter->drawTextBox(ARect,buf,MIP_TEXT_ALIGN_CENTER,color);
    }
  }

};

#undef MIP_MAX_STATES

//----------------------------------------------------------------------
#endif
