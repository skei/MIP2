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

  int32_t     MMode                     = 0;
  int32_t     MSelected                 = 0;
  bool        MStates[MIP_MAX_STATES]  = {0};
  const char* MLabels[MIP_MAX_STATES]  = {0};

  MIP_Color  MTextColor                = MIP_Color(0.8);
  MIP_Color  MActiveColor              = MIP_Color(0.4);

  bool        MValueIsBits              = false;
  uint32_t    MNumBits                  = 0;

//------------------------------
public:
//------------------------------

    //int32_t ANum, const char** ATxt, int32_t AMode=MIP_BUTTON_ROW_SINGLE)

  MIP_ButtonRowWidget(MIP_FRect ARect, int32_t ANum, const char** ATxt=nullptr, int32_t AMode=MIP_BUTTON_ROW_SINGLE)
  : MIP_GridWidget(ARect,ANum,1) {

    MIP_GridWidget::setName("MIP_ButtonRowWidget");
    setHint("buttonrow");
    setCursor(MIP_CURSOR_FINGER);
    //setBackgroundColor(MIP_GREY);
    //setFlag(kwf_opaque);

    MMode                 = AMode;
    MSelected             = 0;
    MDrawSelectedCells    = false;


    // grid
    //MDrawVerticalLines  = false;

    //MWidth = ANum;//5;
    //MHeight = 1;
    //for (int32 i=0; i<MIP_MAX_STATES; i++) {

    if (ATxt) {
      for (int32_t i=0; i<ANum; i++) {
        MStates[i] = false;
        //MLabels[i] = MIP_NULL;
        MLabels[i] = (char*)ATxt[i];
      }
    }

  }

  //----------

  virtual ~MIP_ButtonRowWidget() {
  }

//------------------------------
public:
//------------------------------

  void setValueIsBits(bool AIsBits=true, uint32_t ANumBits=8) {
    MValueIsBits = AIsBits;
    MNumBits = ANumBits;
  }

  int32_t getSelected(void)         { return MSelected; }
  bool    getState(uint32_t AIndex) { return MStates[AIndex]; }

  //----------

  float getValue() override {
    if (MValueIsBits) {
      uint32_t bits = getButtonBits();
      //MIP_Print("getButtonBits %i\n",bits);
      float v = (float)bits / 255.0;
      MIP_GridWidget::setValue(v);
      return v;
    }
    else {
      return MIP_GridWidget::getValue();
    }
  }

  //----------

  void setValue(float AValue) override {
    if (MValueIsBits) {
      float f = AValue * 255.0;
      int i = (int)f;
      setButtonBits(i);
      //MIP_Print("setButtonBits %i\n",i);
      MIP_GridWidget::setValue(AValue);
    }
    else {
      MIP_GridWidget::setValue(AValue);
      selectValue(AValue);
    }
  }


//------------------------------
public:
//------------------------------

    void setNumButtons(uint32_t ANum) {
      setNumColumns(ANum);
    }

    bool getButtonState(int32_t i) {
      return MStates[i];
    }

    //----------

    void setButtonState(int32_t i, bool s) {
      MStates[i] = s;
    }

    //----------

    const char* getButtonName(int32_t i) {
      return MLabels[i];
    }

    //----------

    void setButtonName(int32_t i, const char* AName) {
      MLabels[i] = (char*)AName;
    }

    //----------

    void selectButton(int32_t index/*, bool ARedraw=false*/) {
      MSelected = index;
      //KTrace(['click cell: ',AX,',',AY,',',AB,MIP_CR]);
      if (MMode == MIP_BUTTON_ROW_SINGLE) {
        //for (int32 i=0; i<MWidth; i++) { // FHeight if vertical

        for (int32_t i=0; i<MNumColumns; i++) { // FHeight if vertical
          if (i==MSelected) MStates[i] = true;
          else MStates[i] = false;
        }

        /*
        5 steps => 0,1,2,3,4 => 0, 0.25, 0.50, 0.75, 1.0
        selected/(steps-1)
        */
        float v = (float)MSelected / ((float)MNumColumns - 1.0f);
        MIP_Widget::setValue(v);
        //MIP_Trace("%0.3f\n",v);
      }
      else {
        MStates[MSelected] = MStates[MSelected] ? false : true;
      }
      //if (ARedraw) do_redraw(this,MRect);
    }

    //----------

    //function KWidget_ButtonRow.getValue: Single;
    //begin
    //  result := FValue;
    //end;

    void selectValue(float AValue) {
      float num = AValue * MNumColumns; // 0 = 0, 1 = 4, 0.999 = 3
      num = MIP_Min(num,float(MNumColumns-1));
      selectButton( (int)num );
      //do_redraw(self,FRect);
      //do_update(self);
    }

    //----------

    uint32_t getButtonBits() {
      uint32_t bits = 0;
      for (uint32_t i=0; i<MNumBits; i++) {
        if (MStates[i] == true) bits |= (1 << i);
      }
      return bits;
    }

    //----------

    // value 0..1
    // valuee 0..255
    // value int
    // &  << i, MStates

    void setButtonBits(uint32_t ABits) {
      for (uint32_t i=0; i<MNumBits; i++) {
        bool b = ( ABits & (1 << i) );
        MStates[i] = b;
      }
    }

//------------------------------
public:
//------------------------------

  void on_clickCell(/*KWidget* AWidget,*/ int32_t AX, int32_t AY, int32_t AB) override {
    selectButton(AX);
    do_widget_update(this);
    do_widget_redraw(this,getRect(),0);
  }

  //----------

  void on_paintCell(/*KWidget* AWidget,*/ MIP_Painter* APainter, MIP_FRect ARect, int32_t AX, int32_t AY) override {
    //MIP_Print("%i %i\n",AX,AY);

    char buf[256];
    //APainter->setPenSize(1);
    //if (MStates[AX]) APainter->setFillColor( MActiveColor );
    //else APainter->setFillColor( MBackgroundColor );
    //APainter->fillRectangle(ARect);

    if (MStates[AX]) APainter->fillRectangle(ARect,MActiveColor);
    else APainter->fillRectangle(ARect,MBackgroundColor);

    //if ((AX > 0) /*&& (AX < (MWidth-1))*/  ) {
    //  APainter->setDrawColor( MBorderColor );
    //  APainter->drawVLine(ARect.x,ARect.y,ARect.y2());
    //}

    //APainter->setTextColor(MTextColor);

    const char* txt = MLabels[AX];
    if (txt) {
      APainter->drawText(ARect,txt,MIP_TEXT_ALIGN_CENTER,MTextColor);
    }
    else {
      //MIP_IntToString(buf,AX);
      sprintf(buf,"%i",AX);
      APainter->drawText(ARect,buf,MIP_TEXT_ALIGN_CENTER,MTextColor);
    }

  }

};

#undef MIP_MAX_STATES

//----------------------------------------------------------------------
#endif
