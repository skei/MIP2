#include <string.h>   // strcmp
#include <stdio.h>    // sprintf
#include <stdlib.h>   // atof

#include "extern/clap/clap.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t ClapDescriptor = {
  CLAP_VERSION,
  "skei.gain_clap.v0",
  "Gain (clap)",
  "Tor-Helge Skei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
  "Simple Gain plugin (CLAP version)",
  "audio_effect"
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class ClapPlugin {

//------------------------------
private:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor = nullptr;
  const clap_host_t* MHost = nullptr;

  //----------

  float MGain = 0.0;

//------------------------------
public:
//------------------------------

  ClapPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    MDescriptor   = ADescriptor;
    MHost         = AHost;
    MPlugin.desc  = ADescriptor;
  }

  //----------

  virtual ~ClapPlugin() {
  }

  //----------

  const clap_plugin_t* getPtr() {
    return &MPlugin;
  }

//------------------------------
private:
//------------------------------


  void handle_events(const clap_input_events_t* events) {
    uint32_t num_events = events->size(events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = events->get(events,i);
      switch (header->type) {
        case CLAP_EVENT_PARAM_VALUE:
          const clap_event_param_value_t* param_event = (const clap_event_param_value_t*)header;
          switch (param_event->param_id) {
            case 0:
              MGain = param_event->value;
              break;
          }
          break;
      }
    }
  }

  //----------

  void process_audio(const clap_process_t* process) {
    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<process->frames_count; i++) {
      *out0++ = *in0++ * MGain;
      *out1++ = *in1++ * MGain;
    }
  }

//------------------------------
public:
//------------------------------

  bool init() {
    return true;
  }

  //----------

  void destroy() {
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    return true;
  }

  //----------

  void deactivate() {
  }

  //----------

  bool start_processing() {
    return true;
  }

  //----------

  void stop_processing() {
  }

  //----------

  clap_process_status process(const clap_process_t *process) {
    handle_events(process->in_events);
    process_audio(process);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) {
    if (strcmp(id,CLAP_EXT_PARAMS) == 0) { return &MParams; }
    return nullptr;
  }

  //----------

  void on_main_thread() {
  }

//------------------------------
public: // extensions
//------------------------------

  // parameters

  //----------

  uint32_t params_count() {
    return 1;
  }

  //----------

  bool params_get_info(int32_t param_index, clap_param_info_t* param_info) {
    param_info->id            = 0;
    param_info->flags         = 0;
    param_info->cookie        = nullptr;
    strncpy(param_info->name,"Gain",CLAP_NAME_SIZE);
    strncpy(param_info->module,"",CLAP_MODULE_SIZE);
    param_info->min_value     = 0.0;
    param_info->max_value     = 1.0;
    param_info->default_value = 0.0;
    return true;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) {
    switch (param_id) {
      case 0:
        *value = MGain;
        return true;
    }
    return false;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) {
    switch (param_id) {
      case 0:
        sprintf(display,"%.2f",value);
        return true;;
    }
    return false;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) {
    switch (param_id) {
      case 0:
        *value = atof(display);
        return true;
    }
    return false;
  }

  //----------

  void params_flush(const clap_input_events_t* in, const clap_output_events_t* out) {
    handle_events(in);
  }

//------------------------------
private: // callbacks
//------------------------------

  static bool clap_plugin_init_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->init();
  }

  static void clap_plugin_destroy_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    plug->destroy();
  }

  static bool clap_plugin_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static void clap_plugin_deactivate_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    plug->deactivate();
  }

  static bool clap_plugin_start_processing_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static void clap_plugin_stop_processing_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    plug->stop_processing();
  }

  static clap_process_status clap_plugin_process_callback(const struct clap_plugin *plugin, const clap_process_t *process) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static const void* clap_plugin_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static void clap_plugin_on_main_thread_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->on_main_thread();
  }

  //----------

  //const
  clap_plugin_t MPlugin = {
    nullptr, // set in ClapPlugin constructor
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
private: // extensions
//------------------------------

  //--------------------
  // clap.params
  //--------------------

  static uint32_t clap_plugin_params_count_callback(const clap_plugin_t *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_count();
  }

  static bool clap_plugin_params_get_info_callback(const clap_plugin_t *plugin, int32_t param_index, clap_param_info_t* param_info) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_get_info(param_index,param_info);
  }

  static bool clap_plugin_params_get_value_callback(const clap_plugin_t *plugin, clap_id param_id, double *value) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_get_value(param_id,value);
  }

  static bool clap_plugin_params_value_to_text_callback(const clap_plugin_t *plugin, clap_id param_id, double value, char *display, uint32_t size) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_value_to_text(param_id,value,display,size);
  }

  static bool clap_plugin_params_text_to_value_callback(const clap_plugin_t *plugin, clap_id param_id, const char *display, double *value) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_text_to_value(param_id,display,value);
  }

  static void clap_plugin_params_flush_callback(const clap_plugin_t* plugin, const clap_input_events_t* in, const clap_output_events_t* out) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_flush(in,out);
  }

  clap_plugin_params_t MParams = {
    clap_plugin_params_count_callback,
    clap_plugin_params_get_info_callback,
    clap_plugin_params_get_value_callback,
    clap_plugin_params_value_to_text_callback,
    clap_plugin_params_text_to_value_callback,
    clap_plugin_params_flush_callback
  };

};

//----------------------------------------------------------------------
//
// factory
//
//----------------------------------------------------------------------

class ClapFactory {

//------------------------------
public:
//------------------------------

  static
  uint32_t get_plugin_count(const struct clap_plugin_factory *factory) {
    return 1;
  }

  //----------

  static
  const clap_plugin_descriptor_t* get_plugin_descriptor(const struct clap_plugin_factory *factory, uint32_t index) {
    if (index == 0) {
      return &ClapDescriptor;
    }
    return nullptr;
  }

  //----------

  static
  const clap_plugin_t* create_plugin(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
    if (strcmp(plugin_id,ClapDescriptor.id) == 0) {
      ClapPlugin* plugin = new ClapPlugin(&ClapDescriptor,host);
      return plugin->getPtr();
    }
    return nullptr;
  }

};

//----------------------------------------------------------------------

const ClapFactory clap_factory;

//----------

const clap_plugin_factory CLAP_FACTORY = {
  clap_factory.get_plugin_count,
  clap_factory.get_plugin_descriptor,
  clap_factory.create_plugin
};


//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

class ClapEntry {

//------------------------------
public:
//------------------------------

  static
  bool init(const char *plugin_path) {
    return true;
  }

  //----------

  static
  void deinit() {
  }

  //----------

  static
  const void* get_factory(const char *factory_id) {
    if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
      return &CLAP_FACTORY;
    }
    return nullptr;
  }

};

//----------------------------------------------------------------------

const ClapEntry CLAP_ENTRY;

//----------

CLAP_EXPORT const clap_plugin_entry clap_entry = {
  CLAP_VERSION,
  CLAP_ENTRY.init,
  CLAP_ENTRY.deinit,
  CLAP_ENTRY.get_factory
};

