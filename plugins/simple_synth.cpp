
#define MIP_DEBUG
#define MIP_DEBUG_PRINT

// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG_PRINT_SOCKET

#define MIP_GUI_XCB
#define MIP_USE_XCB

#include "mip.h"
#include "audio/mip_audio_math.h"
//#include "audio/mip_voices.h"
#include "base/utils/mip_convert.h"
#include "plugin/mip_editor.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_voices.h"

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

private:

  MIP_ClapVoiceContext* MContext      = nullptr;

  float MNote         = 0.0f;
  float MBend         = 0.0f;
  float MMasterBend   = 0.0f;
  float MMasterPress  = 0.0f;

  float MSampleRate   = 0.0;

  // osc
  float ph    = 0.0;
  float phadd = 0.0;
  // filter
  float z0    = 0.0;
  float w     = 0.0;
  float wm    = 0.0;

public:

  bool prepare(MIP_ClapVoiceContext* AContext) {
    MContext = AContext;
    MSampleRate = AContext->samplerate;
    return true;
  }

  //----------

  // each voice are added to the buffer!

  uint32_t process(uint32_t state) {
    float* out0 = MContext->buffers[0];
    float* out1 = MContext->buffers[1];
    for (uint32_t i=0; i<MContext->length; i++) {
      float out = (ph * 0.1);
      ph += phadd;
      ph = MIP_Fract(ph);
      float ww = MIP_Clamp((w + wm),0,1);
      z0 += ((out - z0) * ww);
      out = z0; // lp
      //out -= z0; // hp
      *out0++ += out;
      *out1++ += out;
    }
    return state;
  }

  //----------

  uint32_t strike(uint32_t note, float velocity) {
    MNote = note;
    MBend = 0.0f;
    float hz = MIP_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    ph = 0.0f;
    phadd = hz / MSampleRate;
    z0 = 0;
    return MIP_VOICE_PLAYING;
  }

  //----------

  uint32_t lift(float v) {
    return MIP_VOICE_FINISHED;
  }

  //----------

  void bend(float b) {
    MBend = b;
    float hz = MIP_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    phadd = hz / MSampleRate;
  }

  //----------

  void press(float v) {
  }

  //----------

  void timbre(float s) {
  }

  //----------

  void ctrl(uint32_t i, float v) {
  }

  //----------

  void parameter(uint32_t i, float v) {
    //MIP_Print("%i = %.2f\n",i,v);
    if (i == 1) w = (v*v);
  }

  //----------

  void modulation(uint32_t i, float v) {
    //MIP_Print("%i = %.2f\n",i,v);
    if (i == 1) wm = v;
  }

  //----------
  //----------

  void master_press(float mp) {
  }

  //----------

  void master_bend(float mb) {
    MMasterBend = mb;
    float hz = MIP_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    phadd = hz / MSampleRate;
  }

  //----------

  void master_ctrl(uint32_t i, float v) {
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_ClapPlugin {

//------------------------------
public:
//------------------------------

  MIP_Editor*                 MEditor     = nullptr;
  MIP_ClapVoices<myVoice,16>  MVoices     = {};

  float MGain       = 0.0;
  float MGainMod    = 0.0;
  float MFilter     = 0.0;
  float MFilterMod  = 0.0;

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
          switch (event->param_value.param_id) {
            case 0:
              MGain = event->param_value.value;
              break;
            //default
            case 1:
              MFilter = event->param_value.value;
              MVoices.parameter(&event->param_value);
              break;
          }
          break;
        case CLAP_EVENT_PARAM_MOD:
          switch (event->param_mod.param_id) {
            case 0:
              MGainMod = event->param_mod.amount;
              break;
            //default:
            case 1:
              MFilterMod = event->param_mod.amount;
              MVoices.modulation(&event->param_mod);
              break;
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
    // send param_mod : gain + mod
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    return true;
  }

  //----------

  void destroy() final {
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoices.prepare(sample_rate);
    return true;
  }

  //----------

  void deactivate() final {
  }

  //----------

  bool start_processing() final {
    return true;
  }

  //----------

  void stop_processing() final {
  }

  //----------

  clap_process_status process(const clap_process_t* process) final {
    process_input_events(process->in_events);
    MVoices.process(process);
    MIP_ScaleStereoBuffer(process->audio_outputs->data32,MGain,MGain,process->frames_count);
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

  // we don't have a window yet!

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
    if (MEditor->attach(display_name,window)) {

//----- test -----

MIP_Window* win = MEditor->getWindow();
MIP_Widget* wdg1 = new MIP_Widget( MIP_FRect(10,10,100,20) );
MIP_Widget* wdg2 = new MIP_Widget( MIP_FRect(10,40,100,20) );
wdg1->setName("wdg1");
wdg2->setName("wdg2");
win->appendWidget(wdg1);
win->appendWidget(wdg2);

//----- test -----

    }
    return true;
  }

//------------------------------
public: // params
//------------------------------

  uint32_t params_count() final {
    return 2;
  }

  //----------

  bool params_get_info(int32_t param_index, clap_param_info_t* param_info) final {
    switch (param_index) {
      case 0:
        strncpy(param_info->name,   "gain",CLAP_NAME_SIZE-1);
        strncpy(param_info->module, "master",CLAP_MODULE_SIZE-1);
        param_info->id            = 0;
        param_info->flags         = CLAP_PARAM_IS_MODULATABLE;
        param_info->cookie        = nullptr;
        param_info->min_value     = 0.0;
        param_info->max_value     = 1.0;
        param_info->default_value = 0.5;
        return true;
      case 1:
        strncpy(param_info->name,   "filter",CLAP_NAME_SIZE-1);
        strncpy(param_info->module, "per voice",CLAP_MODULE_SIZE-1);
        param_info->id            = 1;
        param_info->flags         = CLAP_PARAM_IS_MODULATABLE
                                  | CLAP_PARAM_IS_PER_NOTE;
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
    switch (param_id) {
      case 0: *value = MGain;   return true;
      case 1: *value = MFilter; return true;
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
    stream->write(stream,&MGain,sizeof(float));
    stream->write(stream,&MFilter,sizeof(float));
    return true;
  }

  //----------

  bool state_load(clap_istream_t *stream) final {
    stream->read(stream,&MGain,sizeof(float));
    stream->read(stream,&MFilter,sizeof(float));
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
