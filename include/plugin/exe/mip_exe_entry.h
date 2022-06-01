#ifndef mip_exe_entry_included
#define mip_exe_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_registry.h"
#include "plugin/exe/mip_exe.h"
#include "plugin/exe/mip_exe_host.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ExeWindow
: public MIP_BasicWindow {

//------------------------------
private:
//------------------------------

  const clap_plugin_t*      plugin  = nullptr;
  const clap_plugin_gui_t*  gui     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ExeWindow(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener, const clap_plugin_t* APlugin, const clap_plugin_gui_t* AGui)
  : MIP_BasicWindow(AWidth,AHeight,false) {
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
    //MIP_Window::on_window_resize(AWidth,AHeight);
    gui->set_size(plugin,AWidth,AHeight);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ExeEntry
: public MIP_WindowListener {

//------------------------------
private:
//------------------------------

  const clap_plugin_t*      plugin  = nullptr;
  const clap_plugin_gui_t*  gui     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ExeEntry() {
  }

  //----------

  virtual ~MIP_ExeEntry() {
  }

//------------------------------
public:
//------------------------------

  void open_editor() {
    #ifndef MIP_NO_GUI
      gui = (const clap_plugin_gui_t*)plugin->get_extension(plugin,CLAP_EXT_GUI);
      if (gui && gui->is_api_supported(plugin,CLAP_WINDOW_API_X11,false)) {
        gui->create(plugin,CLAP_WINDOW_API_X11,false);
        gui->set_scale(plugin,1.0);
        uint32_t width = 0.0;
        uint32_t height = 0.0;
        gui->get_size(plugin,&width,&height);
        MIP_ExeWindow* window = new MIP_ExeWindow(width,height,this,plugin,gui);
        //window->setup(plugin,gui);
        window->open();
        clap_window_t clap_window = {};
        clap_window.api = CLAP_WINDOW_API_X11;
        clap_window.x11 = window->getXcbWindow();
        gui->set_parent( plugin, &clap_window );
        gui->show(plugin);
        window->eventLoop();
        window->close();
        delete window;
        //gui->hide(plugin);
        gui->destroy(plugin);
      }
    #endif
  }

  //----------

  // todo. index from command line

  int main(int argc, char** argv) {
    uint32_t index = 0;
    const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);
    MIP_ExeHost* host = new MIP_ExeHost(argc,argv);
    if (host) {
      MIP_ClapPlugin* mipplugin = nullptr;
      if (MIP_CreatePlugin) {
        mipplugin = MIP_CreatePlugin(0,descriptor,host->getHost());
        plugin = mipplugin->getPlugin();
        plugin->init(plugin);
        plugin->activate(plugin,44100,256,256);
        //plugin->start_processing(plugin);

        open_editor();

        //plugin->stop_processing(plugin);
        plugin->deactivate(plugin);
        plugin->destroy(plugin);
      }
    }
    delete host;
    return 0;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_ExeEntry MIP_EXE_ENTRY;

//----------

int main(int argc, char** argv) {
  return MIP_EXE_ENTRY.main(argc,argv);
}

//----------------------------------------------------------------------
#endif
