

#include <string.h> // strcmp
#include <stdio.h>  // sprintf
#include <math.h>   // atof

// change this to point to clap.h
#include "../../include/extern/clap/clap.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const char* myFeatures[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "torhelgeskei/clap-minimal/0.0.0",
  "clap-minimal",
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
  "blablabla..",
  myFeatures
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin {

//------------------------------
private:
//------------------------------

  float gain = 0.0;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_host_t *host) {
    MPlugin.plugin_data = this;
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public:
//------------------------------

  virtual bool init() {
    return true;
  }

  //----------

  virtual void destroy() {
  }

  //----------

  virtual bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    return true;
  }

  //----------

  virtual void deactivate() {
  }

  //----------

  virtual bool start_processing() {
    return true;
  }

  //----------

  virtual void stop_processing() {
  }

  //----------

  virtual clap_process_status process(const clap_process_t *process) {
    handle_input_events(process->in_events);
    process_audio(process);
    handle_output_events(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  virtual const void* get_extension(const char *id) {
    if (strcmp(id,CLAP_EXT_PARAMS) == 0) return &MParams;
    return nullptr;
  }

  //----------

  virtual void on_main_thread() {
  }

//------------------------------
public: // params
//------------------------------

  virtual uint32_t params_count() {
    return 1;
  }

  //----------

  virtual bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) {
    switch (param_index) {
      case 0:
        param_info->id              = 0;
        param_info->flags           = 0;
        param_info->cookie          = nullptr;
        param_info->min_value       = 0.0;
        param_info->max_value       = 1.0;
        param_info->default_value   = 0.0;
        strncpy(param_info->name,   "gain", CLAP_NAME_SIZE-1);
        strncpy(param_info->module, "", CLAP_MODULE_SIZE-1);
        return true;
    }
    return false;
  }

  //----------

  virtual bool params_get_value(clap_id param_id, double *value) {
    switch (param_id) {
      case 0:
        *value = gain;
        return true;
    }
    return false;
  }

  //----------

  virtual bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) {
    switch (param_id) {
      case 0:
        sprintf(display,"%.3f",value);
        return true;
    }
    return false;
  }

  //----------

  virtual bool params_text_to_value(clap_id param_id, const char *display, double *value) {
    switch (param_id) {
      case 0:
        float f = atof(display);
        *value = f;
        return true;
    }
    return false;
  }

  //----------

  virtual void params_flush(const clap_input_events_t* in, const clap_output_events_t* out) {
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
        gain = v;
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
      *out0++ = *in0++ * gain;
      *out1++ = *in1++ * gain;
    }
  }

//------------------------------
public: // callbacks
//------------------------------

  static bool clap_plugin_init_callback(const struct clap_plugin *plugin) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->init();
  }

  static void clap_plugin_destroy_callback(const struct clap_plugin *plugin) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    plug->destroy();
    delete plug;
  }

  static bool clap_plugin_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static void clap_plugin_deactivate_callback(const struct clap_plugin *plugin) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    plug->deactivate();
  }

  static bool clap_plugin_start_processing_callback(const struct clap_plugin *plugin) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static void clap_plugin_stop_processing_callback(const struct clap_plugin *plugin) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    plug->stop_processing();
  }

  static clap_process_status clap_plugin_process_callback(const struct clap_plugin *plugin, const clap_process_t *process) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static const void* clap_plugin_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static void clap_plugin_on_main_thread_callback(const struct clap_plugin *plugin) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->on_main_thread();
  }

  //----------

  //const
  clap_plugin_t MPlugin = {
    &myDescriptor,
    this,
    clap_plugin_init_callback,
    clap_plugin_destroy_callback,
    clap_plugin_activate_callback,
    clap_plugin_deactivate_callback,
    clap_plugin_start_processing_callback,
    clap_plugin_stop_processing_callback,
    clap_plugin_process_callback,
    clap_plugin_get_extension_callback,
    clap_plugin_on_main_thread_callback
  };

//------------------------------
public: // params
//------------------------------

  static uint32_t clap_plugin_params_count_callback(const clap_plugin_t *plugin) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->params_count();
  }

  static bool clap_plugin_params_get_info_callback(const clap_plugin_t *plugin, uint32_t param_index, clap_param_info_t* param_info) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->params_get_info(param_index,param_info);
  }

  static bool clap_plugin_params_get_value_callback(const clap_plugin_t *plugin, clap_id param_id, double *value) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->params_get_value(param_id,value);
  }

  static bool clap_plugin_params_value_to_text_callback(const clap_plugin_t *plugin, clap_id param_id, double value, char *display, uint32_t size) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->params_value_to_text(param_id,value,display,size);
  }

  static bool clap_plugin_params_text_to_value_callback(const clap_plugin_t *plugin, clap_id param_id, const char *display, double *value) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->params_text_to_value(param_id,display,value);
  }

  static void clap_plugin_params_flush_callback(const clap_plugin_t* plugin, const clap_input_events_t* in, const clap_output_events_t* out) {
    myPlugin* plug = (myPlugin*)plugin->plugin_data;
    return plug->params_flush(in,out);
  }

  //----------

  clap_plugin_params_t MParams = {
    clap_plugin_params_count_callback,
    clap_plugin_params_get_info_callback,
    clap_plugin_params_get_value_callback,
    clap_plugin_params_value_to_text_callback,
    clap_plugin_params_text_to_value_callback,
    clap_plugin_params_flush_callback
  };

//------------------------------

};

//----------------------------------------------------------------------
//
// factory
//
//----------------------------------------------------------------------

uint32_t clap_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  return 1;
}

const clap_plugin_descriptor_t* clap_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  return &myDescriptor;
}

const clap_plugin_t* clap_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  myPlugin* plugin = new myPlugin(host); // deleted in myPlugin.clap_plugin_destroy_callback()
  return &plugin->MPlugin;
}

//----------

  //static
  //constexpr
  const clap_plugin_factory clap_factory = {
    clap_factory_get_plugin_count_callback,
    clap_factory_get_plugin_descriptor_callback,
    clap_factory_create_plugin_callback
  };

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

bool clap_entry_init_callback(const char *plugin_path) {
  return true;
}

void clap_entry_deinit_callback() {
}

const void* clap_entry_get_factory_callback(const char *factory_id) {
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &clap_factory;
  }
  return nullptr;
}

//----------

  // get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wattributes"

  CLAP_EXPORT
  const clap_plugin_entry clap_entry = {
    CLAP_VERSION,
    clap_entry_init_callback,
    clap_entry_deinit_callback,
    clap_entry_get_factory_callback,
  };

  #pragma GCC diagnostic pop
