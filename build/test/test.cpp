

// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG
#define MIP_DEBUG_PRINT
#define MIP_DEBUG_PRINT_SOCKET

#define MIP_USE_XCB

#include "mip.h"
#include "audio/mip_voices.h"
#include "base/utils/mip_convert.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/mip_editor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor : public MIP_ClapPluginDescriptor {
public:
  myDescriptor() {
    setName("myPlugin");
    setVendor("Tor-Helge Skei");
    setUrl("https://torhelgeskei.com");
    setVersion("0.0.0");
    setType(CLAP_PLUGIN_INSTRUMENT);
  }
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice {
public:

  bool prepare(MIP_VoiceContext* AContext) {
    return true;
  }

  uint32_t process(uint32_t state) {
    return state;
  }

  uint32_t strike(uint32_t n, float v) {
    return MIP_VOICE_PLAYING;
  }
  uint32_t lift(float v) {
    return MIP_VOICE_FINISHED;
  }

  void bend(float b) {
  }

  void press(float v) {
  }

  void slide(float s) {
  }

  void ctrl(uint32_t i, float v) {
  }

  void master_press(float mp) {
  }

  void master_bend(float mb) {
  }

  void master_ctrl(uint32_t i, float v) {
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public MIP_ClapPlugin {

//------------------------------
public:
//------------------------------

  MIP_Editor*             MEditor = nullptr;
  MIP_Voices<myVoice,16>  MVoices = {};

  float       MParamValue = 0.0;
  float       MParamMod   = 0.0;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor* desc, const clap_host* host)
  : MIP_ClapPlugin(desc,host) {
  }

//------------------------------
public:
//------------------------------

  void process_input_events(const clap_event_list_t* events) {
    uint32_t num_events = events->size(events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_t* event = events->get(events,i);
      switch (event->type) {
        case CLAP_EVENT_NOTE_ON:
          MVoices.note_on(&event->note);
          break;
        case CLAP_EVENT_NOTE_OFF:
          MVoices.note_off(&event->note);
          break;
        case CLAP_EVENT_NOTE_END:
          MVoices.note_end(&event->note);
          break;
        case CLAP_EVENT_NOTE_CHOKE:
          MVoices.note_choke(&event->note);
          break;
        case CLAP_EVENT_NOTE_EXPRESSION:
          MVoices.note_expression(&event->note_expression);
          break;
        case CLAP_EVENT_NOTE_MASK:
          MVoices.note_mask(&event->note_mask);
          break;
        case CLAP_EVENT_PARAM_VALUE:
          if (event->param_value.param_id == 0) {
            MParamValue = event->param_value.value;
          }
          break;
        case CLAP_EVENT_PARAM_MOD:
          if (event->param_mod.param_id == 0) {
            MParamMod = event->param_mod.amount;
          }
          break;
        case CLAP_EVENT_TRANSPORT:
          break;
        case CLAP_EVENT_MIDI:
          MVoices.midi(&event->midi);
          break;
        case CLAP_EVENT_MIDI_SYSEX:
          break;
      }
    }
  }

  //----------

  void process_output_events(const clap_event_list_t* events) {
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
    MVoices.prepare(sample_rate);
    return true;
  }

  void deactivate() final {
  }

  bool start_processing() final {
    return true;
  }

  void stop_processing() final {
  }

  //----------

  clap_process_status process(const clap_process_t* process) final {
    process_input_events(process->in_events);

    MVoices.process(process);

    uint32_t num_samples = process->frames_count;
    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<num_samples; i++) {
      *out0++ = *in0++ * (MParamValue + MParamMod);
      *out1++ = *in1++ * (MParamValue + MParamMod);
    }
    process_output_events(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

//------------------------------
public: // audio ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) final {
    if (is_input) return 1;
    else return 1;
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) final {
    if (is_input) {
      if (index == 0) {
        strncpy(info->name,"input port",CLAP_NAME_SIZE-1);
        info->id            = 0;
        info->channel_count = 2;
        info->channel_map   = CLAP_CHMAP_STEREO;
        info->sample_size   = 32;
        info->is_main       = true;
        info->is_cv         = false;
        info->in_place      = false;
        return true;
      }
    }
    else {
      if (index == 0) {
        strncpy(info->name,"output port",CLAP_NAME_SIZE-1);
        info->id            = 0;
        info->channel_count = 2;
        info->channel_map   = CLAP_CHMAP_STEREO;
        info->sample_size   = 32;
        info->is_main       = true;
        info->is_cv         = false;
        info->in_place      = false;
        return true;
      }
    }
    return false;
  }

//------------------------------
public: // event filter
//------------------------------

  bool event_filter_accepts(clap_event_type event_type) override {
    switch (event_type) {
      case CLAP_EVENT_NOTE_ON:          return true;
      case CLAP_EVENT_NOTE_OFF:         return true;
      case CLAP_EVENT_NOTE_END:         return true;
      case CLAP_EVENT_NOTE_CHOKE:       return true;
      case CLAP_EVENT_NOTE_EXPRESSION:  return true;
      case CLAP_EVENT_NOTE_MASK:        return true;
      case CLAP_EVENT_PARAM_VALUE:      return true;
      case CLAP_EVENT_PARAM_MOD:        return true;
      case CLAP_EVENT_TRANSPORT:        return true;
      case CLAP_EVENT_MIDI:             return true;
      case CLAP_EVENT_MIDI_SYSEX:       return true;
    }
    return false;
  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create() final {
    MEditor = new MIP_Editor();
    return true;
  }

  //----------

  void gui_destroy() final {
    delete MEditor;
  }

  //----------

  void gui_set_scale(double scale) final {
    MEditor->setScale(scale);
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) final {
    *width = MEditor->getWidth();
    *height = MEditor->getHeight();
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
    MEditor->setSize(width,height);
    return true;
  }

  //----------

  void gui_show() final {
    MEditor->show();
  }

  //----------

  void gui_hide() final {
    MEditor->hide();
  }

//------------------------------
public: // gui x11
//------------------------------

  bool gui_x11_attach(const char *display_name, unsigned long window) final {
    MEditor->attach(display_name,window);
    return true;
  }

//------------------------------
public: // params
//------------------------------

  uint32_t params_count() final {
    return 1;
  }

  //----------

  bool params_get_info(int32_t param_index, clap_param_info_t* param_info) final {
    if (param_index == 0) {
      strncpy(param_info->name,"param",CLAP_NAME_SIZE-1);
      strncpy(param_info->module,"module",CLAP_MODULE_SIZE-1);
      param_info->id            = 0;
      param_info->flags         = CLAP_PARAM_IS_MODULATABLE;
      param_info->cookie        = nullptr;
      param_info->min_value     = 0.0;
      param_info->max_value     = 1.0;
      param_info->default_value = 0.5;
      return true;
    }
    return false;
  }


  //----------

  bool params_get_value(clap_id param_id, double *value) final {
    if (param_id == 0) {
      *value = MParamValue;
      return true;
    }
    return false;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) final {
    MIP_FloatToString(display,value,3);
    return true;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) final {
    *value = MIP_StringToFloat((char*)display);
    return true;
  }

  //----------

  void params_flush(const clap_event_list_t *input_parameter_changes, const clap_event_list_t *output_parameter_changes) final {
    process_input_events(input_parameter_changes);
    //process_output_events(output_parameter_changes);
  }

//------------------------------
public: // state
//------------------------------

  bool state_save(clap_ostream_t *stream) final {
    stream->write(stream,&MParamValue,sizeof(float));
    return true;
  }

  //----------

  bool state_load(clap_istream_t *stream) final {
    stream->read(stream,&MParamValue,sizeof(float));
    return true;
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins() {
  registerPlugin( new myDescriptor() );
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t index, const clap_plugin_descriptor* desc, const clap_host_t *host) {
  if (index == 0) return new myPlugin(desc,host);
  return nullptr;
}
