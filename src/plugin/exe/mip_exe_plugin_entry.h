#ifndef mip_exe_plugin_entry_included
#define mip_exe_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"

#include "audio/mip_audio_jack.h"

#include "plugin/mip_descriptor.h"
#include "plugin/mip_plugin.h"
#include "plugin/exe/mip_exe.h"
#include "plugin/exe/mip_exe_host_proxy.h"

//#ifndef MIP_NO_GUI
#include "plugin/mip_editor.h"
//#endif


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ExePluginEntry
: public MIP_AudioIOListener {

//------------------------------
private:
//------------------------------

  MIP_AudioJack MAudio;

//------------------------------
public:
//------------------------------

  MIP_ExePluginEntry() {
    //MIP_RegisterPlugins(&MIP_GLOBAL_PLUGIN_LIST);
  }

  //----------

  virtual ~MIP_ExePluginEntry() {
    //MIP_UnregisterPlugins();
  }

//------------------------------
public:
//------------------------------

  int main(int argc, char** argv) {

    //MAudio.config(44100.0,256,3,2,2);
    MAudio.init(this);
    MAudio.start();


    MIP_Descriptor* descriptor = MIP_GLOBAL_PLUGIN_LIST.getPluginDescriptor(0);
    MIP_ExeHostProxy* hostproxy = new MIP_ExeHostProxy();
    MIP_Plugin* plugin = MIP_GLOBAL_PLUGIN_LIST.createPlugin(0,hostproxy); // deleted in MIP_Vst2Plugin destructor (???)

    plugin->on_plugin_init();
    plugin->on_plugin_activate(44100,256,256);
    plugin->on_plugin_start_processing();

    if (descriptor->hasEditor()) {
//      MIP_Editor* editor = plugin->on_plugin_open_editor(nullptr);
//      if (editor) {
//        uint32_t width = descriptor->getEditorWidth();
//        uint32_t height = descriptor->getEditorHeight();
//        editor->attach(nullptr,0);
//        editor->setSize(width,height);
//        editor->show();
//        plugin->on_plugin_update_editor();
//        MIP_Window* window = editor->getWindow();
//        if (window) window->eventLoop();
//        editor->hide();
//        //editor->detach();
//        plugin->on_plugin_close_editor();
//      }
    }

    plugin->on_plugin_stop_processing();
    plugin->on_plugin_deactivate();
    plugin->on_plugin_deinit();

    delete plugin;
    delete descriptor;

    MAudio.stop();
    MAudio.exit();

    return 0;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_ExePluginEntry GLOBAL_EXE_PLUGIN_ENTRY;

//----------

int main(int argc, char** argv) {
  return GLOBAL_EXE_PLUGIN_ENTRY.main(argc,argv);
}

//----------------------------------------------------------------------
#endif
