
#define MIP_NO_GUI

#include "mip.h"
#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class Plugin
: public MIP_ClapPlugin {

//------------------------------
private:
//------------------------------

  double MParam1 = 0.0;

//------------------------------
public:
//------------------------------

  Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
    MIP_Print("Plugin()\n");
  }

  //----------

  virtual ~Plugin() {
    MIP_Print("~Plugin()\n");
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    return true;
  }

  void destroy() final {
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    return true;
  }

  void deactivate() final {
  }

  bool start_processing() final {
    return true; }

  void stop_processing() final {
  }

  clap_process_status process(const clap_process_t *process) final {
    return CLAP_PROCESS_CONTINUE; }

  void on_main_thread() final {
  }

  //const void* get_extension(const char *id) final {
  //  return MIP_ClapPlugin::get_extension(id);
  //}

//------------------------------
public:
//------------------------------

  uint32_t params_count() final {
    return 1;
  }

  bool params_get_info(int32_t param_index, clap_param_info_t* param_info) final {
    switch (param_index) {
      case 0:
        param_info->id              = 0;
        param_info->flags           = 0;
        param_info->cookie          = nullptr;
        param_info->min_value       = 0.0;
        param_info->max_value       = 1.0;
        param_info->default_value   = 0.0;
        strncpy(param_info->name,   "Param1", CLAP_NAME_SIZE-1);
        strncpy(param_info->module, "Module", CLAP_MODULE_SIZE-1);
        return true;
    }
    return false;
  }

  bool params_get_value(clap_id param_id, double *value) final {
    switch (param_id) {
      case 0:
        *value = MParam1;
        return true;
    }
    return false;
  }

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) final {
    switch (param_id) {
      case 0:
        sprintf(display,"%.3f",value);
        return true;
    }
    return false;
  }

  bool params_text_to_value(clap_id param_id, const char *display, double *value) final {
    switch (param_id) {
      case 0:
        float f = atof(display);
        *value = f;
        return true;
    }
    return false;
  }

  void params_flush(const clap_input_events_t* in, const clap_output_events_t* out) final {
    //TODO
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t Descriptor = {
  CLAP_VERSION,
  "plugin_id",
  "name",
  "vendor",
  "url",
  "manual_url",
  "support_url",
  "0.0.0",
  "description",
  "audio_effect"
};

//----------------------------------------------------------------------

uint32_t MIP_GetNumPlugins() {
  MIP_Print("MIP_GetNumPlugins()\n");
  return 1;
}

//----------

const clap_plugin_descriptor_t* MIP_GetDescriptor(uint32_t index) {
  MIP_Print("MIP_GetDescriptor(%i)\n",index);
  return &Descriptor;
}

//----------

const clap_plugin_t* MIP_CreatePlugin(const clap_host_t *host, const char *plugin_id) {
  MIP_Print("MIP_CreatePlugin(\"%s\")\n",plugin_id);
  MIP_ClapPlugin* plugin = new Plugin(&Descriptor,host);
  return  plugin->getPlugin();
}

