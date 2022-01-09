#ifndef mip_scroll_box_widget_includedbutton
#define mip_scroll_box_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_scroll_bar_widget.h"

class MIP_ScrollBoxWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool showHorizontalScrollBar  = true;
  bool showVerticalScrollBar    = true;

  MIP_ScrollBarWidget* MVerticalScrollBar    = nullptr;
  MIP_ScrollBarWidget* MHorizontalScrollBar  = nullptr;
  MIP_PanelWidget*     MContent              = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ScrollBoxWidget(MIP_FRect ARect=MIP_FRect(), bool AVertical=true, bool AHorizontal=true)
  : MIP_PanelWidget(ARect) {
    showVerticalScrollBar = AVertical;
    showHorizontalScrollBar = AHorizontal;
    setup(ARect);
  }

  //----------

  //MIP_ScrollBoxWidget()
  //: MIP_PanelWidget() {
  //  setup(MIP_FRect());
  //}

  //----------

  virtual ~MIP_ScrollBoxWidget() {
  }

//------------------------------
public:
//------------------------------

  void setup(MIP_FRect ARect=MIP_FRect()) {
    setName("MIP_ScrollBoxWidget");
    setHint("scrollbox");
    setFillBackground(false);
    setDrawBorder(true);
    if (showVerticalScrollBar) {
      MVerticalScrollBar = new MIP_ScrollBarWidget( MIP_FRect(10,10) );
      MVerticalScrollBar->layout.alignment = MIP_WIDGET_ALIGN_FILL_RIGHT;
      MVerticalScrollBar->setDirection(MIP_VERTICAL);
      if (showHorizontalScrollBar) {
        MVerticalScrollBar->layout.extraBorder.h = 10;
      }
      MIP_PanelWidget::appendWidget(MVerticalScrollBar);
    }
    if (showHorizontalScrollBar) {
      MHorizontalScrollBar = new MIP_ScrollBarWidget( MIP_FRect(10,10) );
      MHorizontalScrollBar->layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      MHorizontalScrollBar->setDirection(MIP_HORIZONTAL);
      //MHorizontalScrollBar->layout.extraBorder.w = 10;
      MIP_PanelWidget::appendWidget(MHorizontalScrollBar);
    }
    MContent = new MIP_PanelWidget();
    MContent->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    //MContent->layout.spacing = 5;
    //MContent->layout.innerBorder = 0;
    MIP_PanelWidget::appendWidget(MContent);

  }

  //----------

  virtual MIP_Widget* getContentWidget() {
    return MContent;
  }

  //----------

  void updateScroll(MIP_Widget* ASender, float visible, float pos, float prev=0) {
    if (ASender == MVerticalScrollBar) {
      //float pos  = MVerticalScrollBar->getThumbPos();
      //float prev = MVerticalScrollBar->getPrevThumbPos();
      float delta = prev - pos;
      //float visible = MVerticalScrollBar->getThumbSize();
      float can_scroll = 1.0 - visible;
      can_scroll *= MContent->getContentRect().h;
      float scroll = can_scroll * delta; // * pos;
      MContent->scrollChildren(0,scroll);
      do_widget_redraw(MContent,MContent->getRect(),0);
    }
    else if (ASender == MHorizontalScrollBar) {
      //float pos  = MHorizontalScrollBar->getThumbPos();
      //float prev = MHorizontalScrollBar->getPrevThumbPos();
      float delta = prev - pos;
      //float visible = MHorizontalScrollBar->getThumbSize();
      float can_scroll = 1.0 - visible;
      can_scroll *= MContent->getContentRect().w;
      float scroll = can_scroll * delta; // * pos;
      MContent->scrollChildren(scroll,0);
      do_widget_redraw(MContent,MContent->getRect(),0);
    }
  }

//------------------------------
public:
//------------------------------

  MIP_Widget* appendWidget(MIP_Widget* AWidget) override {
    return MContent->appendWidget(AWidget);
  }

  //----------

  void alignWidgets(bool ARecursive=true) override {
    MIP_Widget::alignWidgets(ARecursive);
    MIP_FRect content = MContent->getContentRect();
    float rect_w = MContent->getRect().w;
    float rect_h = MContent->getRect().h;
    if (showVerticalScrollBar) {
      if (rect_h > 0) {
        float thumb_ratio = rect_h / content.h;         // szize of thumb (0..1)
        float thumb_size = MIP_Clamp(thumb_ratio,0,1);
        MVerticalScrollBar->setThumbSize(thumb_size,false);
        float visible = MVerticalScrollBar->getThumbSize();
        float pos     = MVerticalScrollBar->getThumbPos();
        float prev    = 0.0f; // MVerticalScrollBar->getPrevThumbPos();
        updateScroll(MVerticalScrollBar,visible,pos,prev);
      }
      else {
        MVerticalScrollBar->setThumbSize(1,false);
      }
    }

    if (showHorizontalScrollBar) {
      if (rect_w > 0) {
        float thumb_ratio = rect_w / content.w;         // szize of thumb (0..1)
        float thumb_size = MIP_Clamp(thumb_ratio,0,1);
        MHorizontalScrollBar->setThumbSize(thumb_size,false);
        float visible = MHorizontalScrollBar->getThumbSize();
        float pos     = MHorizontalScrollBar->getThumbPos();
        float prev    = 0.0f; // MHorizontalScrollBar->getPrevThumbPos();
        updateScroll(MHorizontalScrollBar,visible,pos,prev);
      }
      else {
        MHorizontalScrollBar->setThumbSize(1,false);
      }
    }

  }


//------------------------------
public:
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    if (ASender == MVerticalScrollBar) {
      float visible = MVerticalScrollBar->getThumbSize();
      float pos     = MVerticalScrollBar->getThumbPos();
      float prev    = MVerticalScrollBar->getPrevThumbPos();
      updateScroll(MVerticalScrollBar,visible,pos,prev);
    }
    else if (ASender == MHorizontalScrollBar) {
      float visible = MHorizontalScrollBar->getThumbSize();
      float pos     = MHorizontalScrollBar->getThumbPos();
      float prev    = MHorizontalScrollBar->getPrevThumbPos();
      updateScroll(MHorizontalScrollBar,visible,pos,prev);
    }
    else {
      MIP_PanelWidget::do_widget_update(ASender);
    }
  }

  //----------

  void do_widget_realign(MIP_Widget* ASender, bool ARecursive=true) override {
    //MIP_PRINT;
    alignWidgets(ARecursive);
    redraw();
  }

};

//----------------------------------------------------------------------
#endif
