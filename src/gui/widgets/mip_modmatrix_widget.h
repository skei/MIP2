#if 0

#ifndef mip_modmatrix_widget_included
#define mip_modmatrix_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/mip_widgets.h"
#include "gui/widgets/mip_selector_widget.h"
#include "gui/widgets/mip_drag_value_widget.h"

//#include "audio/mip_modmatrix.h"

//----------------------------------------------------------------------

class MIP_ModMatrixRow
: public MIP_Widget {

  private:

      MIP_SelectorWidget*   wdg_src     = MIP_NULL;
      MIP_SelectorWidget*   wdg_dst     = MIP_NULL;
      MIP_DragValueWidget*  wdg_amount  = MIP_NULL;
      MIP_DragValueWidget*  wdg_scale   = MIP_NULL;
      MIP_DragValueWidget*  wdg_offset  = MIP_NULL;

    //MIP_Selector*   wdg_rate    = MIP_NULL;
    //MIP_Selector*   wdg_mode    = MIP_NULL;

  //------------------------------
  //
  //------------------------------

  public:

    MIP_ModMatrixRow(MIP_Rect ARect, MIP_MenuWidget* ASrcMenu, MIP_MenuWidget* ADstMenu, uint32_t ASrc, uint32_t ADst, float AAmt, float AScale, float AOffset, uint32_t ARate, uint32_t AMode)
    : MIP_Widget(ARect) {
      MName = "MIP_ModMatrixItemWidget";
      MAlignment = MIP_WIDGET_ALIGN_PARENT;

      //setPadding(1);
      float x  = 0;//MRect.x;
      float y  = 0;//MRect.y;
      float w1 = ((MRect.w-4) * 27) / 100;
      float w2 = ((MRect.w-4) * 15) / 100;
      float h  = ARect.h;

      wdg_src = new MIP_SelectorWidget( MIP_Rect(x,y,w1,h), ASrcMenu);
        wdg_src->setSelected(ASrc);
        wdg_src->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
      appendWidget(wdg_src);
      x += w1 + 1;

      wdg_dst = new MIP_SelectorWidget( MIP_Rect(x,y,w1,h), ADstMenu);
        wdg_dst->setSelected(ADst);
        wdg_dst->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
      appendWidget(wdg_dst);
      x += w1 + 1;

      wdg_amount = new MIP_DragValueWidget( MIP_Rect(x,y,w2,h), AAmt);
        wdg_amount->setValue(AAmt);
        //wdg_amount->drawName(false);
        //wdg_amount->offValue(0);

        wdg_amount->setBackgroundColor(MIP_Grey);
        wdg_amount->setTextColor(MIP_Black);
        wdg_amount->setFillBackground();
        //wdg_amount->setTextAlignment(MIP_TEXT_ALIGN_RIGHT);
        wdg_amount->setDrawText(false);
        wdg_amount->setValueAlignment(MIP_TEXT_ALIGN_RIGHT);
      appendWidget(wdg_amount);
      x += w2 + 1;

      wdg_scale = new MIP_DragValueWidget( MIP_Rect(x,y,w2,h), AScale);
        wdg_scale->setValue(AScale);
        wdg_scale->setSensitivity(0.0003f);   // def 0.01
        //wdg_scale->setSensitivity2(0.05f);  // def 0.05
        //wdg_scale->drawName(false);
        //wdg_scale->offValue(1);
        wdg_scale->setBackgroundColor(MIP_Grey);
        wdg_scale->setTextColor(MIP_Black);
        wdg_scale->setFillBackground();
        //wdg_scale->setTextAlignment(MIP_TEXT_ALIGN_RIGHT);
        wdg_scale->setDrawText(false);
        wdg_scale->setValueAlignment(MIP_TEXT_ALIGN_RIGHT);
      appendWidget(wdg_scale);
      x += w2 + 1;

      wdg_offset = new MIP_DragValueWidget( MIP_Rect(x,y,w2,h), AOffset);
        wdg_offset->setValue(AOffset);
        //wdg_offset->drawName(false);
        //wdg_offset->offValue(0);
        wdg_offset->setBackgroundColor(MIP_Grey);
        wdg_offset->setTextColor(MIP_Black);
        wdg_offset->setFillBackground();
        //wdg_offset->setTextAlignment(MIP_TEXT_ALIGN_RIGHT);
        wdg_offset->setDrawText(false);
        wdg_offset->setValueAlignment(MIP_TEXT_ALIGN_RIGHT);
      appendWidget(wdg_offset);
      //x += w2 + 1;

      //wdg_rate = new MIP_SelectorWidget( MIP_Rect(x,y,w1,h), ARateMenu);
      //  wdg_rate->setSelected(ASrc);
      //  wdg_rate->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
      //appendWidget(wdg_rate);
      //x += w1 + 1;

      //wdg_mode = new MIP_SelectorWidget( MIP_Rect(x,y,w1,h), AModeMenu);
      //  wdg_mode->setSelected(ADst);
      //  wdg_mode->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
      //appendWidget(wdg_mode);
      //x += w1 + 1;

    }

    //----------

    virtual ~MIP_ModMatrixRow() {
      //delete MAmountParam;
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual MIP_SelectorWidget*   getSrcWidget(void)    { return wdg_src; }
    virtual MIP_SelectorWidget*   getDstWidget(void)    { return wdg_dst; }
    virtual MIP_DragValueWidget*  getAmountWidget(void) { return wdg_amount; }
    virtual MIP_DragValueWidget*  getScaleWidget(void)  { return wdg_scale; }
    virtual MIP_DragValueWidget*  getOffsetWidget(void) { return wdg_offset; }

    //virtual MIP_SelectorWidget*   getRateWidget(void)    { return wdg_rate; }
    //virtual MIP_SelectorWidget*   getModeWidget(void)    { return wdg_mode; }

  //------------------------------
  //
  //------------------------------

  public:


};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class MIP_ModMatrixWidget
: public MIP_Widget {

  private:

    MIP_MenuWidget*  MSrcMenu  = MIP_NULL;
    MIP_MenuWidget*  MDstMenu  = MIP_NULL;
    uint32_t          MNumRows  = 0;

//------------------------------
//
//------------------------------

public:

  MIP_ModMatrixWidget(MIP_Rect ARect, MIP_MenuWidget* ASrcMenu, MIP_MenuWidget* ADstMenu)
  : MIP_Widget(ARect) {
    MSrcMenu = ASrcMenu;
    MDstMenu = ADstMenu;
  }

  //----------

  virtual ~MIP_ModMatrixWidget() {
  }

//------------------------------
//
//------------------------------

public:

  virtual MIP_Widget* getWidget(uint32_t x, uint32_t y) {
    MIP_Widget* w1 = getChild(y);
    if (w1) {
      return w1->getChild(x);
    }
    else return MIP_NULL;
  }

//------------------------------
//
//------------------------------

public:

  MIP_ModMatrixRow* appendRow(uint32_t src=0, uint32_t dst=0, float amt=0.0f, float scale=1.0f, float offset=0.0f, uint32_t rate=0, uint32_t mode=0 ) {
    float y = MNumRows * 17;
    MIP_ModMatrixRow* item = new MIP_ModMatrixRow(
      MIP_Rect(0,y, MRect.w, 16),
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

