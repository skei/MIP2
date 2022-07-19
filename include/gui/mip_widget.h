#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_paint_context.h"

#ifndef MIP_NO_GUI
#include "plugin/mip_parameter.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Widget;
typedef MIP_Array<MIP_Widget*> MIP_WidgetArray;

#define MIP_MAX_PARAMETERS_PER_WIDGET 16

//----------

struct MIP_WidgetLayout {
  uint32_t    alignment = MIP_WIDGET_ALIGN_PARENT;
  MIP_DRect   baseRect  = MIP_DRect(0,0,0,0);
  MIP_DRect   border    = MIP_DRect(0,0,0,0);
  MIP_DPoint  spacing   = MIP_DPoint(0,0);
  MIP_DPoint  minSize   = MIP_DPoint(0,0);
  MIP_DPoint  maxSize   = MIP_DPoint(-1,-1);
};

//----------

struct MIP_WidgetOptions {
  bool active       = true;
  bool visible      = true;
  bool vertical     = false;
  bool proportional = false;
  bool captureMouse = true;
};

//----------

struct MIP_WidgetState {
  bool interactive  = false;
  bool dirty        = false;
};

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
protected:
//------------------------------

  const char*       MName       = "MIP_Widget";
  MIP_Widget*       MParent     = nullptr;
  MIP_WidgetArray   MChildren   = {};
  MIP_DRect         MRect       = {0};
  int32_t           MIndex      = 0;
  MIP_WidgetState   MState      = {};

  MIP_Parameter*    MParameters[MIP_MAX_PARAMETERS_PER_WIDGET] = {0};

//------------------------------
public:
//------------------------------

  MIP_WidgetLayout  Layout     = {};
  MIP_WidgetOptions Options    = {};

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

  const char* getWidgetName() { return MName; }

  virtual MIP_Widget* getParentWidget() { return MParent; }
  virtual MIP_DRect   getWidgetRect()   { return MRect; }
  virtual double      getWidgetXPos()   { return MRect.x; }
  virtual double      getWidgetYPos()   { return MRect.y; }
  virtual double      getWidgetWidth()  { return MRect.w; }
  virtual double      getWidgetHeight() { return MRect.h; }
  virtual int32_t     getWidgetIndex()  { return MIndex; }

  #ifndef MIP_NO_GUI
  virtual MIP_Parameter* getParameter(uint32_t AIndex=0)    { return MParameters[0]; }
  #endif

  bool isInteractive()  { return MState.interactive; }
  bool isDirty()        { return MState.dirty; }


//------------------------------
public:
//------------------------------

  virtual void setParentWidget(MIP_Widget* AParent)         { MParent = AParent; }
  virtual void setWidgetRect(MIP_DRect ARect)               { MRect   = ARect; }
  virtual void setWidgetPos(double AXpos, double AYpos)     { MRect.setPos(AXpos,AYpos); }
  virtual void setWidgetXPos(double AXpos)                  { MRect.x = AXpos; }
  virtual void setWidgetYPos(double AYpos)                  { MRect.y = AYpos; }
  virtual void setWidgetSize(double AWidth, double AHeight) { MRect.setSize(AWidth,AHeight); }
  virtual void setWidgetWidth(double AWidth)                { MRect.w = AWidth; }
  virtual void setWidgetHeight(double AHeight)              { MRect.h = AHeight; }
  virtual void setWidgetindex(int32_t AIndex)               { MIndex = AIndex; }

  #ifndef MIP_NO_GUI
  virtual void setParameter(MIP_Parameter* AParameter, uint32_t AIndex=0) { MParameters[AIndex] = AParameter; }
  #endif

  //virtual void setWidgetPos(double AXpos, double AYpos) {
  //  MRect.x = AXpos;
  //  MRect.y = AYpos;
  //}

  //virtual void setWidgetWidth(double AWidth, double AHeight) {
  //  MRect.w = AWidth;
  //  MRect.h = AHeight;
  //}

//------------------------------
public: // parent to child
//------------------------------

  virtual void on_widget_move(double AXpos, double AYpos)  {}
  virtual void on_widget_resize(double AWidth, double AHeight) {}
  virtual void on_widget_align(bool ARecursive=true) {}
  virtual void on_widget_paint(MIP_PaintContext* AContext) { paintChildWidgets(AContext); }
  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_mouse_press(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {} //{ MIP_Print("%s\n",getWidgetName()); }
  virtual void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {} //{ MIP_Print("%s\n",getWidgetName()); }
  virtual void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {} //{ MIP_Print("%s\n",getWidgetName()); }
  virtual void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) {} //{ MIP_Print("%s from %s\n",getWidgetName(),AFrom?AFrom->getWidgetName():"(null)"); }
  virtual void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) {} //{ MIP_Print("%s to %s\n",getWidgetName(),ATo?ATo->getWidgetName():"(null)"); }

  //virtual void on_widget_connect(MIP_Parameter* AParameter) {}

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
public: // hierarchy
//------------------------------

  MIP_Widget* appendChildWidget(MIP_Widget* AChild) {
    int32_t index = MChildren.size();
    AChild->MIndex = index;
    AChild->MParent = this;
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

  void scaleChildWidgets(double AScale, bool ARecursive=true) {
    uint32_t num = 0;
    num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* child = MChildren[i];
      if (child) {
        child->MRect.scale(AScale);
        if (ARecursive) child->scaleChildWidgets(AScale,ARecursive);
      }
    }
  }

  //----------

  // returns 'this' if not hovering over any child widget

  MIP_Widget* findChildWidget(double AXpos, double AYpos) {
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* widget = MChildren[i];
      MIP_DRect rect = widget->MRect;
      if (rect.contains(AXpos,AYpos)) {
        MIP_Widget* child = widget->findChildWidget(AXpos,AYpos);
        return child;
      }
    }
    return this;//nullptr;
  }

  //----------

  void paintChildWidgets(MIP_PaintContext* AContext) {
    //MIP_PRINT;
    MIP_DRect updaterect = AContext->updateRect;
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* widget = MChildren[i];
      MIP_DRect widgetrect = widget->MRect;
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
          widgetrect.x = clientrect.x;
          widgetrect.y = clientrect.y;
          widgetrect.w = clientrect.w;
          widgetrect.h = clientrect.h;
          break;
        }
      }
      //MIP_Print("widgetrect: %.0f,%.0f - %.0f,%.0f\n",widgetrect.x,widgetrect.y,widgetrect.w,widgetrect.h);
      widget->MRect = widgetrect;
      if (ARecursive) {
        widget->alignChildWidgets(ARecursive);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
