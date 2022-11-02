
// sacred geometry, golden spiral, fibonacci
// doesn't look at note_id..


//#define MIP_NO_GUI
//#define MIP_NO_PAINTER

#ifdef __gnu_linux__
  #define MIP_GUI_XCB
  #define MIP_PAINTER_NANOVG
  //#define MIP_PAINTER_XCB
  //#define MIP_WINDOW_BUFFERED
#else
  #define MIP_GUI_WIN32
  #define MIP_PAINTER_NANOVG
#endif

#define MIP_PLUGIN_GENERIC_EDITOR

#ifndef MIP_EXE
  //#define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------------------------------------------------------------------


#include "mip.h"
#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "skei.audio/sa_cred/0.0.1",
   .name          = "sa_cred",
   .vendor        = "skei.audio",
   .url           = "",
   .manual_url    = "",
   .support_url   = "",
   .version       = "0.0.1",
   .description   = "Fibonacci/Pisano Note Modifier",
   .features      =  (const char *[]){ CLAP_PLUGIN_FEATURE_NOTE_EFFECT, "Hello world!", nullptr }
};

//----------

const clap_param_info_t myParameters[] = {
  { 0, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED, nullptr, "Value 1", "", -12, 12, 0  },
  { 1, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED, nullptr, "Value 2", "", -12, 12, 1  },
  { 2, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED, nullptr, "Modulo",  "",  1,  24, 12 }
};

//const clap_audio_port_info_t myAudioInputPorts[] = {
//  { 0, "input1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
//};

//const clap_audio_port_info_t myAudioOutputPorts[] = {
//  { 0, "output1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
//};

const clap_note_port_info_t myNoteInputPorts[] = {
  { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "input1" }
};

const clap_note_port_info_t myNoteOutputPorts[] = {
  { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "output1" }
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

  int32_t par_value1            = 0;
  int32_t par_value2            = 1;

  int32_t MNoteOffsets[16*128]  = {0};
  int32_t MValue1               = 0;
  int32_t MValue2               = 1;
  int32_t MModulo               = 12;
  bool    MWasPlaying           = false;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public: // fibonacci
//------------------------------

  int32_t get_fibo() {
    return MValue1;
  }

  //----------

  void next_fibo() {
    int32_t v = (MValue1 + MValue2) % MModulo;
    MValue2 = MValue1;
    MValue1 = v;
  }

  //----------

  void restart_fibo() {
    MValue1 = par_value1;
    MValue2 = par_value2;
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendNoteInputPort(  &myNoteInputPorts[0]  );
    appendNoteOutputPort( &myNoteOutputPorts[0] );
    appendParameter( new MIP_Parameter(&myParameters[0]) );
    appendParameter( new MIP_Parameter(&myParameters[1]) );
    appendParameter( new MIP_Parameter(&myParameters[2]) );
    return true;
  }

  //----------

  void processNoteOn(const clap_event_note_t* event) final {
    MIP_Assert( MProcessContext.process );
    const clap_output_events_t *out_events = MProcessContext.process->out_events;
    int32_t channel = event->channel;
    int32_t key = event->key;
    int32_t value = get_fibo();
    MNoteOffsets[(channel * 128) + key] = value;
    clap_event_note_t new_event;// = {0};
    //memset(&new_event,0,sizeof(clap_event_note_t));
    memcpy(&new_event,event,sizeof(clap_event_note_t));
    new_event.key += value;
    out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
    next_fibo();
  }

  //----------

  void processNoteOff(const clap_event_note_t* event) final {
    MIP_Assert( MProcessContext.process );
    const clap_output_events_t *out_events = MProcessContext.process->out_events;
    int32_t channel = event->channel;
    int32_t key = event->key;
    MIP_Assert( (channel >= 0) && (channel <= 15) );
    MIP_Assert( (key >= 0) && (key <= 127) );
    int32_t value = MNoteOffsets[(channel * 128) + key];
    clap_event_note_t new_event;// = {0};
    //memset(&new_event,0,sizeof(clap_event_note_t));
    memcpy(&new_event,event,sizeof(clap_event_note_t));
    new_event.key += value;
    out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
  }

  //----------

  void processNoteChoke(const clap_event_note_t* event) final {
    MIP_Assert( MProcessContext.process );
    const clap_output_events_t *out_events = MProcessContext.process->out_events;
    int32_t channel = event->channel;
    int32_t key = event->key;
    MIP_Assert( (channel >= 0) && (channel <= 15) );
    MIP_Assert( (key >= 0) && (key <= 127) );
    int32_t value = MNoteOffsets[(channel*128)+key];
    clap_event_note_t new_event;// = {0};
    //memset(&new_event,0,sizeof(clap_event_note_t));
    memcpy(&new_event,event,sizeof(clap_event_note_t));
    new_event.key += value;
    out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
  }

  //----------

  void processNoteExpression(const clap_event_note_expression_t* event) final {
    MIP_Assert( MProcessContext.process );
    const clap_output_events_t *out_events = MProcessContext.process->out_events;
    int32_t channel = event->channel;
    int32_t key = event->key;
    MIP_Assert( (channel >= 0) && (channel <= 15) );
    MIP_Assert( (key >= 0) && (key <= 127) );
    int32_t value = MNoteOffsets[(channel*128)+key];
    clap_event_note_expression_t new_event;// = {0};
    //memset(&new_event,0,sizeof(clap_event_note_expression_t));
    memcpy(&new_event,event,sizeof(clap_event_note_expression_t));
    new_event.key += value;
    out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
  }

  //----------

  void processParamValue(const clap_event_param_value_t* event) final {
    //MIP_Plugin::processParamValueEvent(event);
    uint32_t index = event->param_id;
    double value = event->value;
    switch (index) {
      case 0:
        par_value1 = value;
        restart_fibo();
        break;
      case 1:
        par_value2 = value;
        restart_fibo();
        break;
      case 2:
        MModulo = value;
        break;
    }
  }

  //----------

  void processTransport(const clap_event_transport_t* transport) final {
    bool is_playing = (transport->flags & CLAP_TRANSPORT_IS_PLAYING);
    if (is_playing && !MWasPlaying) {
      MIP_Print("start\n");
      // do something when we start...
    }
    MWasPlaying = is_playing;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap_entry.h"
//#include "plugin/exe/mip_exe_entry.h"
//#include "plugin/vst2/mip_vst2_entry.h"
#include "plugin/vst3/mip_vst3_entry.h"


void MIP_Register() {
  MIP_REGISTRY.appendDescriptor(&myDescriptor);
};

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
