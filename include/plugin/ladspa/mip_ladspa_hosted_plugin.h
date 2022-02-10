#ifndef mip_ladspa_hosted_plugin_included
#define mip_ladspa_hosted_plugin_included
//----------------------------------------------------------------------

#include <dlfcn.h>
#include "base/system/mip_library.h"
#include "plugin/ladspa/mip_ladspa.h"

//----------

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
#define MIP_LADSPA_PORT_AUDIO             0
#define MIP_LADSPA_PORT_CONTROL           0
#define MIP_LADSPA_PORT_INPUT             0
#define MIP_LADSPA_PORT_OUTPUT            0

#define MIP_LADSPA_PARAM_NONE             0
#define MIP_LADSPA_PARAM_TOGGLED          0
#define MIP_LADSPA_PARAM_SAMPLERATE       0
#define MIP_LADSPA_PARAM_LOGARITHMIC      0
#define MIP_LADSPA_PARAM_INTEGER          0


//----------

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

class MIP_LadspaHostedPlugin {

//------------------------------
protected:
//------------------------------

  uint32_t                    MLadspaFlags          = MIP_LADSPA_FLAGS_NONE;
  LADSPA_Handle               MLadspaHandle         = nullptr;
  const LADSPA_Descriptor*    MLadspaDescriptor     = nullptr;
  LADSPA_Descriptor_Function  MGetLadspaDescriptor  = nullptr;

  uint32_t                    MNumAudioInputs       = 0;
  uint32_t                    MNumAudioOutputs      = 0;
  uint32_t                    MNumControlInputs     = 0;
  uint32_t                    MNumControlOutputs    = 0;
  MIP_LadspaPort              MLadspaPorts[MIP_LADSPA_MAX_PORTS]                  = {0};
  int32_t                     MAudioInputIndex[MIP_LADSPA_MAX_AUDIO_INPUTS]       = {0};
  int32_t                     MAudioOutputIndex[MIP_LADSPA_MAX_AUDIO_OUTPUTS]     = {0};
  int32_t                     MControlInputIndex[MIP_LADSPA_MAX_CONTROL_INPUTS]   = {0};
  int32_t                     MControlOutputIndex[MIP_LADSPA_MAX_CONTROL_OUTPUTS] = {0};

  bool                        MIsInstantiated       = false;
  bool                        MIsActivated          = false;

//------------------------------
public:
//------------------------------

  MIP_LadspaHostedPlugin() {
  }

  //----------

  virtual ~MIP_LadspaHostedPlugin() {
  }

//------------------------------
public:
//------------------------------

  //bool              instantiated(void)            { return MInstantiated; }
  //bool              activated(void)               { return MActivated; }
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

//------------------------------
public:
//------------------------------

  bool load(const char* AFilename) {
    MIP_Print("%s\n",AFilename);
//    if (MIP_Library::load(AFilename)) {
//      MGetLadspaDescriptor = (LADSPA_Descriptor_Function)getSymbol("ladspa_descriptor");
//      return true;
//    }
    return false;
  }

  //----------

  uint32_t countDescriptors(void) {
    MIP_Print("\n");
    uint32_t num = 0;
    if (MGetLadspaDescriptor) {
      const LADSPA_Descriptor* desc = MGetLadspaDescriptor(num);
      while (desc) {
        num++;
        desc = MGetLadspaDescriptor(num);
      }
    }
    return num;
  }

  //----------

  bool initDescriptor(uint32_t AIndex) {
    MIP_Print("%i\n",AIndex);
    if (MGetLadspaDescriptor) {
      MLadspaDescriptor = MGetLadspaDescriptor(AIndex);
      return (MLadspaDescriptor);
    }
    return false;
  }

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
        if (hints&LADSPA_HINT_BOUNDED_BELOW)  { MLadspaPorts[i].minval = MLadspaDescriptor->PortRangeHints[i].LowerBound; }
        if (hints&LADSPA_HINT_BOUNDED_ABOVE)  { MLadspaPorts[i].maxval = MLadspaDescriptor->PortRangeHints[i].UpperBound; }
        if (hints&LADSPA_HINT_TOGGLED)        { MLadspaPorts[i].flags |= MIP_LADSPA_PARAM_TOGGLED; }
        if (hints&LADSPA_HINT_SAMPLE_RATE)    { MLadspaPorts[i].flags |= MIP_LADSPA_PARAM_SAMPLERATE; }
        if (hints&LADSPA_HINT_LOGARITHMIC)    { MLadspaPorts[i].flags |= MIP_LADSPA_PARAM_LOGARITHMIC; }
        if (hints&LADSPA_HINT_INTEGER)        { MLadspaPorts[i].flags |= MIP_LADSPA_PARAM_INTEGER; }
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
        if ( (MLadspaPorts[i].flags&MIP_LADSPA_PORT_AUDIO)   && (MLadspaPorts[i].flags&MIP_LADSPA_PORT_INPUT)  ) MAudioInputIndex[   MNumAudioInputs++]    = i;
        if ( (MLadspaPorts[i].flags&MIP_LADSPA_PORT_AUDIO)   && (MLadspaPorts[i].flags&MIP_LADSPA_PORT_OUTPUT) ) MAudioOutputIndex[  MNumAudioOutputs++]   = i;
        if ( (MLadspaPorts[i].flags&MIP_LADSPA_PORT_CONTROL) && (MLadspaPorts[i].flags&MIP_LADSPA_PORT_INPUT)  ) MControlInputIndex[ MNumControlInputs++]  = i;
        if ( (MLadspaPorts[i].flags&MIP_LADSPA_PORT_CONTROL) && (MLadspaPorts[i].flags&MIP_LADSPA_PORT_OUTPUT) ) MControlOutputIndex[MNumControlOutputs++] = i;
      }
    }
  }

  //----------

  void dumpLadspaDescriptor(void) {
    MIP_DPrint("LADSPA_Descriptor:\n");
    MIP_DPrint("  name:                     '%s'\n",    MLadspaDescriptor->Name);
    MIP_DPrint("  label:                    '%s'\n",    MLadspaDescriptor->Label);
    MIP_DPrint("  maker:                    '%s'\n",    MLadspaDescriptor->Maker);
    MIP_DPrint("  copyright:                '%s'\n",    MLadspaDescriptor->Copyright);
    MIP_DPrint("  unique id:                %i\n",      MLadspaDescriptor->UniqueID);
    MIP_DPrint("  num ports:                %i\n",      MLadspaDescriptor->PortCount);
    MIP_DPrint("  flags:                    0x%08x\n",  MLadspaFlags);
    MIP_DPrint("  instantiate:              0x%08x\n",  MLadspaDescriptor->instantiate);
    MIP_DPrint("  connect_port:             0x%08x\n",  MLadspaDescriptor->connect_port);
    MIP_DPrint("  activate:                 0x%08x\n",  MLadspaDescriptor->activate);
    MIP_DPrint("  run:                      0x%08x\n",  MLadspaDescriptor->run);
    MIP_DPrint("  run_adding:               0x%08x\n",  MLadspaDescriptor->run_adding);
    MIP_DPrint("  set_run_adding_gain:      0x%08x\n",  MLadspaDescriptor->set_run_adding_gain);
    MIP_DPrint("  deactivate:               0x%08x\n",  MLadspaDescriptor->deactivate);
    MIP_DPrint("  cleanup:                  0x%08x\n",  MLadspaDescriptor->cleanup);
    MIP_DPrint("  num audio input ports:    %i\n",      MNumAudioInputs );
    MIP_DPrint("  num audio output ports:   %i\n",      MNumAudioOutputs );
    MIP_DPrint("  num control input ports:  %i\n",      MNumControlInputs );
    MIP_DPrint("  num control output ports: %i\n",      MNumControlOutputs );
  }

//------------------------------
public:
//------------------------------

  void instantiate(uint32_t ASampleRate) {
//    if (MLibLoaded) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->instantiate) {
          MLadspaHandle = MLadspaDescriptor->instantiate(MLadspaDescriptor,ASampleRate);
          if (MLadspaHandle) MIsInstantiated = true;
        }
      }
//    }
  }

  //----------

  void connect_port(uint32_t APort, float* APtr) {
    if (MIsInstantiated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->connect_port) {
          MLadspaDescriptor->connect_port(MLadspaHandle,APort,APtr);
        }
      }
    }
  }

  //----------

  void activate(void) {
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

  void run(uint32_t ANum) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->run) {
          MLadspaDescriptor->run(MLadspaHandle,ANum);
        }
      }
    }
  }

  //----------

  void run_adding(uint32_t ANum) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->run_adding) {
          MLadspaDescriptor->run_adding(MLadspaHandle,ANum);
        }
      }
    }
  }

  //----------

  void set_run_adding_gain(float AGain) {
    if (MIsActivated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->set_run_adding_gain) {
          MLadspaDescriptor->set_run_adding_gain(MLadspaHandle,AGain);
        }
      }
    }
  }

  //----------

  void deactivate(void) {
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

  void cleanup(void) {
    if (MIsInstantiated) {
      if (MLadspaDescriptor) {
        if (MLadspaDescriptor->cleanup) {
          MLadspaDescriptor->cleanup(MLadspaHandle);
          MIsInstantiated = false;
        }
      }
    }
  }

};

//----------------------------------------------------------------------
#endif



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
