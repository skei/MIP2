#ifndef kode_scroll_box_widget_includedbutton
#define kode_scroll_box_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"
#include "gui/widgets/kode_panel_widget.h"
#include "gui/widgets/kode_scroll_bar_widget.h"

class KODE_ScrollBoxWidget
: public KODE_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool showHorizontalScrollBar  = true;
  bool showVerticalScrollBar    = true;

  KODE_ScrollBarWidget* MVerticalScrollBar    = KODE_NULL;
  KODE_ScrollBarWidget* MHorizontalScrollBar  = KODE_NULL;
  KODE_PanelWidget*     MContent              = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_ScrollBoxWidget(KODE_FRect ARect=KODE_FRect(), bool AVertical=true, bool AHorizontal=true)
  : KODE_PanelWidget(ARect) {
    showVerticalScrollBar = AVertical;
    showHorizontalScrollBar = AHorizontal;
    setup(ARect);
  }

  //----------

  //KODE_ScrollBoxWidget()
  //: KODE_PanelWidget() {
  //  setup(KODE_FRect());
  //}

  //----------

  virtual ~KODE_ScrollBoxWidget() {
  }

//------------------------------
public:
//------------------------------

  void setup(KODE_FRect ARect=KODE_FRect()) {
    setName("KODE_ScrollBoxWidget");
    setHint("scrollbox");
    setFillBackground(false);
    setDrawBorder(true);
    if (showVerticalScrollBar) {
      MVerticalScrollBar = new KODE_ScrollBarWidget( KODE_FRect(10,10) );
      MVerticalScrollBar->layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
      MVerticalScrollBar->setDirection(KODE_VERTICAL);
      if (showHorizontalScrollBar) {
        MVerticalScrollBar->layout.extraBorder.h = 10;
      }
      KODE_PanelWidget::appendWidget(MVerticalScrollBar);
    }
    if (showHorizontalScrollBar) {
      MHorizontalScrollBar = new KODE_ScrollBarWidget( KODE_FRect(10,10) );
      MHorizontalScrollBar->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
      MHorizontalScrollBar->setDirection(KODE_HORIZONTAL);
      //MHorizontalScrollBar->layout.extraBorder.w = 10;
      KODE_PanelWidget::appendWidget(MHorizontalScrollBar);
    }
    MContent = new KODE_PanelWidget();
    MContent->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    //MContent->layout.spacing = 5;
    //MContent->layout.innerBorder = 0;
    KODE_PanelWidget::appendWidget(MContent);

  }

  //----------

  virtual KODE_Widget* getContentWidget() {
    return MContent;
  }

  //----------

  void updateScroll(KODE_Widget* ASender, float visible, float pos, float prev=0) {
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

  KODE_Widget* appendWidget(KODE_Widget* AWidget) override {
    return MContent->appendWidget(AWidget);
  }

  //----------

  void alignChildren(bool ARecursive=true) override {
    KODE_Widget::alignChildren();
    KODE_FRect content = MContent->getContentRect();
    float rect_w = MContent->getRect().w;
    float rect_h = MContent->getRect().h;
    if (showVerticalScrollBar) {
      if (rect_h > 0) {
        float thumb_ratio = rect_h / content.h;         // szize of thumb (0..1)
        float thumb_size = KODE_Clamp(thumb_ratio,0,1);
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
        float thumb_size = KODE_Clamp(thumb_ratio,0,1);
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

  void do_widget_update(KODE_Widget* ASender) override {
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
      KODE_PanelWidget::do_widget_update(ASender);
    }
  }

  //----------

  void do_widget_realign(KODE_Widget* ASender, bool ARecursive=true) override {
    //KODE_PRINT;
    alignChildren(ARecursive);
    redraw();
  }

};

//----------------------------------------------------------------------
#endif
