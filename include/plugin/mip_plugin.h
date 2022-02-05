#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "plugin/mip_editor.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_entry.h"

#include "plugin/wrapper/mip_exe_wrapper.h"
#include "plugin/wrapper/mip_lv2_wrapper.h"
#include "plugin/wrapper/mip_vst2_wrapper.h"
#include "plugin/wrapper/mip_vst3_wrapper.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

typedef MIP_Array<const clap_param_info_t*>       MIP_Parameters;
typedef MIP_Array<const clap_audio_port_info_t*>  MIP_AudioPorts;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Plugin
: public MIP_ClapPlugin {

//------------------------------
protected:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor       = nullptr;
  const clap_host_t*              MHost             = nullptr;

  MIP_Parameters  MParameters   = {};
  MIP_AudioPorts  MAudioInputs  = {};
  MIP_AudioPorts  MAudioOutputs = {};

  float*          MParamVal     = nullptr;
  float*          MParamMod     = nullptr;
  MIP_Editor*     MEditor       = nullptr;
  bool            MIsProcessing = false;
  bool            MIsEditorOpen = false;

//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
    MDescriptor       = ADescriptor;
    MHost             = AHost;
  }

  //----------

  virtual ~MIP_Plugin() {
  }

//------------------------------
public:
//------------------------------

  float getParamMod(uint32_t AIndex) { return MParamMod[AIndex]; }
  float getParamVal(uint32_t AIndex) { return MParamVal[AIndex]; }

  void  setParamMod(uint32_t AIndex, float AValue) { MParamMod[AIndex] = AValue; }
  void  setParamVal(uint32_t AIndex, float AValue) { MParamVal[AIndex] = AValue; }

//------------------------------
public:
//------------------------------

  virtual void handle_parameter_event(const clap_event_param_value_t* param_value) {
    uint32_t i = param_value->param_id;
    float v = param_value->value;
    setParamVal(i,v);
    if (MEditor) MEditor->updateParameterFromHost(i,v);
  }

  //----------

  virtual void handle_modulation_event(const clap_event_param_mod_t* param_mod) {
    uint32_t i = param_mod->param_id;
    float v = param_mod->amount;
    setParamMod(i,v);
    if (MEditor) MEditor->updateModulationFromHost(i,v);
  }

  //----------

  virtual void handle_process(const clap_process_t *process) {
    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    uint32_t num = process->frames_count;
    for (uint32_t i=0; i<num; i++) {
      *out0++ = *in0++;
      *out1++ = *in1++;
    }
  }

//------------------------------
protected:
//------------------------------

  virtual void handle_input_events(const clap_input_events_t* in_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        switch (header->type) {
          case CLAP_EVENT_PARAM_VALUE:  handle_parameter_event((clap_event_param_value_t*)header); break;
          case CLAP_EVENT_PARAM_MOD:    handle_modulation_event((clap_event_param_mod_t*)header); break;
        }
      }
    }
  }

  //----------

  virtual void handle_output_events(const clap_output_events_t* out_events) {
    if (MEditor && MIsEditorOpen) MEditor->flushHostParams(out_events);
  }

  //----------

  void setupParameters(clap_param_info_t* params, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_param_info_t* info = &params[i];
      MParameters.append(info);
    }
  }

  void setDefaultParameterValues(clap_param_info_t* params, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_param_info_t* info = &params[i];
      MParamVal[i] = info->default_value;
      MParamMod[i] = 0.0;
    }
  }

  void setupAudioInputs(clap_audio_port_info_t* inputs, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_audio_port_info_t* info = &inputs[i];
      MAudioInputs.append(info);
    }
  }

  void setupAudioOutputs(clap_audio_port_info_t* outputs, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_audio_port_info_t* info = &outputs[i];
      MAudioOutputs.append(info);
    }
  }


//------------------------------
public: // plugin
//------------------------------

  bool init() override {
    uint32_t num = MParameters.size();
    MParamVal    = (float*)malloc(num * sizeof(float));
    MParamMod    = (float*)malloc(num * sizeof(float));
    for (uint32_t i=0; i<num; i++) {
      float v = MParameters[i]->default_value;
      setParamVal(i,v);
      setParamMod(i,0);
    }
    return true;
  }

  //----------

  void destroy() override {
    free(MParamVal);
    free(MParamMod);
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    return true;
  }

  //----------

  void deactivate() override {
  }

  //----------

  bool start_processing() override {
    MIsProcessing = true;
    return true;
  }

  //----------

  void stop_processing() override {
    MIsProcessing = false;
  }

  //----------

  clap_process_status process(const clap_process_t *process) final {
    handle_input_events(process->in_events);
    handle_process(process);
    handle_output_events(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)   return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_EVENT_FILTER) == 0)  return &MEventFilter;
    if (strcmp(id,CLAP_EXT_GUI) == 0)           return &MGui;
    if (strcmp(id,CLAP_EXT_GUI_X11) == 0)       return &MGuiX11;
    if (strcmp(id,CLAP_EXT_PARAMS) == 0)        return &MParams;
    //if (strcmp(id,CLAP_EXT_TIMER_SUPPORT) == 0) return &MTimerSupport;
    return nullptr;
  }

  //----------

  void on_main_thread() override {
  }

//------------------------------
public: // event-filter
//------------------------------

  bool event_filter_accepts(uint16_t space_id, uint16_t event_type) override {
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
public: // params
//------------------------------

  uint32_t params_count() override {
    return MParameters.size();
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) override {
    memcpy(param_info,MParameters[param_index],sizeof(clap_param_info_t));
    return true;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) override {
    *value = getParamVal(param_id);
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) override {
    sprintf(display,"%.3f",value);
    return true;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) override {
    float f = atof(display);
    *value = f;
    return true;
  }

  //----------

  void params_flush(const clap_input_events_t* in, const clap_output_events_t* out) override {
    handle_input_events(in);
    handle_output_events(out);
  }

//------------------------------
public: // gui-x11
//------------------------------

  bool gui_x11_attach(const char *display_name, unsigned long window) override {
    MEditor->attach(display_name,window);
    return true;
  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create() override {
    MEditor = new MIP_Editor(this);
    MIsEditorOpen = false;
    return true;
  }

  //----------

  void gui_destroy() override {
    MIsEditorOpen = false;
    delete MEditor;
  }

  //----------

  bool gui_set_scale(double scale) override {
    return true;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    return MEditor->getSize(width,height);
  }

  //----------

  bool gui_can_resize() override {
    return false;
  }

  //----------

  void gui_round_size(uint32_t *width, uint32_t *height) override {
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    return true;
  }

  //----------

  void gui_show() override {
    MEditor->show();
    MIsEditorOpen = true;
  }

  //----------

  void gui_hide() override {
    MIsEditorOpen = false;
    MEditor->hide();
  }

//------------------------------
public: // audio-ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    if (is_input) return MAudioInputs.size();
    else return MAudioOutputs.size();
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) override {
    if (is_input) {
      memcpy(info,MAudioInputs[index],sizeof(clap_audio_port_info_t));
      return true;
    }
    else {
      memcpy(info,MAudioOutputs[index],sizeof(clap_audio_port_info_t));
      return true;
    }
    return false;
  }

};

//----------------------------------------------------------------------
#endif
