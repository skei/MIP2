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
  uint32_t    alignment   = MIP_WIDGET_ALIGN_PARENT;      // alignment relative to parent
  double      aspectRatio = -1;                           // if > 0, force aspect ratio (scale down)
  uint32_t    horizScale  = MIP_WIDGET_SCALE_PIXELS;      // x,w - PIXELS = normal, RATIO = % of client/parent, SPREAD = fit children
  uint32_t    vertScale   = MIP_WIDGET_SCALE_PIXELS;      // y,h - --"--
  MIP_DRect   initialRect = MIP_DRect(0,0,0,0);           // initial/creation rect (start realigning from this)
  MIP_DRect   baseRect    = MIP_DRect(0,0,0,0);           // initial/creation rect (start realigning from this)
  MIP_DRect   border      = MIP_DRect(0,0,0,0);           // inner border
  MIP_DRect   extraBorder = MIP_DRect(0,0,0,0);           // additional border
  MIP_DPoint  spacing     = MIP_DPoint(0,0);              // spacing between child widgets
  MIP_DPoint  minSize     = MIP_DPoint(0,0);              // minimum size
  MIP_DPoint  maxSize     = MIP_DPoint(999999,999999);    // maximum size
};

//----------

struct MIP_WidgetFlags {
  bool active           = true;
  bool interactive      = false;
  bool visible          = true;
  bool captureMouse     = true;
  bool doubleClick      = false;
  bool autoSetCursor    = true;
  bool autoHideCursor   = false;
  bool autoLockCursor   = false;
  bool dirty            = false;
  bool highlighted      = false;
  bool focused          = false;
  //bool clipChildren     = false;
  bool autoSize         = false;
  bool autoSizeContent  = false;
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
  //MIP_Widget*       MOwnerWindow  = nullptr;
  MIP_Widget*       MParent       = nullptr;
  MIP_WidgetArray   MChildren     = {};
  MIP_DRect         MRect         = {0};
  MIP_DRect         MContentRect  = {0};
  int32_t           MIndex        = 0;
  uint32_t          MMouseCursor  = MIP_CURSOR_DEFAULT;
  MIP_Parameter*    MParameter    = {0};
  //MIP_Skin*         MSkin         = nullptr;

//------------------------------
public:
//------------------------------

  // public..  :-/

  MIP_WidgetLayout  Layout     = {};
  MIP_WidgetFlags   Flags      = {};

//------------------------------
public:
//------------------------------

  MIP_Widget(MIP_DRect ARect) {

    Layout.initialRect = ARect;
    Layout.baseRect = ARect;
    MRect = ARect;
    //Layout.aspectRatio = ARect.w / ARect.h;
  }

  //----------

  virtual ~MIP_Widget() {
    #ifndef MIP_NO_AUTODELETE
    deleteChildWidgets();
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual double getValue()                 { return MValue; }
  virtual double getMinValue()              { return MMinValue; }
  virtual double getMaxValue()              { return MMaxValue; }
  virtual double getDefaultValue()          { return MDefValue; }

  virtual void   setValue(double v)         { MValue = v; }
  virtual void   setMinValue(double v)      { MMinValue = v; }
  virtual void   setMaxValue(double v)      { MMaxValue = v; }
  virtual void   setDefaultValue(double v)  { MDefValue = v; }

//------------------------------
public:
//------------------------------

  virtual const char*     getWidgetName()                   { return MName; }

  //virtual MIP_Widget*     getParentWidget() { return MParent; }
  virtual MIP_DRect       getWidgetRect()   { return MRect; }
  virtual double          getWidgetXPos()   { return MRect.x; }
  virtual double          getWidgetYPos()   { return MRect.y; }
  virtual double          getWidgetWidth()  { return MRect.w; }
  virtual double          getWidgetHeight() { return MRect.h; }
  virtual int32_t         getWidgetIndex()  { return MIndex; }

  virtual MIP_Parameter*  getParameter()    { return MParameter; }

  virtual bool              isInteractive()  { return Flags.interactive; }
  virtual bool              isDirty()        { return Flags.dirty; }

  virtual MIP_DRect         getRect()         { return MRect; }
  virtual MIP_DRect         getContentRect()  { return MContentRect; }

  virtual uint32_t          getNumChildWidgets()        { return MChildren.size(); }
  virtual MIP_Widget*       getChildWidget(uint32_t i)  { return MChildren[i]; }
  virtual MIP_WidgetArray*  getChildWidgets()           { return &MChildren; }

//------------------------------
public:
//------------------------------

  virtual void setWidgetName(const char* AName)             { MName = AName; }

  virtual void setParentWidget(MIP_Widget* AParent)         { MParent = AParent; }
  virtual void setWidgetRect(MIP_DRect ARect)               { MRect   = ARect; }
  virtual void setWidgetPos(double AXpos, double AYpos)     { MRect.setPos(AXpos,AYpos); }
  virtual void setWidgetXPos(double AXpos)                  { MRect.x = AXpos; }
  virtual void setWidgetYPos(double AYpos)                  { MRect.y = AYpos; }
  virtual void setWidgetSize(double AWidth, double AHeight) { MRect.setSize(AWidth,AHeight); }
  virtual void setWidgetWidth(double AWidth)                { MRect.w = AWidth; }
  virtual void setWidgetHeight(double AHeight)              { MRect.h = AHeight; }
  virtual void setWidgetindex(int32_t AIndex)               { MIndex = AIndex; }

  virtual void setParameter(MIP_Parameter* AParameter)  { MParameter = AParameter; }
  virtual void setWidgetCursor(uint32_t ACursor)        { MMouseCursor = ACursor; }

//------------------------------
public: // parent to child
//------------------------------

  virtual void on_widget_config(MIP_Widget* AOwnerWindow)  {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      MChildren[i]->on_widget_config(AOwnerWindow);
    }
  }

  virtual void on_widget_move(double AXpos, double AYpos)  {}
  virtual void on_widget_resize(double AWidth, double AHeight) {}
  virtual void on_widget_align(bool ARecursive=true) {}
  virtual void on_widget_paint(MIP_PaintContext* AContext) { paintChildWidgets(AContext); }

  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}

  virtual void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {}
  virtual void on_widget_mouse_dblclick(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {}
virtual void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {}
  virtual void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {}

  virtual void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) {
    if (Flags.autoSetCursor) do_widget_cursor(this,MMouseCursor);
  }

  virtual void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) {}
  //virtual void on_widget_connect(MIP_Parameter* AParameter) {}
  virtual void on_widget_unmodal() {}

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

//  virtual MIP_Widget* do_widget_get_owner_window(MIP_Widget* ASender) {
//    if (MParent) return MParent->do_widget_get_owner_window(ASender);
//    else return nullptr;
//  }

  /*
    called by MIP_WidgetSizer (etc?)
    resize ourselves, and tell parent to realign all its children
    also set initial-rect, since alignment is starting from that
  */

  virtual void do_widget_resized(MIP_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f, uint32_t AMode=0) {
    //if (MParent) MParent->do_widget_resized(ASender,ADeltaX,ADeltaY);
    MRect.w += ADeltaX;
    MRect.h += ADeltaY;
    //MInitialRect.w += ADeltaX;
    //MInitialRect.h += ADeltaY;
    Layout.baseRect.w += ADeltaX;
    Layout.baseRect.h += ADeltaY;
    //resize(ADeltaX,ADeltaY);
    if (MParent) {
      MParent->alignChildWidgets();
      MParent->redraw();
    }
  }

//------------------------------
public:
//------------------------------

  void redraw() {
    do_widget_redraw(this);
  }

//------------------------------
public: // hierarchy
//------------------------------

  virtual MIP_Widget* appendChildWidget(MIP_Widget* AChild) {
    int32_t index = MChildren.size();
    AChild->MIndex = index;
    AChild->MParent = this;
    //AChild->on_widget_append(this);
    MChildren.append(AChild);
    return AChild;
  }

  //----------

  virtual void deleteChildWidgets() {
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

  virtual void scaleChildWidgets(double AScale, bool ARecursive=true) {
    uint32_t num = 0;
    num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* child = MChildren[i];
      if (child) {
        child->MRect.scale(AScale);
        //child->Layout.baseRect.scale(AScale);
        if (ARecursive) child->scaleChildWidgets(AScale,ARecursive);
      }
    }
  }

  //----------

//  virtual MIP_Widget* getOwnerWindow() {
//    if (MParent) return MParent->getOwnerWindow();
//    else return nullptr;
//  }

  //----------

  virtual void scrollChildren(float AOffsetX, float AOffsetY) {
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* child = MChildren[i];
      if (child->Flags.visible) {
        //child->setChildrenOffset(AOffsetX,AOffsetY);
        child->MRect.x += AOffsetX;
        child->MRect.y += AOffsetY;
        child->scrollChildren(AOffsetX,AOffsetY);
      }
    }
  }

  //----------

  //virtual void setOwnerWindow(MIP_Widget* AWindow, bool ARecursive=true) {
  //  MOwnerWindow = AWindow;
  //  if (ARecursive) {
  //    uint32_t num = MChildren.size();
  //    for (uint32_t i=0; i<num; i++) {
  //      MIP_Widget* child = MChildren[i];
  //      child->setOwnerWindow(AWindow,ARecursive);
  //    }
  //  }
  //}

  //----------

  /*
  virtual void setSkin(MIP_Skin* ASkin, bool ARecursive=true) {
    MSkin = ASkin;
    if (ARecursive) {
      uint32_t num = MChildren.size();
      for (uint32_t i=0; i<num; i++) {
        MIP_Widget* child = MChildren[i];
        child->setSkin(ASkin,ARecursive);
      }
    }
  }
  */

  //----------

  // returns 'this' if not hovering over any child widget

  virtual MIP_Widget* findChildWidget(double AXpos, double AYpos) {
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

  virtual void paintChildWidgets(MIP_PaintContext* AContext) {
    //MIP_PRINT;
    uint32_t num = MChildren.size();
    if (num > 0) {
      MIP_DRect cliprect = MRect;
      cliprect.shrink(Layout.border);

      //if (Flags.clipChildren) {
      //  AContext->painter->pushOverlapClip(cliprect);
      //}

      for (uint32_t i=0; i<num; i++) {
        MIP_Widget* widget = MChildren[i];
        if (widget->Flags.visible) {
          MIP_DRect widgetrect = widget->MRect;
          widgetrect.overlap(cliprect);
          if (widgetrect.isNotEmpty()) {
            AContext->painter->pushOverlapClip(widgetrect);
            widget->on_widget_paint(AContext);
            AContext->painter->popClip();
          } // intersect
        } // visible
      } // for

      //if (Flags.clipChildren) {
      //  AContext->painter->popClip();
      //}

    } // num
  }

  //----------

  //virtual void prepareForAlignment() {
  //}

  //----------

  virtual void alignChildWidgets(bool ARecursive=true) {

    // todo: take sizemode into account.. (ratios)
//    double parent_xscale = MRect.w / Layout.baseRect.w;
//    double parent_yscale = MRect.h / Layout.baseRect.h;

    // stacking

    float stackx = 0;
    float stacky = 0;
    float stack_highest = 0;
    float stack_widest = 0;
    uint32_t prev_alignment = MIP_WIDGET_ALIGN_NONE;

    MIP_DRect border = Layout.border;
    MIP_DPoint spacing = Layout.spacing;

    //if (Layout.scaleChildren) {
    //  border.scale(n);
    //  spacing.scale(n);
    //}

    //

    MIP_DRect parent_rect = MRect;
    MIP_DRect client_rect = MRect;
    MContentRect = MRect;

    parent_rect.shrink(border);
    client_rect.shrink(border);
    MContentRect.shrink(border);

    //content.setPos(0,0);
    MContentRect.setSize(0,0);

    uint32_t num = MChildren.size();
    if (num > 0) {

      // for all children..
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MIP_Widget* child = MChildren[i];
        if (child->Flags.visible) {
// !!!
          //child->prepareForAlignment();
// !!!
          MIP_DRect child_rect = child->Layout.baseRect;



          uint32_t  alignment = child->Layout.alignment;

          // sizemode

          switch (child->Layout.horizScale) {
            case MIP_WIDGET_SCALE_PIXELS:
              break;
            case MIP_WIDGET_SCALE_PARENT_RATIO:
              child_rect.x *= parent_rect.w;
              child_rect.w *= parent_rect.w;
              break;
            case MIP_WIDGET_SCALE_CLIENT_RATIO:
              child_rect.x *= client_rect.w;
              child_rect.w *= client_rect.w;
              break;
          }
          switch (child->Layout.vertScale) {
            case MIP_WIDGET_SCALE_PIXELS:
              break;
            case MIP_WIDGET_SCALE_PARENT_RATIO:
              child_rect.y *= parent_rect.h;
              child_rect.h *= parent_rect.h;
              break;
            case MIP_WIDGET_SCALE_CLIENT_RATIO:
              child_rect.y *= client_rect.h;
              child_rect.h *= client_rect.h;
              break;
          }

          //  if we were stacking, but isn't now (end stacking)
          if (prev_alignment == MIP_WIDGET_ALIGN_STACK_HORIZ) {
            if (alignment != MIP_WIDGET_ALIGN_STACK_HORIZ) {
              float h = (stacky + stack_highest + spacing.y);
              client_rect.y += h;//(stacky + stack_highest + spacing.y);
              client_rect.h -= h;//
            }
          }
          if (prev_alignment == MIP_WIDGET_ALIGN_STACK_VERT) {
            if (alignment != MIP_WIDGET_ALIGN_STACK_VERT) {
              float w = (stackx + stack_widest + spacing.x);
              client_rect.x += w;//(stackx + stack_widest + spacing.x);
              client_rect.w -= w;

            }
          }

          // start new stacking run
          if (alignment == MIP_WIDGET_ALIGN_STACK_HORIZ) {
            if (prev_alignment != MIP_WIDGET_ALIGN_STACK_HORIZ) {
              stackx = 0;
              stacky = 0;
              stack_highest = 0;
              stack_widest = 0;
            }
          }
          if (alignment == MIP_WIDGET_ALIGN_STACK_VERT) {
            if (prev_alignment != MIP_WIDGET_ALIGN_STACK_VERT) {
              stackx = 0;
              stacky = 0;
              stack_highest = 0;
              stack_widest = 0;
            }
          }

          prev_alignment = alignment;

          // alignment
          switch (child->Layout.alignment) {

            case MIP_WIDGET_ALIGN_NONE: {
              break;
            }

            case MIP_WIDGET_ALIGN_PARENT: {
              child_rect.x += parent_rect.x;
              child_rect.y += parent_rect.y;
              break;
            }

            case MIP_WIDGET_ALIGN_CLIENT: {
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              break;
            }

            //-----

            case MIP_WIDGET_ALIGN_LEFT:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              //rect.h = client.h;
              break;

            case MIP_WIDGET_ALIGN_LEFT_TOP:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              break;

            case MIP_WIDGET_ALIGN_LEFT_CENTER:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y + (client_rect.h * 0.5f) - (child_rect.h * 0.5f);
              break;

            case MIP_WIDGET_ALIGN_LEFT_BOTTOM:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y2() - child_rect.h;
              break;

            //-----

            case MIP_WIDGET_ALIGN_RIGHT:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y;
              //rect.h = client_rect.h;
              break;

            case MIP_WIDGET_ALIGN_RIGHT_TOP:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y;
              break;

            case MIP_WIDGET_ALIGN_RIGHT_CENTER:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y + (client_rect.h * 0.5f) - (child_rect.h * 0.5f);
              break;

            case MIP_WIDGET_ALIGN_RIGHT_BOTTOM:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y2() - child_rect.h;
              break;

            //-----

            case MIP_WIDGET_ALIGN_TOP:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              //child_rect.w = client_rect.w;
              client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            case MIP_WIDGET_ALIGN_TOP_LEFT:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            case MIP_WIDGET_ALIGN_TOP_CENTER:
              child_rect.x += client_rect.x + (client_rect.w * 0.5f) - (child_rect.w * 0.5f);
              child_rect.y += client_rect.y;
              client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            case MIP_WIDGET_ALIGN_TOP_RIGHT:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y;
              client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            //-----

            case MIP_WIDGET_ALIGN_BOTTOM:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y2() - child_rect.h;
              //rect.w = client.w;

              break;

            case MIP_WIDGET_ALIGN_BOTTOM_LEFT:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y2() - child_rect.h;
              break;

            case MIP_WIDGET_ALIGN_BOTTOM_CENTER:
              child_rect.x += client_rect.x + (client_rect.w * 0.5f) - (child_rect.w * 0.5f);
              child_rect.y += client_rect.y2() - child_rect.h;
              break;

            case MIP_WIDGET_ALIGN_BOTTOM_RIGHT:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y2() - child_rect.h;
              break;

            //-----

            case MIP_WIDGET_ALIGN_FILL_PARENT: {
              child_rect.x += parent_rect.x;
              child_rect.y += parent_rect.y;
              child_rect.w = parent_rect.w;
              child_rect.h = parent_rect.h;
              // clear client rect
              break;
            }

            case MIP_WIDGET_ALIGN_FILL_CLIENT: {
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              child_rect.w = client_rect.w;
              child_rect.h = client_rect.h;
              // clear client rect
              break;
            }

            //-----

            case MIP_WIDGET_ALIGN_FILL_LEFT: {
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              child_rect.h = client_rect.h;
              client_rect.x += (child_rect.w + spacing.x);
              client_rect.w -= (child_rect.w + spacing.x);
              break;
            }

            case MIP_WIDGET_ALIGN_FILL_LEFT_TOP:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              client_rect.x += (child_rect.w + spacing.x);
              client_rect.w -= (child_rect.w + spacing.x);
              break;

            case MIP_WIDGET_ALIGN_FILL_LEFT_CENTER:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y + (client_rect.h * 0.5f) - (child_rect.h * 0.5f);
              client_rect.x += (child_rect.w + spacing.x);
              client_rect.w -= (child_rect.w + spacing.x);
              break;

            case MIP_WIDGET_ALIGN_FILL_LEFT_BOTTOM:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y2() - child_rect.h;
              client_rect.x += (child_rect.w + spacing.x);
              client_rect.w -= (child_rect.w + spacing.x);
              break;

            //-----

            case MIP_WIDGET_ALIGN_FILL_RIGHT: {
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y;
              child_rect.h = client_rect.h;
              client_rect.w -= (child_rect.w + spacing.x);
              break;
            }

            case MIP_WIDGET_ALIGN_FILL_RIGHT_TOP:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y;
              client_rect.w -= (child_rect.w + spacing.x);
              break;

            case MIP_WIDGET_ALIGN_FILL_RIGHT_CENTER:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y + (client_rect.h * 0.5f) - (child_rect.h * 0.5f);
              client_rect.w -= (child_rect.w + spacing.x);
              break;

            case MIP_WIDGET_ALIGN_FILL_RIGHT_BOTTOM:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y2() - child_rect.h;
              client_rect.w -= (child_rect.w + spacing.x);
              break;

            //-----

            case MIP_WIDGET_ALIGN_FILL_TOP: {
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              child_rect.w = client_rect.w;
              client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;
            }

            case MIP_WIDGET_ALIGN_FILL_TOP_LEFT:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y;
              client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            case MIP_WIDGET_ALIGN_FILL_TOP_CENTER:
              child_rect.x += client_rect.x + (client_rect.w * 0.5f) - (child_rect.w * 0.5f);
              child_rect.y += client_rect.y;
              client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            case MIP_WIDGET_ALIGN_FILL_TOP_RIGHT:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y;
              client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            //-----

            case MIP_WIDGET_ALIGN_FILL_BOTTOM: {
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y2() - child_rect.h;
              child_rect.w = client_rect.w;
              client_rect.h -= (child_rect.h + spacing.y);
              break;
            }

            case MIP_WIDGET_ALIGN_FILL_BOTTOM_LEFT:
              child_rect.x += client_rect.x;
              child_rect.y += client_rect.y2() - child_rect.h;
              //client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            case MIP_WIDGET_ALIGN_FILL_BOTTOM_CENTER:
              child_rect.x += client_rect.x + (client_rect.w * 0.5f) - (child_rect.w * 0.5f);
              child_rect.y += client_rect.y2() - child_rect.h;
              //client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            case MIP_WIDGET_ALIGN_FILL_BOTTOM_RIGHT:
              child_rect.x += client_rect.x2() - child_rect.w;
              child_rect.y += client_rect.y2() - child_rect.h;
              //client_rect.y += (child_rect.h + spacing.y);
              client_rect.h -= (child_rect.h + spacing.y);
              break;

            //-----

            case MIP_WIDGET_ALIGN_CENTER:
              child_rect.x += client_rect.x + (client_rect.w * 0.5f) - (child_rect.w * 0.5f);
              child_rect.y += client_rect.y + (client_rect.h * 0.5f) - (child_rect.h * 0.5f);
              break;

            case MIP_WIDGET_ALIGN_CENTER_HORIZ:
              child_rect.x += client_rect.x + (client_rect.w * 0.5f) - (child_rect.w * 0.5f);
              break;

            case MIP_WIDGET_ALIGN_CENTER_VERT:
              child_rect.y += client_rect.y + (client_rect.h * 0.5f) - (child_rect.h * 0.5f);
              break;

            //-----

            case MIP_WIDGET_ALIGN_STACK_HORIZ:
              if ((stackx + child_rect.w /*+ border.w - spacing.x*/) >= client_rect.w) {
                if (stackx != 0) {  // first widget..
                  stackx = 0;
                  stacky += stack_highest + spacing.y;
                  stack_highest = 0;
                }
              }
              child_rect.x = (client_rect.x + stackx);
              child_rect.y = (client_rect.y + stacky);
              stackx += child_rect.w + spacing.x;
              if (child_rect.h > stack_highest) stack_highest = child_rect.h;
              break;

            case MIP_WIDGET_ALIGN_STACK_VERT:
            //if ((stacky + child_rect.h) >= client_rect.h) {
              if ((stacky + child_rect.h /*+ border.h - spacing.y*/) >= client_rect.h) {
                if (stacky != 0) {  // first widget..
                  stacky = 0;
                  stackx += stack_widest + spacing.x;
                  stack_widest = 0;
                }
              }
              child_rect.x += client_rect.x + stackx;
              child_rect.y += client_rect.y + stacky;
              stacky  += child_rect.h + spacing.y;
              if (child_rect.w > stack_widest) stack_widest = child_rect.w;
              break;

          } // switch

          // min/max size
          child_rect.w = MIP_Clamp( child_rect.w, child->Layout.minSize.w, child->Layout.maxSize.w );
          child_rect.h = MIP_Clamp( child_rect.h, child->Layout.minSize.h, child->Layout.maxSize.h );
          // scale

          // aspect
          if (child->Layout.aspectRatio > 0) {
            if (child_rect.h > 0) {
              double aspect = child_rect.w / child_rect.h;
              if (aspect >= child->Layout.aspectRatio) {
                double w_new = child_rect.h * child->Layout.aspectRatio;
                child_rect.w = w_new;
              }
              else {
                double h_new = child_rect.w / child->Layout.aspectRatio;
                child_rect.h = h_new;
              }
            } // h > 0
          } // aspect > 0

          MIP_DRect extra_border = child->Layout.extraBorder;
          child_rect.shrink(extra_border);
          child->MRect = child_rect;
          MContentRect.combine(child_rect);
          if (ARecursive) {
            child->alignChildWidgets(ARecursive);
          }
        } //  visible
      } // for
    } // num > 0
    if (Flags.autoSizeContent) {
      MRect.w = MContentRect.w;
      MRect.h = MContentRect.h;
    }
    MContentRect.w += (border.x + border.w);
    MContentRect.h += (border.y + border.h);

  }

};

//----------------------------------------------------------------------
#endif
