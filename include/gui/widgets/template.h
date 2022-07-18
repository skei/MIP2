#ifndef mip_template_widget_included
#define mip_template_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"
#include "gui/mip_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_TemplateWidget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

//------------------------------
public:
//------------------------------

  MIP_TemplateWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
  }

  //----------

  virtual ~MIP_TemplateWidget() {
  }

//------------------------------
public: // parent to child
//------------------------------

//  void on_widget_move(MIP_DPoint APos) override {}
//  void on_widget_resize(MIP_DPoint ASize) override {}
//  void on_widget_align(bool ARecursive=true) override {}
//  void on_widget_paint(MIP_PaintContext* AContext) override { paintChildWidgets(AContext); }
//  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_mouse_press(uint32_t AButton, uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_enter(MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_DPoint APos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

};

//----------------------------------------------------------------------
#endif

