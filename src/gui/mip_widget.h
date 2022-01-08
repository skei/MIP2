#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

#define MIP_WIDGET_MAX_PARAMS 8

#include "mip.h"
#include "base/types/mip_array.h"
#include "base/types/mip_point.h"
#include "base/types/mip_rect.h"
#include "gui/mip_painter.h"
#include "gui/base/mip_base_window.h"
#include "plugin/mip_parameter.h"

//----------------------------------------------------------------------

class MIP_Widget;
typedef MIP_Array<MIP_Widget*> MIP_Widgets;

//----------------------------------------------------------------------

struct MIP_WidgetFlags {
  //bool autoMouseCursor  = false;
  //bool autoMouseHide    = false;
  //bool autoMouseLock    = false;
  //bool autoSendHint     = false;
  ////bool canMove          = false;
  ////bool canResize        = false;
  bool active           = true;
  bool visible          = true;
  bool opaque           = false;
  bool interacting      = false;
  bool sizePercent      = false;
  bool posPercent       = false;
  bool autoCursor       = true;
  bool autoHint         = false;
  bool autoClip         = true;
  bool autoSize         = false;
  bool autoMouseLock    = false;
  bool autoMouseHide    = false;
  bool autoMouseRedraw  = false;
  bool canDrag          = false;
  bool canDrop          = false;
};

//struct MIP_WidgetState {
//};

//----------

struct MIP_WidgetLayout {
  uint32_t    alignment     = MIP_WIDGET_ALIGN_CLIENT;
  //MIP_FRect   initialRect   = {};
  MIP_FRect   innerBorder   = MIP_FRect(0,0);   // space between widgets and parent edges
  //MIP_FPoint  widgetSpacing = MIP_FPoint(0,0);  // space inbetween widgets
  MIP_FPoint  spacing       = MIP_FPoint(0,0);  // space inbetween widgets
  MIP_FRect   extraBorder   = MIP_FRect(0,0);   // extra space/border for each widget
  bool        contentBorder = true;             // true if content rect includes innerBorder of parent
  MIP_FRect   outerBorder   = MIP_FRect(0,0);
  MIP_FPoint  minSize       = MIP_FPoint(0,0);
  MIP_FPoint  maxSize       = MIP_FPoint(999999,999999);
  float       scale         = 1.0;
};

//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
protected:
//------------------------------

  const char*     MName                   = "MIP_Widget";
  const char*     MHint                   = "widget";
  int32_t         MCursor                 = MIP_CURSOR_DEFAULT;
  MIP_Widget*     MParent                 = nullptr;
  MIP_Widgets     MChildren               = {};
  int32_t         MWidgetIndex            = -1;
  int32_t         MParameterIndex         = -1;

  MIP_FRect       MRect                   = {};
  MIP_FRect       MInitialRect            = MIP_FRect(0,0); // starting rect (used by layout
  MIP_FRect       MContentRect            = MIP_FRect(0,0); // rect surrounding child widgets
  float           MChildrenXOffset        = 0.0f;           // offset (relative to parent rect)
  float           MChildrenYOffset        = 0.0f;           // -"-

  float           MModValue               = 0.0;
  float           MValue                  = 0.0;
  float           MDefaultValue           = 0.0;

  MIP_Parameter*  MParameters[MIP_WIDGET_MAX_PARAMS] = {0};            // otrs to connected parameters
//float           MMinValue               = 0.0;
//float           MMaxValue               = 1.0;
//uint32_t        MNumSteps               = 0.0;
//bool            MIsActive               = true;
//bool            MIsVisible              = true;

//  bool            MFillBackground         = false;
//  bool            MDrawBorder             = false;
//  bool            MDrawText               = false;
//  MIP_Color       MBackgroundColor        = MIP_COLOR_GRAY;
//  MIP_Color       MBorderColor            = MIP_COLOR_LIGHT_GRAY;
//  const char*     MText                   = "";
//  MIP_Color       MTextColor              = MIP_COLOR_BLACK;
//  uint32_t        MTextAlignment          = MIP_TEXT_ALIGN_CENTER;

//MIP_Surface*    MImageSurface           = nullptr;
//bool            MImageSurfaceAllocated  = false;
//uint32_t        MTileCount              = 0;
//uint32_t        MTileXCount             = 0;
//uint32_t        MTileYCount             = 0;
//uint32_t        MTileWidth              = 0;
//uint32_t        MTileHeight             = 0;

//------------------------------
public:
//------------------------------

  MIP_WidgetFlags   flags   = {};
  MIP_WidgetLayout  layout  = {};

//------------------------------
public:
//------------------------------

  MIP_Widget(MIP_FRect ARect) {
    //MRect = ARect;
    init(ARect);
  }

  //----------

  virtual ~MIP_Widget() {
    #ifndef MIP_NO_AUTODELETE
    deleteWidgets();
    #endif
//    if (MImageSurface && MImageSurfaceAllocated) delete MImageSurface;
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


  //void  virtual setActive(bool s=true)        { MIsActive = s; }
  //void  virtual setVisible(bool s=true)       { MIsVisible = s; }

  virtual void setChildrenOffset(float AX, float AY)        { MChildrenXOffset = AX; MChildrenYOffset = AY; }
  virtual void setChildrenXOffset(float AX)                 { MChildrenXOffset = AX; }
  virtual void setChildrenYOffset(float AY)                 { MChildrenYOffset = AY; }
  virtual void setContentRect(MIP_FRect ARect)             { MContentRect = ARect; }
  virtual void setCursor(int32_t ACursor)                   { MCursor = ACursor; }
  virtual void setDefaultValue(float AValue)                { MDefaultValue = AValue; }
  virtual void setHeight(float AHeight)                     { MRect.h = AHeight; }
  virtual void setHint(const char* AHint)                   { MHint = AHint; }
//virtual void setIndex(int32_t AIndex)                     { MIndex = AIndex; }
  virtual void  setWidgetIndex(int32_t i)                   { MWidgetIndex = i; }

  virtual void setInitialHeight(float AH)                   { MInitialRect.h = AH; }
  virtual void setInitialPos(float AX, float AY)            { MInitialRect.x = AX; MInitialRect.y = AY; }
  virtual void setInitialRect(MIP_FRect ARect)              { MInitialRect = ARect; }
  virtual void setInitialSize(float AW, float AH)           { MInitialRect.w = AW; MInitialRect.h = AH; }
  virtual void setInitialWidth(float AW)                    { MInitialRect.w = AW; }
  virtual void setModValue(float v)                         { MModValue = v; }
  virtual void setName(const char* AName)                   { MName = AName; }
  virtual void setParameter(MIP_Parameter* AParameter, uint32_t AIndex=0) { MParameters[AIndex] = AParameter; }
  virtual void setParameterIndex(int32_t i)                 { MParameterIndex = i; }
  virtual void setParent(MIP_Widget* AParent)               { MParent = AParent; }
  virtual void setPos(float AXpos, float AYpos)             { MRect.x = AXpos; MRect.y = AYpos; }
  virtual void setRect(MIP_FRect ARect)                     { MRect = ARect; }
  virtual void setRect(float x, float y, float w, float h)  { MRect.x=x; MRect.y=y; MRect.w=w;MRect.h=h; }
  virtual void setSize(float AWidth, float AHeight)         { MRect.w = AWidth; MRect.h = AHeight; }
//  virtual void setText(const char* AText)                   { MText = AText; }
  virtual void setValue(float AValue)                       { MValue = AValue; }
  virtual void setWidth(float AWidth)                       { MRect.w = AWidth; }

  //virtual void setOwner(MIP_BaseWindow* AOwner)        { MOwner = AOwner; }
  //virtual void setSelectedParameter(uint32_t AIndex)    { MSelectedParameter = AIndex; }
  //virtual void setParameterPtr(MIP_Parameter* p)       { MParameterPtr = p; }


//------------------------------
public:
//------------------------------


  virtual MIP_Widget*     getChild(uint32_t AIndex)   { return MChildren[AIndex]; }
  virtual MIP_FRect       getContentRect()            { return MContentRect; }
  virtual int32_t         getCursor()                 { return MCursor; }
  virtual float           getDefaultValue()           { return MDefaultValue; }
  virtual const char*     getHint()                   { return MHint; }
//virtual int32_t         getIndex()                  { return MIndex; }
  virtual int32_t         getWidgetIndex()            { return MWidgetIndex; }
  virtual MIP_FRect       getInitialRect()            { return MInitialRect; }
  virtual float           getModValue()               { return MModValue; }
  virtual const char*     getName()                   { return MName; }
  virtual uint32_t        getNumChildren()            { return MChildren.size(); }
  virtual MIP_Parameter*  getParameter(uint32_t i=0)  { return MParameters[i]; }
  virtual int32_t         getParameterIndex()         { return MParameterIndex; }
  virtual MIP_Widget*     getParent()                 { return MParent; }
  virtual MIP_FRect       getRect()                   { return MRect; }
//  virtual const char*     getText()                   { return MText; }
  virtual float           getValue()                  { return MValue; }

//virtual MIP_Surface*    getImageSurface()           { return MImageSurface; }
//virtual uint32_t        getTileXCount()             { return MTileXCount; }
//virtual uint32_t        getTileYCount()             { return MTileYCount; }


//------------------------------
public:
//------------------------------

  virtual uint32_t appendWidget(MIP_Widget* AWidget) {
    if (AWidget) {
      AWidget->MParent = this;
      uint32_t index = MChildren.size();
      AWidget->MWidgetIndex = index;
      MChildren.append(AWidget);
      return index;
    }
    return -1;
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

  //virtual MIP_Widget* findChild(float AXpos, float AYpos) {
  //  int32_t num = MChildren.size();
  //  if (num > 0) {
  //    for (int32_t i=(num-1); i>=0; i--) {
  //      MIP_Widget* widget = MChildren[i];
  //      //if (widget->flags.visible) {
  //      if (widget->flags.active) {
  //        if ( widget->getRect().contains(AXpos,AYpos) ) {
  //          MIP_Widget* child = widget->findChild(AXpos,AYpos);
  //          if (child) return child;
  //          return widget;
  //        }
  //      }
  //    }
  //  }
  //  return MIP_NULL;
  //}

  MIP_Widget* findWidget(float AXpos, float AYpos, bool ARecursive=true) {
    //for (uint32_t i=0; i<MChildren.size(); i++) {
    if (MChildren.size() > 0) {
      for (int32_t i = MChildren.size()-1; i >= 0; i--) {
        MIP_Widget* child = MChildren[i];
        if (child->flags.active) {
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

  //virtual void paintChildren(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
  //  MIP_FRect mrect = getRect();
  //  mrect.overlap(ARect);
  //  MIP_FRect clip_rect = getRect();
  //  clip_rect.shrink(layout.innerBorder);
  //  clip_rect.overlap(ARect);
  //  if (clip_rect.isEmpty()) return;
  //  if (flags.autoClip) APainter->pushClip(clip_rect);
  //  for (uint32_t i=0; i<MChildren.size(); i++) {
  //    MIP_Widget* child = MChildren[i];
  //    if (child->flags.visible) {
  //      MIP_FRect child_rect = child->getRect();
  //      if (child_rect.isNotEmpty()) {
  //        //if (child_rect.touches(mrect)) {
  //          MIP_FRect overlap_rect = clip_rect;//mrect;
  //          overlap_rect.overlap(child_rect);
  //          if (overlap_rect.isNotEmpty()) {
  //            child->on_widget_paint(APainter,overlap_rect,AMode);  // clip rect
  //          } // !overlap.empty
  //        //} // child.touches
  //      } // !child.empty
  //    } // child.visible
  //  } // for all children
  //  if (flags.autoClip) APainter->popClip();
  //}

  void paintWidgets(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode=0) {
    //MIP_Print("x %.2f y %.2f w %.2f h %.2f\n",ARect.x,ARect.y,ARect.w,ARect.h);;
    if (MChildren.size() > 0) {
      for (int32_t i = MChildren.size()-1; i >= 0; i--) {
        MIP_Widget* child = MChildren[i];
        if (child->flags.visible) {
          MIP_FRect rect = child->getRect();
          if (rect.touches(ARect)) {
            child->on_widget_paint(APainter,ARect,AMode);
          }
        }
      }
    }
  }

  //----------

  //void realignWidgets() {
  //}

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

  virtual void alignWidgets(bool ARecursive=true) {
    MIP_FRect client   = getRect();
    MIP_FRect parent   = client;
    MIP_FRect content  = client;

    client.shrink(layout.innerBorder);
    parent.shrink(layout.innerBorder);

    if (!layout.contentBorder) content.shrink(layout.innerBorder);
    //if (layout.contentBorder) content.grow(layout.innerBorder);

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
      if (child->flags.visible) {

        MIP_FRect  rect      = child->getInitialRect();
        uint32_t    alignment = child->layout.alignment;

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
            if ((stackx + rect.w + layout.innerBorder.w - layout.spacing.x) >= client.w) {
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
            if ((stacky + rect.h) >= client.h) {
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
        content.combine(rect);

        child->MRect.x = rect.x + MChildrenXOffset;
        child->MRect.y = rect.y + MChildrenYOffset;
        child->MRect.w = rect.w;
        child->MRect.h = rect.h;

//
        child->MRect.scale(child->layout.scale);
//

        if (ARecursive) child->alignWidgets(ARecursive);

      } // child visible
    } // for all children
    if (layout.contentBorder) {
      content.w += layout.innerBorder.w;
      content.h += layout.innerBorder.h;
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
      if (child->flags.visible) {
        //child->setChildrenOffset(AOffsetX,AOffsetY);
        child->MRect.x += AOffsetX;
        child->MRect.y += AOffsetY;
        child->scrollChildren(AOffsetX,AOffsetY);
      }
    }
  }

  //virtual void resize(float ADeltaX=0.0f, float ADeltaY=0.0f) {
  //  MRect.w += ADeltaX;
  //  MRect.h += ADeltaY;
  //  MInitialRect.w += ADeltaX;
  //  MInitialRect.h += ADeltaY;
  //}

  //----------

  virtual void attachWindow(MIP_BaseWindow* AWindow) {
    //MOwner = AWindow;
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      MIP_Widget* child = MChildren[i];
      child->attachWindow(AWindow);
    }
  }

//------------------------------
public:
//------------------------------

  virtual void update() {
    do_widget_update(this);
  }

  //----------

  virtual void redraw() {
    do_widget_redraw(this,getRect(),0);
  }

  //----------

//  virtual void setImage(MIP_Drawable* ATarget, MIP_Surface* ASurface) {
//    MImageSurface = ASurface;
//    MImageSurfaceAllocated = false;
//  }
//
//  virtual void setImage(MIP_Drawable* ATarget, MIP_Bitmap* ABitmap) {
//    MImageSurface = new MIP_Surface(ATarget,ABitmap->getWidth(),ABitmap->getHeight());
//    MImageSurfaceAllocated = true;
//    MIP_Painter* painter = new MIP_Painter(MImageSurface);
//    painter->uploadBitmap(0,0,ABitmap);
//    //painter->flush();
//    delete painter;
//  }
//
//  virtual void setImage(MIP_Drawable* ATarget, const uint8_t* ABuffer, uint32_t ASize, MIP_Color ABackground) {
//    MIP_Bitmap* bitmap = new MIP_Bitmap(ABuffer,ASize);
//    bitmap->premultAlpha( (uint32_t)ABackground );
//    setImage(ATarget,bitmap);
//    delete bitmap;
//  }
//
//  virtual void setImage(MIP_Drawable* ATarget, const char* AFilename, MIP_Color ABackground) {
//    MIP_Bitmap* bitmap = new MIP_Bitmap(AFilename);
//    bitmap->premultAlpha( (uint32_t)ABackground );
//    setImage(ATarget,bitmap);
//    delete bitmap;
//  }
//
//  //----------
//
//  virtual void setupTiles(uint32_t AXcount, uint32_t AYcount) {
//    MTileXCount = AXcount;
//    MTileYCount = AYcount;
//    MTileWidth  = MImageSurface->getWidth() / AXcount;
//    MTileHeight = MImageSurface->getHeight() / AYcount;
//  }
//
//  MIP_FRect getTileRect(uint32_t AIndex) {
//    float x = floorf(AIndex % MTileXCount) * MTileWidth;
//    float y = floorf(AIndex / MTileXCount) * MTileHeight;
//    float w = MTileWidth - 1;
//    float h = MTileHeight - 1;
//    return MIP_FRect(x,y,w,h);
//  }

//------------------------------
public:
//------------------------------

  //virtual void updateValue(float AValue) {
  //  MIP_PRINT;
  //}

  //----------

  //virtual void redraw() {
  //  MIP_PRINT;
  //}

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
//    paintChildren(APainter,ARect,AMode);
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
//    if (flags.autoCursor) do_widget_setMouseCursor(this,MCursor);
//    if (flags.autoHint) do_widget_setHint(this,MHint);
  }

  virtual void on_widget_mouseLeave(float AXpos, float AYpos, MIP_Widget* ATo) {
    //MIP_Print("%s : x %.2f y %.2f to %s\n",MName,AXpos,AYpos,(ATo)?ATo->getName():"?");
//    if (flags.autoCursor) do_widget_setMouseCursor(this,MIP_CURSOR_DEFAULT);
//    if (flags.autoHint) do_widget_setHint(this,"");
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

  virtual void on_widget_connect(int32_t AParameterIndex) {
    //MIP_Print("%s : i %i\n",MName,AParameterIndex);
    //MParameterIndex = AParameterIndex;
  }

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

  // widget has changed position or size..
  // parent might need to realign its ch8ld widgets

  virtual void do_widget_realign(MIP_Widget* AWidget, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_realign(AWidget,AMode);
  }

  /*
    called by MIP_WidgetSizer (etc?)
    resize ourselves, and tell parent to realign all its children
    also set initial-rect, since alignment is starting from that
  */

  virtual void do_widget_resized(MIP_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f, uint32_t AMode=0) {
    //if (MParent) MParent->do_widget_resized(ASender,ADeltaX,ADeltaY);
    MRect.w += ADeltaX;
    MRect.h += ADeltaY;
    MInitialRect.w += ADeltaX;
    MInitialRect.h += ADeltaY;
    //resize(ADeltaX,ADeltaY);
    if (MParent) {
      MParent->alignWidgets();
//      MParent->redraw();
    }
  }

  virtual void do_widget_setMouseCursor(MIP_Widget* AWidget, int32_t ACursor) {
    if (MParent) MParent->do_widget_setMouseCursor(AWidget,ACursor);
  }

  virtual void do_widget_setMouseCursorPos(MIP_Widget* AWidget, float AXpos, float AYpos) {
    if (MParent) MParent->do_widget_setMouseCursorPos(AWidget,AXpos,AYpos);
  }

  virtual void do_widget_setHint(MIP_Widget* AWidget, const char* AHint) {
    if (MParent) MParent->do_widget_setHint(AWidget,AHint);
  }

  virtual void do_widget_notify(MIP_Widget* ASender, uint32_t AValue=0) {
    if (MParent) MParent->do_widget_notify(ASender,AValue);
  }

//  virtual void do_widget_grabMouseCursor(MIP_Widget* ASender) {
//    if (MParent) MParent->do_widget_grabMouseCursor(ASender);
//  }
//
//  virtual void do_widget_grabKeyboard(MIP_Widget* ASender) {
//    if (MParent) MParent->do_widget_grabKeyboard(ASender);
//  }
//
//  virtual void do_widget_grabModal(MIP_Widget* ASender) {
//    if (MParent) MParent->do_widget_grabModal(ASender);
//  }

  //virtual void do_widget_setModal(MIP_Widget* AWidget, bool AModal=true) {}
  //virtual void do_widget_captureMouse(MIP_Widget* AWidget, bool ACapture=true) {}
  //virtual void do_widget_captureKeyboard(MIP_Widget* AWidget, bool ACapture=true) {}

};

//----------------------------------------------------------------------
#endif
