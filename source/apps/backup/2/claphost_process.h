#ifndef claphost_process_included
#define claphost_process_included
//----------------------------------------------------------------------

#if 0

// in progress!!

#define MAX_BLOCK_EVENTS  1024
#define MAX_BLOCK_SIZE    65536


#include "audio/mip_audio_file.h"
#include "midi/mip_midi_file.h"
#include "midi/mip_midi_player.h"
#include "plugin/clap/mip_clap.h"

typedef std::vector<clap_event*> clap_events;

//----------------------------------------------------------------------

class ClapHost_Process {

//------------------------------
private:
//------------------------------

  clap_process          MClapContext;
  clap_event_transport  MClapTransport;
  clap_audio_buffer     MClapAudioInputs;
  clap_audio_buffer     MClapAudioOutputs;
  clap_event_list       MClapEventInputs;
  clap_event_list       MClapEventOutputs;

  MIP_AudioFile         MAudioInputFile;
  MIP_AudioFile         MAudioOutputFile;
  MIP_MidiFile          MMidiFile;
  MIP_MidiPlayer        MMidiPlayer;

  MIP_ClapPlugin*       MInstance       = NULL;
  float                 MSampleRate     = 0.0;
  uint32_t              MCurrentSample  = 0;
  float                 MCurrentTime    = 0.0;
  MIP_MidiSequence*     MMidiSequence   = NULL;

  MIP_MidiEvents        MMidiInputEvents;
  clap_events           MClapInputEvents;

  clap_param_info       MRemapParamInfo = {0};

  alignas(32) float MAudioInputBuffer1[MAX_BLOCK_SIZE];
  alignas(32) float MAudioInputBuffer2[MAX_BLOCK_SIZE];
  alignas(32) float MAudioOutputBuffer1[MAX_BLOCK_SIZE];
  alignas(32) float MAudioOutputBuffer2[MAX_BLOCK_SIZE];

  //alignas(32)
  float* MAudioInputBuffers[2] = {
    MAudioInputBuffer1,
    MAudioInputBuffer2
  };

  //alignas(32)
  float* MAudioOutputBuffers[2] = {
    MAudioOutputBuffer1,
    MAudioOutputBuffer2
  };

//------------------------------
public:
//------------------------------

  ClapHost_Process() {
  }

  //----------

  ~ClapHost_Process() {
  }

//------------------------------
public:
//------------------------------

  uint32_t input_events_size() {
    return MClapInputEvents.size();
  }

  //----------

  const clap_event* input_events_get(uint32_t index) {
    return MClapInputEvents[index];
  }

  //----------

  void input_events_push_back(const clap_event* event) {
  }

  //----------

  uint32_t output_events_size() {
    return 0;
  }

  //----------

  const clap_event* output_events_get(uint32_t index) {
    return NULL;
  }

  //----------

  void output_events_push_back(const clap_event* event) {
  }

  //----------

//------------------------------
public: // callbacks
//------------------------------

  static
  uint32_t process_input_events_size(const struct clap_event_list* list) {
    ClapHost_Process* process = (ClapHost_Process*)list->ctx;
    return process->input_events_size();
  }

  //----------

  static
  const clap_event* process_input_events_get(const struct clap_event_list* list, uint32_t index) {
    ClapHost_Process* process = (ClapHost_Process*)list->ctx;
    return process->input_events_get(index);
  }

  //----------

  static
  void process_input_events_push_back(const struct clap_event_list* list, const clap_event *event) {
    ClapHost_Process* process = (ClapHost_Process*)list->ctx;
    process->input_events_push_back(event);
  }

  //----------

  static
  uint32_t process_output_events_size(const struct clap_event_list* list) {
    ClapHost_Process* process = (ClapHost_Process*)list->ctx;
    return process->output_events_size();
  }

  //----------

  static
  const clap_event* process_output_events_get(const struct clap_event_list* list, uint32_t index) {
    ClapHost_Process* process = (ClapHost_Process*)list->ctx;
    return process->output_events_get(index);
  }

  //----------

  static
  void process_output_events_push_back(const struct clap_event_list* list, const clap_event *event) {
    ClapHost_Process* process = (ClapHost_Process*)list->ctx;
    process->output_events_push_back(event);
  }

//------------------------------
public:
//------------------------------

 //TODO: input + output channel counts?

  void prepare_audio_inputs(uint32_t channels, uint32_t latency=0) {
    MClapAudioInputs.data32           = MAudioInputBuffers;
    MClapAudioInputs.data64           = NULL;
    MClapAudioInputs.channel_count    = channels;
    MClapAudioInputs.latency          = latency;
    MClapAudioInputs.constant_mask    = 0;
  }

  void prepare_audio_outputs(uint32_t channels, uint32_t latency=0) {
    MClapAudioOutputs.data32          = MAudioOutputBuffers;
    MClapAudioOutputs.data64          = NULL;
    MClapAudioOutputs.channel_count   = channels;
    MClapAudioOutputs.latency         = latency;
    MClapAudioOutputs.constant_mask   = 0;
  }



  void prepare_event_inputs() {
    MClapEventInputs.ctx              = this; // NULL;                    // reserved pointer for the list
    MClapEventInputs.size             = &process_input_events_size;
    MClapEventInputs.get              = &process_input_events_get;        // Don't free the return event, it belongs to the list
    MClapEventInputs.push_back        = &process_input_events_push_back;  // Makes a copy of the event
  }

  void prepare_event_outputs() {
    MClapEventOutputs.ctx             = this; // NULL;
    MClapEventOutputs.size            = &process_output_events_size;
    MClapEventOutputs.get             = &process_output_events_get;
    MClapEventOutputs.push_back       = &process_output_events_push_back;
  }

  /*
    CLAP_TRANSPORT_HAS_TEMPO
    CLAP_TRANSPORT_HAS_BEATS_TIMELINE
    CLAP_TRANSPORT_HAS_SECONDS_TIMELINE
    CLAP_TRANSPORT_HAS_TIME_SIGNATURE
    CLAP_TRANSPORT_IS_PLAYING
    CLAP_TRANSPORT_IS_RECORDING
    CLAP_TRANSPORT_IS_LOOP_ACTIVE
    CLAP_TRANSPORT_IS_WITHIN_PRE_ROLL

    TODO:
      get tempo/timesig from midifile
      increase counter as we render the blocks
  */

  void prepare_transport() {
    MClapTransport.flags              = CLAP_TRANSPORT_IS_PLAYING;
    MClapTransport.song_pos_beats     = 0;
    MClapTransport.song_pos_seconds   = 0;
    MClapTransport.tempo              = 120.0;
    MClapTransport.tempo_inc          = 0.0;
    MClapTransport.bar_start          = 0;
    MClapTransport.bar_number         = 0;
    MClapTransport.loop_start_beats   = 0;
    MClapTransport.loop_end_beats     = 0;
    MClapTransport.loop_start_seconds = 0;
    MClapTransport.loop_end_seconds   = 0;
    MClapTransport.tsig_num           = 4;
    MClapTransport.tsig_denom         = 4;
  }

  void prepare_context(uint32_t channels, uint32_t blocksize, uint32_t latency=0) {
    MClapContext.steady_time          = MCurrentSample;     // a steady sample time counter, requiered
    MClapContext.frames_count         = blocksize;          // number of frame to process
    MClapContext.transport            = &MClapTransport;
    MClapContext.audio_inputs         = &MClapAudioInputs;
    MClapContext.audio_outputs        = &MClapAudioOutputs;

    MClapContext.audio_inputs_count   = 0; // TODO
    MClapContext.audio_outputs_count  = channels;

    MClapContext.in_events            = &MClapEventInputs;
    MClapContext.out_events           = &MClapEventOutputs;
  }

//------------------------------
private:
//------------------------------

  /*
    free all clap_events we (potentially) appended in convertInputEvents
    and reset both MClapInputEvents and MMidiInputEvents
  */

  void clearInputEvents() {
    uint32_t num_events = MClapInputEvents.size();
    for (uint32_t i=0; i<num_events; i++) {
      if (MClapInputEvents[i]) free( MClapInputEvents[i] );
      MClapInputEvents[i] = NULL;
    }
    MClapInputEvents.clear();

    /*
      we don't allocate any midievents
      (just get pointers to existing ones),
      so nothing to free
    */

    MMidiInputEvents.clear();
  }

  //----------

  /*
    called before processing current audiobuffer
    allocates a clap_event for each midi event in MidiInputEvents
    initializes it, and appends it to MClapInputEvents

  */

  void convertInputEvents(/*arguments_t* arg*/) {
    uint32_t num_events = MMidiInputEvents.size();
    for (uint32_t i=0; i<num_events; i++) {
      MIP_MidiEvent* midievent = MMidiInputEvents[i];
      float   time    = midievent->time - MCurrentTime;
      uint8_t msg1    = midievent->msg1;
      uint8_t msg2    = midievent->msg2;
      uint8_t msg3    = midievent->msg3;
      int32_t offset  = floorf(time * MSampleRate);
      //printf("> MIDI : offset %i : %02x %02x %02x\n",offset,msg1,msg2,msg3);
      clap_event* event;
      switch( msg1 & 0xF0) {

        case 0x80: // note off
          event = (clap_event*)malloc(sizeof(clap_event));  // deleted in deleteInputEvents()
          memset(event,0,sizeof(clap_event));
          event->time             = offset;
          event->type             = CLAP_EVENT_NOTE_OFF;
          event->note.port_index  = 0;
          event->note.key         = msg2;
          event->note.channel     = msg1 & 0x0f;
          event->note.velocity    = msg3 / 127.0;
          MClapInputEvents.push_back(event);
          break;

        case 0x90: // note on
          event = (clap_event*)malloc(sizeof(clap_event));  // deleted in deleteInputEvents()
          memset(event,0,sizeof(clap_event));
          event->time             = offset;
          event->type             = CLAP_EVENT_NOTE_ON;
          event->note.port_index  = 0;
          event->note.key         = msg2;
          event->note.channel     = msg1 & 0x0f;
          event->note.velocity    = msg3 / 127.0;
          MClapInputEvents.push_back(event);
          break;

        /*
        case 0xA0: // poly aftertouch
          event->type                           = CLAP_EVENT_NOTE_EXPRESSION;
          event->note_expression.expression_id  = CLAP_NOTE_EXPRESSION_PRESSURE;
          event->note_expression.port_index     = 0;
          event->note_expression.key            = msg2;
          event->note_expression.channel        = msg1 & 0x0f;
          event->note_expression.value          = msg3 / 127.0; // TODO
          break;
        */

        case 0xB0: // control change
          if (GRemapCC == msg2) {
            event = (clap_event*)malloc(sizeof(clap_event));  // deleted in deleteInputEvents()
            memset(event,0,sizeof(clap_event));
            event->type                   = CLAP_EVENT_PARAM_VALUE;
            event->time                   = offset;
            event->param_value.cookie     = MRemapParamInfo.cookie; //NULL; // !!!
            event->param_value.param_id   = MRemapParamInfo.id;
            event->param_value.port_index = -1;
            event->param_value.key        = -1;
            event->param_value.channel    = -1;//msg1 & 15;
            event->param_value.flags      = 0; // CLAP_EVENT_PARAM_BEGIN_ADJUST | CLAP_EVENT_PARAM_END_ADJUST | CLAP_EVENT_PARAM_SHOULD_RECORD
            event->param_value.value      = (float)msg3 / 127.0;
            MClapInputEvents.push_back(event);
          }
          else {
            event = (clap_event*)malloc(sizeof(clap_event));  // deleted in deleteInputEvents()
            memset(event,0,sizeof(clap_event));
            event->time         = offset;
            event->type         = CLAP_EVENT_MIDI;
            event->midi.data[0] = msg1;
            event->midi.data[1] = msg2;
            event->midi.data[2] = msg3;
            MClapInputEvents.push_back(event);
          }
          break;

        /*
        case 0xC0: // program change
          break;
        */

        /*
        case 0xD0: // channel aftertouch
          event->type                           = CLAP_EVENT_NOTE_EXPRESSION;
          event->note_expression.expression_id  = CLAP_NOTE_EXPRESSION_PRESSURE;
          event->note_expression.port_index     = 0;
          event->note_expression.key            = msg2;
          event->note_expression.channel        = msg1 & 0x0f;
          event->note_expression.value          = msg3 / 127.0; // TODO
          break;
        */

        /*
        case 0xE0: // pitch bend
          event->type                           = CLAP_EVENT_NOTE_EXPRESSION;
          event->note_expression.expression_id  = CLAP_NOTE_EXPRESSION_TUNING;
          event->note_expression.port_index     = 0;
          event->note_expression.key            = 0;
          event->note_expression.channel        = msg1 & 0x0f;
          event->note_expression.value          = (float)((msg2 * 128) + msg3) / 127.0;
          break;
        */

        /*
        default:
          event = (clap_event*)malloc(sizeof(clap_event));  // deleted in deleteInputEvents()
          memset(event,0,sizeof(clap_event));
          event->time         = offset;
          event->type         = CLAP_EVENT_MIDI;
          event->midi.data[0] = msg1;
          event->midi.data[1] = msg2;
          event->midi.data[2] = msg3;
          MClapInputEvents.push_back(event);
        */

      }  // switch
    } // for all events
  }


//------------------------------
public:
//------------------------------

  void process(MIP_ClapPlugin* instance/*, arguments_t* arg*/) {

    MInstance = instance;
    MSampleRate = GSampleRate;
    const clap_plugin* plugin = instance->getClapPlugin();
    uint32_t num_samples = 0;

    // midi input

    if (GMidiInputFile) {
      /*int result =*/ MMidiFile.load(GMidiInputFile);
      MMidiPlayer.initialize(&MMidiFile,GSampleRate);
      MMidiSequence = MMidiPlayer.getMidiSequence();
      float midi_length = MMidiSequence->length;
      num_samples = GSampleRate * midi_length;
    }

    // audio input

    if (GAudioInputFile) {
      printf("Opening audio input file: '%s'\n",GAudioInputFile);
      bool result = MAudioInputFile.open(GAudioInputFile,MIP_AUDIO_FILE_READ);
      if (!result) {
        printf("* Error: Couldn't open audio input file\n");
        return;
      }
      printf("Audio input file opened (length = %i frames\n",(int)MAudioInputFile.getInfo()->frames);
      if (MAudioInputFile.getInfo()->frames > num_samples) {
        num_samples = MAudioInputFile.getInfo()->frames;
      }
    }

    // audio output

    if (GAudioOutputFile) {
      printf("Opening audio output file: '%s'\n",GAudioOutputFile);
      bool result = MAudioOutputFile.open(GAudioOutputFile,MIP_AUDIO_FILE_WRITE,GSampleRate,GNumAudioOutputs);
      if (!result ){
        printf("* Error: Couldn't open audio output file\n");
        return;
      }
      printf("Audio output file opened\n");
    }
    else {
      printf("* Error: No audio output file specified\n");
      return;
    }

    // length

    printf("> length = %f seconds\n",num_samples / GSampleRate);

    if (num_samples == 0) {
      printf("* Error: num_samples == 0\n");
      //return;
    }

    // because of larry.mid (20 minutes or something..)
    if (num_samples >= (GSampleRate * 180.0)) {
      printf("> Truncating to 3 minute\n" );
      num_samples = 180.0 * GSampleRate;
    }

    // remapping

    if ((GRemapCC >= 0) && (GRemapParam >= 0)) {
      MInstance->getParam(GRemapParam,&MRemapParamInfo);
    }

//    uint32_t num_blocks = num_samples / arg->block_size;
//    num_blocks += 1; // just to be sure :-)
//    float seconds_per_block = arg->block_size / arg->sample_rate;

    // prepare

    prepare_transport();
    prepare_audio_inputs(GNumAudioInputs,0);
    prepare_audio_outputs(GNumAudioOutputs,0);
    prepare_event_inputs();
    prepare_event_outputs();
    prepare_context(GNumAudioOutputs,GBlockSize,0);

    srand(1);
    MCurrentSample = 0;   // current position (in samples)
    MCurrentTime = 0.0;   // current position (in seconds)

    while (num_samples > 0) {

      // block size
      uint32_t block_size = GBlockSize;
      if (GFuzzBlockSize) {
        block_size = 1 + (rand() % GBlockSize); // min 16?
      }
      if (block_size > num_samples) {
        block_size = num_samples;
      }
      float block_size_seconds = block_size / GSampleRate;

      // events for current block
      if (GMidiInputFile) {
        clearInputEvents();
        MMidiPlayer.GetEventsForBlock(MCurrentTime,block_size_seconds,&MMidiInputEvents);
        convertInputEvents(/*arg*/);
      }

      //else {
      if (GAudioInputFile) {
        MAudioInputFile.read(GNumAudioInputs,block_size,MAudioInputBuffers);
      }

      // update time
      MClapContext.steady_time = MCurrentSample;

      // process
      MClapContext.frames_count = block_size;
      plugin->process(plugin,&MClapContext);

      // save audio output
      MAudioOutputFile.write(GNumAudioOutputs,block_size, MAudioOutputBuffers);

      MCurrentSample  += block_size;
      MCurrentTime    += block_size_seconds;
      num_samples     -= block_size;

    }

    printf("Finished processing\n");

    // cleanup

    clearInputEvents();

    if (GMidiInputFile) {
      MMidiFile.unload();
      printf("Unloaded midi file\n");
    }

    if (GAudioInputFile) {
      MAudioInputFile.close();
      printf("Audio input file closed\n");
    }

    if (GAudioOutputFile) {
      MAudioOutputFile.close();
      printf("Audio output file closed\n");
    }

  }

  //----------

};




#undef MAX_BLOCK_SIZE

#endif // 0

//----------------------------------------------------------------------
#endif
