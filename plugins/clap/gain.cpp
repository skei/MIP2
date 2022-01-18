
/*
  you might need to change these a little..
  g++ -Wall -std=c++14 -Wl,--as-needed -O2 -fPIC -I../include -c gain_clap.cpp -o build.o
  g++ -shared build.o -o gain.clap -s -fPIE
*/

//----------

#include <string.h>   // strcmp
#include <stdio.h>    // sprintf
#include <stdlib.h>   // atof

// change this to point to the clap headers..
#include "extern/clap/clap.h"
#include "minimal_xcb_window.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t ClapDescriptor = {
  CLAP_VERSION,                 // version
  "torhelgeskei.gain.v0",       // id
  "gain",                       // name
  "tor-helge skei",             // vendor
  "https://torhelgeskei.com",   // url
  "",                           // manual_url
  "",                           // support_url
  "0.0.0",                      // version
  "simple gain plugin",         // description
  "audio_effect"                // features
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

//------------------------------
protected:
//------------------------------

  MinimalXcbWindow* MWindow = nullptr;
  float MGain = 0.0;

//------------------------------
public:
//------------------------------

  ClapPlugin(const clap_plugin_descriptor_t* descriptor, const clap_host_t* host) {
    MPlugin.desc  = descriptor;
    MDescriptor   = descriptor;
    MHost         = host;
  }

  //----------

  ~ClapPlugin() {
  }

  //----------

  // called by clap_factory.create_plugin
  const clap_plugin_t* getPtr() { return &MPlugin; }

//------------------------------
private:
//------------------------------

  // TODO: sample accurate

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
    if (strcmp(id,CLAP_EXT_GUI) == 0) { return &MGui; }
    if (strcmp(id,CLAP_EXT_GUI_X11) == 0) { return &MGuiX11; }
    return nullptr;
  }

  //----------

  void on_main_thread() {
  }

//------------------------------
public: // clap.params
//------------------------------

  uint32_t params_count() {
    return 1;
  }

  //----------

  bool params_get_info(int32_t param_index, clap_param_info_t* param_info) {
    param_info->id            = 0;
    param_info->flags         = 0;
    param_info->cookie        = nullptr;
    strncpy(param_info->name,"gain",CLAP_NAME_SIZE);
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
public: // clap.gui
//------------------------------

  bool gui_create() {
    MWindow = new MinimalXcbWindow();
    return true;
  }

  //----------

  void gui_destroy() {
    delete MWindow;
  }

  //----------

  bool gui_set_scale(double scale) {
    return true;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) {
    *width = 600;
    *height = 400;
    return true;
  }

  //----------

  bool gui_can_resize() {
    return false;
  }

  //----------

  void gui_round_size(uint32_t *width, uint32_t *height) {
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) {
    return true;
  }

  //----------

  void gui_show() {
    MWindow->show();
  }

  //----------

  void gui_hide() {
    MWindow->hide();
  }

//------------------------------
public: // clap.gui-x11
//------------------------------

  bool gui_x11_attach(const char *display_name, unsigned long window) {
    MWindow->attach(display_name,window);
    return true;
  }

//------------------------------
private: // callbacks
//------------------------------

  static bool init_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->init();
  }

  static void destroy_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    plug->destroy();
  }

  static bool activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static void deactivate_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    plug->deactivate();
  }

  static bool start_processing_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static void stop_processing_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    plug->stop_processing();
  }

  static clap_process_status process_callback(const struct clap_plugin *plugin, const clap_process_t *process) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static const void* get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static void on_main_thread_callback(const struct clap_plugin *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->on_main_thread();
  }

  //----------

  //const
  clap_plugin_t MPlugin = {
    nullptr, // set in ClapPlugin constructor
    this,
    init_callback,
    destroy_callback,
    activate_callback,
    deactivate_callback,
    start_processing_callback,
    stop_processing_callback,
    process_callback,
    get_extension_callback,
    on_main_thread_callback
  };

//------------------------------
private: // clap.params
//------------------------------

  static uint32_t params_count_callback(const clap_plugin_t *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_count();
  }

  static bool params_get_info_callback(const clap_plugin_t *plugin, int32_t param_index, clap_param_info_t* param_info) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_get_info(param_index,param_info);
  }

  static bool params_get_value_callback(const clap_plugin_t *plugin, clap_id param_id, double *value) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_get_value(param_id,value);
  }

  static bool params_value_to_text_callback(const clap_plugin_t *plugin, clap_id param_id, double value, char *display, uint32_t size) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_value_to_text(param_id,value,display,size);
  }

  static bool params_text_to_value_callback(const clap_plugin_t *plugin, clap_id param_id, const char *display, double *value) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_text_to_value(param_id,display,value);
  }

  static void params_flush_callback(const clap_plugin_t* plugin, const clap_input_events_t* in, const clap_output_events_t* out) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->params_flush(in,out);
  }

  clap_plugin_params_t MParams = {
    params_count_callback,
    params_get_info_callback,
    params_get_value_callback,
    params_value_to_text_callback,
    params_text_to_value_callback,
    params_flush_callback
  };

//------------------------------
private: // clap.gui
//------------------------------

  static bool gui_create_callback(const clap_plugin_t *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_create();
  }

  static void gui_destroy_callback(const clap_plugin_t *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_destroy();
  }

  static bool gui_set_scale_callback(const clap_plugin_t *plugin, double scale) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_scale(scale);
  }

  static bool gui_get_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_get_size(width,height);
  }

  static bool gui_can_resize_callback(const clap_plugin_t *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_can_resize();
  }

  static void gui_round_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_round_size(width,height);
  }

  static bool gui_set_size_callback(const clap_plugin_t *plugin, uint32_t width, uint32_t height) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_size(width,height);
  }

  static void gui_show_callback(const clap_plugin_t *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_show();
  }

  static void gui_hide_callback(const clap_plugin_t *plugin) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_hide();
  }

  clap_plugin_gui_t MGui = {
    gui_create_callback,
    gui_destroy_callback,
    gui_set_scale_callback,
    gui_get_size_callback,
    gui_can_resize_callback,
    gui_round_size_callback,
    gui_set_size_callback,
    gui_show_callback,
    gui_hide_callback
  };

//------------------------------
private: // clap.gui-x11
//------------------------------

  static bool gui_x11_attach_callback(const clap_plugin_t *plugin, const char *display_name, unsigned long window) {
    ClapPlugin* plug = (ClapPlugin*)plugin->plugin_data;
    return plug->gui_x11_attach(display_name,window);
  }

  clap_plugin_gui_x11_t MGuiX11 = {
    gui_x11_attach_callback
  };


};

//----------------------------------------------------------------------
//
// factory
//
//----------------------------------------------------------------------

static
uint32_t clap_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  return 1;
}

//----------

static
const clap_plugin_descriptor_t* clap_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  if (index == 0) {
    return &ClapDescriptor;
  }
  return nullptr;
}

//----------

static
const clap_plugin_t* clap_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  if (strcmp(plugin_id,ClapDescriptor.id) == 0) {
    ClapPlugin* plugin = new ClapPlugin(&ClapDescriptor,host);
    return plugin->getPtr();
  }
  return nullptr;
}

//----------

const clap_plugin_factory CLAP_FACTORY = {
  clap_factory_get_plugin_count_callback,
  clap_factory_get_plugin_descriptor_callback,
  clap_factory_create_plugin_callback
};


//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------


static bool clap_entry_init_callback(const char *plugin_path) {
  return true;
}

//----------

static void clap_entry_deinit_callback() {
}

//----------

static const void* clap_entry_get_factory_callback(const char *factory_id) {
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &CLAP_FACTORY;
  }
  return nullptr;
}

//----------

CLAP_EXPORT const clap_plugin_entry clap_entry = {
  CLAP_VERSION,
  clap_entry_init_callback,
  clap_entry_deinit_callback,
  clap_entry_get_factory_callback
};

