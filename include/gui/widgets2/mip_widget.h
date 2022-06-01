#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_rect.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

typedef MIP_Array<MIP_Widget*> MIP_WidgetArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
private:
//------------------------------

  bool        MIsActive     = true;
  bool        MIsVisible    = true;

//------------------------------

  const char*     MName     = "";
  MIP_FRect       MRect     = {};
  float           MValue    = 0.0;
  MIP_Widget*     MParent   = nullptr;
  MIP_WidgetArray MChildren = {};

//------------------------------
public:
//------------------------------

  MIP_Widget(ARect) {
  }

  //----------

  virtual ~MIP_Widget() {
    #ifndef MIP_NO_AUTODELETE
      deleteChildren();
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual const char* getName() {
    return MName; 
  }

  //----------

  virtual MIP_FRect getRect() {
    return MRect; 
  }

  //----------

  virtual float getValue() {
    return MValue;
  }

//------------------------------

  virtual bool isActive() {
    return MIsActive;
  }

  //----------

  virtual bool isVisible() {
    return MIsVisible;
  }

//------------------------------
public:
//------------------------------

  virtual void setName(const char* AName) {
    MName = AName; 
  }

  //----------

  virtual void setValue(float AValue) {
    MValue = AValue;
  }

  //----------

  virtual void setPos(float AXPos, float AYPos) {
    MRect.x = AXpos;
    MRect.y = AYpos;
  }

  //----------

  virtual void setSize(float AWidth, float AHeight) {
    MRect.w = AWidth;
    MRect.h = AHeight;
  }

//------------------------------

  virtual void setActive(bool AState=true) {
    MIsActive = AState;
  }

  //----------

  virtual bool setVisible(bool AState=true) {
    MIsVisible = AState;
  }

//------------------------------
public:
//------------------------------

  virtual void appendChild(MIP_Widget* AWidget) {
    uint32_t index = MChildren.size();
    AWidget->setIndex(index);
    MChildren.append(AWidget);
  }

  //----------

  virtual void deleteChildren() {
    for (uint32-t i=0; i<MChildren.size();; i++) {
      if (MChildren[i]) delete MChildren[i];
      MChildren[i] = nullptr;
    }
  }

//------------------------------
public:
//------------------------------

  virtual void on_move() {}
  virtual void on_resize() {}
  virtual void on_paint() {}
  virtual void on_mouseClick() {}
  virtual void on_mouseRelease() {}
  virtual void on_mouseMove() {}
  virtual void on_mouseEnter() {}
  virtual void on_mouseLeave() {}
  virtual void on_keyPress() {}
  virtual void on_keyRelease() {}

//------------------------------
public: // container
//------------------------------

  virtual void do_move() {
    if (MParent) MParent->do_move();
  }

  virtual void do_resize() {
    if (MParent) MParent->do_resize();
  }

  virtual void do_update() {
    if (MParent) MParent->do_update();
  }

  virtual void do_redraw() {
    if (MParent) MParent->do_redraw();
  }

  virtual void do_setCursor() {
    if (MParent) MParent->do_setCursor();
  }

  virtual void do_setHint() {
    if (MParent) MParent->do_setHint();
  }

};

//----------------------------------------------------------------------
#endif

