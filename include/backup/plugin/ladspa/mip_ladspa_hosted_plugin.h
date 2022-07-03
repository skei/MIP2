#ifndef mip_ladspa_hosted_plugin_included
#define mip_ladspa_hosted_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/ladspa/mip_ladspa.h"
#include "plugin/clap/mip_clap.h"

#include <dlfcn.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* MIP_LadspaHostedPlugin_features[] = {
  "audio_effect",
  nullptr
};

#define MIP_LADSPA_MAX_AUDIO_INPUTS     32
#define MIP_LADSPA_MAX_AUDIO_OUTPUTS    32
#define MIP_LADSPA_MAX_CONTROL_INPUTS   512
#define MIP_LADSPA_MAX_CONTROL_OUTPUTS  512
#define MIP_LADSPA_MAX_PORTS            512

#define MIP_LADSPA_FLAGS_NONE             0
#define MIP_LADSPA_FLAGS_REALTIME         1
#define MIP_LADSPA_FLAGS_INPLACE_BROKEN   2
#define MIP_LADSPA_FLAGS_HARD_RT_CAPABLE  4

#define MIP_LADSPA_PORT_NONE              0
#define MIP_LADSPA_PORT_AUDIO             1
#define MIP_LADSPA_PORT_CONTROL           2
#define MIP_LADSPA_PORT_INPUT             4
#define MIP_LADSPA_PORT_OUTPUT            8

#define MIP_LADSPA_PARAM_NONE             0
#define MIP_LADSPA_PARAM_TOGGLED          1
#define MIP_LADSPA_PARAM_SAMPLERATE       2
#define MIP_LADSPA_PARAM_LOGARITHMIC      4
#define MIP_LADSPA_PARAM_INTEGER          8

struct MIP_LadspaPort {
  const char* name;
  uint32_t    flags;
  float       val;
  float       minval;
  float       maxval;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_LadspaHostedPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  const char*               MPath             = "";
  void*                     MLibHandle        = nullptr;
  const LADSPA_Descriptor*  MLadspaDescriptor = nullptr;
  LADSPA_Handle             MLadspaHandle     = nullptr;

  uint32_t                    MLadspaFlags          = MIP_LADSPA_FLAGS_NONE;
  bool                        MIsInstantiated       = false;
  bool                        MIsActivated          = false;

  uint32_t                    MNumAudioInputs       = 0;
  uint32_t                    MNumAudioOutputs      = 0;
  uint32_t                    MNumControlInputs     = 0;
  uint32_t                    MNumControlOutputs    = 0;
  MIP_LadspaPort              MLadspaPorts[MIP_LADSPA_MAX_PORTS]                  = {0};
  int32_t                     MAudioInputIndex[MIP_LADSPA_MAX_AUDIO_INPUTS]       = {0};
  int32_t                     MAudioOutputIndex[MIP_LADSPA_MAX_AUDIO_OUTPUTS]     = {0};
  int32_t                     MControlInputIndex[MIP_LADSPA_MAX_CONTROL_INPUTS]   = {0};
  int32_t                     MControlOutputIndex[MIP_LADSPA_MAX_CONTROL_OUTPUTS] = {0};

//------------------------------
public:
//------------------------------

  MIP_LadspaHostedPlugin(const char* APath, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {

    MLibHandle = dlopen(APath,RTLD_LAZY | RTLD_LOCAL ); // RTLD_NOW, RTLD_LAZY
    if (MLibHandle) {
      //const char* filename = MIP_GetFilenameFromPath(path);
      //MIP_Print("%s\n",filename);
      LADSPA_Descriptor_Function get_ladspa_descriptor = (LADSPA_Descriptor_Function)dlsym(MLibHandle,"ladspa_descriptor");
      MLadspaDescriptor = get_ladspa_descriptor(0);
      initPorts();
      //initParameters();
      //MLadspaHandle = MLadspaDescriptor->instantiate(MLadspaDescriptor,44100);
    }
  }

  //----------

  virtual ~MIP_LadspaHostedPlugin() {
    //if (MLadspaDescriptor) MLadspaDescriptor->cleanup(MLadspaHandle);
    if (MLibHandle) dlclose(MLibHandle);
  }

//------------------------------
public: // clap plugin
//------------------------------

  //bool init() final {
  //  return true;
  //}

  //void destroy() final {
  //}

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    bool result = MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
    ladspa_instantiate(sample_rate);
    ladspa_activate();
    return result;
  }

  void deactivate() final {
    ladspa_deactivate();
    ladspa_cleanup();
    MIP_Plugin::deactivate();
  }

  //bool start_processing() final {
  //  return true;
  //}

  //void stop_processing() final {
  //}

  clap_process_status process(const clap_process_t *process) final {
    return MIP_Plugin::process(process);
  }

  //void on_main_thread() final {
  //}

  const void* get_extension(const char *id) final {
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0) return &MAudioPorts;
    return MIP_Plugin::get_extension(id);
  }


//------------------------------
private: // clap audio ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) final {
    if (is_input) return 1;//MNumAudioInputs;
    else return 1;//MNumAudioOutputs;
  }

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) {
    if (is_input) {
      int32_t port_index = MAudioInputIndex[index];
      const char* name = MLadspaPorts[port_index].name;
      strncpy(info->name,name,CLAP_NAME_SIZE-1);
      info->id = index;
      info->flags = CLAP_AUDIO_PORT_IS_MAIN;
      info->channel_count = MNumAudioInputs;
      if (MNumAudioInputs == 1) info->port_type = CLAP_PORT_MONO;
      else if (MNumAudioInputs == 2) info->port_type = CLAP_PORT_STEREO;
      else info->port_type = 0;
      info->in_place_pair = CLAP_INVALID_ID;
    }
    else {
      int32_t port_index = MAudioOutputIndex[index];
      const char* name = MLadspaPorts[port_index].name;
      strncpy(info->name,name,CLAP_NAME_SIZE-1);
      info->id = index;
      info->flags = CLAP_AUDIO_PORT_IS_MAIN;
      info->channel_count = MNumAudioOutputs;
      if (MNumAudioOutputs == 1) info->port_type = CLAP_PORT_MONO;
      else if (MNumAudioOutputs == 2) info->port_type = CLAP_PORT_STEREO;
      else info->port_type = 0;
      info->in_place_pair = CLAP_INVALID_ID;
    }
    return true;
  }

//------------------------------
private: // clap-params
//------------------------------

  uint32_t params_count() final {
    return MNumControlInputs;
  }

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) final {
    //return MIP_Plugin::params_get_info(param_index,param_info);
    uint32_t port_index = controlInputIndex(param_index);
    const char* name = MLadspaPorts[port_index].name;
    strncpy(param_info->name,name,CLAP_NAME_SIZE-1);
    strncpy(param_info->module,"",CLAP_MODULE_SIZE-1);
    param_info->id = param_index;
    param_info->flags = 0;
    param_info->cookie = nullptr;
    param_info->min_value = 0;
    param_info->max_value = 1;
    param_info->default_value = 0;
    return true;
  }

//  bool params_get_value(clap_id param_id, double *value) final {
//    return MIP_Plugin::params_get_value(param_id,value);
//  }
//
//  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) {
//  }
//
//  bool params_text_to_value_callback(clap_id param_id, const char *display, double *value) {
//  }
//
//  void params_flush_callback(const clap_input_events_t* in, const clap_output_events_t* out) {
//  }

//------------------------------
public: // ladspa
//------------------------------

  const char*     name(void)                      { return MLadspaDescriptor->Name; }
  const char*     label(void)                     { return MLadspaDescriptor->Label; }
  const char*     maker(void)                     { return MLadspaDescriptor->Maker; }
  const char*     copyright(void)                 { return MLadspaDescriptor->Copyright; }
  uint32_t        uniqueId(void)                  { return MLadspaDescriptor->UniqueID; }
  MIP_LadspaPort* port(int i)                     { return &MLadspaPorts[i]; }
  MIP_LadspaPort* audioInputPort(int i)           { return &MLadspaPorts[MAudioInputIndex[i]]; }
  MIP_LadspaPort* audioOutputPort(int i)          { return &MLadspaPorts[MAudioOutputIndex[i]]; }
  MIP_LadspaPort* controlInputPort(int i)         { return &MLadspaPorts[MControlInputIndex[i]]; }
  MIP_LadspaPort* controlOutputPort(int i)        { return &MLadspaPorts[MControlOutputIndex[i]]; }
  uint32_t        numAudioInputs(void)            { return MNumAudioInputs; }
  uint32_t        numAudioOutputs(void)           { return MNumAudioOutputs; }
  uint32_t        numControlInputs(void)          { return MNumControlInputs; }
  uint32_t        numControlOutputs(void)         { return MNumControlOutputs; }
  uint32_t        audioInputIndex(uint32_t i)     { return MAudioInputIndex[i]; }
  uint32_t        audioOutputIndex(uint32_t i)    { return MAudioOutputIndex[i]; }
  uint32_t        controlInputIndex(uint32_t i)   { return MControlInputIndex[i]; }
  uint32_t        controlOutputIndex(uint32_t i)  { return MControlOutputIndex[i]; }

  //----------

  void printLadspaDescriptor(void) {
    MIP_PRINT;
    MIP_DPrint("  name:                     '%s'\n",  MLadspaDescriptor->Name);
    MIP_DPrint("  label:                    '%s'\n",  MLadspaDescriptor->Label);
    MIP_DPrint("  maker:                    '%s'\n",  MLadspaDescriptor->Maker);
    MIP_DPrint("  copyright:                '%s'\n",  MLadspaDescriptor->Copyright);
    MIP_DPrint("  unique id:                %i\n",    MLadspaDescriptor->UniqueID);
    MIP_DPrint("  num ports:                %i\n",    MLadspaDescriptor->PortCount);
    MIP_DPrint("  flags:                    0x%x\n",  MLadspaFlags);
    MIP_DPrint("  instantiate:              0x%x\n",  MLadspaDescriptor->instantiate);
    MIP_DPrint("  connect_port:             0x%x\n",  MLadspaDescriptor->connect_port);
    MIP_DPrint("  activate:                 0x%x\n",  MLadspaDescriptor->activate);
    MIP_DPrint("  run:                      0x%x\n",  MLadspaDescriptor->run);
    MIP_DPrint("  run_adding:               0x%x\n",  MLadspaDescriptor->run_adding);
    MIP_DPrint("  set_run_adding_gain:      0x%x\n",  MLadspaDescriptor->set_run_adding_gain);
    MIP_DPrint("  deactivate:               0x%x\n",  MLadspaDescriptor->deactivate);
    MIP_DPrint("  cleanup:                  0x%x\n",  MLadspaDescriptor->cleanup);
    MIP_DPrint("  num audio input ports:    %i\n",    MNumAudioInputs );
    MIP_DPrint("  num audio output ports:   %i\n",    MNumAudioOutputs );
    MIP_DPrint("  num control input ports:  %i\n",    MNumControlInputs );
    MIP_DPrint("  num control output ports: %i\n",    MNumControlOutputs );
  }

//------------------------------
public: // ladspa
//------------------------------

  void ladspa_instantiate(uint32_t ASampleRate) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->instantiate) {
        MLadspaHandle = MLadspaDescriptor->instantiate(MLadspaDescriptor,ASampleRate);
        if (MLadspaHandle) MIsInstantiated = true;
      }
    }
  }

  //----------

  void ladspa_connect_port(uint32_t APort, float* APtr) {
    if (MIsInstantiated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->connect_port) {
          MLadspaDescriptor->connect_port(MLadspaHandle,APort,APtr);
        }
      }
    }
  }

  //----------

  void ladspa_activate(void) {
    if (MIsInstantiated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->activate) {
          MLadspaDescriptor->activate(MLadspaHandle);
          MIsActivated = true;
        }
      }
    }
  }

  //----------

  void ladspa_run(uint32_t ANum) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->run) {
          MLadspaDescriptor->run(MLadspaHandle,ANum);
        }
      }
    }
  }

  //----------

  void ladspa_run_adding(uint32_t ANum) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->run_adding) {
          MLadspaDescriptor->run_adding(MLadspaHandle,ANum);
        }
      }
    }
  }

  //----------

  void ladspa_set_run_adding_gain(float AGain) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->set_run_adding_gain) {
          MLadspaDescriptor->set_run_adding_gain(MLadspaHandle,AGain);
        }
      }
    }
  }

  //----------

  void ladspa_deactivate(void) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->deactivate) {
          MLadspaDescriptor->deactivate(MLadspaHandle);
          MIsActivated = false;
        }
      }
    }
  }

  //----------

  void ladspa_cleanup(void) {
    if (MIsInstantiated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->cleanup) {
          MLadspaDescriptor->cleanup(MLadspaHandle);
          MIsInstantiated = false;
        }
      }
    }
  }

//------------------------------
private:
//------------------------------

  //uint32_t countDescriptors(void) {
  //  MIP_Print("\n");
  //  uint32_t num = 0;
  //  if (MGetLadspaDescriptor) {
  //    const LADSPA_Descriptor* desc = MGetLadspaDescriptor(num);
  //    while (desc) {
  //      num++;
  //      desc = MGetLadspaDescriptor(num);
  //    }
  //  }
  //  return num;
  //}

  //----------

  //bool initDescriptor(uint32_t AIndex) {
  //  MIP_Print("initializing descriptor #%i\n",AIndex);
  //  if (MGetLadspaDescriptor) {
  //    MLadspaDescriptor = MGetLadspaDescriptor(AIndex);
  //    MIP_Print("MLadspaDescriptor: %p\n",MLadspaDescriptor);
  //    return (MLadspaDescriptor);
  //  }
  //  MIP_Print("error initializing descriptor\n");
  //  return false;
  //}

  //----------

  void initPorts(void) {
    MIP_Print("\n");
    MNumAudioInputs     = 0;
    MNumAudioOutputs    = 0;
    MNumControlInputs   = 0;
    MNumControlOutputs  = 0;
    MLadspaFlags        = MIP_LADSPA_FLAGS_NONE;
    if (MLadspaDescriptor) {

      if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_REALTIME)         { MLadspaFlags |= MIP_LADSPA_FLAGS_REALTIME; }
      if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_INPLACE_BROKEN)   { MLadspaFlags |= MIP_LADSPA_FLAGS_INPLACE_BROKEN; }
      if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_HARD_RT_CAPABLE)  { MLadspaFlags |= MIP_LADSPA_FLAGS_HARD_RT_CAPABLE; }

      for (uint32_t i=0; i<MLadspaDescriptor->PortCount; i++) {

        MLadspaPorts[i].name    = MLadspaDescriptor->PortNames[i];
        MLadspaPorts[i].flags   = MIP_LADSPA_FLAGS_NONE;
        MLadspaPorts[i].val     = 0;
        MLadspaPorts[i].minval  = 0;
        MLadspaPorts[i].maxval  = 1;

        LADSPA_PortDescriptor pd = MLadspaDescriptor->PortDescriptors[i];
        if (pd & LADSPA_PORT_AUDIO)   { MLadspaPorts[i].flags |= MIP_LADSPA_PORT_AUDIO; }
        if (pd & LADSPA_PORT_CONTROL) { MLadspaPorts[i].flags |= MIP_LADSPA_PORT_CONTROL; }
        if (pd & LADSPA_PORT_INPUT)   { MLadspaPorts[i].flags |= MIP_LADSPA_PORT_INPUT; }
        if (pd & LADSPA_PORT_OUTPUT)  { MLadspaPorts[i].flags |= MIP_LADSPA_PORT_OUTPUT; }

        uint32_t hints = MLadspaDescriptor->PortRangeHints[i].HintDescriptor;
        if (hints & LADSPA_HINT_BOUNDED_BELOW)  { MLadspaPorts[i].minval = MLadspaDescriptor->PortRangeHints[i].LowerBound; }
        if (hints & LADSPA_HINT_BOUNDED_ABOVE)  { MLadspaPorts[i].maxval = MLadspaDescriptor->PortRangeHints[i].UpperBound; }
        if (hints & LADSPA_HINT_TOGGLED)        { MLadspaPorts[i].flags |= MIP_LADSPA_PARAM_TOGGLED; }
        if (hints & LADSPA_HINT_SAMPLE_RATE)    { MLadspaPorts[i].flags |= MIP_LADSPA_PARAM_SAMPLERATE; }
        if (hints & LADSPA_HINT_LOGARITHMIC)    { MLadspaPorts[i].flags |= MIP_LADSPA_PARAM_LOGARITHMIC; }
        if (hints & LADSPA_HINT_INTEGER)        { MLadspaPorts[i].flags |= MIP_LADSPA_PARAM_INTEGER; }

        uint32_t defmask = hints & LADSPA_HINT_DEFAULT_MASK;
        if (defmask == LADSPA_HINT_DEFAULT_MINIMUM) MLadspaPorts[i].val =  MLadspaPorts[i].minval;
        if (defmask == LADSPA_HINT_DEFAULT_LOW)     MLadspaPorts[i].val = (MLadspaPorts[i].minval * 0.75 + MLadspaPorts[i].maxval * 0.25);
        if (defmask == LADSPA_HINT_DEFAULT_MIDDLE)  MLadspaPorts[i].val = (MLadspaPorts[i].minval * 0.50 + MLadspaPorts[i].maxval * 0.50);
        if (defmask == LADSPA_HINT_DEFAULT_HIGH)    MLadspaPorts[i].val = (MLadspaPorts[i].minval * 0.25 + MLadspaPorts[i].maxval * 0.75);
        if (defmask == LADSPA_HINT_DEFAULT_MAXIMUM) MLadspaPorts[i].val =  MLadspaPorts[i].maxval;
        if (defmask == LADSPA_HINT_DEFAULT_0)       MLadspaPorts[i].val = 0;
        if (defmask == LADSPA_HINT_DEFAULT_1)       MLadspaPorts[i].val = 1;
        if (defmask == LADSPA_HINT_DEFAULT_100)     MLadspaPorts[i].val = 100;
        if (defmask == LADSPA_HINT_DEFAULT_440)     MLadspaPorts[i].val = 440;

        if ( (MLadspaPorts[i].flags & MIP_LADSPA_PORT_AUDIO)   && (MLadspaPorts[i].flags & MIP_LADSPA_PORT_INPUT)  ) MAudioInputIndex[   MNumAudioInputs++]    = i;
        if ( (MLadspaPorts[i].flags & MIP_LADSPA_PORT_AUDIO)   && (MLadspaPorts[i].flags & MIP_LADSPA_PORT_OUTPUT) ) MAudioOutputIndex[  MNumAudioOutputs++]   = i;
        if ( (MLadspaPorts[i].flags & MIP_LADSPA_PORT_CONTROL) && (MLadspaPorts[i].flags & MIP_LADSPA_PORT_INPUT)  ) MControlInputIndex[ MNumControlInputs++]  = i;
        if ( (MLadspaPorts[i].flags & MIP_LADSPA_PORT_CONTROL) && (MLadspaPorts[i].flags & MIP_LADSPA_PORT_OUTPUT) ) MControlOutputIndex[MNumControlOutputs++] = i;

        //MIP_Print("port %i\n",i);
        //MIP_Print("  name: %s\n",MLadspaPorts[i].name);
        //MIP_Print("  flags: %04x\n",MLadspaPorts[i].flags);

      }
    }
  }

  //----------

  //void initParameters() {
  //}

};


//----------------------------------------------------------------------
#endif






//----------------------------------------------------------------------

#if 0



};

#endif // 0

//----------------------------------------------------------------------



/*
  https://ccrma.stanford.edu/mirrors/lalists/lad/2001/Sep/0014.html

  Hi,
  I would like to know if there is any agreed upon logarithmic function to
  use for LADSPA control ports when the LADSPA_HINT_LOGARITHMIC is set. In
  practice, the function should map display coordinates (the slider) to
  control values.
  The best I have come up with myself is this:
    Control port value, v
    Control port low limit, v_low
    Control port high limit, v_high
    Display coordinates, d
    Display coordinates low limit, d_low
    Display coordinates high limit, d_high
    Transposition to positive range, log_offset
    Setup:
      log_offset = -v_low+1;
      d_low = log(v_low + log_offset);
      d_high = log(v_high + log_offset);
      d = log(v + log_offset);
    Slider moved:
      v = exp(d) - log_offset;
  Marcus
*/

//----------

/*
  http://nwoeanhinnogaehr.github.io/ladspa.rs/ladspa/enum.DefaultValue.html
  Enum ladspa::DefaultValue
  Minimum
    Equal to the lower_bound of the Port.
  Low
    For ports with LADSPA_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.75 + log(upper_bound) * 0.25)
    Otherwise, this should be
      (lower_bound * 0.75 + upper_bound * 0.25)
  Middle
    For ports with CONTROL_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.5 + log(upper_bound) * 0.5)
    Otherwise, this should be
      (lower_bound * 0.5 + upper_bound * 0.5)
  High
    For ports with LADSPA_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.25 + log(upper_bound) * 0.75)
      Otherwise, this should be
      (lower_bound * 0.25 + upper_bound * 0.75)
*/

//----------

/*
  http://lists.linuxaudio.org/pipermail/linux-audio-dev/2006-April/015714.html
  if a is the users parameter UI control value in [0,1], u is the lower
  bound, v is the upper bound, and b is the log base, then:
  port val  = ( (1-a) log(u) + a log(v) )^b
            = ( -a log(u) + log(u) + a log(v) )^b
            = log( ( u+v^a ) / u^a )^b
            = (u + v^a) / u^a
*/
