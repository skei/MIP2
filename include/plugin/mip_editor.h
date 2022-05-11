#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

/*
  TODO:
    widget/automation 'fighting'..
    - MClickedValue could have changed becaouse of modulation
*/


#include "mip.h"
#include "base/system/mip_timer.h"
#include "base/types/mip_queue.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"
#include "plugin/clap/mip_clap_utils.h"
#include "gui/mip_window.h"

#define MIP_EDITOR_TIMER_RATE 30

/*
  plugin/host -> gui
  could potentially be all parameters
  (loading preset..)
*/

#define EVENTS_PER_BLOCK 4096

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
class MIP_EditorWindow
: public MIP_Window {

public:

  MIP_EditorWindow(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener, bool AEmbedded)
  : MIP_Window(AWidth,AHeight,AListener,AEmbedded) {
  }

  virtual ~MIP_EditorWindow() {
  }

public:

//  void do_widget_redraw(MIP_Widget* AWidget, MIP_FRect ARect, uint32_t AMode=0) override {
//    //MIP_PRINT;
//    //int32_t param_index = AWidget->getParamIndex();
//    //if (param_index < 0) {
//      MIP_Window::do_widget_redraw(AWidget,ARect,AMode);
//    //}
//  }

};
*/

//----------------------------------------------------------------------

class MIP_EditorListener{
public:
  virtual void on_beginUpdateParameterFromEditor(uint32_t AIndex) {}
  virtual void on_updateParameterFromEditor(uint32_t AIndex, float AValue) {}
  virtual void on_endUpdateParameterFromEditor(uint32_t AIndex) {}
  virtual void on_resizeFromEditor(uint32_t AWidth, uint32_t AHeight) {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Editor
: public MIP_WindowListener
, public MIP_TimerListener {

//------------------------------
private:
//------------------------------

  MIP_EditorListener*       MListener         = nullptr;
  MIP_ClapPlugin*           MPlugin           = nullptr;
  uint32_t                  MNumParams        = 0;
  uint32_t                  MWidth            = 0;
  uint32_t                  MHeight           = 0;
  double                    MScale            = 1.0;
  bool                      MCanResize        = false;
  bool                      MEmbedded         = false;
  //MIP_EditorWindow*         MWindow           = nullptr;
  MIP_Window*               MWindow           = nullptr;
  MIP_Timer*                MTimer            = nullptr;
  MIP_Widget**              MParamToWidget    = nullptr;

  //MIP_ClapIntQueue          MGuiParamQueue    = {};
  //MIP_ClapIntQueue          MGuiModQueue      = {};
  MIP_Queue<uint32_t,EVENTS_PER_BLOCK> MGuiParamQueue  = {};
  MIP_Queue<uint32_t,EVENTS_PER_BLOCK> MGuiModQueue    = {};

  float*                    MGuiParamVal      = nullptr;
  float*                    MGuiParamMod      = nullptr;

  bool                      MEditorIsOpen     = true;

  //MIP_Widgets               MChangedParmeters = {};
  MIP_Widgets               MDirtyWidgets     = {};

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded) {
    MListener = AListener;
    MPlugin = APlugin;
    MWidth = AWidth;
    MHeight = AHeight;
    MEmbedded = AEmbedded;
    MNumParams = APlugin->params_count();
    uint32_t size = MNumParams * sizeof(MIP_Widget*);
    MParamToWidget = (MIP_Widget**)malloc(size);
    memset(MParamToWidget,0,size);
    size = MNumParams * sizeof(float);
    MGuiParamVal = (float*)malloc(size);
    MGuiParamMod = (float*)malloc(size);
    memset(MGuiParamVal,0,size);
    memset(MGuiParamMod,0,size);
    MTimer = new MIP_Timer(this);
    //MWindow = new MIP_EditorWindow(AWidth,AHeight,this,AEmbedded);
    MWindow = new MIP_Window(AWidth,AHeight,this,AEmbedded);
  }

  //----------

  virtual ~MIP_Editor() {
    delete MTimer;
    free(MParamToWidget);
    free(MGuiParamVal);
    free(MGuiParamMod);

    if (MWindow) delete MWindow;

  }

//------------------------------
public: // clap.gui
//------------------------------

  virtual bool attach(const char* ADisplay, uint32_t AWindow) {
    //MIP_PRINT;
    MWindow->reparent(AWindow);
    return true;
  }

  //----------

  virtual bool show() {
    //MIP_PRINT;
    MWindow->setOwnerWindow(MWindow);
    MWindow->open();
    MTimer->start(MIP_EDITOR_TIMER_RATE);
    MEditorIsOpen = true;
    return true;
  }

  //----------

  virtual bool hide() {
    //MIP_PRINT;
    MEditorIsOpen = false;
    MTimer->stop();
    MWindow->close();
    return true;
  }

  //----------

  virtual bool setScale(double AScale) {
    //MIP_Print("%.2f\n",AScale);
    MScale = AScale;
    //MWindow->setScale(AScale);
    return true;
  }

  //----------

  virtual bool getSize(uint32_t *width, uint32_t *height) {
    //MIP_Print("-> %i,%i\n",MWidth,MHeight);
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  //----------

  virtual bool canResize() {
    //MIP_Print("-> %s\n",MCanResize?"true":"false");
    return MCanResize;
  }

  //----------

  virtual bool adjustSize(uint32_t *width, uint32_t *height) {
    //MIP_Print("*width:%i *height:%i\n",*width,*height);
    //*width = *width & 0xffc0;
    //*height = *height & 0xffc0;
    return true;
  }

  //----------

  virtual bool setSize(uint32_t width, uint32_t height) {
    //MIP_Print("%i,%i\n",width,height);
    MWidth = width;
    MHeight = height;
    MWindow->setWindowSize(width,height);
    return true;
  }

//------------------------------
public:
//------------------------------

  MIP_Window* getWindow() {
    return MWindow;
  }

  //----------

  void setCanResize(bool AResize=true) {
    MCanResize = AResize;
  }

  //----------

  void connect(MIP_Widget* AWidget, MIP_Parameter* AParameter) {
    int32_t index = AParameter->getIndex();
    if (index >= 0) {
      AWidget->setParamIndex(index);
      MParamToWidget[index] = AWidget;
      AWidget->on_widget_connect(AParameter);
    }
  }

  //----------

  void connect(MIP_Widget* AWidget, int32_t AParamIndex) {
    AWidget->setParamIndex(AParamIndex);
    MParamToWidget[AParamIndex] = AWidget;
    clap_param_info_t info;
    if (MPlugin->params_get_info(AParamIndex,&info)) {
      MIP_Parameter* parameter = (MIP_Parameter*)info.cookie;
      AWidget->on_widget_connect(parameter);
    }
    //const char* name = info.name;
    //float def_value = info.default_value;
    //AWidget->setDefaultValue(def_value);
    //AWidget->setValue(def_value);
    //AWidget->setParamName(name);
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

  // [gui]

  void on_updateWidgetFromWindow(MIP_Widget* AWidget) override {
    int32_t index = AWidget->getParamIndex();
    if (index >= 0) {
      float value = AWidget->getValue();
      if (MListener) MListener->on_updateParameterFromEditor(index,value);
    }
  }

  //----------

  void on_resizeFromWindow(uint32_t AWidth, uint32_t AHeight) override {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    if (MListener) MListener->on_resizeFromEditor(AWidth,AHeight);
  }

//------------------------------
protected: // timer listener
//------------------------------

  // [timer]

  void on_timerCallback(void) override {
    //clearChangedParameters();
    clearDirtyWidgets();
    flushGuiParams();
    flushGuiMods();
    //redrawChangedParameters();
    redrawDirtyWidgets();
  }

//------------------------------
public:
//------------------------------

  // [audio thread]

  void updateParameterInProcess(uint32_t AIndex, float AValue) {
    if (MWindow) {
      MGuiParamVal[AIndex] = AValue;
      queueGuiParam(AIndex);
    }
  }

  //----------

  // [audio thread]
  // called from handle_param_mod()

  //void updateModulationFromHost(uint32_t AIndex, float AValue) {
  void updateModulationInProcess(uint32_t AIndex, float AValue) {
    if (MWindow) {
      MGuiParamMod[AIndex] = AValue;
      queueGuiMod(AIndex);
    }
  }

//------------------------------
public:
//------------------------------

  // called from updateParameterFromHost()

  void queueGuiParam(uint32_t AIndex) {
    MGuiParamQueue.write(AIndex);
  }

  //----------

  // [gui]

  void flushGuiParams() {
    if (MEditorIsOpen && MWindow) {
      uint32_t index = 0;
      while (MGuiParamQueue.read(&index)) {
        float value = MGuiParamVal[index];
        MIP_Widget* widget = MParamToWidget[index];
        if (widget) {
          if (widget->getValue() != value)
          widget->setValue(value);
          //MWindow->paintWidget(widget); // -> ->paint(widget)

          //if (MChangedParmeters.findItem(widget) < 0) {
          //  MChangedParmeters.append(widget);
          //}

          if (MDirtyWidgets.findItem(widget) < 0) {
            MDirtyWidgets.append(widget);
          }

        }
      }
    }
  }

  //----------

  void queueGuiMod(uint32_t AIndex) {
    MGuiModQueue.write(AIndex);
  }

  //----------

  void flushGuiMods() {
    if (MEditorIsOpen) { // && MWindow?
      uint32_t index = 0;
      while (MGuiModQueue.read(&index)) {

        //MIP_Print("index %i\n",index);

        float valuemod = MGuiParamMod[index];
        MIP_Widget* widget = MParamToWidget[index];
        if (widget) {
          //if (widget->getModValue() != value)
          widget->setModValue(valuemod);
          //MWindow->paintWidget(widget); // -> ->paint(widget)

          //if (MChangedParmeters.findItem(widget) < 0) {
          //  MChangedParmeters.append(widget);
          //}

          if (MDirtyWidgets.findItem(widget) < 0) {
            MDirtyWidgets.append(widget);
          }

        }
      }
    }
  }

  //----------

  void clearDirtyWidgets() {
  //void clearChangedParameters() {
    //MChangedParmeters.clear();
    MDirtyWidgets.clear();
  }

  //----------

  // called from on_timerCallback

  //void redrawChangedParameters() {
  void redrawDirtyWidgets() {
    if (MEditorIsOpen && MWindow) {
      //for (uint32_t i=0; i<MChangedParmeters.size(); i++) {
      for (uint32_t i=0; i<MDirtyWidgets.size(); i++) {
      //if (MEditorIsOpen && MWindow) {
        MIP_Widget* widget = MDirtyWidgets[i];
        //MWindow->paintWidget(widget); // -> ->paint(widget)
        //MWindow->paint(widget);
        MIP_FRect rect = widget->getRect();
        MWindow->invalidate(rect.x,rect.y,rect.w + 0,rect.h + 0); // +1
        //invalidate(ARect.x,ARect.y,ARect.w + 1,ARect.h + 1);
      }
    }
  }

  //----------

//  void redrawAll() {
//    if (MEditorIsOpen && MWindow) {
//      MWindow->do_widget_redraw(MWindow,MWindow->getRect(),0);
//      //MWindow->redraw();
//      //MWindow->paintWindow();
//    }
//  }

};

#undef EVENTS_PER_BLOCK

//----------------------------------------------------------------------
#endif
