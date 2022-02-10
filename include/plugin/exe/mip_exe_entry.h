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
#include "plugin/clap/mip_clap_list.h"
//#include "plugin/clap/mip_clap_plugin_proxy.h"
#include "plugin/wrapper/exe/mip_exe.h"
#include "plugin/wrapper/exe/mip_exe_plugin.h"

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
    MIP_PRINT;
  }

  //----------

  virtual ~MIP_ExeEntry() {
  }

//------------------------------
public:
//------------------------------

  int main(int argc, char** argv) {

    #if 0


    #ifndef MIP_NO_EXE_AUDIO
      //MAudio.config(44100.0,256,3,2,2);
      MAudio.init(this);
      MAudio.start();
    #endif


//    const clap_plugin_descriptor_t* descriptor  = MIP_GetDescriptor(0);
//    const clap_plugin_t*            plugin      = MIP_CreatePlugin(MHost.ptr(),descriptor->id); // deleted in MIP_ExePlugin destructor
//    MIP_ExePlugin*                  exeplugin   = new MIP_ExePlugin(plugin);




    uint32_t index = 0;
    const clap_plugin_descriptor_t* desc = MIP_GLOBAL_CLAP_LIST.getPlugin(index);
    MIP_ClapHost* host = new MIP_ClapHost();  // TODO: MIP_ExeClapHost : public MIP_ClapHost
    MIP_ClapPlugin* mipplugin = MIP_CreatePlugin(0,desc,host->ptr());

//    MIP_GLOBAL_CLAP_LIST.appendInstance(mipplugin);

    const clap_plugin_t* plugin = mipplugin->ptr();
    plugin->init(plugin);



    const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)plugin->get_extension(plugin,CLAP_EXT_GUI);
    const clap_plugin_gui_x11_t* gui_x11 = (const clap_plugin_gui_x11_t*)plugin->get_extension(plugin,CLAP_EXT_GUI_X11);

//    plugin->on_plugin_init();
//    plugin->on_plugin_activate(44100,256,256);
//    plugin->on_plugin_startProcessing();


    #ifndef MIP_NO_GUI

    gui->create(plugin);
    gui_x11->attach(plugin,"",0);
    gui->show(plugin);

//    MIP_Editor* editor = plugin->getEditor();
//    MIP_Window* window = editor->getWindow();
//    if (window) {
//      window->paint();
//      window->eventLoop();
//    }

    gui->hide(plugin);
    gui->destroy(plugin);

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

    #endif // 0

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




















