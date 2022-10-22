#ifndef sa_grains_included
#define sa_grains_included
//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_MAX_GRAINS 1024
#define MIP_BUFFERSIZE (1024*1024)

//----------

// todo: not array of structs.. struct of/with array..
struct MIP_Grain {
  bool  on;
  float pos;
  float sta;
  float end;
  float dur;
  float spd;
  float ph;
  float pa;
  float dh;
  float da;
};

//----------

const char* freeze_txt[2] = { "off", "on" };

//----------------------------------------------------------------------

const char* sa_grains_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_grains_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_grains",
  "sa_grains",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_grains",
  sa_grains_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_grains_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_precalc = true;
  bool need_recalc = false;
  float MSampleRate = 0.0;

  MIP_Grain MGrains[MIP_MAX_GRAINS];
  float     MBuffer[MIP_BUFFERSIZE];

  float     master;
  int32_t   numgrains;
  int32_t   buffersize;
  bool      freeze;
  float     graindist;
  float     grainsize;
  float     graindur;
  float     grainpitch;
  float     grainenv;
  float     startjit;
  float     pitchjit;
  float     sizejit;
  float     durjit;
  int32_t   index;
  float     countdown;


//------------------------------
public:
//------------------------------

  sa_grains_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    master      = 0.0f;
    numgrains   = 0;
    buffersize  = 0;
    freeze      = false;
    graindist   = 0.0f;
    grainsize   = 0.0f;
    graindur    = 0.0f;
    grainpitch  = 1.0f;
    grainenv    = 0.0f;
    startjit    = 0.2f;
    pitchjit    = 0.2f;
    sizejit     = 0.2f;
    durjit      = 0.2f;
    index = 0;
    countdown = 0;
  }

  //----------

  virtual ~sa_grains_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

    //    appendParameter( new MIP_FloatParameter( "master",     -6,   -60, 6     ));
    //    appendParameter( new MIP_IntParameter(   "num gr",      10,   1,  100   ));
    //    appendParameter( new MIP_FloatParameter( "bufsize",     1000, 1,  1000  ));
    //    appendParameter( new MIP_TextParameter(  "freeze",      0,    2,  freeze_txt )); // text
    //    appendParameter( new MIP_FloatParameter( "gr dist",     20,   0,  100   ));
    //    appendParameter( new MIP_FloatParameter( "gr size",     30,   1,  100   ));
    //    appendParameter( new MIP_FloatParameter( "gr dur",      300,  1,  1000  ));
    //    appendParameter( new MIP_FloatParameter( "gr pitch",    1,    0,  10    ));
    //    appendParameter( new MIP_FloatParameter( "grenv",       0               ));
    //    appendParameter( new MIP_FloatParameter( "dist jit",    0.2             ));
    //    appendParameter( new MIP_FloatParameter( "pitch jit",   0.2             ));
    //    appendParameter( new MIP_FloatParameter( "size jit",    0.2             ));
    //    appendParameter( new MIP_FloatParameter( "dur jit",     0.2             ));

  bool init() final {
    appendParameter(new MIP_Parameter(     0, CLAP_PARAM_IS_AUTOMATABLE, "Master",       "", -60,  6,      -6    ));
    appendParameter(new MIP_IntParameter(  1, CLAP_PARAM_IS_AUTOMATABLE, "Num Grains",   "",  1,   100,    10    ));
    appendParameter(new MIP_Parameter(     2, CLAP_PARAM_IS_AUTOMATABLE, "Buffer Size",  "",  1,   1000,   1000  ));
    appendParameter(new MIP_TextParameter( 3, CLAP_PARAM_IS_AUTOMATABLE, "Freeze",       "",  0,   1,      0, freeze_txt ));
    appendParameter(new MIP_Parameter(     4, CLAP_PARAM_IS_AUTOMATABLE, "Grain Dist",   "",  0,   100,    20    ));
    appendParameter(new MIP_Parameter(     5, CLAP_PARAM_IS_AUTOMATABLE, "Grain Size",   "",  1,   100,    30    ));
    appendParameter(new MIP_Parameter(     6, CLAP_PARAM_IS_AUTOMATABLE, "Grain Dur",    "",  1,   1000,   300   ));
    appendParameter(new MIP_Parameter(     7, CLAP_PARAM_IS_AUTOMATABLE, "Grain Pitch",  "",  0,   10,     1     ));
    appendParameter(new MIP_Parameter(     8, CLAP_PARAM_IS_AUTOMATABLE, "Grain Env",    "",  0,   1,      0     ));
    appendParameter(new MIP_Parameter(     9, CLAP_PARAM_IS_AUTOMATABLE, "Dist Jitter",  "",  0,   1,      0.2   ));
    appendParameter(new MIP_Parameter(    10, CLAP_PARAM_IS_AUTOMATABLE, "Pitch Jitter", "",  0,   1,      0.2   ));
    appendParameter(new MIP_Parameter(    11, CLAP_PARAM_IS_AUTOMATABLE, "Size Jitter",  "",  0,   1,      0.2   ));
    appendParameter(new MIP_Parameter(    12, CLAP_PARAM_IS_AUTOMATABLE, "Dur Jitter",   "",  0,   1,      0.2   ));
    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues();
      need_precalc = true;
      return true;
    }
    return false;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

//------------------------------
private:
//------------------------------

  void precalc(float srate) {
    need_precalc = false;
    master = pow(2,(MParameterValues[0] / 6));
    numgrains = trunc(MParameterValues[1]);
    buffersize = trunc( (MParameterValues[2] / 1000) * MSampleRate );
    if (MParameterValues[3] > 0.5) freeze = true;
    else freeze = false;
    graindist = (MParameterValues[4] / 1000) * MSampleRate;
    grainsize = (MParameterValues[5] / 1000) * MSampleRate;
    graindur = (MParameterValues[6] / 1000) * MSampleRate;
  }

  //----------

  void recalc(float srate) {
    need_recalc = false;
  }

//------------------------------
private:
//------------------------------

  void handle_parameter_event(clap_event_param_value_t* param_value) final {
    MIP_Plugin::handle_parameter_event(param_value);
    uint32_t index = param_value->param_id;
    float value = param_value->value;
    //MIP_Print("%i = %f\n",index,value);
    switch (index) {
      case 0:  master      = pow(2,(value / 6)); break;
      case 1:  numgrains   = trunc( value ); break;
      case 2:  buffersize  = trunc( (value / 1000) * MSampleRate ); break;
      case 3:  value > 0.5 ? freeze=true : freeze=false; break;
      case 4:  graindist   = (value / 1000) * MSampleRate; break;
      case 5:  grainsize   = (value / 1000) * MSampleRate; break;
      case 6:  graindur    = (value / 1000) * MSampleRate; break;
      case 7:  grainpitch  = value; break;
      case 8:  grainenv    = value; break;
      case 9:  startjit    = value; break;
      case 10: pitchjit    = value; break;
      case 11: sizejit     = value; break;
      case 12: durjit      = value; break;
    }
    need_recalc = true;
  }

  //----------

  void handle_process(const clap_process_t *process) final {

    if (need_precalc) precalc(MSampleRate);
    if (need_recalc) recalc(MSampleRate);

    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];

    float spl0,spl1;
    for (uint32_t i=0; i<len; i++) {
      spl0 = *in0++;
      spl1 = *in1++;
      processSample(&spl0,&spl1);
      *out0++ = spl0;
      *out1++ = spl1;
    }
  }

//------------------------------
private:
//------------------------------

  void processSample(float* spl0, float* spl1) {
    float in0 = *spl0;
    float in1 = *spl1;
    float out0 = 0;
    float out1 = 0;
    int32_t newgrain = -1;
    //if (numgrains > 0) {
      for (int32_t i=0; i<numgrains; i++) {
        if (MGrains[i].on) {
          MGrains[i].pos += MGrains[i].spd;
          if (MGrains[i].pos >= MGrains[i].end) MGrains[i].pos = MGrains[i].sta;
          if (MGrains[i].pos >= buffersize) MGrains[i].pos -= buffersize;
          MGrains[i].ph += (MGrains[i].pa*2);
          if (MGrains[i].ph >= 2) MGrains[i].ph -= 2;
          MGrains[i].dh += (MGrains[i].da*2);
          if (MGrains[i].dh >=2) MGrains[i].dh -= 2;
          MGrains[i].dur -= 1;
          if (MGrains[i].dur <= 0) MGrains[i].on = false;
          float gvol = MGrains[i].ph * (2-abs(MGrains[i].ph)); // abs-neg ?
          float dvol = MGrains[i].dh * (2-abs(MGrains[i].dh));
          int32_t temp = trunc( MGrains[i].pos );
          temp *= 2;
          out0 += MBuffer[ temp ] * dvol * gvol;
          out1 += MBuffer[ temp+1 ] * dvol * gvol;
        }
        else newgrain = i;
      }
    //}
    if (countdown <= 0) {
      countdown = graindist;
      if (newgrain > 0) {
        float startrnd = 1 + (startjit * MIP_RandomSigned());
        float pitchrnd = 1 + (pitchjit * MIP_RandomSigned());
        float sizernd  = 1 + (sizejit  * MIP_RandomSigned());
        float durrnd   = 1 + (durjit   * MIP_RandomSigned());
        float siz = (grainsize*sizernd);
        float st = index * startrnd;
        if (st >= buffersize) st -= buffersize;
        if (st <  0) st += buffersize;
        float en = st + siz;
        if (en >= buffersize) en = buffersize; // clamp
        if (en <  0) en = 0;
        float du = graindur*durrnd;
        MGrains[newgrain].on   = true;
        MGrains[newgrain].pos  = st;
        MGrains[newgrain].sta  = st;
        MGrains[newgrain].end  = en;
        MGrains[newgrain].dur  = du;
        MGrains[newgrain].spd  = grainpitch * pitchrnd;
        MGrains[newgrain].ph   = 0;
        MGrains[newgrain].pa   = 1 / siz;
        MGrains[newgrain].dh   = 0;
        MGrains[newgrain].da   = 1 / du;
      }
    }
    countdown -= 1;
    if (!freeze) {
      MBuffer[index*2] = in0;
      MBuffer[index*2+1] = in1;
    }
    index += 1;
    if (index >= buffersize) index -= buffersize;
    *spl0 = out0 * master;
    *spl1 = out1 * master;
  }

  //----------

};

//----------------------------------------------------------------------
#endif

















#if 0

// /DISKS/sda2/code/git/kode3/plugins/backup/backup/fx_grains.cpp




//------------------------------
public:
//------------------------------


  //void on_midiEvent(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {}
  //MIP_Editor* on_openEditor(void* AParent) override { return nullptr; }
  //void on_closeEditor() override {}
  //void on_idleEditor() override {}

  void on_processBlock(MIP_ProcessContext* AContext) override {
    MSampleRate = AContext->samplerate;

    /*
    if (need_init) init();
    if (need_recalc) recalc();
    */

    //KMemcpy(AOutputs[0],AInputs[0],ANumSamples*sizeof(KSample));
    //KMemcpy(AOutputs[1],AInputs[1],ANumSamples*sizeof(KSample));


    float* ins[2];
    float* outs[2];
    ins[0] = AContext->inputs[0];
    ins[1] = AContext->inputs[1];
    outs[0] = AContext->outputs[0];
    outs[1] = AContext->outputs[1];
    uint32_t len = AContext->length;
    while (len > 0) {
      processSample(ins,outs);
      ins[0]++;
      ins[1]++;
      outs[0]++;
      outs[1]++;
      len--;
    }

  }

};



#endif // 0
