#ifndef sa_transient_killer_included
#define sa_transient_killer_included
//----------------------------------------------------------------------

// based on the 'transientkiller' jesusonic effect by 'loser'

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define MIP_NO_GUI

//----------

#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* sa_transient_killer_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_transient_killer_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_transient_killer",
  "sa_transient_killer",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_transient_killer",
  sa_transient_killer_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_transient_killer_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc  = true;
  float MSampleRate = 0.0;
  float cAmpDB_     = 8.65617025;
  float vol         = 0.0f;
  float db0         = 0.0f;
  float db1         = 0.0f;

//  float treshDB = 0.0f;
//  float ratio   = 0.0f;

  const clap_audio_port_info_t myAudioInputPorts[1] = {
    { 0, "input1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_audio_port_info_t myAudioOutputPorts[1] = {
    { 0, "output1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  //appendAudioInputPort(  &myAudioInputPorts[0]  );
  //appendAudioOutputPort( &myAudioOutputPorts[0] );



//------------------------------
public:
//------------------------------

  sa_transient_killer_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_transient_killer_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {

    float treshDB = MParameters[0]->getValue();
    float ratio   = MParameters[1]->getValue();

    vol = 1/exp( (treshDB -treshDB/ratio) / cAmpDB_);
    need_recalc = false;
  }

//------------------------------
private:
//------------------------------

//  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
//    //MIP_Trace("%i %.3f\n",AIndex,AValue);
//    switch(AIndex) {
//      case 0: treshDB = AValue; break;
//      case 1: ratio   = AValue; break;
//    }
//  }

  void processParamValueEvent(const clap_event_param_value_t* param_value) final {
    MIP_Plugin::processParamValueEvent(param_value);
    need_recalc = true;
  }

  //----------

  void processAudioBlock(const clap_process_t *process) final {

    float treshDB = MParameters[0]->getValue();
    float ratio   = MParameters[1]->getValue();

    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      db0 = log(abs(spl0)) * cAmpDB_;
      if (db0 > treshDB) spl0 = exp( (treshDB + (db0 - treshDB)/ratio) / cAmpDB_) * MIP_Sign(spl0);
      db1 = log(abs(spl1)) * cAmpDB_;
      if (db1 > treshDB) spl1 = exp( (treshDB + (db1 - treshDB)/ratio) / cAmpDB_) * MIP_Sign(spl1);
      *out0++ = spl0 * vol;
      *out1++ = spl1 * vol;
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {

    appendAudioInputPort(  &myAudioInputPorts[0]  );
    appendAudioOutputPort( &myAudioOutputPorts[0] );

    appendParameter(new MIP_Parameter( 0, "Threshold", "",  -12, 1,  0, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 1, "Ratio",     "",   1,  50, 1, CLAP_PARAM_IS_AUTOMATABLE ));
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





/*

(C) 2006, Michael Gruhn.

NO WARRANTY IS GRANTED. THIS PLUG-IN IS PROVIDED ON AN "AS IS" BASIS, WITHOUT
WARRANTY OF ANY KIND. NO LIABILITY IS GRANTED, INCLUDING, BUT NOT LIMITED TO,
ANY DIRECT OR INDIRECT,  SPECIAL,  INCIDENTAL OR CONSEQUENTIAL DAMAGE ARISING
OUT OF  THE  USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER FAILTURE  OF
MALFUNCTION INCLUDED.  THE USE OF THE SOURCE CODE,  EITHER  PARTIALLY  OR  IN
TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S INTENTION,  AND
USED WITH ACKNOWLEDGEMENT OF THE AUTHOR. FURTHERMORE IS THIS PLUG-IN A  THIRD
PARTY CONTRIBUTION,  EVEN IF INCLUDED IN REAPER(TM),  COCKOS INCORPORATED  OR
ITS AFFILIATES HAVE NOTHING TO DO WITH IT.  LAST BUT NOT LEAST, BY USING THIS
PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR, AS WELL AS THE CLAIM TO
ENTRUST SOMEBODY ELSE WITH DOING SO.

*/
