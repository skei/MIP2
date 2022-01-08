#if 0

#ifndef kode_modmatrix_widget_included
#define kode_modmatrix_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"
#include "gui/kode_widgets.h"
#include "gui/widgets/kode_selector_widget.h"
#include "gui/widgets/kode_drag_value_widget.h"

//#include "audio/kode_modmatrix.h"

//----------------------------------------------------------------------

class KODE_ModMatrixRow
: public KODE_Widget {

  private:

      KODE_SelectorWidget*   wdg_src     = KODE_NULL;
      KODE_SelectorWidget*   wdg_dst     = KODE_NULL;
      KODE_DragValueWidget*  wdg_amount  = KODE_NULL;
      KODE_DragValueWidget*  wdg_scale   = KODE_NULL;
      KODE_DragValueWidget*  wdg_offset  = KODE_NULL;

    //KODE_Selector*   wdg_rate    = KODE_NULL;
    //KODE_Selector*   wdg_mode    = KODE_NULL;

  //------------------------------
  //
  //------------------------------

  public:

    KODE_ModMatrixRow(KODE_Rect ARect, KODE_MenuWidget* ASrcMenu, KODE_MenuWidget* ADstMenu, uint32_t ASrc, uint32_t ADst, float AAmt, float AScale, float AOffset, uint32_t ARate, uint32_t AMode)
    : KODE_Widget(ARect) {
      MName = "KODE_ModMatrixItemWidget";
      MAlignment = KODE_WIDGET_ALIGN_PARENT;

      //setPadding(1);
      float x  = 0;//MRect.x;
      float y  = 0;//MRect.y;
      float w1 = ((MRect.w-4) * 27) / 100;
      float w2 = ((MRect.w-4) * 15) / 100;
      float h  = ARect.h;

      wdg_src = new KODE_SelectorWidget( KODE_Rect(x,y,w1,h), ASrcMenu);
        wdg_src->setSelected(ASrc);
        wdg_src->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
      appendWidget(wdg_src);
      x += w1 + 1;

      wdg_dst = new KODE_SelectorWidget( KODE_Rect(x,y,w1,h), ADstMenu);
        wdg_dst->setSelected(ADst);
        wdg_dst->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
      appendWidget(wdg_dst);
      x += w1 + 1;

      wdg_amount = new KODE_DragValueWidget( KODE_Rect(x,y,w2,h), AAmt);
        wdg_amount->setValue(AAmt);
        //wdg_amount->drawName(false);
        //wdg_amount->offValue(0);

        wdg_amount->setBackgroundColor(KODE_Grey);
        wdg_amount->setTextColor(KODE_Black);
        wdg_amount->setFillBackground();
        //wdg_amount->setTextAlignment(KODE_TEXT_ALIGN_RIGHT);
        wdg_amount->setDrawText(false);
        wdg_amount->setValueAlignment(KODE_TEXT_ALIGN_RIGHT);
      appendWidget(wdg_amount);
      x += w2 + 1;

      wdg_scale = new KODE_DragValueWidget( KODE_Rect(x,y,w2,h), AScale);
        wdg_scale->setValue(AScale);
        wdg_scale->setSensitivity(0.0003f);   // def 0.01
        //wdg_scale->setSensitivity2(0.05f);  // def 0.05
        //wdg_scale->drawName(false);
        //wdg_scale->offValue(1);
        wdg_scale->setBackgroundColor(KODE_Grey);
        wdg_scale->setTextColor(KODE_Black);
        wdg_scale->setFillBackground();
        //wdg_scale->setTextAlignment(KODE_TEXT_ALIGN_RIGHT);
        wdg_scale->setDrawText(false);
        wdg_scale->setValueAlignment(KODE_TEXT_ALIGN_RIGHT);
      appendWidget(wdg_scale);
      x += w2 + 1;

      wdg_offset = new KODE_DragValueWidget( KODE_Rect(x,y,w2,h), AOffset);
        wdg_offset->setValue(AOffset);
        //wdg_offset->drawName(false);
        //wdg_offset->offValue(0);
        wdg_offset->setBackgroundColor(KODE_Grey);
        wdg_offset->setTextColor(KODE_Black);
        wdg_offset->setFillBackground();
        //wdg_offset->setTextAlignment(KODE_TEXT_ALIGN_RIGHT);
        wdg_offset->setDrawText(false);
        wdg_offset->setValueAlignment(KODE_TEXT_ALIGN_RIGHT);
      appendWidget(wdg_offset);
      //x += w2 + 1;

      //wdg_rate = new KODE_SelectorWidget( KODE_Rect(x,y,w1,h), ARateMenu);
      //  wdg_rate->setSelected(ASrc);
      //  wdg_rate->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
      //appendWidget(wdg_rate);
      //x += w1 + 1;

      //wdg_mode = new KODE_SelectorWidget( KODE_Rect(x,y,w1,h), AModeMenu);
      //  wdg_mode->setSelected(ADst);
      //  wdg_mode->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
      //appendWidget(wdg_mode);
      //x += w1 + 1;

    }

    //----------

    virtual ~KODE_ModMatrixRow() {
      //delete MAmountParam;
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual KODE_SelectorWidget*   getSrcWidget(void)    { return wdg_src; }
    virtual KODE_SelectorWidget*   getDstWidget(void)    { return wdg_dst; }
    virtual KODE_DragValueWidget*  getAmountWidget(void) { return wdg_amount; }
    virtual KODE_DragValueWidget*  getScaleWidget(void)  { return wdg_scale; }
    virtual KODE_DragValueWidget*  getOffsetWidget(void) { return wdg_offset; }

    //virtual KODE_SelectorWidget*   getRateWidget(void)    { return wdg_rate; }
    //virtual KODE_SelectorWidget*   getModeWidget(void)    { return wdg_mode; }

  //------------------------------
  //
  //------------------------------

  public:


};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_ModMatrixWidget
: public KODE_Widget {

  private:

    KODE_MenuWidget*  MSrcMenu  = KODE_NULL;
    KODE_MenuWidget*  MDstMenu  = KODE_NULL;
    uint32_t          MNumRows  = 0;

//------------------------------
//
//------------------------------

public:

  KODE_ModMatrixWidget(KODE_Rect ARect, KODE_MenuWidget* ASrcMenu, KODE_MenuWidget* ADstMenu)
  : KODE_Widget(ARect) {
    MSrcMenu = ASrcMenu;
    MDstMenu = ADstMenu;
  }

  //----------

  virtual ~KODE_ModMatrixWidget() {
  }

//------------------------------
//
//------------------------------

public:

  virtual KODE_Widget* getWidget(uint32_t x, uint32_t y) {
    KODE_Widget* w1 = getChild(y);
    if (w1) {
      return w1->getChild(x);
    }
    else return KODE_NULL;
  }

//------------------------------
//
//------------------------------

public:

  KODE_ModMatrixRow* appendRow(uint32_t src=0, uint32_t dst=0, float amt=0.0f, float scale=1.0f, float offset=0.0f, uint32_t rate=0, uint32_t mode=0 ) {
    float y = MNumRows * 17;
    KODE_ModMatrixRow* item = new KODE_ModMatrixRow(
      KODE_Rect(0,y, MRect.w, 16),
      MSrcMenu,
      MDstMenu,
      src,
      dst,
      amt,
      scale,
      offset,
      rate,
      mode
    );
    appendWidget(item);
    MNumRows += 1;
    return item;
  }

//------------------------------
//
//------------------------------

public:

};

//----------------------------------------------------------------------
#endif

#endif // 0

