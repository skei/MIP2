
#define MIP_GUI_XCB
#define MIP_PAINTER_NANOVG

#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/widgets/mip_widgets.h"

//----------------------------------------------------------------------

const clap_plugin_descriptor_t gain_descriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "skei.audio/gain/0",
   .name          = "gain",
   .vendor        = "skei.audio",
   .url           = "https://torhelgeskei.com",
   .manual_url    = "",
   .support_url   = "",
   .version       = "0.0.1",
   .description   = "",
   .features      = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------

class gain_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  enum gain_parameter_enums {
    PAR_GAIN = 0,
    PARAM_COUNT
  };

  const clap_param_info_t gain_parameters[PARAM_COUNT] = {
    { PAR_GAIN, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Gain", "", 0, 1, 0 }
  };

  const clap_audio_port_info_t gain_audioInputPorts[1] = {
    { 0, "audio in 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_audio_port_info_t gain_audioOutputPorts[1] = {
    { 0, "audio out 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_note_port_info_t gain_noteInputPorts[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note in 1" }
  };

  const clap_note_port_info_t gain_noteOutputPorts[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note out 1" }
  };

  //----------

  double par_gain = 0.0;

//------------------------------
public:
//------------------------------

  gain_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MEditorWidth = 420;
    MEditorHeight = 620;
  }

  //----------

  virtual ~gain_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendAudioInputPort( &gain_audioInputPorts[0] );
    appendAudioOutputPort(&gain_audioOutputPorts[0]);
    appendNoteInputPort(  &gain_noteInputPorts[0]  );
    appendNoteOutputPort( &gain_noteOutputPorts[0] );
    for (uint32_t i=0; i<PARAM_COUNT; i++) {
      appendParameter( new MIP_Parameter(&gain_parameters[i]) );
    }
    return true;
  }

  //----------

  void processParamValueEvent(const clap_event_param_value_t* event) final {
    MIP_Plugin::processParamValueEvent(event);
    if (event->param_id == 0) {
      par_gain = event->value;
    }
  }

  //----------

  void processAudioBlock(const clap_process_t* process) final {
    uint32_t len = process->frames_count;
    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      *out0++ = *in0++ * par_gain;
      *out1++ = *in1++ * par_gain;
    }
  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) override {
    MIP_Plugin::gui_create(api,is_floating);
    MEditor->setWindowFillBackground(false);
    MIP_ColorWidget* background = new MIP_ColorWidget( MIP_DRect( 0,0,420,620), MIP_COLOR_GRAY );
    background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    background->Layout.border = MIP_DRect(10,10,10,10);
    MEditor->appendChildWidget(background);
    MIP_Knob2Widget* gain_knob = new MIP_Knob2Widget( MIP_DRect( 1,1 ), "Gain", 0.0 );
    gain_knob->Layout.alignment = MIP_WIDGET_ALIGN_CENTER;
    gain_knob->Layout.sizeModeX = MIP_WIDGET_SIZE_MODE_RATIO;
    gain_knob->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_RATIO;
    gain_knob->Layout.aspectRatio = (4.0 / 6.0);
    background->appendChildWidget(gain_knob);
    MEditor->connectWidget(MParameters[0],gain_knob);
    return true;
  }

};

//----------------------------------------------------------------------

#include "plugin/mip_registry.h"

#include "plugin/clap/mip_clap_entry.h"
#include "plugin/exe/mip_exe_entry.h"
//#include "plugin/vst2/mip_vst2_entry.h"
//#include "plugin/vst3/mip_vst3_entry.h"

//----------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&gain_descriptor);
};

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new gain_plugin(ADescriptor,AHost);
  }
  return nullptr;
}
