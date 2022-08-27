#ifndef mip_group_box_widget_included
#define mip_group_box_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"
#include "gui/widgets/mip_button_widget.h"
#include "gui/widgets/mip_panel_widget.h"


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

  MIP_GroupBoxWidget(MIP_DRect ARect, bool AClosed=false)
  : MIP_Widget(ARect) {
    MName = "MIP_GroupBoxWidget";
    //MHint("groupbox");
    //setCursor(MIP_CURSOR_);

    MTitleBar = new MIP_ButtonWidget( MIP_DRect(0,HEADER_HEIGHT),"Open","Closed",1.0 );
    MTitleBar->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    MTitleBar->setIsToggle();
    MTitleBar->setTexts("Open","Closed");

//    MTitleBar->setDrawBorder(true);
//    //MTitleBar->setRoundedCorners(MIP_CORNER_NONE);
//    MTitleBar->setFillGradient(false);
//    //MTitleBar->setBackgroundColors(MIP_Color(0.4),MIP_Color(0.4));
//    //MTitleBar->setTextColors(MIP_Color(0.8),MIP_Color(0.8));

    MTitleBar->setFillBackground(true);
    MTitleBar->setFillGradient(true);
    MTitleBar->setDrawBorder(true);
    MTitleBar->setDrawRoundedCorners(true);
    MTitleBar->setCornerRadius(5,5,0,0);

    MContainer = new MIP_PanelWidget( MIP_DRect());
    MContainer->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

    MContainer->setFillBackground(false);
    MContainer->setDrawBorder(false);
    //MContainer->setCursor(MIP_CURSOR_CROSS);

//    MContainer->setFillBackground(true);
//    MContainer->setFillGradient(true);
//    MContainer->setDrawBorder(true);
//    MContainer->setDrawRoundedCorners(true);
//    MContainer->setCornerRadius(5,5,0,0);

    MIP_Widget::appendChildWidget( MTitleBar );
    MIP_Widget::appendChildWidget( MContainer );

    MOpenSize = ARect.h;
    MClosedSize = MTitleBar->getRect().h;
    MClosed = AClosed;

    if (MClosed) {
      //close();
      MTitleBar->setOff();
      MContainer->State.active = false;
      MContainer->State.visible = false;
      //float rs = MClosedSize - getInitialRect().h;
      //setResized( MIP_Point(0,rs) );
      //setHeight(MClosedSize);
      //setInitialHeight(MClosedSize);
      MRect.h = MClosedSize;
      Layout.baseRect = MClosedSize;
    }
    else {
      //open();
      MTitleBar->setOn();
      MContainer->State.active = true;
      MContainer->State.visible = true;
      //setResized( MIP_Point(0,0) );
      //setHeight(MOpenSize);
      //setInitialHeight(MOpenSize);
      MRect.h = MOpenSize;
      Layout.baseRect = MOpenSize;
    }

  }

  //----------

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
    MClosed = false;
    MContainer->State.active = true;
    MContainer->State.visible = true;
    //setResized( MIP_Point(0,0) );
//    setHeight(MOpenSize);
//    setInitialHeight(MOpenSize);
    MRect.h = MOpenSize;
    Layout.baseRect = MOpenSize;
    if (MParent) MParent->do_widget_resized(this/*,MRect.w,MOpenSize*/);
  }

  //----------

  void close(void) {
    MClosed = true;
    MContainer->State.active = false;
    MContainer->State.visible = false;
    //float rs = MClosedSize - getInitialRect().h;
    //setResized( MIP_Point(0,rs) );
//    setHeight(MClosedSize);
//    setInitialHeight(MClosedSize);
    MRect.h = MClosedSize;
    Layout.baseRect = MClosedSize;
    if (MParent) MParent->do_widget_resized(this/*,MRect.w,MClosedSize*/);
  }

  //----------

  void toggle(void) {
    if (MClosed) open();
    else close();
  }

//------------------------------
public:
//------------------------------

  MIP_Widget* appendChildWidget(MIP_Widget* AWidget) override {
    return MContainer->appendChildWidget(AWidget);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    if (ASender == MTitleBar) {
      if (MClosable) {
        if (MTitleBar->getValue() >= 0.5f) {
          open();
        }
        else {
          close();
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

