#ifndef kode_debug_watch_panel_included
#define kode_debug_watch_panel_included
//----------------------------------------------------------------------

#include "kode.h"
#include "base/types/kode_array.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

enum KODE_EDebugWatchTypes {
  KODE_DEBUG_WATCH_NONE       = 0,
  KODE_DEBUG_WATCH_SEPARATOR  = 1,
  KODE_DEBUG_WATCH_INT        = 2,
  KODE_DEBUG_WATCH_BOOL       = 3,
  KODE_DEBUG_WATCH_HEX        = 4,
  KODE_DEBUG_WATCH_BIN        = 5,
  KODE_DEBUG_WATCH_PTR        = 6,
  KODE_DEBUG_WATCH_FLOAT      = 7,
  KODE_DEBUG_WATCH_STR        = 8
};

//----------

class KODE_DebugWatch {

public:

  const char*       name        = "";
  uint32_t          type        = KODE_DEBUG_WATCH_NONE;
  void*             pointer     = KODE_NULL;
  KODE_ValueWidget* widget      = KODE_NULL;
  char              buffer[32]  = {0};

public:

  KODE_DebugWatch(const char* AName, uint32_t AType, void* APtr, KODE_ValueWidget* AWidget) {
    name    = AName;
    type    = AType;
    pointer = APtr;
    widget  = AWidget;
    memset(buffer,0,32);
  }
};

typedef KODE_Array<KODE_DebugWatch*>  KODE_DebugWatches;

//----------------------------------------------------------------------

class KODE_DebugWatchPanel
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

  KODE_DebugWatches MWatches;

//------------------------------
public:
//------------------------------

  KODE_DebugWatchPanel(KODE_FRect ARect=KODE_FRect())
  : KODE_PanelWidget(ARect) {
    setName("KODE_DebugWidget");
    setHint("panel");
    setFillBackground(false);
    setBackgroundColor(KODE_COLOR_GRAY);
    setDrawBorder(true);
    setBorderColor(KODE_COLOR_DARK_GRAY);
    layout.innerBorder = KODE_FRect(10,10,10,10);
    layout.spacing = KODE_FPoint(10,0);
  }

  virtual ~KODE_DebugWatchPanel() {
    #ifndef KODE_NO_AUTODELETE
      deleteWatches();
    #endif
  }

//------------------------------
public:
//------------------------------

  void appendWatch(uint32_t AType, const char* AName="", void* APointer=KODE_NULL) {

    if (AType == KODE_DEBUG_WATCH_SEPARATOR) {
      KODE_Widget* widget = new KODE_Widget(KODE_FRect(0,0,0,10));
      widget->layout.alignment = KODE_WIDGET_ALIGN_STACK_VERT;
      appendWidget(widget);
    }
    else {
      KODE_ValueWidget* widget = new KODE_ValueWidget(KODE_FRect(200,15),AName);
      widget->layout.alignment = KODE_WIDGET_ALIGN_STACK_VERT;
      widget->setFillBackground(false);
      widget->setDrawBorder(false);
      switch(AType) {
        case KODE_DEBUG_WATCH_INT:
          widget->setDigits(0);
          break;
        case KODE_DEBUG_WATCH_FLOAT:
          widget->setDigits(3);
          break;
        case KODE_DEBUG_WATCH_PTR:
          widget->setDigits(0);
          break;
        case KODE_DEBUG_WATCH_BOOL:
          widget->setDigits(0);
          break;
      }
      appendWidget(widget);
      KODE_DebugWatch* watch = new KODE_DebugWatch(AName,AType,APointer,widget);
      MWatches.append(watch);
    }
  }

  //----------

  void deleteWatches() {
    for (uint32_t i=0; i<MWatches.size(); i++) {
      delete MWatches[i];
      MWatches[i] = KODE_NULL;
    }
  }

  //----------

  void updateWatches() {
    uint32_t num = MWatches.size();
    for (uint32_t i=0; i<num; i++) {
      KODE_DebugWatch* watch = MWatches[i];
      switch(watch->type) {
        case KODE_DEBUG_WATCH_INT:
          {
            int value = *(int*)watch->pointer;
            watch->widget->setValue(value);
          }
          break;
        case KODE_DEBUG_WATCH_FLOAT:
          {
            float value = *(float*)watch->pointer;
            watch->widget->setValue(value);
          }
          break;
        case KODE_DEBUG_WATCH_PTR:
          {
            intptr_t value = *(intptr_t*)watch->pointer;
            watch->widget->setValue(value);
          }
          break;
        case KODE_DEBUG_WATCH_BOOL:
          {
            bool value = *(bool*)watch->pointer;
            if (value) watch->widget->setValue(1);
            else watch->widget->setValue(0);
          }
          break;
      }
    }
  }

};


//----------------------------------------------------------------------
#endif
