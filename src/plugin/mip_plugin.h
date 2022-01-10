#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_host_proxy.h"
#include "plugin/mip_process_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Plugin
: public MIP_EditorListener {

//------------------------------
protected:
//------------------------------

  MIP_Descriptor* MDescriptor = nullptr;
  MIP_HostProxy*  MHost       = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Plugin(MIP_Descriptor* ADescriptor, MIP_HostProxy* AHost) {
    MDescriptor = ADescriptor;
    MHost       = AHost;
  }

  //----------

  virtual ~MIP_Plugin() {
    if (MHost) delete MHost;
  }

//------------------------------
public:
//------------------------------

  MIP_Descriptor* getDescriptor() {
    return MDescriptor;
  }

//------------------------------
public:
//------------------------------

  virtual bool        on_plugin_init() { return true; }
  virtual void        on_plugin_deinit() {}
  virtual bool        on_plugin_activate(float sample_rate, uint32_t min_frames, uint32_t max_frames) { return true; }
  virtual void        on_plugin_deactivate() {}
  virtual bool        on_plugin_start_processing() { return true; }
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
    MIP_Print("index %i value %.3f\n",AIndex,AValue);
  }

};

//----------------------------------------------------------------------
#endif
