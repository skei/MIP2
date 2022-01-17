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

class MIP_Plugin {

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
  virtual bool        on_plugin_startProcessing() { return true; }
  virtual void        on_plugin_stopProcessing() {}

  virtual void        on_plugin_process(MIP_ProcessContext* AContext) {}

  virtual void        on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue) {}
  virtual void        on_plugin_modulation(uint32_t AOffset, uint32_t AIndex, float AValue) {}
  virtual void        on_plugin_midi(uint32_t AOffset, uint8_t AMsg1,uint8_t AMsg2,uint8_t AMsg3) {}
  virtual void        on_plugin_noteExpression(uint32_t AOffset, uint32_t AType, uint32_t ANote, uint32_t AChannel, float AValue) {}

  virtual void        on_plugin_openEditor(MIP_Editor* AEditor) {}
  virtual void        on_plugin_closeEditor() {}
  virtual void        on_plugin_updateEditor() {}

};

//----------------------------------------------------------------------
#endif
