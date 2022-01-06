#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "base/types/mip_point.h"
#include "base/types/mip_rect.h"
#include "gui/mip_painter.h"

//----------------------------------------------------------------------

class MIP_Widget;
typedef MIP_Array<MIP_Widget*> MIP_Widgets;

//----------------------------------------------------------------------

//struct MIP_WidgetOptions {
//  bool autoMouseCursor  = false;
//  bool autoMouseHide    = false;
//  bool autoMouseLock    = false;
//  bool autoSendHint     = false;
//  //bool canMove          = false;
//  //bool canResize        = false;
//};

//----------

struct MIP_WidgetLayout {
  uint32_t    alignment     = MIP_WIDGET_ALIGN_NONE;
  MIP_FRect   initialRect   = {};
  MIP_FRect   innerBorder   = {};
  MIP_FRect   outerBorder   = {};
  MIP_FPoint  widgetSpacing = {};
};

//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
protected:
//------------------------------

  const char*       MName           = "MIP_Widget";

  MIP_Widget*       MParent         = nullptr;
  MIP_Widgets       MChildren       = {};

  MIP_FRect         MRect           = {};
  int32_t           MWidgetIndex    = -1;
  int32_t           MParameterIndex = -1;

  float             MValue          = 0.0;
  float             MModValue       = 0.0;

  float             MMinValue       = 0.0;
  float             MMaxValue       = 1.0;
  float             MDefValue       = 0.0;
  uint32_t          MNumSteps       = 0.0;

  bool              MIsActive       = true;
  bool              MIsVisible      = true;

//MIP_WidgetOptions MOptions        = {};
  MIP_WidgetLayout  MLayout         = {};

//------------------------------
public:
//------------------------------

  MIP_Widget(MIP_FRect ARect) {
    MRect = ARect;
  }

  //----------

  virtual ~MIP_Widget() {
    #ifndef MIP_NO_AUTODELETE
    deleteWidgets();
    #endif
  }

//------------------------------
public:
//------------------------------

  const char* getName()           { return MName; }
  float       getValue()          { return MValue; }
  float       getModValue()       { return MModValue; }
  int32_t     getParameterIndex() { return MParameterIndex; }
  int32_t     getWidgetIndex()    { return MWidgetIndex; }
  MIP_FRect   getRect()           { return MRect; }

  bool        isActive()          { return MIsActive; }
  bool        isVisible()         { return MIsVisible; }

  //----------

  void  setName(const char* s)        { MName = s; }
  void  setValue(float v)             { MValue = v; }
  void  setModValue(float v)          { MModValue = v; }
  void  setParameterIndex(int32_t i)  { MParameterIndex = i; }
  void  setWidgetIndex(int32_t i)     { MWidgetIndex = i; }

  void  setActive(bool s=true)        { MIsActive = s; }
  void  setVisible(bool s=true)       { MIsVisible = s; }

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

  MIP_Widget* findWidget(float AXpos, float AYpos, bool ARecursive=true) {
    //for (uint32_t i=0; i<MChildren.size(); i++) {
    if (MChildren.size() > 0) {
      for (int32_t i = MChildren.size()-1; i >= 0; i--) {
        MIP_Widget* child = MChildren[i];
        if (child->MIsActive) {
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

  void realignWidgets() {
  }

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

  virtual void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect) {
    //MIP_Print("%s : x %.2f y %.2f w %.2f h %.2f\n",MName,ARect.x,ARect.y,ARect.w,ARect.h);
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
  }

  virtual void on_widget_mouseLeave(float AXpos, float AYpos, MIP_Widget* ATo) {
    //MIP_Print("%s : x %.2f y %.2f to %s\n",MName,AXpos,AYpos,(ATo)?ATo->getName():"?");
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

  virtual void do_widget_update(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_update(AWidget);
  }

  // widget needs to be redrawn

  virtual void do_widget_redraw(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_redraw(AWidget);
  }

  // widget has changed position or size..
  // parent might need to realign its ch8ld widgets

  //virtual void do_widget_realign(MIP_Widget* AWidget) {
  //  if (MParent) MParent->do_widget_realign(AWidget);
  //}

  virtual void do_widget_setMouseCursor(MIP_Widget* AWidget, int32_t ACursor) {
    if (MParent) MParent->do_widget_setMouseCursor(AWidget,ACursor);
  }

  virtual void do_widget_setHint(MIP_Widget* AWidget, const char* AHint) {
    if (MParent) MParent->do_widget_setHint(AWidget,AHint);
  }

  //virtual void do_widget_setModal(MIP_Widget* AWidget, bool AModal=true) {}
  //virtual void do_widget_captureMouse(MIP_Widget* AWidget, bool ACapture=true) {}
  //virtual void do_widget_captureKeyboard(MIP_Widget* AWidget, bool ACapture=true) {}

};

//----------------------------------------------------------------------
#endif
