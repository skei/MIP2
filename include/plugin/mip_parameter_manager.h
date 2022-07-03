#ifndef mip_parameter_manager_included
#define mip_parameter_manager_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_queue.h"
#include "plugin/mip_parameter.h"

//----------------------------------------------------------------------

#define MIP_PLUGIN_MAX_PARAM_EVENTS 4096
#define MIP_PLUGIN_MAX_GUI_EVENTS   32

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ParameterManager {

//------------------------------
private:
//------------------------------

  MIP_ParameterArray MParameterArray = {};

  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_PARAM_EVENTS> MAudioParamQueue  = {};
  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_PARAM_EVENTS> MGuiParamQueue    = {};
  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_GUI_EVENTS>   MHostParamQueue   = {};

  //double  MQueuedAudioParamValues[1024] = {0};
  //double  MQueuedGuiParamValues[1024]   = {0};
  //double  MQueuedHostParamValues[1024]  = {0};


//------------------------------
public:
//------------------------------

  MIP_ParameterManager() {
  }

  //----------

  virtual ~MIP_ParameterManager() {
  }

//------------------------------

  MIP_Parameter* operator [] (const uint32_t AIndex) {
    return MParameterArray[AIndex];
  }

  uint32_t size() {
    return MParameterArray.size();
  }

  void append(MIP_Parameter* AParameter) {
    MParameterArray.append(AParameter);
  }

//------------------------------
public:
//------------------------------

  void appendParameter(MIP_Parameter* AParameter) {
    MParameterArray.append(AParameter);
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameterArray.size(); i++) {
      delete MParameterArray[i];
      MParameterArray[i] = nullptr;
    }
  }

  //----------

  MIP_Parameter* getParameter(uint32_t AIndex) {
    return MParameterArray[AIndex];
  }

  //----------

  double getParameterValue(uint32_t AIndex) {
    return MParameterArray[AIndex]->getValue();
  }

  //----------

  void setParameterValue(uint32_t AIndex, double AValue) {
    MParameterArray[AIndex]->setValue(AValue);
  }

//------------------------------
public: // modulation
//------------------------------

  double  getParameterModulation(uint32_t AIndex) {
    return MParameterArray[AIndex]->getModulation();
  }

  //----------

  void setParameterModulation(uint32_t AIndex, double AValue) {
    MParameterArray[AIndex]->setModulation(AValue);
  }

//------------------------------
public: // queues
//------------------------------

  /*
    gui -> audio

    when we are changing a parameter from the gui,
    we also need to tell the audio processor about the changed parameters,
    so we queue up the events, and flush them all at the beginning of the
    next process (or params_flush) call
  */

  void queueAudioParam(uint32_t AIndex, double AValue) {
    //MQueuedAudioParamValues[AIndex] = AValue;
    MAudioParamQueue.write(AIndex);
  }

  //----------

  void flushAudioParams() {
    uint32_t index;
    while (MAudioParamQueue.read(&index)) {
      //double value = MQueuedAudioParamValues[index];
      double value = MParameterArray[index]->getValue();
      MIP_Print("%i = %.3f\n",index,value);
      //MAudioprocessor->updateParameter(index,value);
    }
  }

  //----------

  /*
    gui -> host

    ..and we also need to tell the host about the changed parameter
    we send all events at the end of process (or params_flush)
  */

  void queueHostParam(uint32_t AIndex, double AValue) {
    //MQueuedHostParamValues[AIndex] = AValue;
    MHostParamQueue.write(AIndex);
  }

  //----------

  void flushHostParams(const clap_output_events_t* out_events) {
    uint32_t index;
    while (MHostParamQueue.read(&index)) {
      //double value = MQueuedHostParamValues[index];
      double value = MParameterArray[index]->getValue();
      MIP_Print("%i = %.3f\n",index,value);
      clap_event_param_value_t event;
      event.header.size     = sizeof(clap_event_param_value_t);
      event.header.time     = 0;
      event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
      event.header.type     = CLAP_EVENT_PARAM_VALUE;
      event.header.flags    = 0;  // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
      event.param_id        = index;
      event.cookie          = nullptr;
      event.note_id         = -1;
      event.port_index      = -1;
      event.channel         = -1;
      event.key             = -1;
      event.value           = value;
      out_events->try_push(out_events,(clap_event_header_t*)&event);
    }
  }

  //----------

  /*
    host -> gui

    when a parameter is updated, we also want to update the gui..
    we queue the events, and fliush them all in a timer callback
  */

  void queueGuiParam(uint32_t AIndex, double AValue) {
    //MQueuedGuiParamValues[AIndex] = AValue;
    MGuiParamQueue.write(AIndex);
  }

  //----------

  void flushGuiParams() {
    uint32_t index;
    while (MGuiParamQueue.read(&index)) {
      //double value = MQueuedGuiParamValues[index];
      double value = MParameterArray[index]->getValue();
      MIP_Print("%i = %.3f\n",index,value);
      //MEditor->updateParameter(index,value);
    }
  }

};

//----------------------------------------------------------------------
#endif
