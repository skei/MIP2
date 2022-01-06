#ifndef claphost_process_included
#define claphost_process_included
//----------------------------------------------------------------------

// in progress!!

#define MAX_BLOCK_EVENTS  1024
#define MAX_BLOCK_SIZE    65536


#include "audio/mip_audio_file.h"
#include "base/types/mip_array.h"
#include "midi/mip_midi_file.h"
#include "midi/mip_midi_player.h"
#include "plugin/clap/mip_clap_hosted_plugin.h"

#include "gui/mip_window.h"

typedef MIP_Array<clap_event_header_t*> MIP_ClapEventHeaders;

//----------------------------------------------------------------------

class Process
: public MIP_WindowListener {

//------------------------------
private:
//------------------------------

  MIP_ClapHostedPlugin*     MPlugin = nullptr;
  MIP_ClapEventHeaders      MClapInputEvents  = {};     // (fill this before every block)
  MIP_ClapEventHeaders      MClapOutputEvents = {};     // (send these to host after each block)
  MIP_AudioFile             MAudioInputFile   = {};
  MIP_AudioFile             MAudioOutputFile  = {};
  MIP_MidiFile              MMidiFile         = {};
  MIP_MidiPlayer            MMidiPlayer       = {};
  float                     MSampleRate       = 0.0;
  uint32_t                  MCurrentSample    = 0;
  float                     MCurrentTime      = 0.0;
  MIP_MidiSequence*         MMidiSequence     = nullptr;
  MIP_MidiEvents            MMidiEvents       = {};
  MIP_ClapEventHeaders      MClapEvents       = {};
  clap_param_info           MRemapParamInfo   = {0};

  MIP_Window*               MWindow           = nullptr;;


//------------------------------
private:
//------------------------------

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

  Process(MIP_ClapHostedPlugin* APlugin) {
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

  void output_events_push_back(const clap_event_header_t* event) {
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
  void process_output_events_push_back(const clap_output_events_t* list, const clap_event_header_t *event) {
    Process* proc = (Process*)list->ctx;
    proc->output_events_push_back(event);
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

 //TODO: input + output channel counts?

  void prepare_audio_inputs(uint32_t channels, uint32_t latency=0) {
    MContextAudioInputs.data32           = MAudioInputBuffers;
    MContextAudioInputs.data64           = NULL;
    MContextAudioInputs.channel_count    = channels;
    MContextAudioInputs.latency          = latency;
    MContextAudioInputs.constant_mask    = 0;
  }

  void prepare_audio_outputs(uint32_t channels, uint32_t latency=0) {
    MContextAudioOutputs.data32          = MAudioOutputBuffers;
    MContextAudioOutputs.data64          = NULL;
    MContextAudioOutputs.channel_count   = channels;
    MContextAudioOutputs.latency         = latency;
    MContextAudioOutputs.constant_mask   = 0;
  }

  void prepare_event_inputs() {
    MContextInputEvents.ctx             = this; // NULL;                    // reserved pointer for the list
    MContextInputEvents.size             = &process_input_events_size;
    MContextInputEvents.get              = &process_input_events_get;        // Don't free the return event, it belongs to the list
  }

  void prepare_event_outputs() {
    MContextOutputEvents.ctx             = this; // NULL;
    MContextOutputEvents.push_back       = &process_output_events_push_back;
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
    MContextTransport.flags              = CLAP_TRANSPORT_IS_PLAYING;
    MContextTransport.song_pos_beats     = 0;
    MContextTransport.song_pos_seconds   = 0;
    MContextTransport.tempo              = 120.0;
    MContextTransport.tempo_inc          = 0.0;
    MContextTransport.bar_start          = 0;
    MContextTransport.bar_number         = 0;
    MContextTransport.loop_start_beats   = 0;
    MContextTransport.loop_end_beats     = 0;
    MContextTransport.loop_start_seconds = 0;
    MContextTransport.loop_end_seconds   = 0;
    MContextTransport.tsig_num           = 4;
    MContextTransport.tsig_denom         = 4;
  }

  void prepare_context(uint32_t channels, uint32_t blocksize, uint32_t latency=0) {
    MContext.steady_time          = MCurrentSample;     // a steady sample time counter, requiered
    MContext.frames_count         = blocksize;          // number of frame to process
    MContext.transport            = &MContextTransport;
    MContext.audio_inputs         = &MContextAudioInputs;
    MContext.audio_outputs        = &MContextAudioOutputs;
    MContext.audio_inputs_count   = 0; // TODO
    MContext.audio_outputs_count  = channels;
    MContext.in_events            = &MContextInputEvents;
    MContext.out_events           = &MContextOutputEvents;
  }

//------------------------------
private:
//------------------------------

  /*
    free all clap_events we (potentially) appended in convertInputEvents
    and reset both MClapInputEvents and MMidiInputEvents
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
    initializes it, and appends it to MClapInputEvents

  */

  void convertInputEvents() {
    uint32_t num_events = MMidiEvents.size();
    for (uint32_t i=0; i<num_events; i++) {
      MIP_MidiEvent* midievent = MMidiEvents[i];
      float   time    = midievent->time - MCurrentTime;
      uint8_t msg1    = midievent->msg1;
      uint8_t msg2    = midievent->msg2;
      uint8_t msg3    = midievent->msg3;
      int32_t offset  = floorf(time * MSampleRate);
      //printf("> MIDI : offset %i : %02x %02x %02x\n",offset,msg1,msg2,msg3);
      switch( msg1 & 0xF0) {

        case MIP_MIDI_NOTE_OFF: {
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
          MClapEvents.append(event);
          break;
        }

        case MIP_MIDI_NOTE_ON: {
          clap_event_note_t* note_event = (clap_event_note_t*)malloc(sizeof(clap_event_note_t)); // deleted in deleteInputEvents()
          clap_event_header_t* event = (clap_event_header_t*)note_event;
          memset(note_event,0,sizeof(clap_event_note_t));
          note_event->header.size     = sizeof(clap_event_note_t);  // event size including this header, eg: sizeof (clap_event_note)
          note_event->header.time     = offset;                     // time at which the event happens
          note_event->header.space_id = 0;                          // event space, see clap_host_event_registry
          note_event->header.type     = CLAP_EVENT_NOTE_ON;        // event type
          note_event->header.flags    = 0;                          // see clap_event_flags
          note_event->port_index      = 0;
          note_event->key             = msg2;
          note_event->channel         = msg1 & 0x0f;
          note_event->velocity        = msg3 / 127.0;
          MClapEvents.append(event);
          break;
        }

        case MIP_MIDI_POLY_AFTERTOUCH: {
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
          expression_event->value           = msg3 / 127.0; // TODO
          MClapEvents.append(event);
          break;
        }

        case MIP_MIDI_CONTROL_CHANGE: {
          if (arg_remap_cc == msg2) {
            clap_event_param_value_t* param_value_event = (clap_event_param_value_t*)malloc(sizeof(clap_event_param_value_t)); // deleted in deleteInputEvents()
            clap_event_header_t* event = (clap_event_header_t*)param_value_event;
            memset(param_value_event,0,sizeof(clap_event_param_value_t));
            param_value_event->header.size      = sizeof(clap_event_param_value_t);  // event size including this header, eg: sizeof (clap_event_note)
            param_value_event->header.time      = offset;                            // time at which the event happens
            param_value_event->header.space_id  = 0;                                 // event space, see clap_host_event_registry
            param_value_event->header.type      = CLAP_EVENT_PARAM_VALUE;               // event type
            param_value_event->header.flags     = 0;                                 // see clap_event_flags
            param_value_event->cookie           = MRemapParamInfo.cookie; //NULL; // !!!
            param_value_event->param_id         = MRemapParamInfo.id;
            param_value_event->port_index       = -1;
            param_value_event->key              = -1;
            param_value_event->channel          = -1;//msg1 & 15;
            param_value_event->value            = (float)msg3 / 127.0;
            MClapEvents.append(event);
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
            MClapEvents.append(event);
            break;
          }
          break;
        }

        //case MIP_MIDI_PROGRAM_CHANGE: {
        //  break;
        //}

        //case MIP_MIDI_CHANNEL_AFTERTOUCH: {
        //  event->type                           = CLAP_EVENT_NOTE_EXPRESSION;
        //  event->note_expression.expression_id  = CLAP_NOTE_EXPRESSION_PRESSURE;
        //  event->note_expression.port_index     = 0;
        //  event->note_expression.key            = msg2;
        //  event->note_expression.channel        = msg1 & 0x0f;
        //  event->note_expression.value          = msg3 / 127.0; // TODO
        //  break;
        //}

        //case MIP_MIDI_PITCHBEND: {
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

  int process(MIP_ClapHostedPlugin* hosted_plugin) {
    MSampleRate = arg_sample_rate;
    const clap_plugin* plugin = hosted_plugin->getClapPlugin();
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
      bool result = MAudioInputFile.open(arg_audio_input_file,MIP_AUDIO_FILE_READ);
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
      bool result = MAudioOutputFile.open(arg_audio_output_file,MIP_AUDIO_FILE_WRITE,arg_sample_rate,arg_num_audio_outputs);
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

    printf("> length = %f seconds\n",num_samples / arg_sample_rate);

    if (num_samples == 0) {
      printf("* Error: num_samples == 0\n");
      //return -13;
    }

    // because of larry.mid (20 minutes or something..)
    if (num_samples >= (arg_sample_rate * 180.0)) {
      printf("> Truncating to 3 minute\n" );
      num_samples = 180.0 * arg_sample_rate;
    }

    // remapping

    //if ((arg_remap_cc >= 0) && (arg_remap_param >= 0)) {
    //  MInstance->getParam(arg_remap_param,&MRemapParamInfo);
    //}

    //uint32_t num_blocks = num_samples / arg->block_size;
    //num_blocks += 1; // just to be sure :-)
    //float seconds_per_block = arg->block_size / arg->sample_rate;

    // prepare

    prepare_transport();
    prepare_audio_inputs(arg_num_audio_inputs,0);
    prepare_audio_outputs(arg_num_audio_outputs,0);
    prepare_event_inputs();
    prepare_event_outputs();
    prepare_context(arg_num_audio_outputs,arg_block_size,0);

    srand(1);
    MCurrentSample = 0;   // current position (in samples)
    MCurrentTime = 0.0;   // current position (in seconds)

//----------
// editor
    const clap_plugin_gui_t*     gui     = (const clap_plugin_gui_t*    )plugin->get_extension(plugin,CLAP_EXT_GUI    );
    const clap_plugin_gui_x11_t* gui_x11 = (const clap_plugin_gui_x11_t*)plugin->get_extension(plugin,CLAP_EXT_GUI_X11);
    uint32_t width = 256;
    uint32_t height = 256;
    if (gui && gui_x11) {
      gui->create(plugin);
      gui->get_size(plugin,&width,&height);
      MWindow = new MIP_Window(width,height,"plugin");
      MWindow->open();
      xcb_window_t xcbwin = MWindow->getXcbWindow();
      gui_x11->attach(plugin,nullptr,xcbwin);
      gui->show(plugin);
    }
//----------

    printf("processing %i samples\n",num_samples);

    while (num_samples > 0) {

      // block size

      uint32_t block_size = arg_block_size;
      if (arg_fuzz_block_size) {
        //block_size = 1 + (rand() % arg_block_size); // min 16?
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

      //else {
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

    MIP_Sleep(10000);

//----------
// editor
    gui->hide(plugin);
    gui->destroy(plugin);
    MWindow->close();
    delete MWindow;
//----------

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
