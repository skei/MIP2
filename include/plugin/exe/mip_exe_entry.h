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

class MIP_ExeEntry
: public MIP_WindowListener {

//------------------------------
private:
//------------------------------

  //MIP_ExeHost* MHost = nullptr;

  #ifndef MIP_NO_EXE_AUDIO
  MIP_AudioJack MAudio;
  #endif

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
public:
//------------------------------

  int main(int argc, char** argv) {

MIP_PRINT;

    #ifndef MIP_NO_EXE_AUDIO
      //MAudio.config(44100.0,256,3,2,2);
      MAudio.init(this);
      MAudio.start();
    #endif

    uint32_t index = 0;
    const clap_plugin_descriptor_t* desc = MIP_CLAP_REGISTRY.getPlugin(index);
    MIP_ExeHost* host = new MIP_ExeHost(argc,argv);  // TODO: MIP_ExeClapHost : public MIP_ClapHost
    MIP_ClapPlugin* mipplugin = nullptr;
    if (MIP_CreatePlugin) {
      mipplugin = MIP_CreatePlugin(0,desc,host->ptr());
    }
    else {
    }
    const clap_plugin_t* plugin = mipplugin->ptr();
    plugin->init(plugin);


//    plugin->on_plugin_init();
//    plugin->on_plugin_activate(44100,256,256);
//    plugin->on_plugin_startProcessing();


    #ifndef MIP_NO_GUI

    /*
      Showing the GUI works as follow:
       1. clap_plugin_gui->is_api_supported(), check what can work
       2. clap_plugin_gui->create(), allocates gui resources
       3. if the plugin window is floating
       4.    -> clap_plugin_gui->set_transient()
       5.    -> clap_plugin_gui->suggest_title()
       6. else
       7.    -> clap_plugin_gui->set_scale(), if the function pointer is provided by the plugin
       8.    -> clap_plugin_gui->get_size(), gets initial size
       9.    -> clap_plugin_gui->can_resize()
      10. clap_plugin_gui->show()
      11. clap_plugin_gui->hide()/show() ...
      12. clap_plugin_gui->destroy() when done with the gui
    */

    const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)plugin->get_extension(plugin,CLAP_EXT_GUI);
    if (gui && gui->is_api_supported(plugin,CLAP_WINDOW_API_X11,false)) {
      gui->create(plugin,CLAP_WINDOW_API_X11,false);
      gui->set_scale(plugin,1.0);
      uint32_t width = 0.0;
      uint32_t height = 0.0;
      gui->get_size(plugin,&width,&height);
      //if (gui->can_resize(plugin)) {}

      MIP_Window* window = new MIP_Window(width,height,this);
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

//    delete editor;

    #endif

//        editor->attach("",0);
//        uint32_t w = 256;
//        uint32_t h = 256;
//        //editor->getSize(&w,&h);
//        editor->setSize(w,h);
//        editor->setScale(1.0);
////        plugin->on_plugin_openEditor(editor);
//        editor->show();
////        plugin->on_plugin_updateEditor();
//        MIP_Window* window = editor->getWindow();
//        if (window) window->eventLoop();
////        plugin->on_plugin_closeEditor();
//        editor->hide();
//        delete editor;
//      }
//    }

//    plugin->on_plugin_stopProcessing();
//    plugin->on_plugin_deactivate();
//    plugin->on_plugin_deinit();

//    delete exeplugin;
//    plugin->destroy(plugin);

    plugin->destroy(plugin);

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




















