

// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG_PRINT_SOCKET
#define MIP_DEBUG_PRINT_THREAD
#define MIP_DEBUG_PRINT_TIME

#define MIP_USE_XCB
#define MIP_GUI_XCB

#include "mip.h"
#include "audio/mip_audio_math.h"
#include "base/types/mip_queue.h"
#include "base/utils/mip_convert.h"
#include "plugin/mip_editor.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_voices.h"

#include "gui//mip_widget.h"
#include "gui//mip_test_widget.h"

typedef MIP_Queue<int32_t,1024> MIP_ParamQueue;

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public MIP_ClapPluginDescriptor {

//------------------------------
public:
//------------------------------

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
// editor
//
//----------------------------------------------------------------------

class myEditor
: public MIP_Editor {

//------------------------------
public:
//------------------------------

  myEditor(uint32_t num_params)
  : MIP_Editor(num_params) {
  }

  //----------

  virtual ~myEditor() {
  };

//------------------------------
public:
//------------------------------

  void show() final {
    MIP_Widget* wdg1 = new MIP_TestWidget( MIP_FRect(10,10,100,20), "wdg1", 0.5 );
    MIP_Widget* wdg2 = new MIP_TestWidget( MIP_FRect(10,40,100,20), "wdg2", 0.2 );
    wdg1->setModValue(0.7);
    wdg2->setModValue(0.4);
    MWindow->appendWidget(wdg1);
    MWindow->appendWidget(wdg2);
    connectParameter(wdg1,0);
    connectParameter(wdg2,1);
    MIP_Editor::show();
  }

};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice {

//------------------------------
private:
//------------------------------

  MIP_ClapVoiceContext* MContext      = nullptr;
  float                 MNote         = 0.0f;
  float                 MBend         = 0.0f;
  float                 MMasterBend   = 0.0f;
  float                 MMasterPress  = 0.0f;
  float                 MSampleRate   = 0.0;

  // osc
  float ph    = 0.0;
  float phadd = 0.0;
  // filter
  float z0    = 0.0;
  float w     = 0.0;
  float wm    = 0.0;

//------------------------------
public:
//------------------------------

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
    if (i == 1) w = (v*v);
  }

  //----------

  void modulation(uint32_t i, float v) {
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

//------------------------------

typedef MIP_ClapVoices<myVoice,16> myVoices;

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_ClapPlugin
, public MIP_EditorListener {

//------------------------------
public:
//------------------------------

  myEditor*       MEditor           = nullptr;
  myVoices        MVoices           = {};
  MIP_ParamQueue  MHostParamQueue   = {};
  float*          MHostParamValues  = nullptr;
  bool            MIsProcessing     = false;
  bool            MIsActivated      = false;
  float           MGain             = 0.0;
  float           MGainMod          = 0.0;
  float           MFilter           = 0.0;
  float           MFilterMod        = 0.0;
  bool            MIsEditorOpen     = false;
  uint32_t        MTimerId          = 0;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor* desc, const clap_host* host)
  : MIP_ClapPlugin(desc,host) {
    uint32_t num_params = params_count();
    MHostParamValues = (float*)malloc(num_params * sizeof(uint32_t));
  }

  //----------

  virtual ~myPlugin() {
    free(MHostParamValues);
  }


//------------------------------
public:
//------------------------------

  bool isProcessing() { return MIsProcessing; }
  bool isActivated() { return MIsActivated; }

//------------------------------
public:
//------------------------------

  void queueHostParam(int32_t AIndex, float AValue) {
    if (AIndex >= 0) {
      MHostParamValues[AIndex] = AValue;
      MHostParamQueue.write(AIndex);
    }
  }

  //----------

  void flushHostParams(const clap_event_list_t* out_events) {
    int32_t index;
    while (MHostParamQueue.read(&index)) {
      if (index >= 0) {
        float value = MHostParamValues[index];
        clap_event event;
        event.type                    = CLAP_EVENT_PARAM_VALUE;
        event.time                    = 0;
        event.param_value.cookie      = nullptr;
        event.param_value.param_id    = index;
        event.param_value.port_index  = -1;
        event.param_value.key         = -1;
        event.param_value.channel     = -1;
        event.param_value.flags       = CLAP_EVENT_PARAM_BEGIN_ADJUST | CLAP_EVENT_PARAM_END_ADJUST | CLAP_EVENT_PARAM_SHOULD_RECORD;
        event.param_value.value       = value;
        out_events->push_back(out_events,&event);
      }
    }
  }

  //----------

  void process_input_events(const clap_event_list_t* in_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_t* event = in_events->get(in_events,i);
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
          if (MEditor && MIsEditorOpen) {
            MEditor->queueEditorParam(event->param_value.param_id,event->param_value.value);
          }
          switch (event->param_value.param_id) {
            case 0:
              MGain = event->param_value.value;
              break;
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

  void process_output_events(const clap_event_list_t* out_events) {
    if (out_events) {
      flushHostParams(out_events);
    }
  }

//------------------------------
public:
//------------------------------

  // gui thread

  void do_editor_updateParameter(uint32_t AIndex, float AValue) final {
    queueHostParam(AIndex,AValue);
    if (!MIsProcessing) MHost->params_request_flush();
    switch (AIndex) {
      case 0:
        MGain = AValue;
        break;
      case 1:
        MFilter = AIndex;
        MVoices.handle_master_param(1,AValue);
        break;
    }
  }

//------------------------------
public: // clap plugin
//------------------------------

  /*
    Must be called after creating the plugin.
    If init returns false, the host must destroy the plugin instance.
  */

  bool init() final {
    return true;
  }

  //----------

  /*
    Free the plugin and its resources.
    It is not required to deactivate the plugin prior to this call.
  */

  void destroy() final {
  }

  //----------

  /*
    Activate and deactivate the plugin.
    In this call the plugin may allocate memory and prepare everything needed
    for the process call. The process's sample rate will be constant and
    process's frame count will included in the [min, max] range, which is
    bounded by [1, INT32_MAX].
    Once activated the latency and port configuration must remain constant,
    until deactivation.
    [main-thread]
  */

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MIsActivated = true;
    MVoices.prepare(sample_rate);
    return true;
  }

  //----------

  void deactivate() final {
    MIsActivated = false;
  }

  //----------

  /*
    Set to true before processing, and to false before sending the plugin to
    sleep.
    [audio-thread]
  */

  bool start_processing() final {
    MIsProcessing = true;
    return true;
  }

  //----------

  void stop_processing() final {
    MIsProcessing = false;
  }

  //----------

  /*
    process audio, events, ...
    [audio-thread]
  */

  clap_process_status process(const clap_process_t* process) final {
    process_input_events(process->in_events);
    MVoices.process(process);
    MIP_ScaleStereoBuffer(process->audio_outputs->data32,MGain,MGain,process->frames_count);
    process_output_events(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------
  /*
    Query an extension.
    The returned pointer is owned by the plugin.
    [thread-safe]
  */

  // see MIP_ClapPlugin

  //const void* get_extension(const struct clap_plugin *plugin, const char *id) final {
  //  return nullptr;
  //}

  //----------
  /*
    Called by the host on the main thread in response to a previous call to:
    host->request_callback(host);
    [main-thread]
  */

   void on_main_thread() final {
   }

//------------------------------
public: // audio ports
//------------------------------

/*
  This extension provides a way for the plugin to describe its current audio
  ports. If the plugin does not implement this extension, it will have a
  default stereo input and output. The plugin is only allowed to change its
  ports configuration while it is deactivated.
*/

//------------------------------

  /*
    number of ports, for either input or output
    [main-thread]
  */

  uint32_t audio_ports_count(bool is_input) final {
    if (is_input) return 1;
    else return 1;
  }

  //----------

  /*
    get info about about an audio port.
    [main-thread]
  */

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
public: // audio ports config
//------------------------------

  //uint32_t audio_ports_config_count() final {
  //  return 0;
  //}

  //----------

  //bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t* config) final {
  //  config->id
  //  config->name
  //  config->input_channel_count
  //  config->input_channel_map
  //  config->output_channel_count
  //  config->output_channel_map
  //  return false;
  //}

  //----------

  //bool audio_ports_config_select(clap_id config_id) final {
  //  return false;
  //}

//------------------------------
public: // event filter
//------------------------------

/*
  This extension lets the host know which event types the plugin is interested
  in. The host will cache the set of accepted events before activating the
  plugin. The set of accepted events can't change while the plugin is active.
*/

//------------------------------

  /*
    Returns true if the plugin is interested in the given event type.
    [main-thread]
  */

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

/*
  This extension is split in two interfaces:
  - `gui` which is the generic part
  - `gui_XXX` which is the platform specific interfaces; @see clap_gui_win32.
  Showing the GUI works as follow:
  1. clap_plugin_gui->create(), allocates gui resources
  2. clap_plugin_gui->set_scale()
  3. clap_plugin_gui->get_size(), gets initial size
  4. clap_plugin_gui_win32->embed(), or any other platform specific interface
  5. clap_plugin_gui->show()
  6. clap_plugin_gui->hide()/show() ...
  7. clap_plugin_gui->destroy() when done with the gui
  Resizing the window:
  1. Only possible if clap_plugin_gui->can_resize() returns true
  2. Mouse drag -> new_size
  3. clap_plugin_gui->round_size(new_size) -> working_size
  4. clap_plugin_gui->set_size(working_size)
*/

//------------------------------

  // Create and allocate all resources necessary for the gui.
  // After this call, the GUI is ready to be shown but it is not yet visible.
  // [main-thread]

  // we don't have a window yet!

  bool gui_create() final {
    MEditor = new myEditor( params_count() );
    MEditor->setListener(this);
    return true;
  }

  //----------

  // Free all resources associated with the gui.
  // [main-thread]

  void gui_destroy() final {
    if (MIsEditorOpen) {
      gui_hide();
    }
    delete MEditor;
  }

  //----------

  // Set the absolute GUI scaling factor.
  // [main-thread]

  void gui_set_scale(double scale) final {
    MEditor->setScale(scale);
  }

  //----------

  // Get the current size of the plugin UI, with the scaling applied.
  // clap_plugin_gui->create() must have been called prior to asking the size.
  // [main-thread]

  bool gui_get_size(uint32_t *width, uint32_t *height) final {
    *width = MEditor->getWidth();
    *height = MEditor->getHeight();
    return true;
  }

  //----------

  // [main-thread]

  bool gui_can_resize() final {
    return false;
  }

  //----------

  /*
    If the plugin gui is resizable, then the plugin will calculate the closest
    usable size to the given arguments.
    The scaling is applied.
    This method does not change the size.
    [main-thread]
  */

  void gui_round_size(uint32_t *width, uint32_t *height) final {
  }

  //----------

  /*
    Sets the window size
    Returns true if the size is supported.
    [main-thread]
  */

  bool gui_set_size(uint32_t width, uint32_t height) final {
    MEditor->setSize(width,height);
    return true;
  }

  //----------

  /*
    Show the window.
    [main-thread]
  */

  void gui_show() final {
    MIsEditorOpen = true;
    MEditor->show();
    MHost->timer_support_register_timer(30,&MTimerId);
  }

  //----------

  /*
    Hide the window, this method do not free the resources, it just hides
    the window content. Yet it maybe a good idea to stop painting timers.
    [main-thread]
  */

  void gui_hide() final {
    MIsEditorOpen = false;
    MEditor->hide();
    MHost->timer_support_unregister_timer(MTimerId);
  }

//------------------------------
public: // gui x11
//------------------------------

  /*
    Use the protocol XEmbed
    https://specifications.freedesktop.org/xembed-spec/xembed-spec-latest.html
    [main-thread]
  */

  bool gui_x11_attach(const char *display_name, unsigned long window) final {
    if (MEditor->attach(display_name,window)) {
      return true;
    }
    return false;
  }

//------------------------------
public: // latency
//------------------------------

  //uint32_t latency_get() final {
  //  return 0;
  //}

//------------------------------
public: // note name
//------------------------------

  //uint32_t note_name_count() final {
  //  return 0;
  //}

  //----------

  //bool note_name_get(uint32_t index, clap_note_name_t* note_name) final {
  //  return false;
  //}

//------------------------------
public: // midi mappings
//------------------------------

  //uint32_t midi_mappings_count() final {
  //  return 0;
  //}

  //----------

  //bool midi_mappings_get(uint32_t index, clap_midi_mapping_t* mapping) final {
  //  return false;
  //}

//------------------------------
public: // note ports
//------------------------------

  //uint32_t note_ports_count(bool is_input) final {
  //  return 0;
  //}

  //----------

  //bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t* info) final {
  //  return false;
  //}

//------------------------------
public: // params
//------------------------------

/*
  @page Parameters
  @brief parameters management

  Main idea:

  The host sees the plugin as an atomic entity; and acts as a controler on top of its parameters.
  The plugin is responsible to keep in sync its audio processor and its GUI.

  The host can read at any time parameters value on the [main-thread] using
  @ref clap_plugin_params.value().

  There is two options to communicate parameter value change, and they are not concurrent.
  - send automation points during clap_plugin.process()
  - send automation points during clap_plugin_params.flush(), this one is used when the plugin is
    not processing

  When the plugin changes a parameter value, it must inform the host.
  It will send @ref CLAP_EVENT_PARAM_VALUE event during process() or flush().
  - set the flag CLAP_EVENT_PARAM_BEGIN_ADJUST to mark the begining of automation recording
  - set the flag CLAP_EVENT_PARAM_END_ADJUST to mark the end of automation recording
  - set the flag CLAP_EVENT_PARAM_SHOULD_RECORD if the event should be recorded

  @note MIDI CCs are a tricky because you may not know when the parameter adjustment ends.
  Also if the hosts records incoming MIDI CC and parameter change automation at the same time,
  there will be a conflict at playback: MIDI CC vs Automation.
  The parameter automation will always target the same parameter because the param_id is stable.
  The MIDI CC may have a different mapping in the future and may result in a different playback.

  When a MIDI CC changes a parameter's value, set @ref clap_event_param.should_record to false.
  That way the host may record the MIDI CC automation, but not the parameter change and there
  won't be conflict at playback.

  Scenarios:

  I. Loading a preset
  - load the preset in a temporary state
  - call @ref clap_host_params.changed() if anything changed
  - call @ref clap_host_latency.changed() if latency changed
  - invalidate any other info that may be cached by the host
  - if the plugin is activated and the preset will introduce breaking change
    (latency, audio ports, new parameters, ...) be sure to wait for the host
    to deactivate the plugin to apply those changes.
    If there are no breaking changes, the plugin can apply them them right away.
    The plugin is resonsible to update both its audio processor and its gui.

  II. Turning a knob on the DAW interface
  - the host will send an automation event to the plugin via a process() or flush()

  III. Turning a knob on the Plugin interface
  - if the plugin is not processing, call clap_host_params->request_flush() or
    clap_host->request_process().
  - send an automation event and don't forget to set begin_adjust, end_adjust and should_record
    attributes
  - the plugin is responsible to send the parameter value to its audio processor

  IV. Turning a knob via automation
  - host sends an automation point during clap_plugin->process() or clap_plugin_params->flush().
  - the plugin is responsible to update its GUI

  V. Turning a knob via plugin's internal MIDI mapping
  - the plugin sends a CLAP_EVENT_PARAM_SET output event, set should_record to false
  - the plugin is responsible to update its GUI

  VI. Adding or removing parameters
  - if the plugin is activated call clap_host->restart()
  - once the plugin isn't active:
    - apply the new state
    - call clap_host_params->rescan(CLAP_PARAM_RESCAN_ALL)
    - if a parameter is created with an id that may have been used before,
      call clap_host_params.clear(host, param_id, CLAP_PARAM_CLEAR_ALL)
*/

//------------------------------

  /*
    Returns the number of parameters.
    [main-thread]
  */

  uint32_t params_count() final {
    return 2;
  }

  //----------

  /*
    Copies the parameter's info to param_info and returns true on success.
    [main-thread]
  */

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

  /*
    Gets the parameter plain value.
    [main-thread]
  */

  bool params_get_value(clap_id param_id, double *value) final {
    switch (param_id) {
      case 0:
        *value = MGain;
        return true;
      case 1:
        *value = MFilter;
        return true;
    }
    return false;
  }

  //----------

  /*
    Formats the display text for the given parameter value.
    The host should always format the parameter value to text using this function
    before displaying it to the user.
    [main-thread]
  */

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) final {
    MIP_FloatToString(display,value,3);
    return true;
  }

  //----------

  /*
    Converts the display text to a parameter value.
    [main-thread]
  */

  bool params_text_to_value(clap_id param_id, const char *display, double *value) final {
    *value = MIP_StringToFloat((char*)display);
    return true;
  }

  //----------

  /*
    Flushes a set of parameter changes.
    This method must not be called concurrently to clap_plugin->process().
    This method must not be used if the plugin is processing.
    [active && !processing : audio-thread]
    [!active : main-thread]
  */

  void params_flush(const clap_event_list_t *input_parameter_changes, const clap_event_list_t *output_parameter_changes) final {
    MIP_PRINT;
    process_input_events(input_parameter_changes);
    process_output_events(output_parameter_changes);
  }

//------------------------------
public: // state
//------------------------------

  /*
    Saves the plugin state into stream.
    Returns true if the state was correctly saved.
    [main-thread]
  */

  bool state_save(clap_ostream_t *stream) final {
    stream->write(stream,&MGain,sizeof(float));
    stream->write(stream,&MFilter,sizeof(float));
    return true;
  }

  //----------

  /*
    Loads the plugin state from stream.
    Returns true if the state was correctly restored.
    [main-thread]
  */

  bool state_load(clap_istream_t *stream) final {
    stream->read(stream,&MGain,sizeof(float));
    stream->read(stream,&MFilter,sizeof(float));
    return true;
  }

//------------------------------
public: // timner support
//------------------------------

  void timer_support_on_timer(clap_id timer_id) final {
    if (MEditor) MEditor->flushEditorParams();
  }

//------------------------------
public: // track info
//------------------------------

  //void track_info_changed() {
  //}

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
