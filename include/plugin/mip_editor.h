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
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"
#include "plugin/clap/mip_clap_utils.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_EditorWindow
: public MIP_Window {

public:

  MIP_EditorWindow(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener=nullptr)
  : MIP_Window(AWidth,AHeight,AListener,true) {
  }

  virtual ~MIP_EditorWindow() {
  }

public:

//  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode=0) override {
//    //MIP_Window::on_widget_paint(APainter,ARect,AMode);
//    MEditor->MGuiParamVal[AIndex] = AValue;
//    MEditor->queueGuiParam(AIndex);
//  }

  void do_widget_redraw(MIP_Widget* AWidget, MIP_FRect ARect, uint32_t AMode=0) override {
    MIP_PRINT;
    //if (MListener) MListener->do_window_redrawWidget(AWidget);
    //invalidate(ARect.x,ARect.y,ARect.w + 1,ARect.h + 1);
    MIP_Window::do_widget_redraw(AWidget,ARect,AMode);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_EditorListener{
public:
  virtual void on_editor_updateParameter(uint32_t AIndex, float AValue) {}
  virtual void on_editor_resize(uint32_t AWidth, uint32_t AHeight) {}
};

//----------------------------------------------------------------------

class MIP_Editor
: public MIP_WindowListener
, public MIP_TimerListener {

//------------------------------
private:
//------------------------------

  MIP_EditorListener* MListener         = nullptr;
  MIP_ClapPlugin*     MPlugin           = nullptr;
  uint32_t            MNumParams        = 0;
  uint32_t            MWidth            = 0;
  uint32_t            MHeight           = 0;
  double              MScale            = 1.0;
  bool                MCanResize        = false;
  //MIP_Window*         MWindow           = nullptr;
  MIP_EditorWindow*   MWindow           = nullptr;
  MIP_Timer*          MTimer            = nullptr;
  MIP_Widget**        MParamToWidget    = nullptr;
  MIP_ClapIntQueue    MGuiParamQueue    = {};
  MIP_ClapIntQueue    MGuiModQueue      = {};
  float*              MGuiParamVal      = nullptr;
  float*              MGuiParamMod      = nullptr;
  bool                MEditorIsOpen     = true;

//  uint32_t            MResizeWidth      = 0;
//  uint32_t            MResizeHeight     = 0;

  MIP_Widgets        MChangedParmeters = {};

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight) {
    MListener = AListener;
    MPlugin = APlugin;
    MWidth = AWidth;
    MHeight = AHeight;
//    MResizeWidth = AWidth;
//    MResizeHeight = AHeight;
    MNumParams = APlugin->params_count();
    uint32_t size = MNumParams * sizeof(MIP_Widget*);
    MParamToWidget = (MIP_Widget**)malloc(size);
    memset(MParamToWidget,0,size);
    size = MNumParams * sizeof(float);
    MGuiParamVal  = (float*)malloc(size);
    MGuiParamMod  = (float*)malloc(size);
    memset(MGuiParamVal,0,size);
    memset(MGuiParamMod,0,size);
    MTimer = new MIP_Timer(this);

    //createWindow(AWidth,AHeight);
    //MWindow = new MIP_Window(AWidth,AHeight,this,true);
    MWindow = new MIP_EditorWindow(AWidth,AHeight,this);

  }

  //----------

  virtual ~MIP_Editor() {
    delete MTimer;
    free(MParamToWidget);
    free(MGuiParamVal);
    free(MGuiParamMod);

    //destroyWindow();
    if (MWindow) delete MWindow;

  }

//------------------------------
public:
//------------------------------

  //void createWindow() {
  //  MWindow = new MIP_Window(MWidth,MHeight,this,true);
  //}

  //----------

  //void destroyWindow() {
  //  if (MWindow) delete MWindow;
  //  MWindow = nullptr;
  //}

//------------------------------
public: // clap.gui
//------------------------------

  virtual bool attach(const char* ADisplay, uint32_t AWindow) {
    //MIP_PRINT;
    //MWindow = new MIP_Window(256,256,this,true);
    MWindow->reparent(AWindow);
    return true;
  }

  //----------

  virtual void show() {
    //MIP_PRINT;
    MWindow->setOwnerWindow(MWindow);
    //MWindow->alignWidgets();
    MWindow->open();
    //
    MTimer->start(30);
    MEditorIsOpen = true;
  }

  //----------

  virtual void hide() {
    //MIP_PRINT;
    MEditorIsOpen = false;
    MTimer->stop();
    MWindow->close();
  }

  virtual bool setScale(double AScale) {
    //MIP_Print("%.2f\n",AScale);
    MScale = AScale;
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

  virtual void adjustSize(uint32_t *width, uint32_t *height) {
    //MIP_Print("-> %i,%i\n",MResizeWidth,MResizeHeight);
//    *width = MResizeWidth;
//    *height = MResizeHeight;
//    MWidth  = *width;
//    MHeight = *height;
    //*width  = *width;//  & 0xffc0;
    //*height = *height;// & 0xffc0;
  }

  //----------

  virtual bool setSize(uint32_t width, uint32_t height) {
    //MIP_Print("%i,%i\n",width,height);
//    if ((width != MWidth) || (height != MHeight)) {
      MWidth = width;
      MHeight = height;
//      MResizeWidth = width;
//      MResizeHeight = height;
//      MWindow->resizeWindow(MWidth,MHeight);
//      MWindow->paintWindow();
      // blit?
      //MWindow->on_window_resize(width,height);
      MWindow->resizeWindow(width,height);
      MWindow->on_window_paint(0,0,width,height);

//    }
    return true;
  }

//------------------------------
public:
//------------------------------

  MIP_Window* getWindow() {
    return MWindow;
  }

  void setCanResize(bool AResize=true) {
    MCanResize = AResize;
  }

  //----------

  void connect(MIP_Widget* AWidget, int32_t AParamIndex/*, int32_t ASubParamIndex=-1*/) {
    AWidget->setParamIndex(AParamIndex);
    //AWidget->setSubParamIndex(ASubParamIndex);
    MParamToWidget[AParamIndex] = AWidget;
    //AWidget->on_widget_connect(AParamIndex,ASubParamIndex);
    clap_param_info_t info;
    MPlugin->params_get_info(AParamIndex,&info);
    const char* name = info.name;
    float def_value = info.default_value;
    //float min_value = info.min_value;
    //float max_value = info.max_value;
    AWidget->setDefaultValue(def_value);
    AWidget->setValue(def_value);
    AWidget->setParamName(name);
    //AWidget->setParamDefValue(name);
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

  void on_updateWidgetFromWindow(MIP_Widget* AWidget) final {
    int32_t index = AWidget->getParamIndex();
    //MIP_Print("index: %i\n",index);
    if (index >= 0) {
      float value = AWidget->getValue();
      if (MListener) MListener->on_editor_updateParameter(index,value);
    }
  }

  //----------

  void on_resizeFromWindow(uint32_t AWidth, uint32_t AHeight) final {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
//    MResizeWidth = AWidth;
//    MResizeHeight = AHeight;
    if (MListener) MListener->on_editor_resize(AWidth,AHeight);
  }


//------------------------------
private: // timer listener
//------------------------------

  /*

    * avoid drawing widget multiple times
      (param & mod changed)
      - create list (getQueuedGuiParams)
      - add to list : getQueuedGuiMods
      - remove duplicates from list
          just sort the ints queued values = parameter indices,
          duplicates should be easy to spot?
      - paint list

   * paintmode = default, value, mod

  */

  // [timer]

  void on_timerCallback(void) final {
    clearChangedParameters();
    flushGuiParams();
    flushGuiMods();
    redrawChangedParameters();
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

  void updateModulationFromHost(uint32_t AIndex, float AValue) {
    if (MWindow) {
      MGuiParamMod[AIndex] = AValue;
      queueGuiMod(AIndex);
    }
  }

//------------------------------
public:
//------------------------------

  /*
    if both parameter value and modulation is modified, the widget will be
    painted twice..

    TODO: check for duplicates and use latest
          max redraws per frame
  */

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
//          MWindow->paintWidget(widget);
          if (MChangedParmeters.findItem(widget) < 0) {
            MChangedParmeters.append(widget);
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
    if (MEditorIsOpen) {
      uint32_t index = 0;
      while (MGuiModQueue.read(&index)) {
        float value = MGuiParamMod[index];
        MIP_Widget* widget = MParamToWidget[index];
        if (widget) {
          //if (widget->getModValue() != value)
          widget->setModValue(value);
//          MWindow->paintWidget(widget);
          if (MChangedParmeters.findItem(widget) < 0) {
            MChangedParmeters.append(widget);
          }
        }
      }
    }
  }

  //----------

  void clearChangedParameters() {
    MChangedParmeters.clear();
  }

  //----------

  void redrawChangedParameters() {
    for (uint32_t i=0; i<MChangedParmeters.size(); i++) {
      if (MEditorIsOpen && MWindow) {
        MIP_Widget* widget = MChangedParmeters[i];
        MWindow->paintWidget(widget);
      }
    }
  }


};

//----------------------------------------------------------------------
#endif
