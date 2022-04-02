#ifndef sa_fibonacci_included
#define sa_fibonacci_included
//----------------------------------------------------------------------

/*
  changes notes based on the fibonacci series

  version history:
  0: initial release (preview)
  1: changed from note-creation to note-modification
  2: adapted to skeilib
  3: -> MIP2
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------

static const char* txt_dir[] = {"positive","negative"};

//----------

const char* sa_fibonacci_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_fibonacci_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_fibonacci",
  "sa_fibonacci",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_fibonacci",
  sa_fibonacci_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_fibonacci_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc = false;
  float MSampleRate = 0.0;

    int32_t MNoteBuffer[128*16];
    int32_t MChanBuffer[128*16];
    int32_t MNum1;
    int32_t MNum2;

    int32_t p_num1;
    int32_t p_num2;
    int32_t p_mod;
    int32_t p_dir;

//------------------------------
public:
//------------------------------

  sa_fibonacci_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_fibonacci_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {
    need_recalc = false;
  }

  //----------

    void _fib_init(void) {
      //KTrace("_fib_init\n");
      MNum1 = p_num1;
      MNum2 = p_num2;
      for (int32_t chan=0; chan<16; chan++) {
        for (int32_t note=0; note<128; note++) {
          int32_t index = (chan*128)+note;
          MNoteBuffer[index] = -1;
          MChanBuffer[index] = 0;
        }
      }
      //KTrace("_fib_init ok\n");
    }

    //----------

    /*
      aha!
      midi is processed before process(), and since we check the transport in the
      start of process, there is already a note playing, and it will be shut off
    */

    void _fib_reset(void) {
      //KTrace("_fib_reset\n");
      MNum1 = p_num1;
      MNum2 = p_num2;
      for (int32_t chan=0; chan<16; chan++) {
        for (int32_t note=0; note<128; note++) {
          int32_t index = (chan*128)+note;
//          int32_t n = MNoteBuffer[index];
//          int32_t c = MChanBuffer[index];
//          if (n != -1) sendMidiToHost(0,0x80+c,n,0); // $80 = note off
          MNoteBuffer[index] = -1;
          MChanBuffer[index] = 0;
        }
      }
      //KTrace("_fib_reset ok\n");
    }

    //----------

    int32_t _fib_next(void) {
      //KTrace("_fib_next\n");
      int32_t next;
      if (p_dir < 1) next = (MNum1 + MNum2) % p_mod;
      else next = (MNum1 - MNum2) % p_mod;
      int32_t result = MNum1 % p_mod; // mod to catch 1-note length series.. 0,1,<next>
      MNum1 = MNum2; // 0 -> 1
      MNum2 = next;  // 1 -> 1
      //KTrace("_fib_next ok\n");
      return result;
    }

    //----------

    void _noteOn(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
      //KTrace("_noteOn\n");
      int32_t chan = AMsg1 & 0x0f;
      int32_t note = AMsg2 & 0x7f;
      int32_t index = (chan*128)+note;
      int32_t n = note + _fib_next();
      int32_t c = chan;
      MNoteBuffer[index] = n;
      MChanBuffer[index] = c;
//      sendMidiToHost(AOffset,0x90+c,n,AMsg3); // $90 = note on
      //KTrace("_noteOn ok\n");
    }

    //----------

    void _noteOff(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
      //KTrace("_noteOff\n");
      int32_t chan = AMsg1 & 0x0f;
      int32_t note = AMsg2 & 0x7f;
      int32_t index = (chan*128)+note;
//      int32_t n = MNoteBuffer[index];
//      int32_t c = MChanBuffer[index]; // AMsg1 and 15;
//      if (n != -1) sendMidiToHost(AOffset,0x80+c,n,AMsg3); // $80 = note off
//      else sendMidiToHost(AOffset,AMsg1,AMsg2,AMsg3);
      MNoteBuffer[index] = -1;
      MChanBuffer[index] = 0;
      //KTrace("_noteOff ok\n");
    }


//------------------------------
private:
//------------------------------

/*
    void on_parameterChange(int32 AIndex, float AValue) final {
      float v = AValue;
      int32 i = KTrunc(v);
      switch(AIndex) {
        case 0:
          p_num1 = i;
          MNum1 = i;
          break;
        case 1:
          p_num2 = i;
          MNum2 = i;
          break;
        case 2:
          p_mod = i;
          _fib_reset();
          break;
        case 3:
          p_dir = i;
          _fib_reset();
          break;
      }
    }
*/

  void handle_input_events(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        switch (header->type) {
          case CLAP_EVENT_NOTE_ON:
            //handle_note_on_event((clap_event_note_t*)header);
            break;
          case CLAP_EVENT_NOTE_OFF:
            //handle_note_off_event((clap_event_note_t*)header);
            break;
          case CLAP_EVENT_PARAM_VALUE:
            handle_parameter_event((clap_event_param_value_t*)header);
            need_recalc = true;
            break;
          //case CLAP_EVENT_NOTE_END:
          //case CLAP_EVENT_NOTE_CHOKE:
          //case CLAP_EVENT_NOTE_EXPRESSION:
          //case CLAP_EVENT_MIDI:
          //case CLAP_EVENT_MIDI2:
          //case CLAP_EVENT_MIDI_SYSEX:
        }
      }
    }
  }

  //void handle_parameter_event(const clap_event_param_value_t* param_value) final {
  //  MIP_Plugin::handle_parameter_event(param_value);
  //  need_recalc = true;
  //}

  //----------

/*
    //virtual
    void on_midiEvents(KMidiEvent* AEvents, uint32 ANum) final {
      for (uint32 i=0; i<ANum; i++) {
        KTrace("midi events: %i\n",ANum);
        uint32 ofs  = AEvents[i].offset;
        uint8  msg1 = AEvents[i].msg1;
        uint8  msg2 = AEvents[i].msg2;
        uint8  msg3 = AEvents[i].msg3;
        on_midiEvent(ofs,msg1,msg2,msg3);
      }
    }

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) final {
      switch(AMsg1 & 0xF0) {
        case 0x80:
          _noteOff(AOffset,AMsg1,AMsg2,AMsg3);
          break;
        case 0x90:
          _noteOn(AOffset,AMsg1,AMsg2,AMsg3);
          break;
        default:
          sendMidiToHost(AOffset,AMsg1,AMsg2,AMsg3);
          break;
      }
    }
*/

  //----------

  void handle_process(const clap_process_t *process) final {
    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      *out0++ = *in0++;
      *out1++ = *in1++;
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendParameter(new MIP_IntParameter(  0, CLAP_PARAM_IS_AUTOMATABLE, "Num 1",     "", -24, 24,  0          ));
    appendParameter(new MIP_IntParameter(  1, CLAP_PARAM_IS_AUTOMATABLE, "Num 2",     "", -24, 24,  1          ));
    appendParameter(new MIP_IntParameter(  2, CLAP_PARAM_IS_AUTOMATABLE, "Modulo",    "",  1,  48,  12         ));
    appendParameter(new MIP_TextParameter( 3, CLAP_PARAM_IS_AUTOMATABLE, "Direction", "",  0,   1,  0, txt_dir ));
    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues();
      need_recalc = true;
      return true;
    }
    return false;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

};

//----------------------------------------------------------------------
#endif
