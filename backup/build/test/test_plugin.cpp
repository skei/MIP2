
// nc -U -l -k /tmp/mip.socket
//#define MIP_DEBUG_PRINT_SOCKET

#define MIP_PLUGIN_CLAP
#define MIP_PLUGIN_VST2
#define MIP_PLUGIN_VST3
#define MIP_PLUGIN_LV2
#define MIP_PLUGIN_EXE

#define MIP_USE_XCB
#define MIP_GUI_XCB

//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_plugin_entry.h"

//#include "extern/tcc/libtcc.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public MIP_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {
    MHasEditor    = true;
    MEditorWidth  = 640;
    MEditorHeight = 480;
  }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  MIP_Descriptor* MDescriptor = nullptr;
  MIP_Editor*     MEditor     = nullptr;

  //TCCState* tcc = nullptr;

//------------------------------
public:
//------------------------------

  myPlugin(MIP_Descriptor* ADescriptor)
  : MIP_Plugin(ADescriptor) {
    MDescriptor = ADescriptor;
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public:
//------------------------------

  MIP_Editor* on_plugin_open_editor(void* AParent) final {
    MEditor = new MIP_Editor(this,MDescriptor);
    return MEditor;
  }

  //----------

  void on_plugin_close_editor() final {
    delete MEditor;
    MEditor = nullptr;
  }

  //----------

  void on_plugin_update_editor() final {
    if (MEditor) MEditor->update();
  }


};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins(MIP_PluginList* AList) {
  MIP_PRINT;
  MIP_Descriptor* descriptor = new myDescriptor();
  AList->appendPlugin(descriptor);
}

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, MIP_Descriptor* ADescriptor) {
  MIP_PRINT;
  return new myPlugin(ADescriptor);
}
