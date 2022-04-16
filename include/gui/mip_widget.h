#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

/*
  when resizing widgets, they notify their parents (to realign & redraw),
  but if we use advanced stacking and fill alignments for the main window,
  everything have to be redrawn when resizing one of the panels..
  so it's better to split up panels as much as possible, so that only the
  necessary widgets need to be redrawn..
*/

//#define MIP_WIDGET_MAX_PARAMS 8

#include "mip.h"
#include "base/types/mip_array.h"
#include "base/types/mip_point.h"
#include "base/types/mip_rect.h"
#include "plugin/mip_parameter.h"
#include "gui/mip_painter.h"
#include "gui/mip_skin.h"

//----------------------------------------------------------------------

class MIP_Widget;
typedef MIP_Array<MIP_Widget*> MIP_Widgets;

//----------------------------------------------------------------------

struct MIP_WidgetFlags {
  bool opaque           = false;    // fully covers its parent (no transparent areas)
  bool sizePercent      = false;    // widget size is percent (of client area), not pixels
  bool posPercent       = false;    // widget pos is percent (of client area), not pixels
  bool autoCursor       = true;     // set mouse cursor automatically when hovering over a widget (entering)
  bool autoHint         = false;    // send hint automatically when hovering over a widget (entering)
  bool autoClip         = true;     // clip child widgets
  bool autoSize         = false;    // set size to content (child widgets) size (not used yet)
  bool autoMouseLock    = false;    // lock/glue mouse position when interacting (hiding)
  bool autoMouseHide    = false;    // hide mouse cursor when interacting
  bool redrawHover      = false;
  bool redrawInteract   = false;    // redraw widget at start/end of interaction
  bool canDrag          = false;    // not used yet
  bool canDrop          = false;    // not used yet
  //bool autoMouseCursor  = false;
  //bool autoMouseHide    = false;
  //bool autoMouseLock    = false;
  //bool autoSendHint     = false;
  //bool canMove          = false;
  //bool canResize        = false;
};

//----------

struct MIP_WidgetState {
  bool active           = true;     // send events to widget
  bool visible          = true;     // draw widget
  bool interacting      = false;    // is currently interacting with widget
  bool hovering         = false;    // mouse cursor is hovering above widget
  bool focused          = false;
};

//----------

struct MIP_WidgetLayout {
  uint32_t    alignment     = MIP_WIDGET_ALIGN_PARENT;    // how to align widget related to parent widget
  MIP_FRect   innerBorder   = MIP_FRect(0,0);             // space between widgets and parent edges
  MIP_FPoint  spacing       = MIP_FPoint(0,0);            // space inbetween widgets
  MIP_FRect   extraBorder   = MIP_FRect(0,0);             // extra space/border for each widget
  bool        contentBorder = true;                       // true if content rect includes innerBorder of parent
  MIP_FRect   outerBorder   = MIP_FRect(0,0);             //
  MIP_FPoint  minSize       = MIP_FPoint(0,0);            // minimum widget size (when resizing)
  MIP_FPoint  maxSize       = MIP_FPoint(999999,999999);  // maximum widget size (when resizing)
  float       scale         = 1.0;                        // widget drawing scale..
};

//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
protected:
//------------------------------

  const char*     MName                   = "MIP_Widget";         //
  const char*     MHint                   = "widget";             //
  int32_t         MCursor                 = MIP_CURSOR_DEFAULT;   //
  MIP_Widget*     MOwnerWindow            = nullptr;              // = MIP_Window;
  MIP_Widget*     MParent                 = nullptr;              //
  MIP_Widgets     MChildren               = {};                   //
  int32_t         MWidgetIndex            = -1;                   //
  MIP_FRect       MRect                   = {};                   //
  MIP_FRect       MInitialRect            = MIP_FRect(0,0);       // starting rect (used by layout/alignment
  MIP_FRect       MContentRect            = MIP_FRect(0,0);       // rect containing/surrounding child widgets
  float           MChildrenXOffset        = 0.0f;                 // offset (relative to parent rect)
  float           MChildrenYOffset        = 0.0f;                 // -"-
  float           MModValue               = 0.0;                  //
  float           MValue                  = 0.0;                  //
  float           MMinValue               = 0.0;                  //
  float           MMaxValue               = 1.0;                  //
  float           MDefaultValue           = 0.0;                  //

  int32_t         MParamIndex             = -1;                   //
  char            MParamName[256]         = {0};                  //
  MIP_Parameter*  MParameter              = nullptr;              //
  //MIP_Parameter*  MParameters[MIP_WIDGET_MAX_PARAMS] = {0};       //

  MIP_Skin*       MSkin                   = &MIP_DEFAULT_SKIN;

//------------------------------
public:
//------------------------------

  // public to make it easier to access,
  // and avoid needing tons and tons of setters/getters
  // hacky/dangerous.. but, well..

  MIP_WidgetFlags   flags   = {};
  MIP_WidgetLayout  layout  = {};
  MIP_WidgetState   state   = {};

//------------------------------
public:
//------------------------------

  MIP_Widget(MIP_FRect ARect) {
    init(ARect);
  }

  //----------

  virtual ~MIP_Widget() {
    #ifndef MIP_NO_AUTODELETE
    deleteWidgets();
    #endif
  }

//------------------------------
public: // set
//------------------------------

  void init(MIP_FRect ARect) {
    setRect(ARect);
    setInitialRect(ARect);
    setContentRect(MIP_FRect(0,0));
  }

//------------------------------
public: // set
//------------------------------

  virtual void setScale(float AScale, bool ARecursive=true) {
    layout.scale = AScale;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->setScale(AScale,ARecursive);
      }
    }
  }

  virtual void setChildrenOffset(float AX, float AY)        { MChildrenXOffset = AX; MChildrenYOffset = AY; }
  virtual void setChildrenXOffset(float AX)                 { MChildrenXOffset = AX; }
  virtual void setChildrenYOffset(float AY)                 { MChildrenYOffset = AY; }
  virtual void setContentRect(MIP_FRect ARect)              { MContentRect = ARect; }
  virtual void setCursor(int32_t ACursor)                   { MCursor = ACursor; }
  virtual void setMinValue(float AValue)                    { MMinValue = AValue; }
  virtual void setMaxValue(float AValue)                    { MMaxValue = AValue; }
  virtual void setDefaultValue(float AValue)                { MDefaultValue = AValue; }
  virtual void setHeight(float AHeight)                     { MRect.h = AHeight; }
  virtual void setHint(const char* AHint)                   { MHint = AHint; }
  virtual void setWidgetIndex(int32_t i)                   { MWidgetIndex = i; }

  virtual void setInitialHeight(float AH)                   { MInitialRect.h = AH; }
  virtual void setInitialPos(float AX, float AY)            { MInitialRect.x = AX; MInitialRect.y = AY; }
  virtual void setInitialRect(MIP_FRect ARect)              { MInitialRect = ARect; }
  virtual void setInitialSize(float AW, float AH)           { MInitialRect.w = AW; MInitialRect.h = AH; }
  virtual void setInitialWidth(float AW)                    { MInitialRect.w = AW; }
  virtual void setModValue(float v)                         { MModValue = v; }
  virtual void setName(const char* AName)                   { MName = AName; }
  virtual void setParent(MIP_Widget* AParent)               { MParent = AParent; }
  virtual void setPos(float AXpos, float AYpos)             { MRect.x = AXpos; MRect.y = AYpos; }
  virtual void setRect(MIP_FRect ARect)                     { MRect = ARect; }
  virtual void setRect(float x, float y, float w, float h)  { MRect.x=x; MRect.y=y; MRect.w=w;MRect.h=h; }
  virtual void setSize(float AWidth, float AHeight)         { MRect.w = AWidth; MRect.h = AHeight; }
  virtual void setValue(float AValue)                       { MValue = AValue; }
  virtual void setWidth(float AWidth)                       { MRect.w = AWidth; }

  virtual void setParameter(MIP_Parameter* AParameter)      { MParameter = AParameter; }
  virtual void setParamIndex(int32_t AIndex)                { MParamIndex = AIndex; }
  virtual void setParamName(const char* AName)              { strcpy(MParamName,AName); }

  //virtual void setParameter(MIP_Parameter* AParameter, uint32_t AIndex=0) { MParameters[AIndex] = AParameter; }
  //virtual void setOwner(MIP_Window* AOwner)             { MOwner = AOwner; }
  //virtual void setSelectedParameter(uint32_t AIndex)        { MSelectedParameter = AIndex; }

  //virtual void setParameter(MIP_Parameter* p) {
  //  MParameter = p;
  //  MParamIndex = MParameter->getIndex();
  //}

  //virtual void setSubParamIndex(uint32_t AIndex)            { MSubParamIndex = AIndex; }


//------------------------------
public:
//------------------------------

  virtual MIP_Widget*     getChild(uint32_t AIndex)   { return MChildren[AIndex]; }
  virtual MIP_Widgets*    getChildren()               { return &MChildren; }
  virtual MIP_FRect       getContentRect()            { return MContentRect; }
  virtual int32_t         getCursor()                 { return MCursor; }
  virtual float           getMinValue()               { return MMinValue; }
  virtual float           getMaxValue()               { return MMaxValue; }
  virtual float           getDefaultValue()           { return MDefaultValue; }
  virtual const char*     getHint()                   { return MHint; }
  virtual int32_t         getWidgetIndex()            { return MWidgetIndex; }
  virtual MIP_FRect       getInitialRect()            { return MInitialRect; }
  virtual float           getModValue()               { return MModValue; }
  virtual const char*     getName()                   { return MName; }
  virtual uint32_t        getNumChildren()            { return MChildren.size(); }
  virtual MIP_Widget*     getParent()                 { return MParent; }
  virtual MIP_FRect       getRect()                   { return MRect; }
  virtual float           getValue()                  { return MValue; }

  virtual int32_t         getParamIndex()             { return MParamIndex; }
  MIP_Parameter*          getParameter()              { return MParameter; }

  //virtual int32_t         getSubParamIndex()          { return MSubParamIndex; }

//------------------------------
public:
//------------------------------

  virtual MIP_Widget* appendWidget(MIP_Widget* AWidget) {
    if (AWidget) {
      AWidget->MParent = this;
      uint32_t index = MChildren.size();
      AWidget->MWidgetIndex = index;
      MChildren.append(AWidget);
      return AWidget;
    }
    return nullptr;
  }

  //----------

  virtual void deleteWidgets() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      delete MChildren[i];
      MChildren[i] = nullptr;
    }
    MChildren.clear();
  }

  //----------

  // returns this if no child widget at x/y

  MIP_Widget* findWidget(float AXpos, float AYpos, bool ARecursive=true) {
    if (MChildren.size() > 0) {
      for (int32_t i = MChildren.size()-1; i >= 0; i--) {
        MIP_Widget* child = MChildren[i];
        if (child->state.active) {
          MIP_FRect rect = child->getRect();
          if (rect.contains(AXpos,AYpos)) {
            if (ARecursive) {
              return child->findWidget(AXpos,AYpos,ARecursive);
            }
            else {
              return child;
            }
          } // child.rect.contains(x,y)
        }
      } // for all children
    } // children > 0
    return this;
  }

  //----------

  /*
    todo:
    - cull/clip children using ARect (clipping rect)
  */

  void paintWidgets(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode=0) {
    MIP_FRect mrect = getRect();
    mrect.overlap(ARect);
    MIP_FRect clip_rect = getRect();
    clip_rect.shrink(layout.innerBorder);
    clip_rect.overlap(ARect);
    if (clip_rect.isEmpty()) return;
    if (MChildren.size() > 0) {
      if (flags.autoClip) APainter->pushClip(clip_rect);
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MIP_Widget* child = MChildren[i];
        if (child->state.visible) {
          MIP_FRect child_rect = child->getRect();
          if (child_rect.isNotEmpty()) {
            //if (child_rect.touches(mrect)) {
              MIP_FRect overlap_rect = clip_rect;//mrect;
              overlap_rect.overlap(child_rect);
              if (overlap_rect.isNotEmpty()) {
                //if (child->flags.autoClip) APainter->pushClip(child->getRect());
                child->on_widget_paint(APainter,overlap_rect,AMode);  // clip rect
                //if (child->flags.autoClip) APainter->popClip();
              } // !overlap.empty
            //} // child.touches
          } // !child.empty
        } // child.visible
      } // for all children
      if (flags.autoClip) APainter->popClip();
    }
  }

  //----------

  /*
    the widgets are aligned from their MInitialRect, set up when the
    widget is created.. so, if we resize the widget, it will be lost next
    time the widgets are being 'realigned
    maybe:
      a) have a MInitialDelta rect? difference from MInitialRect? in case
         of runtime moving, resizing,.
      b) modify the MInitialRect directly when needed (resize widget, etc)
      c) pre/post for scrollboxes, to find size of child widgets, etc..
      d) take scroll/offset position into account while realigning
      e) like prev lib, un_scroll, realign, re_scroll

    todo:
    - relative positions
    - percentages
  */

  // content includes innerBorder?

  // TODO:
  // after aligning, go through widgets, and disable those that
  // fall outside the parent rectangle
  // (they don't need to be visible or active)

  virtual void alignWidgets(bool ARecursive=true) {
    MIP_FRect client   = getRect();
    MIP_FRect parent   = client;
    MIP_FRect content  = client;
    client.shrink(layout.innerBorder);
    parent.shrink(layout.innerBorder);
    if (!layout.contentBorder) content.shrink(layout.innerBorder);
    //content.setPos(0,0);
    content.setSize(0,0);
    float stackx = 0;
    float stacky = 0;
    float stack_highest = 0;
    float stack_widest = 0;
    //bool prev_was_stacking = false;
    uint32_t prev_alignment = MIP_WIDGET_ALIGN_NONE;

    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* child = MChildren[i];
      if (child->state.visible) {

        MIP_FRect rect      = child->getInitialRect();
        uint32_t  alignment = child->layout.alignment;

        if (child->flags.sizePercent) {
          rect.w = client.w * (rect.w * 0.01f);
          rect.h = client.h * (rect.h * 0.01f);
        }
        if (child->flags.posPercent) {
          rect.x = client.w * (rect.x * 0.01f);
          rect.y = client.w * (rect.y * 0.01f);
        }

        //----------

        //  if we were stacking, but isn't now..

        //bool is_stack   = ((alignment == MIP_WIDGET_ALIGN_STACK_HORIZ) || (alignment == MIP_WIDGET_ALIGN_STACK_VERT));
        //bool was_stack  = ((prev_alignment == MIP_WIDGET_ALIGN_STACK_HORIZ) || (prev_alignment == MIP_WIDGET_ALIGN_STACK_VERT));
        //if (was_stack && !is_stack) {
        //}

        if (prev_alignment == MIP_WIDGET_ALIGN_STACK_HORIZ) {
          if (alignment != MIP_WIDGET_ALIGN_STACK_HORIZ) {
            float h = (stacky + stack_highest + layout.spacing.y);
            client.y += h;//(stacky + stack_highest + layout.spacing.y);
            client.h -= h;//
          }
        }
        if (prev_alignment == MIP_WIDGET_ALIGN_STACK_VERT) {
          if (alignment != MIP_WIDGET_ALIGN_STACK_VERT) {
            float w = (stackx + stack_widest + layout.spacing.x);
            client.x += w;//(stackx + stack_widest + layout.spacing.x);
            client.w -= w;

          }
        }

        //

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

        //

        prev_alignment = alignment;

        //MIP_FRect new_client_rect = client;

        //----------

        switch (alignment) {

          //case MIP_WIDGET_ALIGN_NONE:
          //  break;

          case MIP_WIDGET_ALIGN_PARENT:
            rect.x += parent.x;
            rect.y += parent.y;
            break;

          case MIP_WIDGET_ALIGN_CLIENT: // = MIP_ALIGN_LEFT_TOP
            rect.x += client.x;
            rect.y += client.y;
            break;

          //-----

          case MIP_WIDGET_ALIGN_LEFT:
            rect.x += client.x;
            rect.y += client.y;
            //rect.h = client.h;
            break;

          case MIP_WIDGET_ALIGN_LEFT_TOP:
            rect.x += client.x;
            rect.y += client.y;
            break;

          case MIP_WIDGET_ALIGN_LEFT_CENTER:
            rect.x += client.x;
            rect.y += client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            break;

          case MIP_WIDGET_ALIGN_LEFT_BOTTOM:
            rect.x += client.x;
            rect.y += client.y2() - rect.h;
            break;

          //-----

          case MIP_WIDGET_ALIGN_RIGHT:
            rect.x += client.x2() - rect.w;
            rect.y += client.y;
            //rect.h = client.h;
            break;

          case MIP_WIDGET_ALIGN_RIGHT_TOP:
            rect.x += client.x2() - rect.w;
            rect.y += client.y;
            break;

          case MIP_WIDGET_ALIGN_RIGHT_CENTER:
            rect.x += client.x2() - rect.w;
            rect.y += client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            break;

          case MIP_WIDGET_ALIGN_RIGHT_BOTTOM:
            rect.x += client.x2() - rect.w;
            rect.y += client.y2() - rect.h;
            break;

          //-----

          case MIP_WIDGET_ALIGN_TOP:
            rect.x += client.x;
            rect.y += client.y;
            //rect.w = client.w;
            break;

          case MIP_WIDGET_ALIGN_TOP_LEFT:
            rect.x += client.x;
            rect.y += client.y;
            break;

          case MIP_WIDGET_ALIGN_TOP_CENTER:
            rect.x += client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            rect.y += client.y;
            break;

          case MIP_WIDGET_ALIGN_TOP_RIGHT:
            rect.x += client.x2() - rect.w;
            rect.y += client.y;
            break;

          //-----

          case MIP_WIDGET_ALIGN_BOTTOM:
            rect.x += client.x;
            rect.y += client.y2() - rect.h;
            //rect.w = client.w;
            break;

          case MIP_WIDGET_ALIGN_BOTTOM_LEFT:
            rect.x += client.x;
            rect.y += client.y2() - rect.h;
            break;

          case MIP_WIDGET_ALIGN_BOTTOM_CENTER:
            rect.x += client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            rect.y += client.y2() - rect.h;
            break;

          case MIP_WIDGET_ALIGN_BOTTOM_RIGHT:
            rect.x += client.x2() - rect.w;
            rect.y += client.y2() - rect.h;
            break;

          //-----

          case MIP_WIDGET_ALIGN_FILL_CLIENT:
            rect.x += client.x;
            rect.y += client.y;
            rect.w = client.w;
            rect.h = client.h;
            break;

          //-----

          case MIP_WIDGET_ALIGN_FILL_LEFT:
            rect.x += client.x; // was =
            rect.y += client.y; // was =
            rect.h = client.h;
            client.x += (rect.w + layout.spacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;

          case MIP_WIDGET_ALIGN_FILL_LEFT_TOP:
            rect.x += client.x;
            rect.y += client.y;
            client.x += (rect.w + layout.spacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;

          case MIP_WIDGET_ALIGN_FILL_LEFT_CENTER:
            rect.x += client.x;
            rect.y += client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            client.x += (rect.w + layout.spacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;

          case MIP_WIDGET_ALIGN_FILL_LEFT_BOTTOM:
            rect.x += client.x;
            rect.y += client.y2() - rect.h;
            client.x += (rect.w + layout.spacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;

          //-----

          case MIP_WIDGET_ALIGN_FILL_RIGHT:
            rect.x += client.x2() - rect.w; // was =
            rect.y += client.y; // was =
            rect.h = client.h;
            client.w -= (rect.w + layout.spacing.x);
            break;

          case MIP_WIDGET_ALIGN_FILL_RIGHT_TOP:
            rect.x += client.x2() - rect.w;
            rect.y += client.y;
            client.w -= (rect.w + layout.spacing.x);
            break;

          case MIP_WIDGET_ALIGN_FILL_RIGHT_CENTER:
            rect.x += client.x2() - rect.w;
            rect.y += client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            client.w -= (rect.w + layout.spacing.x);
            break;

          case MIP_WIDGET_ALIGN_FILL_RIGHT_BOTTOM:
            rect.x += client.x2() - rect.w;
            rect.y += client.y2() - rect.h;
            client.w -= (rect.w + layout.spacing.x);
            break;

          //-----

          case MIP_WIDGET_ALIGN_FILL_TOP:
            rect.x += client.x; // was =
            rect.y += client.y; // was =
            rect.w = client.w;
            client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;

          case MIP_WIDGET_ALIGN_FILL_TOP_LEFT:
            rect.x += client.x;
            rect.y += client.y;
            client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;

          case MIP_WIDGET_ALIGN_FILL_TOP_CENTER:
            rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            rect.y += client.y;
            client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;

          case MIP_WIDGET_ALIGN_FILL_TOP_RIGHT:
            rect.x += client.x2() - rect.w;
            rect.y += client.y;
            client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;

          //-----

          case MIP_WIDGET_ALIGN_FILL_BOTTOM:
            rect.x += client.x;             // was =
            rect.y += client.y2() - rect.h; // was =
            rect.w = client.w;
            //client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;

          case MIP_WIDGET_ALIGN_FILL_BOTTOM_LEFT:
            rect.x += client.x;
            rect.y += client.y2() - rect.h;
            //client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;

          case MIP_WIDGET_ALIGN_FILL_BOTTOM_CENTER:
            rect.x += client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            rect.y += client.y2() - rect.h;
            //client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;

          case MIP_WIDGET_ALIGN_FILL_BOTTOM_RIGHT:
            rect.x += client.x2() - rect.w;
            rect.y += client.y2() - rect.h;
            //client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;

          //-----

          case MIP_WIDGET_ALIGN_CENTER:
            rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            rect.y = client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            break;

          case MIP_WIDGET_ALIGN_CENTER_HORIZ:
            rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            break;

          case MIP_WIDGET_ALIGN_CENTER_VERT:
            rect.y = client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            break;

          //-----

          case MIP_WIDGET_ALIGN_STACK_HORIZ:
            if ((stackx + rect.w /*+ layout.innerBorder.w - layout.spacing.x*/) >= client.w) {
              if (stackx != 0) {  // first widget..
                stackx = 0;
                stacky += stack_highest + layout.spacing.y;
                stack_highest = 0;
              }
            }
            rect.x = (client.x + stackx);
            rect.y = (client.y + stacky);
            stackx += rect.w + layout.spacing.x;
            if (rect.h > stack_highest) stack_highest = rect.h;
            break;

          case MIP_WIDGET_ALIGN_STACK_VERT:
          //if ((stacky + rect.h) >= client.h) {
            if ((stacky + rect.h /*+ layout.innerBorder.h - layout.spacing.y*/) >= client.h) {
              if (stacky != 0) {  // first widget..
                stacky = 0;
                stackx += stack_widest + layout.spacing.x;
                stack_widest = 0;
              }
            }
            rect.x += client.x + stackx;
            rect.y += client.y + stacky;
            stacky  += rect.h + layout.spacing.y;
            if (rect.w > stack_widest) stack_widest = rect.w;
            break;

        } // switch alignment

        rect.w = MIP_Clamp(rect.w, child->layout.minSize.w, child->layout.maxSize.w);
        rect.h = MIP_Clamp(rect.h, child->layout.minSize.h, child->layout.maxSize.h);
        rect.shrink(child->layout.extraBorder);

        content.combine(rect); // scale?

        child->MRect.x = rect.x + MChildrenXOffset;
        child->MRect.y = rect.y + MChildrenYOffset;
        child->MRect.w = rect.w;
        child->MRect.h = rect.h;

        child->MRect.scale(child->layout.scale);

        if (ARecursive) child->alignWidgets(ARecursive);

      } // child visible
    } // for all children

    if (layout.contentBorder) {
      content.w += layout.innerBorder.w; // scale?
      content.h += layout.innerBorder.h; // scale?
    }
    MContentRect = content;
    //if (flags.autoSize) {
    //  MRect.w = content.w;
    //  MRect.h = content.h;
    //}
  }

  //----------

  virtual void scrollChildren(float AOffsetX, float AOffsetY) {
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* child = MChildren[i];
      if (child->state.visible) {
        //child->setChildrenOffset(AOffsetX,AOffsetY);
        child->MRect.x += AOffsetX;
        child->MRect.y += AOffsetY;
        child->scrollChildren(AOffsetX,AOffsetY);
      }
    }
  }

  //----------

  //virtual void resize(float ADeltaX=0.0f, float ADeltaY=0.0f) {
  //  MRect.w += ADeltaX;
  //  MRect.h += ADeltaY;
  //  MInitialRect.w += ADeltaX;
  //  MInitialRect.h += ADeltaY;
  //}

  //----------

  //virtual void attachWindow(MIP_Window* AWindow) {
  //  //MOwner = AWindow;
  //  uint32_t num = MChildren.size();
  //  for (uint32_t i=0; i<num; i++) {
  //    MIP_Widget* child = MChildren[i];
  //    child->attachWindow(AWindow);
  //  }
  //}

  //----------

  virtual void setOwnerWindow(MIP_Widget* AWindow, bool ARecursive=true) {
    MOwnerWindow = AWindow;
    if (ARecursive) {
      uint32_t num = MChildren.size();
      for (uint32_t i=0; i<num; i++) {
        MIP_Widget* child = MChildren[i];
        child->setOwnerWindow(AWindow,ARecursive);
      }
    }
  }

  //----------

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

  //----------


  virtual void update() {
    do_widget_update(this);
  }

  //----------

  virtual void redraw() {
    do_widget_redraw(this,getRect(),0);
  }

//------------------------------
public:
//------------------------------

  virtual void on_widget_move(float AXpos, float AYpos) {
    //MIP_Print("%s : x %.2f y %.2f\n",MName,AXpos,AYpos);
  }

  virtual void on_widget_resize(float AWidth, float AHeight) {
    //MIP_Print("%s : w %.2f h %.2f\n",MName,AWidth,AHeight);
  }

  virtual void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode=0) {
    //MIP_Print("%s : x %.2f y %.2f w %.2f h %.2f\n",MName,ARect.x,ARect.y,ARect.w,ARect.h);
    paintWidgets(APainter,ARect,AMode);
  }

  virtual void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {
    //MIP_Print("%s : x %.2f y %.2f b %i s %i\n",MName,AXpos,AYpos,AButton,AState);
  }

  virtual void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp) {
    //MIP_Print("%s : x %.2f y %.2f s %i\n",MName,AXpos,AYpos,AState);
  }

  virtual void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {
    //MIP_Print("%s : x %.2f y %.2f b %i s %i\n",MName,AXpos,AYpos,AButton,AState);
  }

  virtual void on_widget_mouseEnter(float AXpos, float AYpos, MIP_Widget* AFrom) {
    //MIP_Print("%s : x %.2f y %.2f from %s\n",MName,AXpos,AYpos,(AFrom)?AFrom->getName():"?");
    state.hovering = true;
    if (flags.redrawHover) do_widget_redraw(this,MRect,0/*MIP_PAINT_REDRAW*/);
    if (flags.autoCursor) do_widget_setMouseCursor(this,MCursor);
    if (flags.autoHint) do_widget_setHint(this,MHint,0);
  }

  virtual void on_widget_mouseLeave(float AXpos, float AYpos, MIP_Widget* ATo) {
    //MIP_Print("%s : x %.2f y %.2f to %s\n",MName,AXpos,AYpos,(ATo)?ATo->getName():"?");
    state.hovering = false;
    if (flags.redrawHover) do_widget_redraw(this,MRect,0/*MIP_PAINT_REDRAW*/);
    if (flags.autoCursor) do_widget_setMouseCursor(this,MIP_CURSOR_DEFAULT);
    if (flags.autoHint) do_widget_setHint(this,"",0);
  }

  virtual void on_widget_mouseDragEnter(float AXpos, float AYpos, MIP_Widget* AFrom) {
    //MIP_Print("%s : x %.2f y %.2f from %s\n",MName,AXpos,AYpos,(AFrom)?AFrom->getName():"?");
  }

  virtual void on_widget_mouseDragLeave(float AXpos, float AYpos, MIP_Widget* ATo) {
    //MIP_Print("%s : x %.2f y %.2f to %s\n",MName,AXpos,AYpos,(ATo)?ATo->getName():"?");
  }

  virtual void on_widget_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {
    //MIP_Print("%s : k %i s %i\n",MName,AKey,AState);
  }

  virtual void on_widget_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {
    //MIP_Print("%s : k %i s %i\n",MName,AKey,AState);
  }

  virtual void on_widget_connect(MIP_Parameter* AParameter) {
    MParameter = AParameter;
    MParamIndex = AParameter->getIndex();
    strncpy(MParamName,AParameter->getName(),255);
    MValue = AParameter->getDefaultValue();
    MMinValue = AParameter->getMinValue();
    MMaxValue = AParameter->getMaxValue();
    MDefaultValue = AParameter->getDefaultValue();
  }

  //virtual void on_widget_connect(uint32_t AIndex, clap_param_info_t* AInfo) {
  //  MParamIndex = AIndex;
  //  strncpy(MParamName,AInfo->name,255);
  //  MValue = AInfo->default_value;
  //  MDefaultValue = AInfo->default_value;
  //}

  //virtual void on_widget_connect(int32_t AParameterIndex) {
  //  //MIP_Print("%s : i %i\n",MName,AParameterIndex);
  //  //MParameterIndex = AParameterIndex;
  //}

  virtual void on_widget_timer() {
    //MIP_Print("%s\n",MName);
  }

//------------------------------
public:
//------------------------------

  // widget value has updated..

  virtual void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_update(AWidget);
  }

  //----------

  // widget needs to be redrawn

  /*
    - if this.opaque and this.mrect covers ARect, it's not visible,
      so we can ignore the update
    - if sender.opaque, nothing below it is visible, so we can start drawing
      from that widget (and downwards), sp pass on ASender, else we need to
      draw ourselves first, so send this as ASender..
    - (when it reaches window, ASender is the first widget to start painting
      at (and downwards), and ARect is the neede update window)
  */

  virtual void do_widget_redraw(MIP_Widget* AWidget, MIP_FRect ARect, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_redraw(AWidget,ARect,AMode);
  }

  //----------

  // widget has changed position or size..
  // parent might need to realign its ch8ld widgets

  virtual void do_widget_realign(MIP_Widget* AWidget, bool ARecursive=true) {
    if (MParent) MParent->do_widget_realign(AWidget,ARecursive);
  }

  //----------

  /*
    called by MIP_WidgetSizer (etc?)
    resize ourselves, and tell parent to realign all its children
    also set initial-rect, since alignment is starting from that
  */

  virtual void do_widget_resized(MIP_Widget* AWidget, float ADeltaX=0.0f, float ADeltaY=0.0f, uint32_t AMode=0) {
    //if (MParent) MParent->do_widget_resized(ASender,ADeltaX,ADeltaY);
    MRect.w += ADeltaX;
    MRect.h += ADeltaY;
    MInitialRect.w += ADeltaX;
    MInitialRect.h += ADeltaY;
    //resize(ADeltaX,ADeltaY);
    if (MParent) {
      MParent->alignWidgets();
      MParent->redraw();
    }
  }

  //----------

  virtual void do_widget_setMouseCursor(MIP_Widget* AWidget, int32_t ACursor) {
    if (MParent) MParent->do_widget_setMouseCursor(AWidget,ACursor);
  }

  //----------

  virtual void do_widget_setMouseCursorPos(MIP_Widget* AWidget, float AXpos, float AYpos) {
    if (MParent) MParent->do_widget_setMouseCursorPos(AWidget,AXpos,AYpos);
  }

  //----------

  virtual void do_widget_setHint(MIP_Widget* AWidget, const char* AHint, uint32_t AType=0) {
    if (MParent) MParent->do_widget_setHint(AWidget,AHint,AType);
  }

  //----------

  virtual void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) {
    if (MParent) MParent->do_widget_notify(AWidget,AValue);
  }

  //----------

  virtual void do_widget_setModal(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_setModal(AWidget);
  }

  //----------

  virtual void do_widget_captureMouse(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_captureMouse(AWidget);
  }

  //----------

  virtual void do_widget_captureKeyboard(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_captureKeyboard(AWidget);
  }


};

//----------------------------------------------------------------------
#endif
