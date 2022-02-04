
#define MIP_USE_XCB
#define MIP_GUI_XCB
//#define MIP_NO_WINDOW_BUFFERING
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

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
  "torhelgeskei/test/v0.0.0",
  #ifdef MIP_DEBUG
    "test_debug",
  #else
    "test",
  #endif
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
  "simple mip2 test plugin",
  myFeatures
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  clap_param_info_t
  MParameters[4] = {
    { 0, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param1", "", 0.0, 1.0, 0.5 },
    { 1, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param2", "", 0.0, 1.0, 0.5 },
    { 2, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param3", "", 0.0, 1.0, 0.5 },
    { 3, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param4", "", 0.0, 1.0, 0.5 }
  };

  clap_audio_port_info_t
  MAudioInputs[2] = {
    { 0, "input1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "input2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  clap_audio_port_info_t
  MAudioOutputs[2] = {
    { 0, "output1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "output2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  //----------

  uint32_t    MNumParams        = 0;
  uint32_t    MNumAudioInputs   = 0;
  uint32_t    MNumAudioOutputs  = 0;

  bool        MIsProcessing     = false;
  MIP_Editor* MEditor           = nullptr;

//------------------------------
public:
//------------------------------

  #define ARRAY_SIZE(x)     ( sizeof(x) / sizeof((x)[0]) )
  #define NUM_PARAMS        ARRAY_SIZE(MParameters)
  #define NUM_AUDIO_INPUTS  ARRAY_SIZE(MAudioInputs)
  #define NUM_AUDIO_OUTPUTS ARRAY_SIZE(MAudioInputs)

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost,NUM_PARAMS) {
    MNumParams        = NUM_PARAMS;
    MNumAudioInputs   = NUM_AUDIO_INPUTS;
    MNumAudioOutputs  = NUM_AUDIO_OUTPUTS;
    //MIP_Assert(MNumAudioOutputs > 0);
  }

  #undef ARRAY_SIZE

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
    if (MEditor) MEditor->flushHostParams(out_events);
  }

  //----------

  void handle_param_value(const clap_event_header_t* header) {
    const clap_event_param_value_t* param_value = (const clap_event_param_value_t*)header;
    uint32_t i = param_value->param_id;
    float v = param_value->value;
    setParamVal(i,v);
    //MIP_Assert(MEditor != nullptr);
    if (MEditor) MEditor->updateParameterFromHost(i,v);
  }

  //----------

  void handle_param_mod(const clap_event_header_t* header) {
    const clap_event_param_mod_t* param_mod = (const clap_event_param_mod_t*)header;
    uint32_t i = param_mod->param_id;
    float v = param_mod->amount;
    setParamMod(i,v);
    //MIP_Assert(MEditor != nullptr);
    if (MEditor) MEditor->updateModulationFromHost(i,v);
  }

  //----------

  void process_audio(const clap_process_t *process) {
    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<process->frames_count; i++) {
      float v = getParamVal(0) + getParamMod(0);
      v = MIP_Clamp(v,0,1);
      *out0++ = *in0++ * v;
      *out1++ = *in1++ * v;
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    for (uint32_t i=0; i<MNumParams; i++) {
      float v = MParameters[i].default_value;
      setParamVal(i,v);
      setParamMod(i,0);
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

    if (MEditor) {
      float v0 = MIP_Clamp( (MParamVal[0] + MParamMod[0]), 0,1);
      float v1 = MIP_Clamp( (MParamVal[1] + MParamMod[1]), 0,1);
      float v2 = MIP_Clamp( (MParamVal[2] + MParamMod[2]), 0,1);
      float v3 = MIP_Clamp( (MParamVal[3] + MParamMod[3]), 0,1);
      MEditor->send_param_mod(0,v0,process->out_events);
      MEditor->send_param_mod(1,v1,process->out_events);
      MEditor->send_param_mod(2,v2,process->out_events);
      MEditor->send_param_mod(3,v3,process->out_events);
    }

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
    //if (strcmp(id,CLAP_EXT_TIMER_SUPPORT) == 0) return &MTimerSupport;
    return nullptr;
  }

  //----------

  void on_main_thread() final {
  }

//------------------------------
public: // audio-ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) final {
    if (is_input) return MNumAudioInputs;
    else return MNumAudioOutputs;
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) final {
    if (is_input) {
      memcpy(info,&MAudioInputs[index],sizeof(clap_audio_port_info_t));
      return true;
    }
    else {
      memcpy(info,&MAudioOutputs[index],sizeof(clap_audio_port_info_t));
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
    MEditor = new MIP_Editor(this);
    return true;
  }

  //----------

  void gui_destroy() final {
    delete MEditor;
  }

  //----------

  bool gui_set_scale(double scale) final {
    return true;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) final {
    return MEditor->getSize(width,height);
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
    MIP_Window* win = MEditor->getWindow();

    win->setFillBackground();
    win->setBackgroundColor(0.6);

    MIP_PanelWidget* panel = new MIP_PanelWidget(MIP_FRect(100));
    panel->setBackgroundColor(0.6);
    panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    win->appendWidget(panel);

      MIP_KnobWidget* knob1 = new MIP_KnobWidget(MIP_FRect( 10,   10, 50,50 ));
      //knob1->layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      knob1->setValue(getParamVal(0));
      panel->appendWidget(knob1);

      MIP_KnobWidget* knob2 = new MIP_KnobWidget(MIP_FRect( 70,   10, 50,50 ));
      //knob2->layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      knob2->setValue(getParamVal(1));
      panel->appendWidget(knob2);

      MIP_KnobWidget* knob3 = new MIP_KnobWidget(MIP_FRect( 130,  10, 50,50 ));
      //knob3->layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      knob3->setValue(getParamVal(2));
      panel->appendWidget(knob3);

      MIP_KnobWidget* knob4 = new MIP_KnobWidget(MIP_FRect( 190,  10, 50,50 ));
      //knob4->layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      knob4->setValue(getParamVal(3));
      panel->appendWidget(knob4);

    MEditor->connect(knob1,0);
    MEditor->connect(knob2,1);
    MEditor->connect(knob3,2);
    MEditor->connect(knob4,3);

    MEditor->show();

  }

  //----------

  void gui_hide() final {
    MEditor->hide();
  }

//------------------------------
public: // gui-x11
//------------------------------

  bool gui_x11_attach(const char *display_name, unsigned long window) final {
    MEditor->attach(display_name,window);
    return true;
  }

//------------------------------
public: // params
//------------------------------

  uint32_t params_count() final {
    return MNumParams;
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) final {
    memcpy(param_info,&MParameters[param_index],sizeof(clap_param_info_t));
    return true;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) final {
    *value = getParamVal(param_id);
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

//------------------------------

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
