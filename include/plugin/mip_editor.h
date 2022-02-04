#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

//#define MIP_NO_WINDOW_BUFFERING

#include "mip.h"
#include "base/system/mip_timer.h"
#include "base/types/mip_queue.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"
#include "gui/mip_window.h"

#define MIP_CLAP_MESSAGE_QUEUE_SIZE   32
typedef MIP_Queue<uint32_t,MIP_CLAP_MESSAGE_QUEUE_SIZE> MIP_ClapIntQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_EditorListener {
public:
  virtual void timerFromEditor() {}
  virtual void updateParameterFromEditor(uint32_t AIndex, float AValue) {}
};

//----------------------------------------------------------------------

class MIP_Editor
: public MIP_WindowListener
, public MIP_TimerListener {

//------------------------------
private:
//------------------------------

  MIP_EditorListener* MListener         = nullptr;
  uint32_t            MWidth            = 250;
  uint32_t            MHeight           = 70;
  double              MScale            = 1.0;
  bool                MCanResize        = false;
  MIP_Window*         MWindow           = nullptr;
  MIP_Timer*          MTimer            = nullptr;
  MIP_Widget**        MParamToWidget    = nullptr;

  MIP_ClapIntQueue    MHostParamQueue   = {};
  float*              MHostParamVal     = nullptr;
  float*              MHostParamMod     = nullptr;

  MIP_ClapIntQueue    MGuiParamQueue    = {};
  float*              MGuiParamVal      = nullptr;
  float*              MGuiParamMod      = nullptr;

  bool                MEditorIsOpen     = true;
  uint32_t MNumParams = 0;


//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, uint32_t ANumParams) {
    MNumParams = ANumParams;
    MListener = AListener;
    uint32_t size = ANumParams * sizeof(MIP_Widget*);
    MParamToWidget = (MIP_Widget**)malloc(size);
    memset(MParamToWidget,0,size);
    MTimer = new MIP_Timer(this);
    MHostParamVal = (float*)malloc(ANumParams * sizeof(float));
    MHostParamMod = (float*)malloc(ANumParams * sizeof(float));
    MGuiParamVal = (float*)malloc(ANumParams * sizeof(float));
    MGuiParamMod = (float*)malloc(ANumParams * sizeof(float));
  }

  //----------

  virtual ~MIP_Editor() {
    free(MParamToWidget);
    if (MWindow) {
      delete MWindow;
    }
    delete MTimer;
    free(MHostParamVal);
    free(MHostParamMod);
    free(MGuiParamVal);
    free(MGuiParamMod);
  }

//------------------------------
public:
//------------------------------

  MIP_Window* getWindow() {
    return MWindow;
  }

  //----------

  void connect(MIP_Widget* AWidget, int32_t AParamIndex, int32_t ASubParamIndex=-1) {
    //MIP_Print("widget: %p index: %i\n",AWidget,AParamIndex);
    AWidget->setParamIndex(AParamIndex);
    AWidget->setSubParamIndex(ASubParamIndex);
    MParamToWidget[AParamIndex] = AWidget;
  }

  //----------

  void setParameterValue(uint32_t AIndex, float AValue) {
    MIP_Widget* widget = MParamToWidget[AIndex];
    if (widget) {
      widget->setValue(AValue);
    }
  }

//------------------------------
private: // window listener
//------------------------------

  // gui thread

  void updateWidgetFromWindow(MIP_Widget* AWidget) final {
    uint32_t index = AWidget->getParamIndex();
    if (index >= 0) {
      float value = AWidget->getValue();
      //MIP_Print("index: %i value: %.3f\n",index,value);
      //if (MListener) MListener->updateParameterFromEditor(index,value);
      MHostParamVal[index] = value;
      queueHostParam(index);
    }
  }

//------------------------------
public:
//------------------------------

  // process thread

  void updateParameterFromHost(uint32_t AIndex, float AValue) {
    //MIP_Print("%i = %.3f\n",AIndex,AValue);
    if (MWindow) {
      MGuiParamVal[AIndex] = AValue;
      queueGuiParam(AIndex);
    }
  }

  void updateModulationFromHost(uint32_t AIndex, float AValue) {
    if (MWindow) {
      MGuiParamMod[AIndex] = AValue;
      queueGuiParam(AIndex);
    }
  }

//------------------------------
private: // timer
//------------------------------

  void on_timerCallback(void) final {
    //MIP_PRINT;
    flushGuiParams();
  }

//------------------------------
public:
//------------------------------

  void queueGuiParam(uint32_t AIndex) {
    MGuiParamQueue.write(AIndex);
  }

  //----------

  void flushGuiParams() {
    if (MEditorIsOpen) {
      uint32_t index = 0;
      while (MGuiParamQueue.read(&index)) {
        float value = MGuiParamVal[index];
        //send_param_value(index,value,out_events);
        MIP_Widget* widget = MParamToWidget[index];
        if (widget) {
          //MIP_Print("redraw widget (%i,%.3f)\n",index,value);
          widget->setValue(value);
          //MIP_FRect rect = widget->getRect();
          //widget->redraw();
          MWindow->paintWidget(widget);
        }
      }
    }
  }

  void queueHostParam(uint32_t AIndex) {
    MHostParamQueue.write(AIndex);
  }

  //----------

  void flushHostParams(const clap_output_events_t* out_events) {
    uint32_t index = 0;
    while (MHostParamQueue.read(&index)) {
      float value = MHostParamVal[index];
      send_param_value(index,value,out_events);
    }
  }

  //----------

  void send_param_mod(uint32_t index, float value, const clap_output_events_t* out_events) {
    clap_event_param_mod_t param_mod;
    param_mod.header.size     = sizeof (clap_event_param_mod_t);
    param_mod.header.time     = 0;
    param_mod.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    param_mod.header.type     = CLAP_EVENT_PARAM_MOD;
    param_mod.header.flags    = 0;//CLAP_EVENT_BEGIN_ADJUST | CLAP_EVENT_END_ADJUST | CLAP_EVENT_SHOULD_RECORD;// | CLAP_EVENT_IS_LIVE;
    param_mod.param_id        = index;
    param_mod.cookie          = nullptr;
    param_mod.port_index      = -1;
    param_mod.key             = -1;
    param_mod.channel         = -1;
    param_mod.amount          = value;
    clap_event_header_t* header = (clap_event_header_t*)&param_mod;
    out_events->push_back(out_events,header);
  }

  //----------

  void send_param_value(uint32_t index, float value, const clap_output_events_t* out_events) {
    clap_event_param_value_t param_value;
    param_value.header.size     = sizeof (clap_event_param_value_t);
    param_value.header.time     = 0;
    param_value.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    param_value.header.type     = CLAP_EVENT_PARAM_VALUE;
    param_value.header.flags    = CLAP_EVENT_BEGIN_ADJUST | CLAP_EVENT_END_ADJUST | CLAP_EVENT_SHOULD_RECORD;// | CLAP_EVENT_IS_LIVE;
    param_value.param_id        = index;
    param_value.cookie          = nullptr;
    param_value.port_index      = -1;
    param_value.key             = -1;
    param_value.channel         = -1;
    param_value.value           = value;
    clap_event_header_t* header = (clap_event_header_t*)&param_value;
    out_events->push_back(out_events,header);
  }

  //----------

//------------------------------
public: // clap.gui
//------------------------------

  virtual bool attach(const char* ADisplay, uint32_t AWindow) {
    MWindow = new MIP_Window(256,256,"",this,AWindow);
    return true;
  }

  //----------

  virtual void show() {
    MWindow->alignWidgets();
    MWindow->open();
    MTimer->start(30);
    MEditorIsOpen = true;
  }

  //----------

  virtual void hide() {
    MEditorIsOpen = false;
    MTimer->stop();
    MWindow->close();
  }

  virtual bool setScale(double AScale) {
    MScale = AScale;
    return true;
  }

  //----------

  virtual bool getSize(uint32_t *width, uint32_t *height) {
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  virtual bool canResize() {
    return MCanResize;
  }

  //----------

  virtual void roundSize(uint32_t *width, uint32_t *height) {
  }

  //----------

  virtual bool setSize(uint32_t width, uint32_t height) {
    MWidth = width;
    MHeight = height;
    return true;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
