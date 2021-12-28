#ifndef mip_editor_incuded
#define mip_editor_incuded
//----------------------------------------------------------------------

#include "base/types/mip_queue.h"
#include "gui/mip_window.h"

typedef MIP_Array<MIP_Widget*>      MIP_WidgetArray;
typedef MIP_Queue<MIP_Widget*,1024> MIP_WidgetQueue;

//----------------------------------------------------------------------
//
// editor listener
//
//----------------------------------------------------------------------

class MIP_EditorListener {
public:
  virtual void do_editor_updateParameter(uint32_t AIndex, float AValue) {}
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class MIP_Editor
: public MIP_WindowListener {

//------------------------------
private:
//------------------------------

  MIP_EditorListener* MListener           = nullptr;
  float*              MEditorParamValues  = nullptr;
  MIP_WidgetQueue     MEditorParamQueue   = {};
  MIP_WidgetArray     MParameterToWidget  = {};

//------------------------------
protected:
//------------------------------

  bool                MIsEditorOpen       = false;
  MIP_Window*         MWindow             = nullptr;
  uint32_t            MWidth              = 400;
  uint32_t            MHeight             = 400;
  double              MScale              = 1.0;

//------------------------------
public:
//------------------------------

  MIP_Editor(uint32_t num_params) {
    MEditorParamValues = (float*)malloc(num_params * sizeof(float));
  }

  //----------

  virtual ~MIP_Editor() {
    free(MEditorParamValues);
    if (MWindow) delete MWindow;
  }

//------------------------------
public:
//------------------------------

  void setListener(MIP_EditorListener* l) {
    MListener = l;
  }

  //----------

  MIP_Window* getWindow() {
    return MWindow;
  }

  //

  void connectParameter(MIP_Widget* AWidget, uint32_t AParamIndex) {
    if (AParamIndex >= MParameterToWidget.size()) {
      MParameterToWidget.resize(AParamIndex+1);
    }
    MParameterToWidget[AParamIndex] = AWidget;
    AWidget->setParameterIndex(AParamIndex);
    AWidget->on_widget_connect(AParamIndex);
  }

  //

//------------------------------
public:
//------------------------------

  // audio thread

  void queueEditorParam(int32_t AIndex, float AValue) {
    MIP_Widget* widget = MParameterToWidget[AIndex];
    //MIP_Print("index %i value %.3f widget %p\n",AIndex,AValue,widget);
    if (widget) {
      MEditorParamValues[AIndex] = AValue;
      MEditorParamQueue.write(widget);
    }
  }

  //----------

  // timer

  void flushEditorParams() {
    if (MWindow && MIsEditorOpen) {
      MIP_Widget* widget;
      while (MEditorParamQueue.read(&widget)) {
        int32_t index = widget->getParameterIndex();
        if (index >= 0) {
          float value = MEditorParamValues[index];
          widget->setValue(value);
          MWindow->paintWidget(widget);
        }
      }
    }
  }

//------------------------------
public:
//------------------------------

  virtual bool attach(const char* display_name, unsigned long window) {
    MWindow = new MIP_Window(MWidth,MHeight,"Title",(void*)window);
    if (MWindow) {
      MWindow->setListener(this);
      return true;
    }
    return false;
  }

  //----------

  virtual void show() {
    MIsEditorOpen = true;
    MWindow->open();
  }

  //----------

  virtual void hide() {
    MIsEditorOpen = false;
    MWindow->close();
  }

  //----------

  virtual void setWidth(uint32_t w) {
    MWidth = w;
    MWindow->setSize(MWidth,MHeight);
  }

  //----------

  virtual void setHeight(uint32_t h) {
    MHeight = h;
    MWindow->setSize(MWidth,MHeight);
  }

  //----------

  virtual void setSize(uint32_t w, uint32_t h) {
    MWidth = w;
    MHeight = h;
    MWindow->setSize(MWidth,MHeight);
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

  void do_window_updateWidget(MIP_Widget* AWidget) override {
    int32_t param_index = AWidget->getParameterIndex();
    float   param_value = AWidget->getValue();
    if (param_index >= 0) {
      if (MListener) MListener->do_editor_updateParameter(param_index,param_value);
    }
  }

  //----------

  void do_window_redrawWidget(MIP_Widget* AWidget) override {
    MWindow->paintWidget(AWidget);
  }

};

//----------------------------------------------------------------------
#endif
