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
  //double    baseScale = 1.0;
  MIP_DRect   border    = MIP_DRect(0,0,0,0);
  MIP_DPoint  spacing   = MIP_DPoint(0,0);
  MIP_DPoint  minSize   = MIP_DPoint(0,0);
  MIP_DPoint  maxSize   = MIP_DPoint(-1,-1);
};

//----------

struct MIP_WidgetFlags {
  bool active         = true;
  bool interactive    = false;
  bool visible        = true;
  bool vertical       = false;
  bool proportional   = false;
  bool captureMouse   = true;
  bool autoSetCursor  = true;
  bool autoHideCursor = false;
  bool autoLockCursor = false;
  bool dirty          = false;
};

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
private:
//------------------------------

  double    MValue      = 0.0;
  double    MMinValue   = 0.0;
  double    MMaxValue   = 1.0;
  double    MDefValue   = 0.0;

//------------------------------
protected:
//------------------------------

  const char*       MName         = "MIP_Widget";
  MIP_Widget*       MParent       = nullptr;
  MIP_WidgetArray   MChildren     = {};
  MIP_DRect         MRect         = {0};
  int32_t           MIndex        = 0;
  uint32_t          MMouseCursor  = MIP_CURSOR_DEFAULT;
  MIP_Parameter*    MParameters[MIP_MAX_PARAMETERS_PER_WIDGET] = {0};

//------------------------------
public:
//------------------------------

  MIP_WidgetLayout  Layout     = {};
  MIP_WidgetFlags   Flags      = {};

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

  virtual double getValue()     { return MValue; }
  virtual double getMinValue()  { return MMinValue; }
  virtual double getMaxValue()  { return MMaxValue; }
  virtual double getDefValue()  { return MDefValue; }

  virtual void   setValue(double v)     { MValue = v; }
  virtual void   setMinValue(double v)  { MMinValue = v; }
  virtual void   setMaxValue(double v)  { MMaxValue = v; }
  virtual void   setDefValue(double v)  { MDefValue = v; }

//------------------------------
public:
//------------------------------

  const char*             getWidgetName() { return MName; }

  virtual MIP_Widget*     getParentWidget() { return MParent; }
  virtual MIP_DRect       getWidgetRect()   { return MRect; }
  virtual double          getWidgetXPos()   { return MRect.x; }
  virtual double          getWidgetYPos()   { return MRect.y; }
  virtual double          getWidgetWidth()  { return MRect.w; }
  virtual double          getWidgetHeight() { return MRect.h; }
  virtual int32_t         getWidgetIndex()  { return MIndex; }

  // ???

  #ifndef MIP_NO_GUI
  virtual MIP_Parameter*  getParameter(uint32_t AIndex=0)    { return MParameters[0]; }
  #endif

  bool isInteractive()  { return Flags.interactive; }
  bool isDirty()        { return Flags.dirty; }

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
  virtual void on_widget_mouse_press(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {}
  virtual void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {}
  virtual void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {}

  virtual void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) {
    if (Flags.autoSetCursor) do_widget_cursor(this,MMouseCursor);
  }

  virtual void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) {
  }

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
      if (widget->Flags.active) {
        MIP_DRect rect = widget->MRect;
        if (rect.contains(AXpos,AYpos)) {
          MIP_Widget* child = widget->findChildWidget(AXpos,AYpos);
          return child;
        }
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
      if (widget->Flags.visible) {
        MIP_DRect widgetrect = widget->MRect;
        if (widgetrect.intersects(updaterect)) {
          // check if  AContext->updateRect
          widget->on_widget_paint(AContext);
        }
      }
    }
  }

  //----------

  void alignChildWidgets(bool ARecursive=true) {
    MIP_DRect parent_rect = MRect;
    MIP_DRect client_rect = MRect;
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* child = MChildren[i];

      MIP_DRect child_rect = child->Layout.baseRect;

      switch (child->Layout.alignment) {

        case MIP_WIDGET_ALIGN_NONE: {
          break;
        }

        case MIP_WIDGET_ALIGN_PARENT: {
          child_rect.x += parent_rect.x;
          child_rect.y += parent_rect.y;
          break;
        }

        case MIP_WIDGET_ALIGN_FILL_CLIENT: {
          child_rect.x = client_rect.x;
          child_rect.y = client_rect.y;
          child_rect.w = client_rect.w;
          child_rect.h = client_rect.h;
          break;
        }

        case MIP_WIDGET_ALIGN_FILL_TOP: {
          child_rect.x = client_rect.x;
          child_rect.y = client_rect.y;
          child_rect.w = client_rect.w;
          client_rect.y += child_rect.h;
          client_rect.h -= child_rect.h;
          break;
        }

        case MIP_WIDGET_ALIGN_FILL_BOTTOM: {
          child_rect.x = client_rect.x;
          child_rect.y = client_rect.y2() - child_rect.h;
          child_rect.w = client_rect.w;
          client_rect.h -= child_rect.h;
          break;
        }

        case MIP_WIDGET_ALIGN_FILL_LEFT: {
          child_rect.x = client_rect.x;
          child_rect.y = client_rect.y;
          child_rect.h = client_rect.h;
          client_rect.x += child_rect.w;
          client_rect.w -= child_rect.w;
          break;
        }

        case MIP_WIDGET_ALIGN_FILL_RIGHT: {
          child_rect.x = client_rect.x2() - child_rect.w;
          child_rect.y = client_rect.y;
          child_rect.h = client_rect.h;
          client_rect.w -= child_rect.w;
          break;
        }


      }
      child->MRect = child_rect;
      if (ARecursive) {
        child->alignChildWidgets(ARecursive);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
