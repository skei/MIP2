#ifndef mip_exe_plugin_entry_included
#define mip_exe_plugin_entry_included
//----------------------------------------------------------------------

#define MIP_NO_EXE_AUDIO

//----------

#include "mip.h"

#ifndef MIP_NO_EXE_AUDIO
#include "audio/mip_audio_jack.h"
#endif

#include "plugin/mip_descriptor.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "plugin/exe/mip_exe.h"
#include "plugin/exe/mip_exe_host_proxy.h"

#ifndef MIP_NO_GUI
  #include "plugin/mip_editor.h"
#endif


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ExePluginEntry
#ifndef MIP_NO_EXE_AUDIO
: public MIP_AudioIOListener
, public MIP_EditorListener {
#else
: public MIP_EditorListener {
#endif

//------------------------------
private:
//------------------------------

  #ifndef MIP_NO_EXE_AUDIO
  MIP_AudioJack MAudio;
  #endif

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

    #ifndef MIP_NO_EXE_AUDIO
      //MAudio.config(44100.0,256,3,2,2);
      MAudio.init(this);
      MAudio.start();
    #endif


    MIP_Descriptor* descriptor = MIP_GLOBAL_PLUGIN_LIST.getPluginDescriptor(0);
    MIP_ExeHostProxy* hostproxy = new MIP_ExeHostProxy();
    MIP_Plugin* plugin = MIP_GLOBAL_PLUGIN_LIST.createPlugin(0,hostproxy); // deleted in MIP_Vst2Plugin destructor (???)

    plugin->on_plugin_init();
    plugin->on_plugin_activate(44100,256,256);
    plugin->on_plugin_startProcessing();

    #ifndef MIP_NO_GUI
    if (descriptor->hasEditor()) {
      MIP_Editor* editor = new MIP_Editor(this,descriptor);
      if (editor) {
        editor->attach("",0);
        uint32_t width = descriptor->getEditorWidth();
        uint32_t height = descriptor->getEditorHeight();
        editor->setSize(width,height);
        editor->setScale(1.0);
        plugin->on_plugin_openEditor(editor);
        editor->show();
        plugin->on_plugin_updateEditor();
        MIP_Window* window = editor->getWindow();
        if (window) window->eventLoop();
        plugin->on_plugin_closeEditor();
        editor->hide();
        delete editor;
      }
    }
    #endif

    plugin->on_plugin_stopProcessing();
    plugin->on_plugin_deactivate();
    plugin->on_plugin_deinit();

    delete plugin;
    delete descriptor;

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

MIP_ExePluginEntry GLOBAL_EXE_PLUGIN_ENTRY;

//----------

int main(int argc, char** argv) {
  return GLOBAL_EXE_PLUGIN_ENTRY.main(argc,argv);
}

//----------------------------------------------------------------------
#endif
