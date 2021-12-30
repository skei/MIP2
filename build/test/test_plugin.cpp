

// nc -U -l -k /tmp/mip.socket
//#define MIP_DEBUG_PRINT_SOCKET

#define MIP_PLUGIN_EXE
#define MIP_USE_XCB
#define MIP_GUI_XCB

#include "mip.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_plugin_entry.h"

//----------------------------------------------------------------------
//
//
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
//
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  MIP_Descriptor* MDescriptor = nullptr;
  MIP_Editor*     MEditor     = nullptr;

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
    MIP_Assert(MEditor);
    delete MEditor;
    MEditor = nullptr;
  }

  //----------

  void on_plugin_update_editor() final {
    //MIP_Assert(MEditor);
    if (MEditor) MEditor->update();
  }


};

//----------------------------------------------------------------------

MIP_PLUGIN_ENTRY(myDescriptor,myPlugin);
