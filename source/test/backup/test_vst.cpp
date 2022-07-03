
//#define MIP_NO_GUI
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

#define EDITOR_WIDTH  640
#define EDITOR_HEIGHT 480

#define MIP_VST2
#define MIP_VST3

//----------

#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* template_features[] = {
  "audio_effect",
  nullptr
};

//----------



const clap_plugin_descriptor_t template_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "skei.audio/plugin_template",
  .name         = "plugin_template",
  .vendor       = "skei.audio",
  .url          = "https://torhelgeskei.com",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "plugin_template description",
  //template_features
  .features     = (const char*[]){
    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
    nullptr
  }
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class template_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc = false;
  float MSampleRate = 0.0;

  clap_param_info_t myParameters[1] = {
    { 0, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param1", "", 0.0, 1.0, 0.5 }
  };

  clap_audio_port_info_t myAudioInputs[1] = {
    { 0, "Audio In", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  clap_audio_port_info_t myAudioOutputs[1] = {
    { 0, "Audio Out", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

//------------------------------
public:
//------------------------------

  template_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~template_plugin() {
  }

//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    need_recalc = false;
  }

//------------------------------
private:
//------------------------------

  void handle_parameter_event(clap_event_param_value_t* param_value) final {
    MIP_Plugin::handle_parameter_event(param_value);
    need_recalc = true;
  }

  //----------

  clap_process_status process(const clap_process_t *process) final {
    //MIP_PRINT;
    flushAudioParams();
    preProcessEvents(process->in_events,process->out_events);
    //..
    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      *in0++ = *out0++;
      *in1++ = *out1++;
    }
    postProcessEvents(process->in_events,process->out_events);
    flushHostParams(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    setupParameters(myParameters,1);
    setupAudioInputs(myAudioInputs,1);
    setupAudioOutputs(myAudioOutputs,1);
    need_recalc = true;
    return MIP_Plugin::init();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  const void* get_extension(const char *id) final {
    if (strcmp(id,CLAP_EXT_GUI) == 0)         return &MGui;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0) return &MAudioPorts;
    return MIP_Plugin::get_extension(id);
  }

  //----------

  bool gui_create(const char *api, bool is_floating) final {
    MEditor = new MIP_Editor(this,this,EDITOR_WIDTH,EDITOR_HEIGHT,true);
    if (MEditor) {
      MIP_Window* window = MEditor->getWindow();
        MIP_PanelWidget* panel = new MIP_PanelWidget(MIP_FRect());
        panel->setFillBackground(true);
        panel->setBackgroundColor(MIP_COLOR_GRAY);
        panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

          MIP_Knob2Widget* knob = new MIP_Knob2Widget(MIP_FRect(50,80),"param");
          knob->layout.alignment = MIP_WIDGET_ALIGN_CENTER;
          panel->appendWidget(knob);

          MEditor->connect(knob,0);

      window->appendWidget(panel);
    }
    return (MEditor);
  }

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&template_descriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new template_plugin(ADescriptor,AHost);
  }
  return nullptr;
}
