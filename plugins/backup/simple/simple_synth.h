
// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG_PRINT_SOCKET
//#define MIP_DEBUG_PRINT_THREAD
//#define MIP_DEBUG_PRINT_TIME
//#define MIP_LIB

#define MIP_NO_GUI
#define MIP_PLUGIN_CLAP

#include "mip.h"
#include "plugin/mip_plugin_descriptor.h"
#include "plugin/mip_plugin_instance.h"
#include "plugin/mip_plugin_host.h"
//#include "plugin/mip_plugin.h"
#include "audio/mip_voice_manager.h"

#include "plugin/clap/mip_clap_plugin_host.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public MIP_PluginDescriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : MIP_PluginDescriptor() {
    setName("myPlugin");
    setAuthor("mySelf");
    setVersion(0x00000001);
    appendInputPort("Input",2);
    appendOutputPort("Output",2);
    appendParameter("Gain",0.5);
    setIsSynth();
  }

};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice;
typedef MIP_VoiceManager<myVoice,16> myVoiceManager;

//----------

class myVoice {

//private:
public:

  MIP_VoiceListener* MListener = nullptr;

  // events
  float     MNote         = 0.0f;
  float     MBend         = 0.0f;
  float     MMasterBend   = 0.0f;
  float     MMasterPress  = 0.0f;
  uint32_t  MChan         = 0;

  // synth
  float srate = 0.0f; // sample rate
  float ph    = 0.0f; // phase
  float phadd = 0.0f; // phase add

public:

  void prepare(float ASampleRate) {
    srate = ASampleRate;
  }

public:

  uint32_t strike(float note, float vel, uint32_t chan) {
    MNote = note;
    MBend = 0.0f;
    MChan = chan;
    float hz = MIP_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    ph = 0.0f;
    phadd = hz / srate;
    return MIP_VOICE_PLAYING;
  }

  uint32_t lift(float vel, uint32_t chan) {
    return MIP_VOICE_FINISHED;
  }


  void bend(float v) {
    MBend = v;
    float hz = MIP_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    phadd = hz / srate;
  }

  void press(float v) {
  }

  void slide(float v) {
  }

  void ctrl(uint32_t i, uint32_t v) {
  }

  void master_bend(float mb) {
    MMasterBend = mb;
    float hz = MIP_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    phadd = hz / srate;
  }

  void master_press(float mp) {
    MMasterPress = mp;
  }

  void master_ctrl(uint32_t i, uint32_t v) {
  }

  void parameter(uint32_t i, float v) {
  }

  uint32_t process(MIP_VoiceContext* AContext, uint32_t AMode) {
    uint32_t num = AContext->processContext->blocksize;
    float* out0 = AContext->processContext->outputs[0];
    float* out1 = AContext->processContext->outputs[1];
    for (uint32_t i=0; i<num; i++) {
      float v = (ph * 2.0f) - 1.0f;
      ph += phadd;
      ph = MIP_Fract(ph);
      *out0++ += v * 0.1f;
      *out1++ += v * 0.1f;
    }
    return AMode;
  }

};


//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public MIP_PluginInstance
, public MIP_VoiceListener {

//------------------------------
private:
//------------------------------

  float           MGain   = 0.0;
  myVoiceManager  MVoices = {};

//------------------------------
public:
//------------------------------

  myInstance(MIP_PluginDescriptor* ADescriptor)
  : MIP_PluginInstance(ADescriptor) {
  }

//------------------------------
public:
//------------------------------

  void on_voice_end(MIP_ProcessContext* AContext, uint32_t time, uint32_t port, uint32_t key, uint32_t chan) final {
    MIP_PRINT;
    clap_event event;
    event.type            = CLAP_EVENT_NOTE_END;
    event.time            = time;
    event.note.port_index = port;
    event.note.key        = key;
    event.note.channel    = chan;
    event.note.velocity   = 0.0;
    const clap_process* process = (const clap_process*)AContext->formatptr;
    process->out_events->push_back(process->out_events,&event);
  }

//------------------------------
public:
//------------------------------

  bool on_plugin_init() final {
    MVoices.setListener(this);
    return true;
  }

  void on_plugin_deinit() final {
  }

  bool on_plugin_activate(float ASampleRate, uint32_t AMinFrames, uint32_t AMaxFrames) final {
    MVoices.prepare(ASampleRate);
    return true;
  }

  void on_plugin_deactivate() final {
  }

  bool on_plugin_startProcessing() final {
    return true;
  }

  void on_plugin_stopProcessing() final {
  }

  //----------

  uint32_t on_plugin_process(MIP_ProcessContext* AContext) final {
    //MVoices.setUserPtr(AContext->userptr);
    MVoices.processBlock(AContext);
    MIP_ScaleStereoBuffer(AContext->outputs, MGain, MGain, AContext->blocksize);
    return 1;
  }

  //----------

  void on_plugin_parameter(uint32_t AIndex, float AValue) final {
    MVoices.parameter(AIndex,AValue);
    if (AIndex == 0) {
      MGain = (AValue * AValue);
    }
  }

  void on_plugin_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
    MVoices.midi(AMsg1,AMsg2,AMsg3);
  }

  void on_plugin_expression(uint32_t AKey, uint32_t AExpression, float AValue) final {
  }

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins() {
  MIP_PLUGIN_LIST.appendPlugin( new myDescriptor() );
}

//----------

MIP_PluginInstance* MIP_CreateInstance(uint32_t AIndex, MIP_PluginDescriptor* ADescriptor) {
  if (AIndex == 0) return new myInstance(ADescriptor);
  else return nullptr;
}

//----------

MIP_MAIN;

