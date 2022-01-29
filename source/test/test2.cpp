
//#define MIP_NO_GUI
#define MIP_USE_XCB
//#define MIP_GUI_XCB

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket


//----------

#include "mip.h"
#include "base/utils/mip_math.h"

#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_list.h"
#include "plugin/clap/mip_clap_entry.h"
#include "plugin/clap/mip_clap_factory.h"
#include "plugin/clap/mip_clap_plugin.h"

#include "plugin/wrapper/mip_exe_wrapper.h"
#include "plugin/wrapper/mip_lv2_wrapper.h"
#include "plugin/wrapper/mip_vst2_wrapper.h"
#include "plugin/wrapper/mip_vst3_wrapper.h"

#include "gui/xcb/mip_xcb_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* myFeatures[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "torhelgeskei/test2/v0.0.0",
  #ifdef MIP_DEBUG
  "mip_debug",
  #else
  "mip_release",
  #endif
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
  "simple MIP2 test plugin",
  myFeatures
};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_ClapPlugin {

//------------------------------
private:
//------------------------------

  clap_param_info_t MParameterInfos[4] = {
    { 0,     CLAP_PARAM_IS_MODULATABLE,   nullptr, "param1", "", 0.0, 1.0, 0.5 },
    { 1,     CLAP_PARAM_IS_MODULATABLE,   nullptr, "param2", "", 0.0, 1.0, 0.5 },
    { 2, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param3", "", 0.0, 5.0, 1.0 },
    { 3, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param4", "", 0.0, 5.0, 1.0 }
  };

  clap_audio_port_info_t MAudioInputInfos[2] = {
    { 0, "input1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "input2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  clap_audio_port_info_t MAudioOutputInfos[2] = {
    { 0, "output1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "output2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  //----------

  #define ARRAY_SIZE(x)     ( sizeof( x ) / sizeof( (x)[0] ))
  #define NUM_PARAMS        ARRAY_SIZE(MParameterInfos)
  #define NUM_AUDIO_INPUTS  ARRAY_SIZE(MAudioInputInfos)
  #define NUM_AUDIO_OUTPUTS ARRAY_SIZE(MAudioOutputInfos)

  float           MParamVal[NUM_PARAMS] = {0};
  float           MParamMod[NUM_PARAMS] = {0};

  bool            MIsProcessing = false;
  MIP_XcbWindow*  MWindow       = nullptr;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
private:
//------------------------------

  void handle_input_events(const clap_input_events_t* in_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        switch (header->type) {
          case CLAP_EVENT_PARAM_VALUE:  handle_param_value(header); break;
          case CLAP_EVENT_PARAM_MOD:    handle_param_mod(header); break;
        }
      }
    }
  }

  //----------

  void handle_output_events(const clap_output_events_t* out_events) {
    send_param_mod(0,out_events);
    send_param_mod(2,out_events);
  }

  //----------

  void process_audio(const clap_process_t *process) {
    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<process->frames_count; i++) {
      float v = MParamVal[0] + MParamMod[0];
      v = MIP_Clamp(v,0,1);
      *out0++ = *in0++ * v;
      *out1++ = *in1++ * v;
    }
  }

//------------------------------
private:
//------------------------------

  void handle_param_value(const clap_event_header_t* header) {
    const clap_event_param_value_t* param_value = (const clap_event_param_value_t*)header;
    uint32_t i = param_value->param_id;
    float v = param_value->value;
    MParamVal[i] = v;
  }

  //----------

  void handle_param_mod(const clap_event_header_t* header) {
    const clap_event_param_mod_t* param_mod = (const clap_event_param_mod_t*)header;
    uint32_t i = param_mod->param_id;
    float v = param_mod->amount;
    MParamMod[i] = v;
  }

//------------------------------
//
//------------------------------

  void send_param_mod(uint32_t index, const clap_output_events_t* out_events) {
    float v = MParamVal[index] + MParamMod[index];
    v = MIP_Clamp(v,0,1);
    clap_event_param_mod_t param_mod;
    param_mod.header.size     = sizeof (clap_event_param_mod_t);
    param_mod.header.time     = 0;
    param_mod.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    param_mod.header.type     = CLAP_EVENT_PARAM_MOD;
    param_mod.header.flags    = 0;//CLAP_EVENT_BEGIN_ADJUST | CLAP_EVENT_END_ADJUST | CLAP_EVENT_SHOULD_RECORD;// | CLAP_EVENT_IS_LIVE;
    param_mod.param_id        = index;
    param_mod.cookie          = nullptr;
    param_mod.port_index      = -1;
    param_mod.key             = -1;
    param_mod.channel         = -1;
    param_mod.amount          = v;  // modulation amount
    clap_event_header_t* header = (clap_event_header_t*)&param_mod;
    out_events->push_back(out_events,header);
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    for (uint32_t i=0; i<NUM_PARAMS; i++) {
      MParamVal[i] = MParameterInfos[i].default_value;
    }
    return true;
  }

  //----------

  void destroy() final {
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    return true;
  }

  //----------

  void deactivate() final {
  }

  //----------

  bool start_processing() final {
    MIsProcessing = true;
    return true;
  }

  //----------

  void stop_processing() final {
    MIsProcessing = false;
  }

  //----------

  clap_process_status process(const clap_process_t *process) final {
    handle_input_events(process->in_events);
    process_audio(process);
    handle_output_events(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) final {
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)   return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_EVENT_FILTER) == 0)  return &MEventFilter;
    if (strcmp(id,CLAP_EXT_GUI) == 0)           return &MGui;
    if (strcmp(id,CLAP_EXT_GUI_X11) == 0)       return &MGuiX11;
    if (strcmp(id,CLAP_EXT_PARAMS) == 0)        return &MParams;
    return nullptr;
  }

  //----------

  void on_main_thread() final {
  }

//------------------------------
public: // audio-ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) final {
    if (is_input) return ARRAY_SIZE(MAudioInputInfos);// 2;
    else return ARRAY_SIZE(MAudioOutputInfos); //2;
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) final {
    if (is_input) {
      memcpy(info,&MAudioInputInfos[index],sizeof(clap_audio_port_info_t));
      return true;
    }
    else {
      memcpy(info,&MAudioOutputInfos[index],sizeof(clap_audio_port_info_t));
      return true;
    }
    return false;
  }

//------------------------------
public: // event-filter
//------------------------------

  bool event_filter_accepts(uint16_t space_id, uint16_t event_type) final {
    if (space_id == CLAP_CORE_EVENT_SPACE_ID) {
      switch (event_type) {
        //case CLAP_EVENT_NOTE_ON:          return true;
        //case CLAP_EVENT_NOTE_OFF:         return true;
        //case CLAP_EVENT_NOTE_CHOKE:       return true;
        //case CLAP_EVENT_NOTE_END:         return true;
        //case CLAP_EVENT_NOTE_EXPRESSION:  return true;
        case CLAP_EVENT_PARAM_VALUE:      return true;
        case CLAP_EVENT_PARAM_MOD:        return true;
        //case CLAP_EVENT_TRANSPORT:        return true;
        //case CLAP_EVENT_MIDI:             return true;
        //case CLAP_EVENT_MIDI_SYSEX:       return true;
        //case CLAP_EVENT_MIDI2:            return true;
      }
    }
    return false;
  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create() final {
    return true;
  }

  //----------

  void gui_destroy() final {
  }

  //----------

  bool gui_set_scale(double scale) final {
    return true;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) final {
    *width = 256;
    *height = 256;
    return true;
  }

  //----------

  bool gui_can_resize() final {
    return false;
  }

  //----------

  void gui_round_size(uint32_t *width, uint32_t *height) final {
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) final {
    return true;
  }

  //----------

  void gui_show() final {
    MWindow->open();
  }

  //----------

  void gui_hide() final {
    MWindow->close();
  }

//------------------------------
public: // gui-x11
//------------------------------

  bool gui_x11_attach(const char *display_name, unsigned long window) final {
    MWindow = new MIP_XcbWindow(256,256,"",(void*)window);
    MWindow->setFillBackground();
    MWindow->setBackgroundColor(0.4);
    return true;
  }

//------------------------------
public: // params
//------------------------------

  uint32_t params_count() final {
    return ARRAY_SIZE(MParameterInfos); // 3;
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) final {
    memcpy(param_info,&MParameterInfos[param_index],sizeof(clap_param_info_t));
    return true;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) final {
    *value = MParamVal[param_id];
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) final {
    sprintf(display,"%.3f",value);
    return true;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) final {
    float f = atof(display);
    *value = f;
    return true;
  }

  //----------

  void params_flush(const clap_input_events_t* in, const clap_output_events_t* out) final {
    handle_input_events(in);
    handle_output_events(out);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins(MIP_ClapList* AList) {
  AList->appendPlugin(&myDescriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
