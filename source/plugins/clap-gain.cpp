
// TODO: does this still work? redo?

#define MIP_DEBUG_PRINT_SOCKET
// in console: nc -U -l -k /tmp/mip.socket
//#define MIP_NO_GUI

//----------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class Gain_Plugin
: public MIP_ClapPlugin {

//------------------------------
private:
//------------------------------

  double MGain = 0.0;

//------------------------------
public:
//------------------------------

  Gain_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
  }

//------------------------------
public:
//------------------------------

  clap_process_status process(const clap_process_t *process) final {
    handle_input_events(process->in_events);
    process_audio(process);
    handle_output_events(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

//------------------------------
public:
//------------------------------

  uint32_t params_count() final {
    return 1;
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) final {
    switch (param_index) {
      case 0:
        param_info->id              = 0;
        param_info->flags           = 0;
        param_info->cookie          = nullptr;
        param_info->min_value       = 0.0;
        param_info->max_value       = 1.0;
        param_info->default_value   = 0.0;
        strncpy(param_info->name,   "Gain", CLAP_NAME_SIZE-1);
        strncpy(param_info->module, "", CLAP_MODULE_SIZE-1);
        return true;
    }
    return false;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) final {
    switch (param_id) {
      case 0:
        *value = MGain;
        return true;
    }
    return false;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) final {
    switch (param_id) {
      case 0:
        sprintf(display,"%.3f",value);
        return true;
    }
    return false;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) final {
    switch (param_id) {
      case 0:
        float f = atof(display);
        *value = f;
        return true;
    }
    return false;
  }

  //----------

  void params_flush(const clap_input_events_t* in, const clap_output_events_t* out) final {
    handle_input_events(in);
    handle_output_events(out);
  }

//------------------------------
private:
//------------------------------

  void handle_input_events(const clap_input_events_t* in_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      switch (header->type) {
        case CLAP_EVENT_PARAM_VALUE:
          handle_param_value(header);
          break;
      }
    }
  }

  //----------

  void handle_output_events(const clap_output_events_t* out_events) {
  }

  //----------

  void handle_param_value(const clap_event_header_t* header) {
    const clap_event_param_value_t* param_value = (const clap_event_param_value_t*)header;
    uint32_t i = param_value->param_id;
    float v = param_value->value;
    switch (i) {
      case 0:
        MGain = v;
        break;
    }
  }

  //----------

  void process_audio(const clap_process_t *process) {
    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<process->frames_count; i++) {
      *out0++ = *in0++ * MGain;
      *out1++ = *in1++ * MGain;
    }
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* Gain_Features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t Gain_Descriptor = {
  CLAP_VERSION,
  "torhelgeskei/Gain/0.0.1",
  "Gain",
  "Tor-Helge Skei",
  "https://torhelgeskei.com",
  "<manual_url>",
  "<support_url>",
  "0.0.1",
  "Simple Gain plugin",
  Gain_Features
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

uint32_t MIP_GetNumPlugins() {
  MIP_Print("MIP_GetNumPlugins()\n");
  return 1;
}

//----------

const clap_plugin_descriptor_t* MIP_GetDescriptor(uint32_t index) {
  MIP_Print("MIP_GetDescriptor(%i)\n",index);
  return &Gain_Descriptor;
}

//----------

const clap_plugin_t* MIP_CreatePlugin(const clap_host_t *host, const char *plugin_id) {
  MIP_Print("MIP_CreatePlugin(\"%s\")\n",plugin_id);
  MIP_ClapPlugin* plugin = new Gain_Plugin(&Gain_Descriptor,host);
  return plugin->ptr();
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/clap/mip_clap_entry.h"
