#ifndef mip_editor_incuded
#define mip_editor_incuded
//----------------------------------------------------------------------

#define MIP_EDITOR_MAX_PARAMS 1024

#include "base/types/mip_queue.h"
#include "gui/mip_window.h"

typedef MIP_Queue<MIP_Widget*,1024> MIP_WidgetQueue;

//----------------------------------------------------------------------

class MIP_EditorListener {
public:
  virtual void do_editor_updateParameter(uint32_t AIndex, float AValue) {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Editor
: public MIP_WindowListener {

//------------------------------
private:
//------------------------------

  MIP_Window*         MWindow                   = nullptr;
  MIP_EditorListener* MListener                 = nullptr;
  double              MScale                    = 1.0;
  uint32_t            MWidth                    = 400;
  uint32_t            MHeight                   = 400;

  MIP_WidgetQueue     MHostToEditor             = {};

  MIP_Widget* MParameterToWidget[MIP_EDITOR_MAX_PARAMS] = {0};

//------------------------------
public:
//------------------------------

  MIP_Editor() {
  }

  //----------

  virtual ~MIP_Editor() {
    if (MWindow) delete MWindow;
  }

//------------------------------
public:
//------------------------------

  void setListener(MIP_EditorListener* l) { MListener = l; }

  //----------

  MIP_Window* getWindow() { return MWindow; }

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
    MWindow->open();
  }

  //----------

  virtual void hide() {
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

  /*
    deliver this to the plugin
  */

  void do_window_updateWidget(MIP_Widget* AWidget) override {
    int32_t param_index = AWidget->getParameterIndex();
    float   param_value = AWidget->getValue();
    MIP_Print("widget: %s param %i value %.3f\n",AWidget->getName(),param_index,param_value);
    if (param_index >= 0) {
      if (MListener) MListener->do_editor_updateParameter(param_index,param_value);
    }
  }

  //----------

  void do_window_redrawWidget(MIP_Widget* AWidget) override {
    MWindow->paintWidget(AWidget);
  }

//------------------------------
public: // instance
//------------------------------

  /*
    audio thread
  */

  void updateParameter(uint32_t AIndex, float AValue) {
    MIP_Widget* widget = MParameterToWidget[AIndex];
    MIP_Print("index %i value %.3f widget %p\n",AIndex,AValue,widget);
    if (widget) {
      //MHostToEditor.write(widget);
    }
  }

};

//----------------------------------------------------------------------
#endif
