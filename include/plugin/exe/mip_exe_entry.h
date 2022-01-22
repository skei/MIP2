#ifndef mip_exe_entry_included
#define mip_exe_entry_included
//----------------------------------------------------------------------

#define MIP_NO_EXE_AUDIO

//----------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_host.h"
#include "plugin/exe/mip_exe.h"
#include "plugin/exe/mip_exe_plugin.h"

#ifndef MIP_NO_EXE_AUDIO
#include "audio/mip_audio_jack.h"
#endif

#ifndef MIP_NO_GUI
  #include "plugin/mip_editor.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ExeEntry {

//------------------------------
private:
//------------------------------

  MIP_ClapHost  MHost = {};

  #ifndef MIP_NO_EXE_AUDIO
  MIP_AudioJack MAudio;
  #endif

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

  int main(int argc, char** argv) {

    #ifndef MIP_NO_EXE_AUDIO
      //MAudio.config(44100.0,256,3,2,2);
      MAudio.init(this);
      MAudio.start();
    #endif

    const clap_plugin_descriptor_t* descriptor  = MIP_GetDescriptor(0);
    const clap_plugin_t*            plugin      = MIP_CreatePlugin(MHost.getHost(),descriptor->id); // deleted in MIP_ExePlugin destructor
    MIP_ExePlugin*                  exeplugin   = new MIP_ExePlugin(plugin);

//    plugin->on_plugin_init();
//    plugin->on_plugin_activate(44100,256,256);
//    plugin->on_plugin_startProcessing();

//    #ifndef MIP_NO_GUI
//    if (descriptor->hasEditor()) {
//      MIP_Editor* editor = new MIP_Editor(this,descriptor);
//      if (editor) {
//        editor->attach("",0);
//        uint32_t width = descriptor->getEditorWidth();
//        uint32_t height = descriptor->getEditorHeight();
//        editor->setSize(width,height);
//        editor->setScale(1.0);
//        plugin->on_plugin_openEditor(editor);
//        editor->show();
//        plugin->on_plugin_updateEditor();
//        MIP_Window* window = editor->getWindow();
//        if (window) window->eventLoop();
//        plugin->on_plugin_closeEditor();
//        editor->hide();
//        delete editor;
//      }
//    }
//    #endif

//    plugin->on_plugin_stopProcessing();
//    plugin->on_plugin_deactivate();
//    plugin->on_plugin_deinit();

    delete exeplugin;
    plugin->destroy(plugin);

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




















