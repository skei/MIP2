#ifndef mip_scroll_box_widget_included
#define mip_scroll_box_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/mip_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

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

  MIP_ScrollBoxWidget(MIP_DRect ARect, bool AVertical=true, bool AHorizontal=true)
  : MIP_PanelWidget(ARect) {
    showVerticalScrollBar = AVertical;
    showHorizontalScrollBar = AHorizontal;
    setup(ARect);
  }

  //----------

  virtual ~MIP_ScrollBoxWidget() {
  }

//------------------------------
public:
//------------------------------

  void setup(MIP_DRect ARect) {
    MName = "MIP_ScrollBoxWidget";
    setFillBackground(false);;
    setDrawBorder(false);
    //Layout.spacing = 5;
    if (showVerticalScrollBar) {
      MVerticalScrollBar = new MIP_ScrollBarWidget( MIP_DRect(10,10) );
      MVerticalScrollBar->setWidgetName("scrollbox/vert.scr");
      MVerticalScrollBar->Layout.alignment = MIP_WIDGET_ALIGN_FILL_RIGHT;
      MVerticalScrollBar->setDirection(MIP_VERTICAL);
      if (showHorizontalScrollBar) {
        MVerticalScrollBar->Layout.extraBorder.h = 10;
      }
      MIP_PanelWidget::appendChildWidget(MVerticalScrollBar);
    }
    if (showHorizontalScrollBar) {
      MHorizontalScrollBar = new MIP_ScrollBarWidget( MIP_DRect(10,10) );
      MHorizontalScrollBar->setWidgetName("scrollbox/hor.scr");
      MHorizontalScrollBar->Layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      MHorizontalScrollBar->setDirection(MIP_HORIZONTAL);
      //MHorizontalScrollBar->layout.extraBorder.w = 10;
      MIP_PanelWidget::appendChildWidget(MHorizontalScrollBar);
    }
    MContent = new MIP_PanelWidget(MIP_DRect());
    MContent->setWidgetName("scrollbox/content");
    MContent->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    //MContent->layout.spacing = 5;
    //MContent->layout.innerBorder = 0;
    MContent->setDrawBorder(false);
    MContent->setFillBackground(true);
    //MContent->setFillBackground(true);
    //MContent->setBackgroundColor(MIP_COLOR_DARK_GREEN);
    MIP_PanelWidget::appendChildWidget(MContent);
  }

  //----------

  virtual MIP_PanelWidget* getContentWidget() {
    return MContent;
  }

  //----------

  //virtual void setHorizontalScrollBar(bool AState=true) { showHorizontalScrollBar = AState; }
  //virtual void setVerticalScrollBar(bool AState=true) { showVerticalScrollBar = AState; }

  //----------

  void updateScroll(MIP_Widget* ASender, float visible, float pos, float prev=0, bool draw=false) {
    if (ASender == MVerticalScrollBar) {
      float delta = prev - pos;
      float can_scroll = 1.0 - visible;
      can_scroll *= MContent->getContentRect().h;
      float scroll = can_scroll * delta;
      MContent->scrollChildren(0,scroll);
      if (draw) MContent->redraw();
    }
    else if (ASender == MHorizontalScrollBar) {
      float delta = prev - pos;
      float can_scroll = 1.0 - visible;
      can_scroll *= MContent->getContentRect().w;
      float scroll = can_scroll * delta;
      MContent->scrollChildren(scroll,0);
      if (draw) MContent->redraw();
    }
  }

//------------------------------
public:
//------------------------------

  MIP_Widget* appendChildWidget(MIP_Widget* AWidget) override {
    return MContent->appendChildWidget(AWidget);
  }

  //----------

  void alignChildWidgets(bool ARecursive=true) override {
    MIP_PanelWidget::alignChildWidgets(ARecursive);
    MIP_DRect content = MContent->getContentRect();
    float rect_w = MContent->getRect().w;

    float rect_h = MContent->getRect().h;

//MIP_Print("\n");
//MIP_DPrint("MContent->getContentRect: %.0f,%.0f, %.0f,%.0f\n",content.x,content.y,content.w,content.h);
//MIP_DPrint("MContent->getRect: %.0f,%.0f\n",rect_w,rect_h);

    if (showVerticalScrollBar) {
      if (rect_h > 0) {
        float thumb_ratio = rect_h / content.h;         // size of thumb (0..1)
        float thumb_size = MIP_Clamp(thumb_ratio,0,1);
        MVerticalScrollBar->setThumbSize(thumb_size,false);
        float visible = MVerticalScrollBar->getThumbSize();
        float pos     = MVerticalScrollBar->getThumbPos();
        float prev    = 0.0f; // MVerticalScrollBar->getPrevThumbPos();
        updateScroll(MVerticalScrollBar,visible,pos,prev,false);
      }
      else {
        MVerticalScrollBar->setThumbSize(1,false);
      }
    }

    if (showHorizontalScrollBar) {
      if (rect_w > 0) {
        float thumb_ratio = rect_w / content.w;         // size of thumb (0..1)
        float thumb_size = MIP_Clamp(thumb_ratio,0,1);
        MHorizontalScrollBar->setThumbSize(thumb_size,false);
        float visible = MHorizontalScrollBar->getThumbSize();
        float pos     = MHorizontalScrollBar->getThumbPos();
        float prev    = 0.0f; // MHorizontalScrollBar->getPrevThumbPos();
        updateScroll(MHorizontalScrollBar,visible,pos,prev,false);
      }
      else {
        MHorizontalScrollBar->setThumbSize(1,false);
      }
    }

  }

//------------------------------
public: // parent to child
//------------------------------

//  void on_widget_move(double AXpos, double AYpos)  override {}
//  void on_widget_resize(double AWidth, double AHeight) override {}
//  void on_widget_align(bool ARecursive=true) override {}
//  void on_widget_paint(MIP_PaintContext* AContext) override { paintChildWidgets(AContext); }
//  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

//  virtual void on_widget_modal(bool AModal) {}


//------------------------------
public: // child to parent
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    if (ASender == MVerticalScrollBar) {
      float visible = MVerticalScrollBar->getThumbSize();
      float pos     = MVerticalScrollBar->getThumbPos();
      float prev    = MVerticalScrollBar->getPrevThumbPos();
      updateScroll(MVerticalScrollBar,visible,pos,prev,true);
    }
    else if (ASender == MHorizontalScrollBar) {
      float visible = MHorizontalScrollBar->getThumbSize();
      float pos     = MHorizontalScrollBar->getThumbPos();
      float prev    = MHorizontalScrollBar->getPrevThumbPos();
      updateScroll(MHorizontalScrollBar,visible,pos,prev,true);
    }
    else {
      MIP_PanelWidget::do_widget_update(ASender);
    }
  }

//  void do_widget_redraw(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_modal(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_cursor(MIP_Widget* ASender, uint32_t ACursor) override {}
//  void do_widget_hint(MIP_Widget* ASender, const char* AHint) override {}
//  void do_widget_notify(MIP_Widget* ASender, uint32_t AMode, int32_t AValue) override {}

};

//----------------------------------------------------------------------
#endif
