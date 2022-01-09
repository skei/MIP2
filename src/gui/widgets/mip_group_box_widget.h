#ifndef mip_group_box_widget_included
#define mip_group_box_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widgets.h"


class MIP_GroupBoxWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  MIP_ButtonWidget* MTitleBar   = nullptr;
  MIP_PanelWidget*  MContainer  = nullptr;
  bool              MClosed     = false;
  bool              MClosable   = true;
  float             MOpenSize   = 0.0f;
  float             MClosedSize = 0.0f;

//------------------------------
public:
//------------------------------

  #define HEADER_HEIGHT 20

  MIP_GroupBoxWidget(MIP_FRect ARect, bool AClosed=false)
  : MIP_Widget(ARect) {
    setName("MIP_GroupBoxWidget");
    setHint("groupbox");
    //setCursor(MIP_CURSOR_);

    MTitleBar = new MIP_ButtonWidget( MIP_FRect(0,HEADER_HEIGHT) );
    MTitleBar->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    MTitleBar->setIsToggle();
    MTitleBar->setText("Closed","Open");
    MTitleBar->setBackgroundColor(MIP_Color(0.4),MIP_Color(0.4));
    MTitleBar->setTextColor(MIP_Color(0.8),MIP_Color(0.8));

    MContainer = new MIP_PanelWidget();
    MContainer->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    MContainer->setFillBackground(false);
    MContainer->setDrawBorder(false);
    //MContainer->setCursor(MIP_CURSOR_CROSS);

    MIP_Widget::appendWidget( MTitleBar );
    MIP_Widget::appendWidget( MContainer );

    MOpenSize   = ARect.h;
    MClosedSize = MTitleBar->getRect().h;

    MClosable = true;
    MClosed = AClosed;
    if (MClosed) {
    //  close();
      MTitleBar->setValue(0);
    }
    else {
    //  open();
      MTitleBar->setValue(1);
    }

  }

  virtual ~MIP_GroupBoxWidget() {
  }

//------------------------------
public:
//------------------------------

  MIP_ButtonWidget* getTitleBar(void) {
    return MTitleBar;
  }

  //----------

  MIP_PanelWidget* getContainer(void) {
    return MContainer;
  }

  //----------

  //void setTitleText(const char* AOnText, const char* AOffText) {
  //  //MTitleBar->setText(AOnText,AOffText);
  //}

  //void setTitleText(const char* AText) {
  //  MTitleBar->setText(AText);
  //}

  virtual void setClosable(bool AClosable=true) { MClosable = AClosable; }
  //virtual void setOpenSize(float ASize)         { MOpenSize = ASize; }
  //virtual void setClosedSize(float ASize)       { MClosedSize = ASize; }

  //----------

  void open(void) {
    //if (MClosed) {
      MClosed = false;
      MContainer->flags.active = true;
      MContainer->flags.visible = true;
//      setResized( MIP_Point(0,0) );
      setHeight(MOpenSize);
      setInitialHeight(MOpenSize);
      do_widget_resized(this/*,MRect.w,MOpenSize*/);
    //}
  }

  //----------

  void close(void) {
    //if (!MClosed) {
      MClosed = true;
      MContainer->flags.active = false;
      MContainer->flags.visible = false;
//      float rs = MClosedSize - getInitialRect().h;
//      setResized( MIP_Point(0,rs) );
      setHeight(MClosedSize);
      setInitialHeight(MClosedSize);
      do_widget_resized(this/*,MRect.w,MClosedSize*/);
    //}
  }

  //----------

  void toggle(void) {
    if (MClosed) open();
    else close();
  }

//------------------------------
public:
//------------------------------

  MIP_Widget* appendWidget(MIP_Widget* AWidget) override {
    return MContainer->appendWidget(AWidget);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    if (ASender == MTitleBar) {
      if (MClosable) {
        if (MTitleBar->getValue() >= 0.5f) {
          open();
          do_widget_realign(this);
        }
        else {
          close();
          do_widget_realign(this);
        }
      }
    }
    else {
      MIP_Widget::do_widget_update(ASender);
    }
  }


};

//----------------------------------------------------------------------
#endif

