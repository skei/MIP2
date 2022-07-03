
#if 0

#define MIP_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/mip.socket

//----------------------------------------------------------------------

#include "plugin/clap/mip_clap_entry.h"
#include "plugin/mip_registry.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "mip3/test_params/0",
   .name          = "test_params",
   .vendor        = "mip3",
   .url           = "",
   .manual_url    = "",
   .support_url   = "",
   .version       = "0.0.0",
   .description   = "",
   .features      =  (const char *[]){ CLAP_PLUGIN_FEATURE_NOTE_EFFECT, nullptr }
};

//----------

const clap_param_info_t myParameters[] = {
  {
    /*.id             =*/ 0,
    /*.flags          =*/ CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
    /*.cookie         =*/ nullptr,
    /*.name           =*/ "Value 1",
    /*.module         =*/ "",
    /*.min_value      =*/ -12,
    /*.max_value      =*/ 12,
    /*.default_value  =*/ 0
  },
  {
    /*.id             =*/ 1,
    /*.flags          =*/ CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
    /*.cookie         =*/ nullptr,
    /*.name           =*/ "Value 2",
    /*.module         =*/ "",
    /*.min_value      =*/ -12,
    /*.max_value      =*/ 12,
    /*.default_value  =*/ 1
  },
  {
    /*.id             =*/ 2,
    /*.flags          =*/ CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
    /*.cookie         =*/ nullptr,
    /*.name           =*/ "Modulo",
    /*.module         =*/ "",
    /*.min_value      =*/ 1,
    /*.max_value      =*/ 24,
    /*.default_value  =*/ 12
  }
};

const clap_audio_port_info_t myAudioInputPorts[] = {
  {
    /*.id             =*/ 0,
    /*.name           =*/ "input1",
    /*.flags          =*/ CLAP_AUDIO_PORT_IS_MAIN,
    /*.channel_count  =*/ 2,
    /*.port_type      =*/ CLAP_PORT_STEREO,
    /*.in_place_pair  =*/ CLAP_INVALID_ID
  }
};

const clap_audio_port_info_t myAudioOutputPorts[] = {
  {
    /*.id             =*/ 0,
    /*.name           =*/ "output1",
    /*.flags          =*/ CLAP_AUDIO_PORT_IS_MAIN,
    /*.channel_count  =*/ 2,
    /*.port_type      =*/ CLAP_PORT_STEREO,
    /*.in_place_pair  =*/ CLAP_INVALID_ID
  }
};

const clap_note_port_info_t myNoteInputPorts[] = {
  {
    /*.id =                 =*/ 0,
    /*.supported_dialects   =*/ CLAP_NOTE_DIALECT_CLAP,
    /*.preferred_dialect    =*/ CLAP_NOTE_DIALECT_CLAP,
    /*.name                 =*/ "input1"
  }
};

const clap_note_port_info_t myNoteOutputPorts[] = {
  {
    /*.id =                 =*/ 0,
    /*.supported_dialects   =*/ CLAP_NOTE_DIALECT_CLAP,
    /*.preferred_dialect    =*/ CLAP_NOTE_DIALECT_CLAP,
    /*.name                 =*/ "output1"
  }
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

  int32_t MNoteOffsets[16*128] = {0};

  int32_t par_value1 = 0;
  int32_t par_value2 = 1;

  int32_t MValue1 = 0;
  int32_t MValue2 = 1;
  int32_t MModulo = 12;


//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor/*, const clap_host_t* AHost*/)
  : MIP_Plugin(ADescriptor/*, AHost*/) {
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    //appendAudioInputPort(   &myAudioInputPorts[0]   );
    //appendAudioOutputPort(  &myAudioOutputPorts[0]  );
    appendNoteInputPort(    &myNoteInputPorts[0]    );
    appendNoteOutputPort(   &myNoteOutputPorts[0]   );
    appendParameter( new MIP_Parameter(&myParameters[0]) );
    appendParameter( new MIP_Parameter(&myParameters[1]) );
    appendParameter( new MIP_Parameter(&myParameters[2]) );
    return true;
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    return true;
  }


  //----------

  //void processNoteOnEvent(const clap_event_note_t* event) final {
  //  MIP_Print("NOTE ON\n");
  //  const clap_output_events_t *out_events = MProcessContext.process->out_events;
  //  const clap_event_header_t* header = (const clap_event_header_t*)event;
  //  out_events->try_push(out_events,header);
  //}

  void processEvent(const clap_event_header_t* header) final {
    MIP_Assert( MProcessContext.process );
    const clap_output_events_t *out_events = MProcessContext.process->out_events;
    switch (header->type) {

      case CLAP_EVENT_NOTE_ON: {
        clap_event_note_t* note_event = (clap_event_note_t*)header;

          int32_t value = MValue1;

        int32_t channel = note_event->channel;
        int32_t key = note_event->key;
        MNoteOffsets[(channel*128)+key] = value;
        note_event->key += value;
        MIP_Print("NOTE ON: time %i key %i note_id %i\n",header->time,note_event->key,note_event->note_id);
        out_events->try_push(out_events,header);
          value   = MValue1 + MValue2;
          value = value % MModulo;
          MValue2 = MValue1;
          MValue1 = value;
        break;
      }

      case CLAP_EVENT_NOTE_OFF: {
        clap_event_note_t* note_event = (clap_event_note_t*)header;
        int32_t channel = note_event->channel;
        int32_t key = note_event->key;
        int32_t offset = MNoteOffsets[(channel*128)+key];
        note_event->key += offset;
        MIP_Print("NOTE OFF: time %i key %i note_id %i\n",header->time,note_event->key,note_event->note_id);
        out_events->try_push(out_events,header);
        break;
      }

      case CLAP_EVENT_PARAM_VALUE: {
        clap_event_param_value_t* event = (clap_event_param_value_t*)header;
        uint32_t index = event->param_id;
        double value = event->value;
        switch (index) {
          case 0:
            par_value1 = value;
            MValue1 = par_value1;
            MValue2 = par_value2;
            break;
          case 1:
            par_value2 = value;
            MValue1 = par_value1;
            MValue2 = par_value2;
            break;
          case 2:
            MModulo = value;
            break;
        }
        break;
      }

    } // switch
  }



};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&myDescriptor);
};

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new myPlugin(ADescriptor/*,AHost*/);
  }
  return nullptr;
}

#endif
