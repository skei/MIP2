#ifndef mip_exe_included
#define mip_exe_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap_host_implementation.h"
#include "plugin/exe/mip_exe_host.h"

#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ExeWindow
: public MIP_ImplementedWindow {
//: public MIP_Window {

//------------------------------
private:
//------------------------------

  const clap_plugin_t*      plugin  = nullptr;
  const clap_plugin_gui_t*  gui     = nullptr;

//------------------------------

public:
//------------------------------

  MIP_ExeWindow(uint32_t AWidth, uint32_t AHeight, const clap_plugin_t* APlugin, const clap_plugin_gui_t* AGui)
  : MIP_ImplementedWindow(AWidth,AHeight,false) {
  //: MIP_Window(AWidth,AHeight,false) {
    plugin = APlugin;
    gui = AGui;
  }

  //----------

  virtual ~MIP_ExeWindow() {
  }

//------------------------------
public:
//------------------------------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    gui->set_size(plugin,AWidth,AHeight);
    //MIP_ImplementedWindow::on_window_resize(AWidth,AHeight);
    //MIP_Window::on_window_resize(AWidth,AHeight);
  }

  //void do_widget_modal(MIP_Widget* ASender, uint32_t AMode=0) override {
  //  MIP_Window::do_widget_modal(ASender,AMode);
  //}


};
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main(int argc, char** argv) {
  //MIP_REGISTRY.getNumDescriptors();
  uint32_t index = 0;
  const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);
  if (descriptor) {
    MIP_ExeHostImplementation* exe_host = new MIP_ExeHostImplementation();
    if (exe_host) {
      const clap_host_t* host = exe_host->getHost();
      if (host) {
        MIP_Plugin* plugin = MIP_CreatePlugin(index,descriptor,host);
        const clap_plugin_t* clap_plugin = plugin->getPlugin();
        if (plugin) {
          plugin->init();
          plugin->activate(44100,0,1024);
          plugin->start_processing();

          clap_plugin_gui_t* gui = (clap_plugin_gui_t*)plugin->get_extension(CLAP_EXT_GUI);
          if (gui) {

            if (gui->is_api_supported(clap_plugin,CLAP_WINDOW_API_X11,false)) {
              gui->create(clap_plugin,CLAP_WINDOW_API_X11,false);
              gui->set_scale(clap_plugin,1.0);
              //bool can_resize = gui->can_resize(clap_plugin);
              //if (can_resize) {
              uint32_t width, height;
              gui->get_size(clap_plugin,&width,&height);
              //}
              //else {
              //}
              //gui->set_size(clap_plugin,width,height);

              MIP_ExeWindow* exe_window = new MIP_ExeWindow(width,height,clap_plugin,gui);
              exe_window->openWindow();

              xcb_window_t xcb_window = exe_window->drawable_getXcbWindow();
              clap_window_t clap_window;
              clap_window.api = CLAP_WINDOW_API_X11;
              clap_window.x11 = xcb_window;
              gui->set_parent(clap_plugin,&clap_window);
              gui->show(clap_plugin);

              exe_window->eventLoop();

              gui->hide(clap_plugin);
              gui->destroy(clap_plugin);

              exe_window->closeWindow();
              delete exe_window;
            }
          }
          plugin->stop_processing();
          plugin->deactivate();
          plugin->destroy();
          delete plugin;
        }
      }
      delete exe_host;
    }
  }
  return 0;
}

//----------------------------------------------------------------------
#endif
