#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "plugin/mip_plugin_entry.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_process_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Plugin
: public MIP_EditorListener {

//------------------------------
public:
//------------------------------

  MIP_Plugin(MIP_Descriptor* ADescriptor) {
  }

  //----------

  virtual ~MIP_Plugin() {
  }

//------------------------------
public:
//------------------------------

  virtual void        on_plugin_init() {}
  virtual void        on_plugin_deinit() {}
  virtual void        on_plugin_activate() {}
  virtual void        on_plugin_deactivate() {}
  virtual void        on_plugin_start_processing() {}
  virtual void        on_plugin_stop_processing() {}

  virtual void        on_plugin_process(MIP_ProcessContext* AContext) {}

  virtual void        on_plugin_parameter(uint32_t AIndex, float AValue) {}
  virtual void        on_plugin_modulation(uint32_t AIndex, float AValue) {}
  virtual void        on_plugin_midi(uint8_t AMsg1,uint8_t AMsg2,uint8_t AMsg3) {}
  virtual void        on_plugin_note_expression(uint32_t AType, uint32_t ANote, uint32_t AChannel, float AValue) {}

  virtual MIP_Editor* on_plugin_open_editor(void* AParent) { return nullptr; }
  virtual void        on_plugin_close_editor() {}
  virtual void        on_plugin_update_editor() {}

//------------------------------
public: // editor listener
//------------------------------

  void do_editor_updateParameter(uint32_t AIndex, float AValue) override {
  }

};

//----------------------------------------------------------------------
#endif
