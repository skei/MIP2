#ifndef mip_exe_plugin_entry_included
#define mip_exe_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "plugin/exe/mip_exe.h"

//----------------------------------------------------------------------

template <class DESC, class PLUG>
class MIP_ExePluginEntry {

//------------------------------
public:
//------------------------------

  MIP_ExePluginEntry() {
    //MIP_PRINT;
    MIP_Descriptor* descriptor = new DESC();
    MIP_Plugin* plugin = new PLUG(descriptor);
    plugin->on_plugin_init();
    plugin->on_plugin_activate();
    plugin->on_plugin_start_processing();

    if (descriptor->hasEditor()) {
      MIP_Editor* editor = plugin->on_plugin_open_editor(nullptr);
      if (editor) {
        uint32_t width = descriptor->getEditorWidth();
        uint32_t height = descriptor->getEditorHeight();
        MIP_Print("w %i h %i\n",width,height);
        editor->attach(nullptr,0);
        editor->setSize(width,height);
        editor->show();
        plugin->on_plugin_update_editor();
        MIP_Window* window = editor->getWindow();
        if (window) window->eventLoop();
        editor->hide();
        //editor->detach();
        plugin->on_plugin_close_editor();
      }
    }

    plugin->on_plugin_stop_processing();
    plugin->on_plugin_deactivate();
    plugin->on_plugin_deinit();

    delete plugin;
    delete descriptor;
  }

  //----------

  virtual ~MIP_ExePluginEntry() {
  }

//------------------------------
public:
//------------------------------

  int main(int argc, char** argv) {
    //MIP_PRINT;
    return 0;
  }

};

//----------------------------------------------------------------------

#define MIP_EXE_PLUGIN_ENTRY(D,P)                   \
                                                    \
  MIP_ExePluginEntry<D,P> GLOBAL_EXE_PLUGIN_ENTRY;  \
                                                    \
  int main(int argc, char** argv) {                 \
    return GLOBAL_EXE_PLUGIN_ENTRY.main(argc,argv); \
  }                                                 \


//----------------------------------------------------------------------
#endif
