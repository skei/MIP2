#ifndef mip_editor_incuded
#define mip_editor_incuded
//----------------------------------------------------------------------

#include "base/types/mip_queue.h"
#include "plugin/mip_descriptor.h"

#ifndef MIP_NO_GUI
  #include "gui/mip_window.h"
  typedef MIP_Array<MIP_Widget*>      MIP_WidgetArray;
  typedef MIP_Queue<MIP_Widget*,1024> MIP_WidgetQueue;
#endif

//----------------------------------------------------------------------
//
// editor listener
//
//----------------------------------------------------------------------

class MIP_EditorListener {
public:
  virtual void updateParameterFromEditor(uint32_t AIndex, float AValue) {}
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

#ifndef MIP_NO_GUI

class MIP_Editor
: public MIP_WindowListener {

#else

class MIP_Editor {

#endif

//------------------------------
private:
//------------------------------

  MIP_Descriptor*     MDescriptor         = nullptr;
  MIP_EditorListener* MListener           = nullptr;
  float*              MEditorParamValues  = nullptr;
  float*              MEditorModValues    = nullptr;

  #ifndef MIP_NO_GUI
  MIP_WidgetQueue     MEditorParamQueue   = {};
  MIP_WidgetQueue     MEditorModQueue     = {};
  MIP_WidgetArray     MParameterToWidget  = {};
  #endif

//------------------------------
protected:
//------------------------------

  bool                MIsEditorOpen       = false;
  uint32_t            MWidth              = 400;
  uint32_t            MHeight             = 400;
  double              MScale              = 1.0;

  int32_t             MPreviousWidth      = -1;
  int32_t             MPreviousHeight     = -1;

  #ifndef MIP_NO_GUI
  MIP_Window*         MWindow             = nullptr;
  #endif

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, MIP_Descriptor* ADescriptor) {
    MDescriptor = ADescriptor;
    MListener = AListener;
    MWidth = ADescriptor->getEditorWidth();
    MHeight = ADescriptor->getEditorHeight();
    uint32_t num_params = ADescriptor->getNumParameters();
    MEditorParamValues = (float*)malloc(num_params * sizeof(float));
    MEditorModValues = (float*)malloc(num_params * sizeof(float));
  }

  //----------

  virtual ~MIP_Editor() {
    #ifndef MIP_NO_GUI
    if (MWindow) delete MWindow;
    #endif
    free(MEditorParamValues);
    free(MEditorModValues);
  }

//------------------------------
public:
//------------------------------

  //void setListener(MIP_EditorListener* l) {
  //  MListener = l;
  //}

  //----------

  #ifndef MIP_NO_GUI

  MIP_Window* getWindow() {
    return MWindow;
  }

  //

  void connectParameter(MIP_Widget* AWidget, uint32_t AParamIndex) {
    if (AParamIndex >= MParameterToWidget.size()) {
      MParameterToWidget.resize(AParamIndex+1);
    }
    MParameterToWidget[AParamIndex] = AWidget;
    MIP_Parameter* param = MDescriptor->getParameter(AParamIndex);
    //AWidget->setParameterIndex(AParamIndex);
    AWidget->setParameter(param);
    AWidget->on_widget_connect(param);
  }

  //

  void update() {
  }

  #endif

//------------------------------
public:
//------------------------------

  // audio thread

  void queueEditorParam(int32_t AIndex, float AValue) {
    #ifndef MIP_NO_GUI
    MIP_Widget* widget = MParameterToWidget[AIndex];
    if (widget) {
      MEditorParamValues[AIndex] = AValue;
      MEditorParamQueue.write(widget);
    }
    #endif
  }

  void queueEditorMod(int32_t AIndex, float AValue) {
    #ifndef MIP_NO_GUI
    MIP_Widget* widget = MParameterToWidget[AIndex];
    if (widget) {
      MEditorModValues[AIndex] = AValue;
      MEditorModQueue.write(widget);
    }
    #endif
  }


  //----------

  // timer

  void flushEditorParams() {
    #ifndef MIP_NO_GUI
    if (MWindow && MIsEditorOpen) {
      MIP_Widget* widget;
      while (MEditorParamQueue.read(&widget)) {
        MIP_Parameter* param = widget->getParameter();
        int32_t index = param->getIndex();
        if (index >= 0) {
          float value = MEditorParamValues[index];
          widget->setValue(value);
          MIP_FRect rect = widget->getRect();
          MWindow->invalidate(rect.x,rect.y,rect.w,rect.h);
        }
      }
    }
    #endif
  }

  //

  void flushEditorMods() {
    #ifndef MIP_NO_GUI
    if (MWindow && MIsEditorOpen) {
      MIP_Widget* widget;
      while (MEditorModQueue.read(&widget)) {
        MIP_Parameter* param = widget->getParameter();
        int32_t index = param->getIndex();
        if (index >= 0) {
          float value = MEditorModValues[index];
          widget->setModValue(value);
          MIP_FRect rect = widget->getRect();
          MWindow->invalidate(rect.x,rect.y,rect.w,rect.h);
        }
      }
    }
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual bool attach(const char* display_name, unsigned long window) {
    #ifndef MIP_NO_GUI
    MWindow = new MIP_Window(MWidth,MHeight,"Title",this,(void*)window);
    if (MWindow) {
      MWindow->setFillWindowBackground();
      return true;
    }
    #endif
    return false;
  }

  //----------

  virtual void show() {
    MIsEditorOpen = true;
    #ifndef MIP_NO_GUI
    MWindow->open();
    #endif
  }

  //----------

  virtual void hide() {
    MIsEditorOpen = false;
    #ifndef MIP_NO_GUI
    //MPreviousWidth =
    //MPreviousHeight =
    MWindow->close();
    #endif
  }

  //----------

  virtual void setWidth(uint32_t w) {
    MWidth = w;
    #ifndef MIP_NO_GUI
    MWindow->setWindowSize(MWidth,MHeight);
    #endif
  }

  //----------

  virtual void setHeight(uint32_t h) {
    MHeight = h;
    #ifndef MIP_NO_GUI
    MWindow->setWindowSize(MWidth,MHeight);
    #endif
  }

  //----------

  virtual void setSize(uint32_t w, uint32_t h) {
    MWidth = w;
    MHeight = h;
    #ifndef MIP_NO_GUI
    MWindow->setWindowSize(MWidth,MHeight);
    #endif
  }

  //----------

  virtual void setScale(double scale) {
  }

  //----------

  virtual uint32_t getWidth() {
    return MWidth;
  }

  //----------

  virtual uint32_t getHeight() {
    return MHeight;
  }

//------------------------------
public: // window listener
//------------------------------

  // called from:
  //   MIP_Window.do_widget_update()
  // MListener =

  #ifndef MIP_NO_GUI

  void updateWidgetFromWindow(MIP_Widget* AWidget) override {
    MIP_Parameter* parameter = AWidget->getParameter();
    if (parameter) {
      int32_t index = parameter->getIndex();
      if (index >= 0) {
        float value = AWidget->getValue();
        if (MListener) MListener->updateParameterFromEditor(index,value); // MIP_ClapPlugin
      }
    }
  }

  #endif

};

//----------------------------------------------------------------------
#endif
