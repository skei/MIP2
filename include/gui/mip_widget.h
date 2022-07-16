#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_point.h"
#include "base/types/mip_rect.h"
#include "gui/mip_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Widget;
typedef MIP_Array<MIP_Widget*> MIP_WidgetArray;

//----------

struct MIP_WidgetLayout {
  uint32_t    alignment   = MIP_WIDGET_ALIGN_PARENT;
  MIP_DRect   baseRect    = MIP_DRect(0,0,0,0);
  MIP_DRect   innerBorder = MIP_DRect(0,0,0,0);
  MIP_DPoint  spacing     = MIP_DPoint(0,0);
  MIP_DPoint  minSize     = MIP_DPoint(0,0);
  MIP_DPoint  maxSize     = MIP_DPoint(-1,-1);
};

//----------

struct MIP_WidgetOptions {
};

//----------

struct MIP_WidgetState {
  bool active       = true;
  bool visible      = true;
  bool interactive  = false;
  bool dirty        = false;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
protected:
//------------------------------

  MIP_Widget*       MParent     = nullptr;
  MIP_WidgetArray   MChildren   = {};
  MIP_DRect         MRect       = {0};
  uint32_t          MIndex      = 0;

//------------------------------
public:
//------------------------------

  MIP_WidgetLayout  Layout     = {};
  MIP_WidgetOptions Options    = {};
  MIP_WidgetState   State      = {};


//------------------------------
public:
//------------------------------

  MIP_Widget(MIP_DRect ARect) {
    Layout.baseRect = ARect;
    MRect = ARect;
  }

  //----------

  virtual ~MIP_Widget() {
  }

//------------------------------
public:
//------------------------------

  MIP_Widget* getParentWidget() { return MParent; }
  MIP_DRect   getWidgetRect()   { return MRect; }
  double      getWidgetXPos()   { return MRect.x; }
  double      getWidgetYPos()   { return MRect.y; }
  double      getWidgetWidth()  { return MRect.w; }
  double      getWidgetHeight() { return MRect.h; }

//------------------------------
public:
//------------------------------

  void setParentWidget(MIP_Widget* AParent) { MParent = AParent; }
  void setWidgetRect(MIP_DRect ARect)       { MRect   = ARect; }
  void setWidgetXPos(double AXpos)          { MRect.x = AXpos; }
  void setWidgetYPos(double AYpos)          { MRect.y = AYpos; }
  void setWidgetWidth(double AWidth)        { MRect.w = AWidth; }
  void setWidgetHeight(double AHeight)      { MRect.h = AHeight; }

  void setWidgetPos(double AXpos, double AYpos) {
    MRect.x = AXpos;
    MRect.y = AYpos;
  }

  void setWidgetWidth(double AWidth, double AHeight) {
    MRect.w = AWidth;
    MRect.h = AHeight;
  }

//------------------------------
public: // parent to child
//------------------------------

  virtual void on_widget_move(MIP_DPoint APos) {
    MRect.setPos(APos);
  }

  virtual void on_widget_resize(MIP_DPoint ASize) {
    MRect.setSize(ASize);
  }

  virtual void on_widget_align(bool ARecursive=true) {
    alignChildWidgets(ARecursive);
  }

  virtual void on_widget_paint(MIP_PaintContext* AContext) {
    paintChildWidgets(AContext);
  }

  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}

  virtual void on_widget_mouse_press(uint32_t AButton, uint32_t AState, MIP_DPoint APos, uint32_t ATime) {}
  virtual void on_widget_mouse_release(uint32_t AButton, uint32_t AState, MIP_DPoint APos, uint32_t ATime) {}
  virtual void on_widget_mouse_move(uint32_t AState, MIP_DPoint APos, uint32_t ATime) {}

  virtual void on_widget_enter(MIP_DPoint APos, uint32_t ATime) {}
  virtual void on_widget_leave(MIP_DPoint APos, uint32_t ATime) {}

  //virtual void on_widget_connect(MIP_Parameter* AParameter) {
  //}


//------------------------------
public: // child to parent
//------------------------------

  virtual void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_update(ASender,AMode);
  }

  virtual void do_widget_redraw(MIP_Widget* ASender, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_redraw(ASender,AMode);
  }

  virtual void do_widget_modal(MIP_Widget* ASender, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_modal(ASender,AMode);
  }

  virtual void do_widget_cursor(MIP_Widget* ASender, uint32_t ACursor) {
    if (MParent) MParent->do_widget_cursor(ASender,ACursor);
  }

  virtual void do_widget_hint(MIP_Widget* ASender, const char* AHint) {
    if (MParent) MParent->do_widget_hint(ASender,AHint);
  }

  virtual void do_widget_notify(MIP_Widget* ASender, uint32_t AMode, int32_t AValue) {
    if (MParent) MParent->do_widget_notify(ASender,AMode,AValue);
  }

//------------------------------
public:
//------------------------------

  MIP_Widget* appendChildWidget(MIP_Widget* AChild) {
    uint32_t index = MChildren.size();
    AChild->MIndex = index;
    //AChild->on_widget_append(this);
    MChildren.append(AChild);
    return AChild;
  }

  //----------

  void deleteChildWidgets() {
    uint32_t num = 0;
    num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* child = MChildren[i];
      //child->on_widget_delete();
      if (child) delete child;
    }
    MChildren.clear();
  }

  //----------

  MIP_Widget* findChildWidget(MIP_DPoint APos) {
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* widget = MChildren[i];
      if (widget) {
        if (MRect.contains(APos.x,APos.y)) {
          MIP_Widget* child = widget->findChildWidget(APos);
          return child;
        }
      }
    }
    return nullptr;
  }

  //----------

  void paintChildWidgets(MIP_PaintContext* AContext) {
    //MIP_PRINT;
    MIP_DRect updaterect = AContext->updateRect;
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* widget = MChildren[i];
      MIP_DRect widgetrect = widget->getWidgetRect();
      if (widgetrect.intersects(updaterect)) {
        // check if  AContext->updateRect
        widget->on_widget_paint(AContext);
      }
    }
  }

  //----------

  void alignChildWidgets(bool ARecursive=true) {
    MIP_DRect parentrect = MRect;
    MIP_DRect clientrect = MRect;
    //MIP_Print("rect: %.0f,%.0f - %.0f,%.0f\n",rect.x,rect.y,rect.w,rect.h);

    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {

      MIP_Widget* widget = MChildren[i];
      MIP_DRect widgetrect = widget->Layout.baseRect;

      switch (widget->Layout.alignment) {
        case MIP_WIDGET_ALIGN_NONE: {
          break;
        }
        case MIP_WIDGET_ALIGN_PARENT: {
          widgetrect.x += parentrect.x;
          widgetrect.y += parentrect.y;
          break;
        }
        case MIP_WIDGET_ALIGN_CLIENT: {
          widgetrect.x += clientrect.x;
          widgetrect.y += clientrect.y;
          break;
        }
      }

      //MIP_Print("widgetrect: %.0f,%.0f - %.0f,%.0f\n",widgetrect.x,widgetrect.y,widgetrect.w,widgetrect.h);
      widget->setWidgetRect(widgetrect);

      if (ARecursive) {
        widget->alignChildWidgets(ARecursive);
      }


    }
  }

};

//----------------------------------------------------------------------
#endif
