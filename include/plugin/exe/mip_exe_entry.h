#ifndef mip_exe_entry_included
#define mip_exe_entry_included
//----------------------------------------------------------------------

#define MIP_NO_EXE_AUDIO

//----------

#include "mip.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_plugin.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_host.h"
#include "plugin/clap/mip_clap_registry.h"
//#include "plugin/clap/mip_clap_plugin_proxy.h"
#include "plugin/exe/mip_exe.h"
#include "plugin/exe/mip_exe_plugin.h"
#include "plugin/exe/mip_exe_host.h"

#ifndef MIP_NO_EXE_AUDIO
#include "audio/mip_audio_jack.h"
#endif

//#ifndef MIP_NO_GUI
//  #include "plugin/mip_editor.h"
//#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ExeWindow
: public MIP_Window {

private:

  const clap_plugin_t*      plugin  = nullptr;
  const clap_plugin_gui_t*  gui     = nullptr;

public:

  MIP_ExeWindow(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener, bool AEmbedded)
  : MIP_Window(AWidth,AHeight,AListener,AEmbedded) {
  }

  virtual ~MIP_ExeWindow() {
  }

  void setup (const clap_plugin_t* APlugin, const clap_plugin_gui_t* AGui) {
    plugin = APlugin;
    gui = AGui;
  }

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    MIP_Window::on_window_resize(AWidth,AHeight);
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

  //MIP_ExeHost* MHost = nullptr;

  #ifndef MIP_NO_EXE_AUDIO
  MIP_AudioJack MAudio;
  #endif

//  const clap_plugin_descriptor_t* descriptor
//   const clap_plugin_t* plugin = mipplugin->ptr();
//  MIP_ExeHost* host
//  MIP_ClapPlugin* mipplugin

  const clap_plugin_t*      plugin  = nullptr;
  const clap_plugin_gui_t*  gui     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ExeEntry() {
    MIP_PRINT;
  }

  //----------

  virtual ~MIP_ExeEntry() {
  }

//------------------------------
public: // window listener
//------------------------------

  //void on_updateWidgetFromWindow(MIP_Widget* AWidget) override {
  //  MIP_PRINT;
  //}

  //void on_resizeFromWindow(uint32_t AWidth, uint32_t AHeight) override {
  //  MIP_PRINT;
  //}

//------------------------------
public:
//------------------------------

  int main(int argc, char** argv) {

    #ifndef MIP_NO_EXE_AUDIO
      //MAudio.config(44100.0,256,3,2,2);
      MAudio.init(this);
      MAudio.start();
    #endif

    uint32_t index = 0;
    const clap_plugin_descriptor_t* descriptor = MIP_CLAP_REGISTRY.getPlugin(index);
    MIP_ExeHost* host = new MIP_ExeHost(argc,argv);
    if (host) {
      MIP_ClapPlugin* mipplugin = nullptr;
      if (MIP_CreatePlugin) {
        mipplugin = MIP_CreatePlugin(0,descriptor,host->ptr());
        plugin = mipplugin->ptr();
        plugin->init(plugin);
        plugin->activate(plugin,44100,128,128);
        //plugin->start_processing(plugin);

        #ifndef MIP_NO_GUI

        //const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)plugin->get_extension(plugin,CLAP_EXT_GUI);
        gui = (const clap_plugin_gui_t*)plugin->get_extension(plugin,CLAP_EXT_GUI);

        //if (gui && gui->is_api_supported(plugin,CLAP_WINDOW_API_X11,false)) {
        //  gui->create(plugin,CLAP_WINDOW_API_X11,false);

        if (gui && gui->is_api_supported(plugin,CLAP_WINDOW_API_X11,false)) {
          gui->create(plugin,CLAP_WINDOW_API_X11,false);

          gui->set_scale(plugin,1.0);
          uint32_t width = 0.0;
          uint32_t height = 0.0;
          gui->get_size(plugin,&width,&height);
          //if (gui->can_resize(plugin)) {}

          MIP_ExeWindow* window = new MIP_ExeWindow(width,height,this,false);
          window->setup(plugin,gui);

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

        //plugin->stop_processing(plugin);
        plugin->deactivate(plugin);
        plugin->destroy(plugin);
      }
    }
    delete host;

    #ifndef MIP_NO_EXE_AUDIO
      MAudio.stop();
      MAudio.exit();
    #endif

    return 0;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_ExeEntry GLOBAL_EXE_ENTRY;

//----------

int main(int argc, char** argv) {
  return GLOBAL_EXE_ENTRY.main(argc,argv);
}

//----------------------------------------------------------------------
#endif




















