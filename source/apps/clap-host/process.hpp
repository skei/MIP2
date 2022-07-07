#ifndef process_included
#define process_included
//----------------------------------------------------------------------

#define NO_GUI

#define MAX_BLOCK_EVENTS  1024
#define MAX_BLOCK_SIZE    65536

#define MIDI_NOTE_OFF            0x80
#define MIDI_NOTE_ON             0x90
#define MIDI_POLY_AFTERTOUCH     0xA0
#define MIDI_CONTROL_CHANGE      0xB0
#define MIDI_PROGRAM_CHANGE      0xC0
#define MIDI_CHANNEL_AFTERTOUCH  0xD0
#define MIDI_PITCHBEND           0xE0
#define MIDI_SYS                 0xF0

//----------

#include <math.h>

#include "clap/clap.h"
#include "audio_file.hpp"
#include "midi_file.hpp"
#include "midi_player.hpp"

//----------

typedef std::vector<clap_event_header_t*> EventHeaders;

//----------------------------------------------------------------------

class Process {

//------------------------------
private:
//------------------------------

  const clap_plugin_t*  MPlugin           = nullptr;

  AudioFile             MAudioInputFile   = {};
  AudioFile             MAudioOutputFile  = {};
  MidiFile              MMidiFile         = {};
  MidiPlayer            MMidiPlayer       = {};
  float                 MSampleRate       = 0.0;
  uint32_t              MCurrentSample    = 0;
  float                 MCurrentTime      = 0.0;
  MidiSequence*         MMidiSequence     = nullptr;
  MidiEvents            MMidiEvents       = {};
  EventHeaders          MClapEvents       = {};
  clap_param_info       MRemapParamInfo   = {0};

//------------------------------
private:
//------------------------------

  alignas(32) float   MAudioInputBuffer1[MAX_BLOCK_SIZE];
  alignas(32) float   MAudioInputBuffer2[MAX_BLOCK_SIZE];
  alignas(32) float   MAudioOutputBuffer1[MAX_BLOCK_SIZE];
  alignas(32) float   MAudioOutputBuffer2[MAX_BLOCK_SIZE];
  alignas(32) float*  MAudioInputBuffers[2] = { MAudioInputBuffer1, MAudioInputBuffer2 };
  alignas(32) float*  MAudioOutputBuffers[2] = { MAudioOutputBuffer1, MAudioOutputBuffer2 };

  alignas(64) double  MAudioInputBuffer1_64[MAX_BLOCK_SIZE];
  alignas(64) double  MAudioInputBuffer2_64[MAX_BLOCK_SIZE];
  alignas(64) double  MAudioOutputBuffer1_64[MAX_BLOCK_SIZE];
  alignas(64) double  MAudioOutputBuffer2_64[MAX_BLOCK_SIZE];
  alignas(64) double* MAudioInputBuffers_64[2] = { MAudioInputBuffer1_64, MAudioInputBuffer2_64 };
  alignas(64) double* MAudioOutputBuffers_64[2] = { MAudioOutputBuffer1_64, MAudioOutputBuffer2_64 };

//------------------------------
public:
//------------------------------

  Process(const clap_plugin_t* APlugin) {
    MPlugin = APlugin;
  }

  //----------

  ~Process() {
  }

//------------------------------
public:
//------------------------------

 // called by plugin

  uint32_t input_events_size() {
    return MClapEvents.size();
  }

  //----------

  const clap_event_header_t* input_events_get(uint32_t index) {
    return MClapEvents[index];
  }

  //----------

  bool output_events_try_push(const clap_event_header_t* event) {
    return false;
  }

  //----------

//------------------------------
private: // callbacks
//------------------------------

  static
  uint32_t process_input_events_size(const clap_input_events_t* list) {
    Process* proc = (Process*)list->ctx;
    return proc->input_events_size();
  }

  //----------

  static
  const clap_event_header_t* process_input_events_get(const clap_input_events_t* list, uint32_t index) {
    Process* proc = (Process*)list->ctx;
    return proc->input_events_get(index);
  }

  //----------

  static
  bool process_output_events_try_push(const clap_output_events_t* list, const clap_event_header_t *event) {
    Process* proc = (Process*)list->ctx;
    return proc->output_events_try_push(event);
  }

//------------------------------
private:
//------------------------------

  clap_process          MContext;
  clap_event_transport  MContextTransport;
  clap_audio_buffer     MContextAudioInputs;
  clap_audio_buffer     MContextAudioOutputs;
  clap_input_events_t   MContextInputEvents;
  clap_output_events_t  MContextOutputEvents;

//------------------------------
private:
//------------------------------

  void prepare_audio_inputs(uint32_t channels, uint32_t latency) {
    MContextAudioInputs.data32          = MAudioInputBuffers;
    MContextAudioInputs.data64          = MAudioInputBuffers_64; // should this be null if not used/supported?
    MContextAudioInputs.channel_count   = channels;
    MContextAudioInputs.latency         = latency;
    MContextAudioInputs.constant_mask   = 0;
  }

  void prepare_audio_outputs(uint32_t channels, uint32_t latency) {
    MContextAudioOutputs.data32         = MAudioOutputBuffers;
    MContextAudioOutputs.data64         = MAudioOutputBuffers_64; // should this be null if not used/supported?
    MContextAudioOutputs.channel_count  = channels;
    MContextAudioOutputs.latency        = latency;
    MContextAudioOutputs.constant_mask  = 0;
  }

  void prepare_event_inputs() {
    MContextInputEvents.ctx             = this;
    MContextInputEvents.size            = &process_input_events_size;
    MContextInputEvents.get             = &process_input_events_get;
  }

  void prepare_event_outputs() {
    MContextOutputEvents.ctx            = this;
    MContextOutputEvents.try_push       = &process_output_events_try_push;
  }

  /*
    TODO:
      get tempo/timesig from midifile
      increase counter as we render the blocks
  */

  void prepare_transport() {

    //double tempo = 120.0; // bpm
    double tempo = arg_tempo; // seeMMidiSequence.tempo

    double speedFactor = tempo / 60.0;
    double beatsPosFloat = MCurrentTime * speedFactor;
      
    /*
    printf("prepare_transport: tempo: %f\n", tempo);
    printf("prepare_transport: MCurrentTime: %f\n", MCurrentTime);
    printf("prepare_transport: beatsPosFloat: %f\n", beatsPosFloat);
    printf("prepare_transport: barStart: %f\n", beatsPosFloat - fmod(beatsPosFloat, 4.0));
     */

    clap_beattime bTime = round(CLAP_BEATTIME_FACTOR * beatsPosFloat);
    clap_beattime barStart = CLAP_BEATTIME_FACTOR * (beatsPosFloat - fmod(beatsPosFloat, 4.0));

    MContextTransport.header.size = sizeof(clap_event_transport);
    MContextTransport.header.time         = 0;
    MContextTransport.header.space_id     = 0;
    MContextTransport.header.type         = CLAP_EVENT_TRANSPORT;
    MContextTransport.header.flags        = 0; // CLAP_EVENT_IS_LIVE;
    MContextTransport.flags               = CLAP_TRANSPORT_IS_PLAYING | CLAP_TRANSPORT_HAS_BEATS_TIMELINE | CLAP_TRANSPORT_HAS_TEMPO;
    MContextTransport.song_pos_beats      = bTime;
    MContextTransport.song_pos_seconds    = 0;
    MContextTransport.tempo               = tempo;
    MContextTransport.tempo_inc           = 0.0;
    MContextTransport.bar_start           = barStart;
    MContextTransport.bar_number          = (int)(beatsPosFloat/4);
    MContextTransport.loop_start_beats    = 0;
    MContextTransport.loop_end_beats      = 0;
    MContextTransport.loop_start_seconds  = 0;
    MContextTransport.loop_end_seconds    = 0;
    MContextTransport.tsig_num            = 4;
    MContextTransport.tsig_denom          = 4;
  }

  void prepare_context(uint32_t num_in, uint32_t num_out, uint32_t blocksize, uint32_t latency) {
    MContext.steady_time          = MCurrentSample;
    MContext.frames_count         = blocksize;
    MContext.transport            = &MContextTransport;
    MContext.audio_inputs         = &MContextAudioInputs;
    MContext.audio_outputs        = &MContextAudioOutputs;
    MContext.audio_inputs_count   = num_in;
    MContext.audio_outputs_count  = num_out;
    MContext.in_events            = &MContextInputEvents;
    MContext.out_events           = &MContextOutputEvents;
  }

//------------------------------
private:
//------------------------------

  /*
    free all clap_events we (potentially) appended in convertInputEvents
    and reset both MClapEvents and MMidiEvents
  */

  void clearInputEvents() {
    uint32_t num_events = MClapEvents.size();
    for (uint32_t i=0; i<num_events; i++) {
      if (MClapEvents[i]) free( MClapEvents[i] );
      MClapEvents[i] = nullptr;
    }
    MClapEvents.clear();
    // we don't allocate any midievents, (just get pointers to existing ones),
    // so nothing to free
    MMidiEvents.clear();
  }

  //----------

  /*
    called before processing current audiobuffer
    allocates a clap_event for each midi event in MidiInputEvents
    initializes it, and appends it to MClapEvents

  */

  void convertInputEvents() {
    uint32_t num_events = MMidiEvents.size();
    for (uint32_t i=0; i<num_events; i++) {
      MidiEvent* midievent = MMidiEvents[i];
      float   time    = midievent->time - MCurrentTime;
      uint8_t msg1    = midievent->msg1;
      uint8_t msg2    = midievent->msg2;
      uint8_t msg3    = midievent->msg3;
      int32_t offset  = floorf(time * MSampleRate);
      switch( msg1 & 0xF0) {

        case MIDI_NOTE_OFF: {
          clap_event_note_t* note_event = (clap_event_note_t*)malloc(sizeof(clap_event_note_t)); // deleted in deleteInputEvents()
          clap_event_header_t* event = (clap_event_header_t*)note_event;
          memset(note_event,0,sizeof(clap_event_note_t));
          note_event->header.size     = sizeof(clap_event_note_t);  // event size including this header, eg: sizeof (clap_event_note)
          note_event->header.time     = offset;                     // time at which the event happens
          note_event->header.space_id = 0;                          // event space, see clap_host_event_registry
          note_event->header.type     = CLAP_EVENT_NOTE_OFF;        // event type
          note_event->header.flags    = 0;                          // see clap_event_flags
          note_event->port_index      = 0;
          note_event->key             = msg2;
          note_event->channel         = msg1 & 0x0f;
          note_event->velocity        = msg3 / 127.0;
          MClapEvents.push_back(event);
          break;
        }

        case MIDI_NOTE_ON: {
          clap_event_note_t* note_event = (clap_event_note_t*)malloc(sizeof(clap_event_note_t)); // deleted in deleteInputEvents()
          clap_event_header_t* event = (clap_event_header_t*)note_event;
          memset(note_event,0,sizeof(clap_event_note_t));
          note_event->header.size     = sizeof(clap_event_note_t);  // event size including this header, eg: sizeof (clap_event_note)
          note_event->header.time     = offset;                     // time at which the event happens
          note_event->header.space_id = 0;                          // event space, see clap_host_event_registry
          note_event->header.type     = CLAP_EVENT_NOTE_ON;         // event type
          note_event->header.flags    = 0;                          // see clap_event_flags
          note_event->port_index      = 0;
          note_event->key             = msg2;
          note_event->channel         = msg1 & 0x0f;
          note_event->velocity        = msg3 / 127.0;
          MClapEvents.push_back(event);
          break;
        }

        case MIDI_POLY_AFTERTOUCH: {
          clap_event_note_expression_t* expression_event = (clap_event_note_expression_t*)malloc(sizeof(clap_event_note_expression_t)); // deleted in deleteInputEvents()
          clap_event_header_t* event = (clap_event_header_t*)expression_event;
          memset(expression_event,0,sizeof(clap_event_note_expression_t));
          expression_event->header.size     = sizeof(clap_event_note_expression_t);  // event size including this header, eg: sizeof (clap_event_note)
          expression_event->header.time     = offset;                     // time at which the event happens
          expression_event->header.space_id = 0;                          // event space, see clap_host_event_registry
          expression_event->header.type     = CLAP_EVENT_NOTE_EXPRESSION; // event type
          expression_event->header.flags    = 0;                          // see clap_event_flags
          expression_event->expression_id   = CLAP_NOTE_EXPRESSION_PRESSURE;
          expression_event->port_index      = 0;
          expression_event->key             = msg2;
          expression_event->channel         = msg1 & 0x0f;
          expression_event->value           = msg3 / 127.0; // TODO !!!
          MClapEvents.push_back(event);
          break;
        }

        case MIDI_CONTROL_CHANGE: {
          if (arg_remap_cc == msg2) {
            clap_event_param_value_t* param_value_event = (clap_event_param_value_t*)malloc(sizeof(clap_event_param_value_t)); // deleted in deleteInputEvents()
            clap_event_header_t* event = (clap_event_header_t*)param_value_event;
            memset(param_value_event,0,sizeof(clap_event_param_value_t));
            param_value_event->header.size      = sizeof(clap_event_param_value_t);  // event size including this header, eg: sizeof (clap_event_note)
            param_value_event->header.time      = offset;                            // time at which the event happens
            param_value_event->header.space_id  = 0;                                 // event space, see clap_host_event_registry
            param_value_event->header.type      = CLAP_EVENT_PARAM_VALUE;               // event type
            param_value_event->header.flags     = 0;                                 // see clap_event_flags
            param_value_event->cookie           = MRemapParamInfo.cookie;
            param_value_event->param_id         = MRemapParamInfo.id;
            param_value_event->port_index       = -1;
            param_value_event->key              = -1;
            param_value_event->channel          = -1;//msg1 & 15;
            param_value_event->value            = (float)msg3 / 127.0;
            MClapEvents.push_back(event);
          }
          else {
            clap_event_midi_t* midi_event = (clap_event_midi_t*)malloc(sizeof(clap_event_midi_t));  // deleted in deleteInputEvents()
            clap_event_header_t* event = (clap_event_header_t*)midi_event;
            memset(midi_event,0,sizeof(clap_event_midi_t));
            midi_event->header.size     = sizeof(clap_event_midi_t);  // event size including this header, eg: sizeof (clap_event_note)
            midi_event->header.time     = offset;                     // time at which the event happens
            midi_event->header.space_id = 0;                          // event space, see clap_host_event_registry
            midi_event->header.type     = CLAP_EVENT_MIDI;            // event type
            midi_event->header.flags    = 0;                          // see clap_event_flags
            midi_event->data[0]         = msg1;
            midi_event->data[1]         = msg2;
            midi_event->data[2]         = msg3;
            MClapEvents.push_back(event);
            break;
          }
          break;
        }

        // TODO?

        //case MIDI_PROGRAM_CHANGE: {
        //  break;
        //}

        //case MIDI_CHANNEL_AFTERTOUCH: {
        //  event->type                           = CLAP_EVENT_NOTE_EXPRESSION;
        //  event->note_expression.expression_id  = CLAP_NOTE_EXPRESSION_PRESSURE;
        //  event->note_expression.port_index     = 0;
        //  event->note_expression.key            = msg2;
        //  event->note_expression.channel        = msg1 & 0x0f;
        //  event->note_expression.value          = msg3 / 127.0; // TODO
        //  break;
        //}

        //case MIDI_PITCHBEND: {
        //  event->type                           = CLAP_EVENT_NOTE_EXPRESSION;
        //  event->note_expression.expression_id  = CLAP_NOTE_EXPRESSION_TUNING;
        //  event->note_expression.port_index     = 0;
        //  event->note_expression.key            = 0;
        //  event->note_expression.channel        = msg1 & 0x0f;
        //  event->note_expression.value          = (float)((msg2 * 128) + msg3) / 127.0;
        //  break;
        //}

        //default: {
        //    clap_event_midi_t* midi_event = (clap_event_midi_t*)malloc(sizeof(clap_event_midi_t));  // deleted in deleteInputEvents()
        //    clap_event_header_t* event = (clap_event_header_t*)midi_event;
        //    memset(midi_event,0,sizeof(clap_event_midi_t));
        //    midi_event->header.size     = sizeof(clap_event_midi_t);  // event size including this header, eg: sizeof (clap_event_note)
        //    midi_event->header.time     = offset;                     // time at which the event happens
        //    midi_event->header.space_id = 0;                          // event space, see clap_host_event_registry
        //    midi_event->header.type     = CLAP_EVENT_MIDI;            // event type
        //    midi_event->header.flags    = 0;                          // see clap_event_flags
        //    midi_event->data[0]         = msg1;
        //    midi_event->data[1]         = msg2;
        //    midi_event->data[2]         = msg3;
        //    MClapEvents.append(event);
        //    break;
        //  break;
        //}

      }  // switch
    } // for all events
  }

//------------------------------
public:
//------------------------------

  int process() {

    MSampleRate = arg_sample_rate;
    uint32_t num_samples = 0;

    // midi input

    if (arg_midi_input_file) {
      /*int result =*/ MMidiFile.load(arg_midi_input_file);
      MMidiPlayer.initialize(&MMidiFile,arg_sample_rate);
      MMidiSequence = MMidiPlayer.getMidiSequence();
      float midi_length = MMidiSequence->length;
      num_samples = arg_sample_rate * midi_length;
    }

    // audio input

    if (arg_audio_input_file) {
      printf("Opening audio input file: '%s'\n",arg_audio_input_file);
      bool result = MAudioInputFile.open(arg_audio_input_file,AUDIO_FILE_READ);
      if (!result) {
        printf("* Error: Couldn't open audio input file\n");
        return -10;
      }
      printf("Audio input file opened (length = %i frames\n",(int)MAudioInputFile.getInfo()->frames);
      if (MAudioInputFile.getInfo()->frames > num_samples) {
        num_samples = MAudioInputFile.getInfo()->frames;
      }
    }

    // audio output

    if (arg_audio_output_file) {
      printf("Opening audio output file: '%s'\n",arg_audio_output_file);
      bool result = MAudioOutputFile.open(arg_audio_output_file,AUDIO_FILE_WRITE,arg_sample_rate,arg_num_audio_outputs);
      if (!result ){
        printf("* Error: Couldn't open audio output file\n");
        return -11;
      }
      printf("Audio output file opened\n");
    }
    else {
      printf("* Error: No audio output file specified\n");
      return -12;
    }

    // length

    num_samples += (arg_decay_seconds * arg_sample_rate);

    printf("length = %f seconds\n",num_samples / arg_sample_rate);

    if (num_samples == 0) {
      printf("* Error: num_samples == 0\n");
      //return -13;
    }

    // because of larry.mid (20 minutes or something..)
    if (num_samples >= (arg_sample_rate * 180.0)) {
      printf("truncating to 3 minutes\n" );
      num_samples = 180.0 * arg_sample_rate;
    }

    // prepare
    prepare_audio_inputs(arg_num_audio_inputs,0);
    prepare_audio_outputs(arg_num_audio_outputs,0);
    prepare_event_inputs();
    prepare_event_outputs();
    prepare_context(arg_num_audio_inputs,arg_num_audio_outputs,arg_block_size,0);
    srand(1);
    MCurrentSample = 0;   // current position (in samples)
    MCurrentTime = 0.0;   // current position (in seconds)

    printf("processing %i samples\n",num_samples);

    while (num_samples > 0) {

    prepare_transport();

      // block size

      uint32_t block_size = arg_block_size;
      if (arg_fuzz_block_size) {
        block_size = rand() % (arg_block_size+1); // (0..blocksize, incl)
      }
      if (block_size > num_samples) {
        block_size = num_samples;
      }
      float block_size_seconds = block_size / arg_sample_rate;

      // midi input

      if (arg_midi_input_file) {
        clearInputEvents();
        MMidiPlayer.GetEventsForBlock(MCurrentTime,block_size_seconds,&MMidiEvents);
        convertInputEvents();
      }

      // audio input

      if (arg_audio_input_file) {
        MAudioInputFile.read(arg_num_audio_inputs,block_size,MAudioInputBuffers);
      }

      // update time
      MContext.steady_time = MCurrentSample;

      // process
      MContext.frames_count = block_size;
      plugin->process(plugin,&MContext);

      // save audio output
      MAudioOutputFile.write(arg_num_audio_outputs,block_size, MAudioOutputBuffers);

      // next..
      MCurrentSample  += block_size;
      MCurrentTime    += block_size_seconds;
      num_samples     -= block_size;

    }

    printf("Finished processing\n");

    // cleanup
    clearInputEvents();
    if (arg_midi_input_file) {
      MMidiFile.unload();
      printf("Unloaded midi file\n");
    }
    if (arg_audio_input_file) {
      MAudioInputFile.close();
      printf("Audio input file closed\n");
    }
    if (arg_audio_output_file) {
      MAudioOutputFile.close();
      printf("Audio output file closed\n");
    }

    return 0;
  }

  //----------

};

#undef MAX_BLOCK_SIZE

//----------------------------------------------------------------------
#endif
