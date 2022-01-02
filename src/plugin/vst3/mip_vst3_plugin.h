#ifndef mip_vst3_plugin_included
#define mip_vst3_plugin_included
//----------------------------------------------------------------------

/*

  TODO:
  - get/setState
  - get/setEditorState

*/

#include "mip.h"
#include "base/types/mip_queue.h"
#include "plugin/mip_descriptor.h"
//#include "plugin/mip_instance_listener.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_process_context.h"
#include "plugin/vst3/mip_vst3.h"
#include "plugin/vst3/mip_vst3_utils.h"



#define MIP_PLUGIN_VST3_TIMER_MS 50

//typedef MIP_Queue<uint32_t,MIP_PLUGIN_MAX_PROCESS_EVENTS> MIP_ParamQueue;

//----------------------------------------------------------------------

//class MIP_Vst3Plugin {
class MIP_Vst3Plugin
: public IComponent
, public IAudioProcessor
, public IUnitInfo
, public IConnectionPoint
, public IMidiMapping
, public IKeyswitchController
//, public INoteExpressionController
, public IEditController
, public IEditController2
, public IPlugView
, public ITimerHandler {
//, public MIP_InstanceListener {

//------------------------------
private:
//------------------------------

  MIP_Plugin*         MPlugin             = nullptr;
  MIP_Descriptor*     MDescriptor         = nullptr;
  #ifndef MIP_NO_GUI
  MIP_Editor*         MEditor             = nullptr;
  #endif // MIP_NO_GUI

  uint32_t            MRefCount           = 1;
  IComponentHandler*  MComponentHandler   = nullptr;
  IComponentHandler2* MComponentHandler2  = nullptr;
  IPlugFrame*         MPlugFrame          = nullptr;
  IHostApplication*   MHostApp            = nullptr;
  ParameterInfo*      MParamInfos         = nullptr;
  IRunLoop*           MRunLoop            = nullptr;
  uint32_t            MIoMode             = 0;
  char                MHostName[129]      = {0};
  bool                MIsProcessing       = false;
  uint32_t            MProcessMode        = 0;
  uint32_t            MSampleSize         = 0;
  uint32_t            MBlockSize          = 0;
  float               MSampleRate         = 0.0;
  MIP_ProcessContext  MProcessContext     = {0};

//------------------------------
public:
//------------------------------

  MIP_Vst3Plugin(MIP_Plugin* APlugin) {
    MIP_PRINT;
    MPlugin = APlugin; // delete this!
    MDescriptor = MPlugin->getDescriptor();
    MRefCount   = 1;
    setupParameterInfo();
  }

  //----------

  virtual ~MIP_Vst3Plugin() {
    MIP_PRINT;
    if (MPlugin) delete MPlugin;
    if (MParamInfos) free(MParamInfos);
  }

//------------------------------
public:
//------------------------------

  //AEffect* getAEffect() {
  //  return &MAEffect;
  //}

//------------------------------
public: // editor listener
//------------------------------

//  #ifndef MIP_NO_GUI
//
//  /*
//    we run our gui in its own thread, but need to communicate with the
//    host on its gui thread.. so, when we tweak a parameter, we queue the
//    parameter (index), and send all updated ones to the host during onTimer
//    (see bottom)
//    the host should communicate the parameter change (audio thread) during
//    next process()
//  */
//
//  // AValue is 0..1 (widget-value)
//  // TODO: convert via parameter if necessary..
//
//  void on_editor_updateParameter(uint32_t AIndex, float AValue) override {
//    //MEditorParameterValues[AIndex] = AValue;
//    //MIP_Parameter* parameter = MDescriptor->parameters[AIndex];
//    //float value = parameter->from01(AValue);
//    float value = AValue;
//    MPlugin->setParameterValue(AIndex,value);
//    MVst2Host->updateParameter(AIndex,value);
//  }
//
//  //----------
//
//  void on_editor_resize(uint32_t AWidth, uint32_t AHeight) override {
//  }
//
//  #endif

//------------------------------
public:
//------------------------------

//  uint32_t getPluginFormat() override {
//    return MIP_PLUGIN_FORMAT_VST3;
//  }

  //----------

//  float getSamplerate() override {
//    return MSampleRate;
//  }

  //float getBlocksize() override {
  //  return MBlockSize;
  //}

  //----------

//  void notifyHostUpdateParameter(uint32_t AIndex, float AValue) override {
//    if (MComponentHandler) {
//      //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
//      MComponentHandler->beginEdit(AIndex);          // click
//      MComponentHandler->performEdit(AIndex,AValue);  // drag
//      MComponentHandler->endEdit(AIndex);            // release
//      //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
//    }
//  }

  //----------

//  void notifyHostResizeWindow(uint32_t AWidth, uint32_t AHeight) override {
//  }

  //----------

//  void notifyHostMidiOutput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {
//  }

//------------------------------
public: // editor listener
//------------------------------

  /*
    aka on_updateHostParameter
    we must call the MComponentHandler from the same thread as we received it,
    so we write to a queue, and read them all in updateParametersInTimer(),
    which is called via IRunLoop (same thread as setComponentHandler)
    see updateParametersInTimer below
  */

//  void updateParameterFromEditor(uint32_t AIndex, float AValue) override {
//    MHostValues[AIndex] = AValue;
//    MUpdatedHostParameters.write(AIndex);
//    MIP_BaseInstance::updateParameterFromEditor(AIndex,AValue);
//  }

  //----------

  //void resizeWindowFromEditor(uint32_t AWidth, uint32_t AHeight) override {
  //  MIP_BaseInstance::resizeWindowFromEditor(AWidth,AHeight) ;
  //}

//----------------------------------------
private:
//----------------------------------------

  /*
    struct ParameterInfo {
      ParamID id;                         // unique identifier of this parameter (named tag too)
      String128 title;                    // parameter title (e.g. "Volume")
      String128 shortTitle;               // parameter shortTitle (e.g. "Vol")
      String128 units;                    // parameter unit (e.g. "dB")
      int32 stepCount;                    // number of discrete steps (0: continuous, 1: toggle, discrete value otherwise
                                          // (corresponding to max - min, for example: 127 for a min = 0 and a max = 127) - see \ref vst3parameterIntro)
      ParamValue defaultNormalizedValue;  // default normalized value [0,1] (in case of discrete value: defaultNormalizedValue = defDiscreteValue / stepCount)
      UnitID unitId;                      // id of unit this parameter belongs to (see vst3UnitsIntro)
      int32 flags;                        // ParameterFlags (see below)
      enum ParameterFlags {
        kCanAutomate      = 1 << 0,   // parameter can be automated
        kIsReadOnly       = 1 << 1,   // parameter cannot be changed from outside (implies that kCanAutomate is false)
        kIsWrapAround     = 1 << 2,   // attempts to set the parameter value out of the limits will result in a wrap around [SDK 3.0.2]
        kIsList           = 1 << 3,   // parameter should be displayed as list in generic editor or automation editing [SDK 3.1.0]
        kIsProgramChange  = 1 << 15,  // parameter is a program change (unitId gives info about associated unit
                                      // - see \ref vst3UnitPrograms)
        kIsBypass         = 1 << 16   // special bypass parameter (only one allowed): Plug-in can handle bypass
                                      // (highly recommended to export a bypass parameter for effect Plug-in)
      };
    };
  */

  /*
    called from
      MIP_Vst3Instance constructor
  */

  void setupParameterInfo() {
    MIP_PRINT;
    if (!MParamInfos) {
      uint32_t num = MDescriptor->getNumParameters();
      MParamInfos = (ParameterInfo*)malloc( num * sizeof(ParameterInfo) );
      for (uint32_t i=0; i<num; i++) {
        MIP_Parameter* param = MDescriptor->getParameter(i);
        MParamInfos[i].id = i;
        VST3_CharToUtf16((char*)param->getName(),MParamInfos[i].title);
        VST3_CharToUtf16((char*)param->getShortName(),MParamInfos[i].shortTitle);
        VST3_CharToUtf16((char*)param->getLabel(),MParamInfos[i].units);
        if (param->getNumSteps() > 1) MParamInfos[i].stepCount = param->getNumSteps() - 1;
        else MParamInfos[i].stepCount = 0;
        MParamInfos[i].defaultNormalizedValue = param->getDefValue();
        MParamInfos[i].unitId = kRootUnitId; //-1;
        int32_t flags = 0;
        if (param->canAutomate()) flags += ParameterInfo::kCanAutomate;
        else flags += ParameterInfo::kIsReadOnly; // ??
        MParamInfos[i].flags = flags;
      }
    }
    MIP_PRINT;
  }

  //----------

  /*
    enum RestartFlags {
      kReloadComponent            // The Component should be reloaded
      kIoChanged                  // Input and/or Output Bus configuration has changed
      kParamValuesChanged         // Multiple parameter values have changed (as result of a program change for example)
      kLatencyChanged             // Latency has changed (IAudioProcessor.getLatencySamples)
      kParamTitlesChanged         // Parameter titles or default values or flags have changed
      kMidiCCAssignmentChanged    // MIDI Controller Assignments have changed
      kNoteExpressionChanged      // Note Expression has changed (info, count...)
      kIoTitlesChanged            // Input and/or Output bus titles have changed
      kPrefetchableSupportChanged // Prefetch support has changed (\see IPrefetchableSupport)
      kRoutingInfoChanged         // RoutingInfo has changed (\see IComponent)
    };
    MComponentHandler->restartComponent(int32 flags)
  */

  /*
    https://github.com/soundradix/JUCE/commit/2e9e66cbc8c65e889be5232ffae83c0ca78f9c7e
    performEdit ((Vst::ParamID) index, (double) newValue);
    // call setParamNormalized too, as without it value will reset at endEdit in Cubase.
    // setParamNormalized does not replace performEdit as it does not record automation.
    setParamNormalized ((Vst::ParamID) index, (double) newValue);

    https://sdk.steinberg.net/viewtopic.php?t=693
    "Remember that everything in the edit controller domain must happen on the
    main thread also calls to the IComponentHandler instance of the host. So
    don't call beginEdit, endEdit or performEdit on a secondary thread."

    NB: Cubase has problems if performEdit is called without setParamNormalized
    EditController::setParamNormalized(AIndex,AValue);

    MComponentHandler->performEdit(AIndex,AValue); // drag

  */

  /*
    called from
      MIP_Vst3Instance.onTimer
  */

//  void notifyHostAboutUpdatedParameters() {
//    uint32_t index;
//    while (MUpdatedHostParameters.read(&index)) {
//      float value = MHostValues[index];
//
//      MIP_Trace("%i %.2f\n",index,value);
//
//      if (MComponentHandler) {
//        //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
//        MComponentHandler->beginEdit(index);          // click
//        MComponentHandler->performEdit(index,value);  // drag
//        MComponentHandler->endEdit(index);            // release
//        //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
//      }
//    }
//  }

  //----------

  /*
    class IParameterChanges: public FUnknown {
      virtual int32             PLUGIN_API getParameterCount();
      virtual IParamValueQueue* PLUGIN_API getParameterData(int32 index);
      virtual IParamValueQueue* PLUGIN_API addParameterData(const Vst::ParamID& id, int32& index);
    };

    class IParamValueQueue {
      virtual ParamID PLUGIN_API getParameterId();
      virtual int32   PLUGIN_API getPointCount();
      virtual tresult PLUGIN_API getPoint (int32 index, int32& sampleOffset, ParamValue& value);
      virtual tresult PLUGIN_API addPoint (int32 sampleOffset, ParamValue value, int32& index);
    };


  */

  void handleParametersInProcess(ProcessData& data) {
    IParameterChanges* paramChanges = data.inputParameterChanges;
    if (paramChanges) {
      int32_t num_param_changes = paramChanges->getParameterCount();
      if (num_param_changes > 0) {
        for (int32_t i=0; i<num_param_changes; i++) {
          IParamValueQueue* paramQueue = paramChanges->getParameterData(i);
          if (paramQueue) {
            uint32_t id = paramQueue->getParameterId();
            if (id < MDescriptor->getNumParameters()) {
              //for (int32_t j=0; j<paramQueue->getPointCount(); j++) {
                int32_t offset = 0;
                double value = 0;
                int32_t pointcount = paramQueue->getPointCount();
                paramQueue->getPoint(pointcount-1,offset,value); // last point
                MIP_Parameter* param = MDescriptor->getParameter(id);
                if (param) value = param->from01(value);
                MPlugin->on_plugin_parameter(id,value);
              //}
            } // id < param
            else {
              // if MIP_PLUGIN_RECEIVE_MIDI
              // IMidiMapping
              int32_t offset = 0;
              double value = 0.5;
              int32_t pointcount = paramQueue->getPointCount();
              paramQueue->getPoint(pointcount-1,offset,value);      // last point
              uint32_t midi_ev = (id & 0xffff0000);
              uint32_t midi_ch = (id & 0x0000ffff);
              switch(midi_ev) {
                case MIP_VST3_PARAM_AFTERTOUCH: {
                  //if (offset != 0)
                  MPlugin->on_plugin_midi(MIP_MIDI_CHANNEL_AFTERTOUCH+midi_ch,value*127.0f,0);
                  break;
                } // at
                case MIP_VST3_PARAM_PITCHBEND: {
                  float v2 = value * 16383.0f;
                  uint32_t i2 = (uint32_t)v2;
                  //if (midi_ch != 0) {
                    uint32_t m2 = i2 & 0x7f;
                    uint32_t m3 = i2 >> 7;
                    //if (offset != 0)
                    MPlugin->on_plugin_midi(MIP_MIDI_PITCHBEND+midi_ch,m2,m3);
                  //}
                  break;
                } // pb
                case MIP_VST3_PARAM_BRIGHTNESS: {
                  //if (offset != 0)
                  MPlugin->on_plugin_midi(MIP_MIDI_CONTROL_CHANGE+midi_ch,74,value*127.0f);
                  break;
                }
              } // switch (midi_ev)
            } // id < num params
          } // paramqueue
        } // for all params
      } // numparams > 0
    } // paramchanges
  }

  //----------

  /*
    class IEventList: public FUnknown {
      virtual int32   PLUGIN_API getEventCount();
      virtual tresult PLUGIN_API getEvent(int32 index, Event& e);
      virtual tresult PLUGIN_API addEvent(Event& e);
    };
  */

  void handleEventsInProcess(ProcessData& data) {
    //if MIP_PLUGIN_RECEIVE_MIDI
    IEventList* inputEvents = data.inputEvents;
    if (inputEvents) {
      int32_t num = inputEvents->getEventCount();
      for (int32_t i=0; i<num; i++) {
        Event event;
        inputEvents->getEvent(i,event);
        //uint32_t offset  = 0;
        uint8_t  msg1    = 0;
        uint8_t  msg2    = 0;
        uint8_t  msg3    = 0;
        //uint32_t type    = 0;//kve_none;
        //uint32_t noteid  = 0;
        //float    value   = 0.0f;
        switch (event.type) {
          case Event::kNoteOnEvent:
            //offset  = event.sampleOffset;
            msg1    = MIP_MIDI_NOTE_ON + event.noteOn.channel;
            msg2    = event.noteOn.pitch;
            msg3    = event.noteOn.velocity * 127;
            //noteid  = event.noteOn.noteId;
            MPlugin->on_plugin_midi(msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case Event::kNoteOffEvent:
            //offset  = event.sampleOffset;
            msg1    = MIP_MIDI_NOTE_OFF + event.noteOff.channel;
            msg2    = event.noteOff.pitch;
            msg3    = event.noteOff.velocity * 127;
            //noteid  = event.noteOff.noteId;
            MPlugin->on_plugin_midi(msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case Event::kDataEvent:
            break;
          case Event::kPolyPressureEvent:
            //offset  = event.sampleOffset;
            msg1    = MIP_MIDI_POLY_AFTERTOUCH + event.polyPressure.channel;
            msg2    = event.polyPressure.pitch;
            msg3    = event.polyPressure.pressure * 127;
            //noteid  = event.polyPressure.noteId;
            MPlugin->on_plugin_midi(msg1,msg2,msg3);
            break;
          case Event::kNoteExpressionValueEvent:
            //offset = event.sampleOffset;
            //noteid = event.noteExpressionValue.noteId;
            //value  = event.noteExpressionValue.value;
            //switch(event.noteExpressionValue.typeId) {
            //  case kTuningTypeID:     type = kve_bend; break;
            //  case kBrightnessTypeID: type = kve_slide; break;
            //  //case kVolumeTypeID:     type = kve_none;  break;
            //  //case kPanTypeID:        type = kve_none;  break;
            //  //case kVibratoTypeID:    type = kve_none;  break;
            //  //case kExpressionTypeID: type = kve_none;  break;
            //}polyPressure
            //on_noteExpression(offset,type,noteid,value);
            break;
          case Event::kNoteExpressionTextEvent:
            break;
          case Event::kChordEvent:
            break;
          case Event::kScaleEvent:
            break;
          default:
            break;
        } // switch
      } // for all events
    } // if input events
  }

//----------------------------------------
public:
//----------------------------------------

  //--------------------
  // FUnknown
  //--------------------

  /*
    Adds a reference and return the new reference count.
    Remarks: The initial reference count after creating an object is 1.
  */

  uint32 PLUGIN_API addRef() override {
    MIP_PRINT;
    MRefCount++;
    return MRefCount;
  }

  //----------

  /*
    Releases a reference and return the new reference count.
    If the reference count reaches zero, the object will be destroyed in
    memory.
  */

  uint32 PLUGIN_API release() override {
    MIP_PRINT;
    const uint32 r = --MRefCount; // const uint32 ?
    if (r == 0) {
      delete this;
    };
    return r;
  }

  //----------

  /*
    Query for a pointer to the specified interface.
    Returns kResultOk on success or kNoInterface if the object does not
    implement the interface.
    The object has to call addRef when returning an interface.
    _iid: (in)  16 Byte interface identifier (-> FUID)
    obj:  (out) On return, *obj points to the requested interface

    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    Although it is not recommended, it is possible to implement both, the
    processing part and the controller part in one component class. The host
    tries to query the IEditController interface after creating an
    IAudioProcessor and on success uses it as controller.

    reaper asks for:
      C3B17BC0-2C174494-80293402-FBC4BBF8 (IContextInfoHandler,   plsextensions)
      31E29A7A-E55043AD-8B95B9B8-DA1FBE1E (IContextInfoHandler2,  pslextensions)

    bitwig asks for:
      6C389611-D391455D-B870B833-94A0EFDD (IUnitData)
      8683B01F-7B354F70-A2651DEC-353AF4FF (IProgramListData)
      7F4EFE59-F3204967-AC27A3AE-AFB63038 (IEditController2)
      65ED9690-8AC44525-8AADEF7A-72EA703F (IPlugViewContentScaleSupport)

    ( 65ed9690-8ac44525-8aadef7a-72ea703f (IPlugInViewScaling, pslextensions) !! )

      633b9012-d19a4953-b477b436-9daaeedb
      8e8bbb11-73384477-ac6b1eeb-383df4f0
      ccc87233-accb4371-bc74a1f2-5ebba43c

      7743fe5e-f3224d6d-a729ada9-aab2333b
      cd291308-775e4492-b5db37be-62bb1a54
      41c5087e-4bbb4e8b-8da21bd2-97dc6679
      001a4b89-86924ed8-b7aacdee-cc12d6dd
      98e97900-c220484c-82a3edcd-d06cdd45

      6bec9099-83c14927-83a8ee72-79e97733

  */

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    MIP_PRINT;
    *obj = nullptr;
    if ( FUnknownPrivate::iidEqual(IAudioProcessor_iid,_iid) ) {
      *obj = (IAudioProcessor*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(IEditController_iid,_iid) ) {
      *obj = (IEditController*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(IMidiMapping_iid,_iid) ) {
      *obj = (IMidiMapping*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(IUnitInfo_iid,_iid) ) {
      *obj = (IUnitInfo*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(INoteExpressionController_iid,_iid) ) {
      //*obj = (INoteExpressionController*)this;
      //addRef();
      //return kResultOk;
      return kNoInterface;
    }
    if ( FUnknownPrivate::iidEqual(IKeyswitchController_iid,_iid) ) {
      //*obj = (IKeyswitchController*)this;
      //addRef();
      //return kResultOk;
      return kNoInterface;
    }
    if ( FUnknownPrivate::iidEqual(IConnectionPoint_iid,_iid) ) {
      *obj = (IConnectionPoint*)this;
      addRef();
      return kResultOk;
    }
    //if ( FUnknownPrivate::iidEqual(ITimerHandler_iid,_iid) ) {
    //  *obj = (ITimerHandler*)this;
    //  addRef();
    //  return kResultOk;
    //}
    return kNoInterface;
  }

  //--------------------
  // IPluginBase
  //--------------------

  /*
    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    The context parameter passed to IPluginBase::initialize is
    Vst::IHostApplication. Hosts should not call others functions before
    initialize is called!, except IComponent::setIoMode which need to be
    called before or IComponent::getControllerClassId which could be called
    before.

    note: Extensive memory allocations etc. should be performed in this
    method rather than in the class' constructor!
    If the method does NOT return kResultOk, the object is released
    immediately. In this case terminate is not called!
  */

  /*
    class IHostApplication: public FUnknown {
      // Gets host application name.
      virtual tresult PLUGIN_API getName (String128 name) = 0;
      // Creates host object (e.g. Vst::IMessage).
      virtual tresult PLUGIN_API createInstance (TUID cid, TUID _iid, void** obj) = 0;
  */

  /*
    3.6.12
    Allow a Plug-in to ask the host if a given Plug-in interface is
    supported/used by the host. It is implemented by the hostContext given
    when the component is initialized.
    tresult PLUGIN_API MyPluginController::initialize (FUnknown* context) {
      // ...
      FUnknownPtr<IPlugInterfaceSupport> plugInterfaceSupport (context);
      if (plugInterfaceSupport) {
        if (plugInterfaceSupport->isPlugInterfaceSupported (IMidiMapping::iid) == kResultTrue)
          // IMidiMapping is used by the host
      }
      // ...
    }
  */

  /*
    IHostApplication: passed as 'context' in to IPluginBase::initialize()
  */

  tresult PLUGIN_API initialize(FUnknown* context) override {
    MIP_PRINT;
    MHostApp = (IHostApplication*)context;
    //context->queryInterface(IHostApplication_iid, (void**)&MHostApp);
    if (MHostApp) {
      String128 u;
      MHostApp->getName(u);
      VST3_Utf16ToChar(u,MHostName);
    }
    else {
    }
    MPlugin->on_plugin_init();
    return kResultOk;
  }

  //----------

  /*
    This function is called before the Plug-in is unloaded and can be used
    for cleanups. You have to release all references to any host application
    interfaces.
  */

  tresult PLUGIN_API terminate() override {
    MIP_PRINT;
    MPlugin->on_plugin_deinit();
    return kResultOk;
  }

  //--------------------
  // IComponent
  //--------------------

  /*
    Called before initializing the component to get information about the
    controller class.

    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    In order to enable the host to create a corresponding edit controller the
    processing component has to provide the matching class-ID. The host uses
    the module's class factory to create the controller component.
    (IComponent::getControllerClassId)
  */

  tresult PLUGIN_API getControllerClassId(TUID classId) override {
    MIP_PRINT;
    if (MDescriptor->hasEditor()) {
      memcpy(classId,MDescriptor->getLongEditorId(),16);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  /*
    Called before 'initialize' to set the component usage (optional).
    See IoModes
  */

  tresult PLUGIN_API setIoMode(IoMode mode) override {
    MIP_PRINT;
    //switch (mode) {
    //  case kSimple:             VST3_Trace("(kSimple)\n"); break;
    //  case kAdvanced:           VST3_Trace("(kAdvanced)\n"); break;
    //  case kOfflineProcessing:  VST3_Trace("(kOfflineProcessing)\n"); break;
    //  default:                  VST3_Trace("(unknown)\n"); break;
    //}
    MIoMode = mode;
    return kResultOk;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections

    enum MediaTypes {
      kAudio = 0,   // audio
      kEvent,       // events
      kNumMediaTypes
    };
    enum BusDirections {
      kInput = 0,   //input bus
      kOutput       // output bus
    };
  */

  int32 PLUGIN_API getBusCount(MediaType type, BusDirection dir) override {
    MIP_PRINT;
    if (type == kAudio) {
      if (dir == kInput) return MDescriptor->getNumAudioInputs();
      else if (dir == kOutput) return MDescriptor->getNumAudioOutputs();
    }
    if (type == kEvent) {
      //if (dir==kOutput) return 1;
      //else
      if (MDescriptor->canReceiveMidi()) {
        if (dir == kInput) return 1;
      }
    }
    return 0;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections

    struct BusInfo {
      MediaType mediaType;    // Media type - has to be a value of \ref MediaTypes
      BusDirection direction; // input or output \ref BusDirections
      int32 channelCount;     // number of channels (if used then need to be recheck after \ref IAudioProcessor::setBusArrangements is called)
      String128 name;         // name of the bus
      BusType busType;        // main or aux - has to be a value of \ref BusTypes
      uint32 flags;           // flags - a combination of \ref BusFlags
      enum BusFlags {
        kDefaultActive = 1 << 0	///< bus active per default
      };
    };
  */

  tresult PLUGIN_API getBusInfo(MediaType type, BusDirection dir, int32 index, BusInfo& bus) override {
    MIP_PRINT;
    if (type == kAudio) {
      bus.mediaType = kAudio;
      if (dir == kInput) {
        MIP_AudioPort* port = MDescriptor->getAudioInput(index);
        bus.direction = kInput;
        bus.channelCount = port->num_channels;
        VST3_CharToUtf16(port->name,bus.name);
      }
      else if (dir == kOutput) {
        MIP_AudioPort* port = MDescriptor->getAudioOutput(index);
        bus.direction = kOutput;
        bus.channelCount = port->num_channels;
        VST3_CharToUtf16(port->name,bus.name);
      }
      bus.flags = 0;//kDefaultActive;
      return kResultOk;
    }
    else if (type == kEvent) {
      bus.mediaType = kEvent;
      if (dir == kInput) {
        bus.direction = kInput;
        bus.channelCount = 1; // 16?
        VST3_CharToUtf16("Midi In",bus.name);
      }
      bus.flags = 0;//kDefaultActive;
      return kResultOk;
    }

    return kResultFalse;
  }

  //----------

  /*
    Retrieves routing information (to be implemented when more than one
    regular input or output bus exists).
    The inInfo always refers to an input bus while the returned outInfo must
    refer to an output bus!

    struct RoutingInfo
    {
      MediaType mediaType;  // media type see \ref MediaTypes
      int32 busIndex;       // bus index
      int32 channel;        // channel (-1 for all channels)
    };
  */

  tresult PLUGIN_API getRoutingInfo(RoutingInfo& inInfo, RoutingInfo& outInfo) override {
    MIP_PRINT;
    outInfo.mediaType = inInfo.mediaType; // MediaTypes::kAudio;
    outInfo.busIndex  = inInfo.busIndex;  // 0;
    outInfo.channel   = -1;
    return kResultOk;
  }

  //----------

  /*
    Called upon (de-)activating a bus in the host application. The Plug-in
    should only processed an activated bus, the host could provide less
    AudioBusBuffers in the process call (see IAudioProcessor::process) if
    last buses are not activated
  */

  tresult PLUGIN_API activateBus(MediaType type, BusDirection dir, int32 index, TBool state) override {
    MIP_PRINT;
    return kResultOk;
  }

  //----------

  /*
    Activates / deactivates the component.
  */

  tresult PLUGIN_API setActive(TBool state) override {
    MIP_PRINT;
    if (state) MPlugin->on_plugin_activate(MSampleRate,0,MBlockSize);
    else MPlugin->on_plugin_deactivate();
    return kResultOk;
  }

  //----------

  /*

    How does persistence work?

    An instantiated Plug-in often has state information that must be saved in
    order to properly re-instantiate that Plug-in at a later time. A VST 3
    Plug-in has two states which are saved and reloaded: its component state
    and its controller state.
    The sequence of actions for saving is:
      component->getState (compState)
      controller->getState (ctrlState)
    The sequence of actions for loading is:
      component->setState (compState)
      controller->setComponentState (compState)
      controller->setState (ctrlState)
    In the latter sequence you can see that the controller part will receive
    the component state. This allows the 2 parts to synchronize their states.

    What's the difference between IEditController::setComponentState and
    IEditController::setState?

    After a preset is loaded, the host calls IEditController::setComponentState
    and IComponent::setState. Both delivering the same information.
    IEditController::setState is called by the host so that the Plug-in is able
    to update its controller dependent parameters, eg. position of scroll bars.
    Prior to this, there should have been a call by the host to
    IEditController::getState, where the Plug-in writes these various
    parameters into the stream.

    Can IComponent::getState()/setState() could be called during processing?

    Yes, setState and getState are called normally from the UI Thread when the
    Plug-in is used in a realtime context, in an offline context set/getState
    could be called in the same thread than the process call. Check the
    workflow diagram Audio Processor Call Sequence for more info about in which
    state which interfaces are called.
  */

  //----------

  /*
    Restore: When the states are restored, the host passes the processor state
    to both the processor and the controller (IEditController::setComponentState).
    A host must always pass that state to the processor first. The controller
    then has to synchronize its parameters to this state (but must not perform
    any IComponentHandler callbacks).
    After restoring a state, the host will rescan the parameters (asking the
    controller) in order to update its intern representation.
  */

  /*
    IBStream->read(
      void*   buffer,             // destination buffer
      int32   numBytes,           // amount of bytes to be read
      int32*  numBytesRead=0)     // how many bytes have been read from stream (set to 0 if this is of no interest)
    )

    IBStream->write(
      void*   buffer,             // destination buffer
      int32   numBytes,           // amount of bytes to be read
      int32*  numBytesWritten=0)  // how many bytes have been written to stream (set to 0 if this is of no interest)
    )

  */

  /*
    Sets complete state of component.
    //state->read(MParamValues,MNumParameters*sizeof(float));
  */

  tresult PLUGIN_API setState(IBStream* state) override {
    MIP_PRINT;
    uint32_t version = 0;
    uint32_t mode = 0;
    int32_t size = 0;
    uint32_t num_params = 0;
    void*    ptr = nullptr;
    int32_t num_read = 0;
    state->read(&version,sizeof(uint32_t),&num_read); //  MIP_Assert( num_read == sizeof(uint32_t) );
    state->read(&mode,sizeof(uint32_t),&num_read);    //  MIP_Assert( num_read == sizeof(uint32_t) );
    state->read(&size,sizeof(int32_t),&num_read);     //  MIP_Assert( num_read == sizeof(int32_t) );
    switch (mode) {
      case 0:
        // is it safe to malloc/free here?
        ptr = malloc(size);
        state->read(&ptr,size,&num_read);
//        MPlugin->on_plugin_restoreState(size,ptr,0);
        free(ptr);
        break;
      case 1:
        num_params = MDescriptor->getNumParameters();
        MIP_Assert( (uint32_t)size == (num_params * sizeof(float)) );
        for (uint32_t i=0; i<num_params; i++) {
          float v = 0.f;
          state->read(&v,sizeof(float),&num_read);
//          MPlugin->setParamValue(i,v);
        }
//        MPlugin->updateAllParameters();
        break;
    }
    return kResultOk;
  }

  //----------

  /*
    Retrieves complete state of component.
    //state->write(MParamValues,MNumParameters*sizeof(float));

    mode
    0 = user
    1 = simple (all params)
  */

  tresult PLUGIN_API getState(IBStream* state) override {
    MIP_PRINT;
//    uint32_t  version = MDescriptor->getVersion();
//    uint32_t  mode    = 0;
//    void*     ptr     = nullptr;
//    uint32_t  size    = MPlugin->on_plugin_saveState(&ptr,0);
//    if ((size == 0) && (ptr == nullptr)) {
//      ptr = MPlugin->getParamValues();
//      size = MDescriptor->getNumParameters() * sizeof(float);
//      mode = 1;
//    }
//    int num_written  = 0;
//    state->write(&version,sizeof(uint32_t),&num_written); //  MIP_Assert( num_written == sizeof(uint32_t) );
//    state->write(&mode,sizeof(uint32_t),&num_written);    //  MIP_Assert( num_written == sizeof(uint32_t) );
//    state->write(&size,sizeof(uint32_t),&num_written);    //  MIP_Assert( num_written == sizeof(uint32_t) );
//    state->write(ptr,size,&num_written);                  //  MIP_Assert( num_written == size );
    return kResultOk;
  }

  //--------------------
  // IAudioProcessor
  //--------------------

  /*
    Try to set (from host) a predefined arrangement for inputs and outputs.
    The host should always deliver the same number of input and output buses
    than the Plug-in needs (see IComponent::getBusCount).
    The Plug-in returns kResultFalse if wanted arrangements are not supported.
    If the Plug-in accepts these arrangements, it should modify its buses to
    match the new arrangements
    (asked by the host with IComponent::getInfo() or
    IAudioProcessor::getBusArrangement ()) and then return kResultTrue.
    If the Plug-in does not accept these arrangements, but can adapt its
    current arrangements (according to the wanted ones), it should modify its
    buses arrangements and return kResultFalse.
  */

  const SpeakerArrangement kStereo = kSpeakerL | kSpeakerR;

  tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) override {
    MIP_PRINT;
    //if ((numIns == 2) && (*inputs == kStereo)) {}
    if ((numIns == 2) && (numOuts == 2)) return kResultTrue;
    return kResultFalse;
  }

  //----------

  /*
    Gets the bus arrangement for a given direction (input/output) and index.
    Note: IComponent::getInfo() and IAudioProcessor::getBusArrangement()
    should be always return the same information about the buses arrangements.
  */

  tresult PLUGIN_API getBusArrangement(BusDirection dir, int32 index, SpeakerArrangement& arr) override {
    MIP_PRINT;
    //if ((dir==kOutput) && (index==0)) {
    //  arr = Steinberg::Vst::kSpeakerL | Steinberg::Vst::kSpeakerR;
    //  return kResultOk;
    //}
    if (dir == kInput) {
      MIP_AudioPort* port = MDescriptor->getAudioInput(index);
      if (port) {
        arr = Steinberg::Vst::kSpeakerL | Steinberg::Vst::kSpeakerR;
        return kResultOk;
      }
    }
    else if (dir == kOutput) {
      MIP_AudioPort* port = MDescriptor->getAudioInput(index);
      if (port) {
        arr = Steinberg::Vst::kSpeakerL | Steinberg::Vst::kSpeakerR;
        return kResultOk;
      }
    }
    return kResultFalse;
  }

  //----------

  /*
    Asks if a given sample size is supported.
    see SymbolicSampleSizes.

    enum SymbolicSampleSizes {
      kSample32,    // 32-bit precision
      kSample64     // 64-bit precision
    };
  */

  tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) override {
    //MIP_PRINT;
    if (symbolicSampleSize==kSample32) {
      return kResultTrue;
    }
    return kResultFalse;// kResultFalse;// kNotImplemented;
  }

  //----------

  /*
    Gets the current Latency in samples.
    The returned value defines the group delay or the latency of the Plug-in.
    For example, if the Plug-in internally needs to look in advance (like
    compressors) 512 samples then this Plug-in should report 512 as latency.
    If during the use of the Plug-in this latency change, the Plug-in has to
    inform the host by using IComponentHandler::restartComponent(kLatencyChanged),
    this could lead to audio playback interruption because the host has to
    recompute its internal mixer delay compensation.
    Note that for player live recording this latency should be zero or small.
  */

  uint32 PLUGIN_API getLatencySamples() override {
    //MIP_PRINT;
    return 0;
  }

  //----------

  /*
    Called in disable state (not active) before processing will begin.
  */

  /*
    enum ProcessModes {
      kRealtime,  // realtime processing
      kPrefetch,  // prefetch processing
      kOffline    // offline processing
    };
    enum SymbolicSampleSizes {
      kSample32,  // 32-bit precision
      kSample64   // 64-bit precision
    };
    struct ProcessSetup {
      int32 processMode;        // ProcessModes
      int32 symbolicSampleSize; // \ref SymbolicSampleSizes
      int32 maxSamplesPerBlock; // maximum number of samples per audio block
      SampleRate sampleRate;    // sample rate (double)
    };
  */

  tresult PLUGIN_API setupProcessing(ProcessSetup& setup) override {
    MIP_PRINT;
    MProcessMode  = setup.sampleRate;         // kRealtime, kPrefetch, kOffline
    MSampleSize   = setup.symbolicSampleSize; // kSample32, kSample64
    MBlockSize    = setup.maxSamplesPerBlock;
    MSampleRate   = setup.sampleRate;
//    MPlugin->on_sampleRate(MSampleRate);
    return kResultOk;
  }

  //----------

  /*
    Informs the Plug-in about the processing state. This will be called
    before any process calls start with true and after with false.
    Note that setProcessing (false) may be called after setProcessing (true)
    without any process calls.
    In this call the Plug-in should do only light operation (no memory
    allocation or big setup reconfiguration), this could be used to reset
    some buffers (like Delay line or Reverb).
  */

  tresult PLUGIN_API setProcessing(TBool state) override {
    MIP_PRINT;
    MIsProcessing = state;
    return kResultOk;
  }

  //----------

  /*
    For any audio bus defined by the Plug-in the host must provide buffer data
    - even for inactive buses. Buses are addressed by index, so leaving out
    inactive buses will mix-up these indices. The actual data buffer can be
    null though. (see AudioBusBuffers).
    Note that channelBuffers32 (or channelBuffers64) buffers pointers could be
    the same or different for input and output: this has to be take into
    account in the process function (for example not reseting the output before
    processing if input and output buffers are the same!). It could be the same
    for multiple inputs or multiple outputs (case of instrument Plug-ins) all
    outputs (or inputs) could share the same buffer!
    Important: the host could call IAudioProcessor::process without buffers
    (numInputs and numOutputs of AudioBusBuffers are zeroed, numSamples too),
    in order to flush parameters (from host to Plug-in). Parameters flush could
    happen only when the host needs to send parameter changes and no processing
    is called.
  */

  /*
    The Process call, where all information (parameter changes, event,
    audio buffer) are passed.

    struct AudioBusBuffers {
      int32   numChannels;            // number of audio channels in bus
      uint64  silenceFlags;           // Bitset of silence state per channel
      union {
        Sample32**  channelBuffers32; // sample buffers to process with 32-bit precision
        Sample64**  channelBuffers64; // sample buffers to process with 64-bit precision
      };
    };

    class IParamValueQueue {
      virtual ParamID PLUGIN_API getParameterId();
      virtual int32   PLUGIN_API getPointCount();
      virtual tresult PLUGIN_API getPoint (int32 index, int32& sampleOffset, ParamValue& value);
      virtual tresult PLUGIN_API addPoint (int32 sampleOffset, ParamValue value, int32& index);
    };

    class IParameterChanges: public FUnknown {
      virtual int32             PLUGIN_API getParameterCount();
      virtual IParamValueQueue* PLUGIN_API getParameterData(int32 index);
      virtual IParamValueQueue* PLUGIN_API addParameterData(const Vst::ParamID& id, int32& index);
    };

    class IEventList: public FUnknown {
      virtual int32   PLUGIN_API getEventCount();
      virtual tresult PLUGIN_API getEvent(int32 index, Event& e);
      virtual tresult PLUGIN_API addEvent(Event& e);
    };

    struct ProcessContext {
      // Transport state & other flags
      enum StatesAndFlags {
        kPlaying                = 1 << 1,   // currently playing
        kCycleActive            = 1 << 2,   // cycle is active
        kRecording              = 1 << 3,   // currently recording
        kSystemTimeValid        = 1 << 8,   // systemTime contains valid information
        kContTimeValid          = 1 << 17,  // continousTimeSamples contains valid information
        kProjectTimeMusicValid  = 1 << 9,   // projectTimeMusic contains valid information
        kBarPositionValid       = 1 << 11,  // barPositionMusic contains valid information
        kCycleValid             = 1 << 12,  // cycleStartMusic and barPositionMusic contain valid information
        kTempoValid             = 1 << 10,  // tempo contains valid information
        kTimeSigValid           = 1 << 13,  // timeSigNumerator and timeSigDenominator contain valid information
        kChordValid             = 1 << 18,  // chord contains valid information
        kSmpteValid             = 1 << 14,  // smpteOffset and frameRate contain valid information
        kClockValid             = 1 << 15   // samplesToNextClock valid
      };
      uint32        state;                // a combination of the values from \ref StatesAndFlags
      double        sampleRate;           // current sample rate (always valid)
      TSamples      projectTimeSamples;   // project time in samples (always valid)
      int64         systemTime;           // system time in nanoseconds (optional)
      TSamples      continousTimeSamples; // project time, without loop (optional)
      TQuarterNotes projectTimeMusic;     // musical position in quarter notes (1.0 equals 1 quarter note)
      TQuarterNotes barPositionMusic;     // last bar start position, in quarter notes
      TQuarterNotes cycleStartMusic;      // cycle start in quarter notes
      TQuarterNotes cycleEndMusic;        // cycle end in quarter notes
      double        tempo;                // tempo in BPM (Beats Per Minute)
      int32         timeSigNumerator;     // time signature numerator (e.g. 3 for 3/4)
      int32         timeSigDenominator;   // time signature denominator (e.g. 4 for 3/4)
      Chord         chord;                // musical info
      int32         smpteOffsetSubframes; // SMPTE (sync) offset in subframes (1/80 of frame)
      FrameRate     frameRate;            // frame rate
      int32         samplesToNextClock;   // MIDI Clock Resolution (24 Per Quarter Note), can be negative (nearest)
    };

    struct ProcessData {
      int32               processMode;            // processing mode - value of \ref ProcessModes
      int32               symbolicSampleSize;     // sample size - value of \ref SymbolicSampleSizes
      int32               numSamples;             // number of samples to process
      int32               numInputs;              // number of audio input buses
      int32               numOutputs;             // number of audio output buses
      AudioBusBuffers*    inputs;                 // buffers of input buses
      AudioBusBuffers*    outputs;                // buffers of output buses
      IParameterChanges*  inputParameterChanges;	// incoming parameter changes for this block
      IParameterChanges*  outputParameterChanges; // outgoing parameter changes for this block (optional)
      IEventList*         inputEvents;            // incoming events for this block (optional)
      IEventList*         outputEvents;           // outgoing events for this block (optional)
      ProcessContext*     processContext;         // processing context (optional, but most welcome)
    };

  */

  /*
    VST3_SDK/doc/vstsdk/faq.html#faqCommunication6
    Q: How my parameter changes (from UI interaction) are send to the
    processor if the host does not process?
    When a parameter is changed in the Plug-in UI by user action, the plug
    sends this change to the host with performEdit (do not forget to call
    beginEdit and endEdit), then the host has the responsibility to transfer
    this parameter change to the processor part: => if the audio engine is
    running (playing) this will be done in the next available process call =>
    if the audio engine is not running, the host has to flush parameter
    changes from time to time by sending them to processor by calling process
    (with audio buffer set to null), in this case the Plug-in should only
    update the parameters changes without processing any audio. This is very
    important that the host supports this flush mechanism else by saving
    Plug-ins state (project/preset) the host will not get the correct updated
    one.

    VST3_SDK/doc/vstinterfaces/index.html#vst3Editing
    Any change to a parameter that is caused by user interaction in the
    Plug-in GUI must be properly reported to the Steinberg::Vst::
    IComponentHandler. The host is responsible for transmitting the change to
    the processor. In order to make recording of automation work accordingly
    it is necessary to call beginEdit, performEdit and endEdit in the
    expected order!
    When the controller transmits a parameter change to the host, the host
    synchronizes the processor by passing the new values as Steinberg::Vst::
    IParameterChanges to the process call.
  */

  /*
    http://www.noelborthwick.com/2014/01/22/developer-notes-sonar-x3-vst3-internals/
    The host may also send parameters to the plugin when no processing is
    active via a special “flush parameters” process call. This happens when
    the audio engine has been stopped in SONAR and the user manipulates an
    envelope or the user interface widget to change a plugin parameter.
    In this call numInputs and numOutputs of Steinberg::Vst::AudioBusBuffers
    are zeroed as well as numSamples. Plugin’s should expect and properly
    handle this call.
  */

  // assume only 1 input/output bus..

  tresult PLUGIN_API process(ProcessData& data) override {

    handleEventsInProcess(data);
    handleParametersInProcess(data);
    bool _flush = ( (data.numInputs == 0) && (data.numOutputs == 0) && (data.numSamples == 0) );

    if (!_flush) {

      MProcessContext.num_inputs = MDescriptor->getNumAudioInputs();
      MProcessContext.num_outputs = MDescriptor->getNumAudioOutputs();

      //for (uint32_t i=0; i<MProcessContext.num_inputs; i++)  { MProcessContext.inputs[i]  = data.inputs[0].channelBuffers32[i];  }
      //for (uint32_t i=0; i<MProcessContext.num_outputs; i++) { MProcessContext.outputs[i] = data.outputs[0].channelBuffers32[i]; }

      MProcessContext.inputs      = data.inputs[0].channelBuffers32;
      MProcessContext.outputs     = data.outputs[0].channelBuffers32;
      MProcessContext.num_samples     = data.numSamples;
      MProcessContext.samplerate    = data.processContext->sampleRate;
      MProcessContext.tempo         = data.processContext->tempo;
      MPlugin->on_plugin_process(&MProcessContext);

    }

    /*
      https://forum.juce.com/t/vst3-plugin-wrapper/12323/5
      I recall the Steinberg Validator complaining that process() should just
      return kResultTrue...
    */
    return kResultOk; // = kResulttrue (0)
  }

  //----------

  /*
    Gets tail size in samples. For example, if the Plug-in is a Reverb
    Plug-in and it knows that the maximum length of the Reverb is 2sec, then
    it has to return in getTailSamples()
    (in VST2 it was getGetTailSize()) : 2*sampleRate.
    This information could be used by host for offline processing, process
    optimization and downmix (avoiding signal cut (clicks)).
    It should return:
    - kNoTail when no tail
    - x * sampleRate when x Sec tail.
    - kInfiniteTail when infinite tail.
  */

  uint32 PLUGIN_API getTailSamples() override {
    //MIP_PRINT;
    return kNoTail;
  }

  //--------------------
  // IMidiMapping
  //--------------------

  /*
    busIndex              index of Input Event Bus
    channel               channel of the bus
    midiControllerNumber  see \ref ControllerNumbers for expected values (could be bigger than 127)
    id                    return the associated ParamID to the given midiControllerNumber

    see ivstmidicontrollers.h

    kCtrlModWheel
    kCtrlFilterResonance (cc74) // Filter Resonance (Brightness)
    kAfterTouch
    kPitchBend

  */

  /*
    todo:
    ParamId, bitmask..
    0x0000.0x0000 'normal' parameter
    0x0001.0x0000 midi
    etc..
  */

  tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id) override {
    //MIP_PRINT;
    //if (busIndex == 0) {
      switch (midiControllerNumber) {
        case kAfterTouch: // 128
          id = MIP_VST3_PARAM_AFTERTOUCH + channel;
          return kResultOk;
        case kPitchBend: // 129
          id = MIP_VST3_PARAM_PITCHBEND + channel;
          return kResultOk;
        case kCtrlFilterResonance: // cc 74 (slide)
          id = MIP_VST3_PARAM_BRIGHTNESS + channel;
          return kResultOk;
      }
    //}
    return kResultFalse;
  }

  //--------------------
  // INoteExpressionController
  //--------------------

      // Returns number of supported note change types for event bus index and channel.

  /*
  int32 PLUGIN_API getNoteExpressionCount(int32 busIndex, int16 channel) override {
    VST3_Trace("vst3: instance/getNoteExpressionCount busIndex:%i channel:%i\n",busIndex,channel);
    //if (busIndex==0) return 1;
    return 2;
  }

  //----------

  //flags: NoteExpressionTypeInfo::kIsBipolar, kIsOneShot, kIsAbsolute, kAssociatedParameterIDValid

  tresult PLUGIN_API getNoteExpressionInfo(int32 busIndex, int16 channel, int32 noteExpressionIndex, NoteExpressionTypeInfo& info) override {
    VST3_Trace("vst3: instance/getNoteExpressionInfo busIndex:%i channel:%i noteExpressionIndex:%i\n",busIndex,channel,noteExpressionIndex);
    //if (busIndex==0) {
      switch(noteExpressionIndex) {
        case 0:
          info.typeId                 = 0; // ??
          char_to_utf16("Tuning",info.title);
          char_to_utf16("Tun",info.shortTitle);
          char_to_utf16("",info.units);
          info.unitId                 = 0;
          info.valueDesc.defaultValue = 0.0;
          info.valueDesc.minimum      = 0;
          info.valueDesc.maximum      = 1;
          info.associatedParameterId  = -1;
          info.flags                  = NoteExpressionTypeInfo::kIsBipolar;
          return kResultOk;
        case 1:
          info.typeId                 = 1; // ??
          char_to_utf16("Brightness",info.title);
          char_to_utf16("Bri",info.shortTitle);
          char_to_utf16("",info.units);
          info.unitId                 = 0;
          info.valueDesc.defaultValue = 0.0;
          info.valueDesc.minimum      = 0;
          info.valueDesc.maximum      = 1;
          info.associatedParameterId  = -1;
          info.flags                  = 0;
          return kResultOk;
      }
    //}
    return kResultFalse;
  }

  //----------

  // Gets a user readable representation of the normalized note change value.

  tresult PLUGIN_API getNoteExpressionStringByValue(int32 busIndex, int16 channel, NoteExpressionTypeID id, NoteExpressionValue valueNormalized, String128 string) override {
    VST3_Trace("vst3: instance/getNoteExpressionStringByValue busIndex:%i channel:%i id:%i valueNormalized:%.3f\n",busIndex,channel,id,valueNormalized);
    char temp[100];
    MIP_FloatToString(temp,valueNormalized);
    char_to_utf16(temp,string);
    return kResultOk;
  }

  //----------

  // Converts the user readable representation to the normalized note change value.

  tresult PLUGIN_API getNoteExpressionValueByString(int32 busIndex, int16 channel, NoteExpressionTypeID id, const TChar* string, NoteExpressionValue& valueNormalized) override{
    VST3_Trace("vst3: instance/getNoteExpressionValueByString busIndex:%i  channel:%i id:%i string:%s\n",busIndex,channel,id,string);
    char temp[129];
    utf16_to_char(string,temp);
    float res = const char*ToFloat(temp);
    valueNormalized = res;
    return kResultOk;
  }

  */

  //--------------------
  // IKeyswitchController
  //--------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/keyswitch.html

  /*
    When a (instrument) Plugin supports such interface, the host could get
    from the Plug-in the current set of used key switches (megatrig/
    articulation) for a given channel of a event bus and then automatically use
    them (like in Cubase 6) to create VST Expression Map (allowing to
    associated symbol to a given articulation / key switch).
  */

  //----------

  // Returns number of supported key switches for event bus index and channel.

  int32 PLUGIN_API getKeyswitchCount(int32 busIndex, int16 channel) override {
    MIP_PRINT;
    return 0;
  }

  //----------

  /*
  struct KeyswitchInfo {
    KeyswitchTypeID typeId; // see KeyswitchTypeID
    String128 title;        // name of key switch (e.g. "Accentuation")
    String128 shortTitle;   // short title (e.g. "Acc")
    int32 keyswitchMin;     // associated main key switch min (value between [0, 127])
    int32 keyswitchMax;     // associated main key switch max (value between [0, 127])
    int32 keyRemapped;      // optional remapped key switch (default -1), the Plug-in could provide one remapped key for a key switch (allowing better location on the keyboard of the key switches)
    int32 unitId;           // id of unit this key switch belongs to (see \ref vst3UnitsIntro), -1 means no unit used.
    int32 flags;            // not yet used (set to 0)
  };
  */

  // Returns key switch info.

  tresult PLUGIN_API getKeyswitchInfo(int32 busIndex, int16 channel, int32 keySwitchIndex, KeyswitchInfo& info) override {
    MIP_PRINT;
    return kResultFalse;
  }

  //--------------------
  // IConnectionPoint
  //--------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/index.html#vst3Communication

  /*
    This interface is used for the communication of separate components.
    Note that some hosts will place a proxy object between the components so
    that they are not directly connected.
  */

  //----------

  // Connects this instance with another connection point.

  /*
    class IConnectionPoint: public FUnknown {
    public:
      virtual tresult PLUGIN_API connect (IConnectionPoint* other) = 0;
      virtual tresult PLUGIN_API disconnect (IConnectionPoint* other) = 0;
      virtual tresult PLUGIN_API notify (IMessage* message) = 0;
      static const FUID iid;
    };
  */

  tresult PLUGIN_API connect(IConnectionPoint* other) override {
    MIP_PRINT;
    //IMessage* msg;
    //msg->setMessageID("test");
    //other->notify(msg);
    return kResultFalse;
  }

  //----------

  //Disconnects a given connection point from this.

  tresult PLUGIN_API disconnect(IConnectionPoint* other)  override {
    MIP_PRINT;
    return kResultFalse;
  }

  //----------

  // Called when a message has been sent from the connection point to this.

  /*
  class IMessage: public FUnknown {
  public:
    virtual FIDString PLUGIN_API getMessageID () = 0;
    virtual void PLUGIN_API setMessageID (FIDString id) = 0;
    virtual IAttributeList* PLUGIN_API getAttributes () = 0;
    static const FUID iid;
  };
  */

  tresult PLUGIN_API notify(IMessage* message) override {
    MIP_PRINT;
    return kResultFalse;
  }

  //--------------------
  // IUnitInfo
  //--------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Units.html

  /*
    IUnitInfo describes the internal structure of the Plug-in.
    - The root unit is the component itself, so getUnitCount must return 1 at least.
    - The root unit id has to be 0 (kRootUnitId).
    - Each unit can reference one program list - this reference must not change.
    - Each unit using a program list, references one program of the list.
  */

  //----------

  // Returns the flat count of units.

  int32 PLUGIN_API getUnitCount() override {
    MIP_PRINT;
    return 1;
  }

  //----------

  // Gets UnitInfo for a given index in the flat list of unit.

  /*
  struct UnitInfo {
    UnitID id;                    // unit identifier
    UnitID parentUnitId;          // identifier of parent unit (kNoParentUnitId: does not apply, this unit is the root)
    String128 name;               // name, optional for the root component, required otherwise
    ProgramListID programListId;  // id of program list used in unit (kNoProgramListId = no programs used in this unit)
  };
  */

  tresult PLUGIN_API getUnitInfo(int32 unitIndex, UnitInfo& info) override {
    MIP_PRINT;
    if (unitIndex==0) {
      info.id = kRootUnitId; //0;
      info.parentUnitId = kNoParentUnitId;
      VST3_CharToUtf16("root",info.name);
      info.programListId = kNoProgramListId;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  // Gets the count of Program List.

  int32 PLUGIN_API getProgramListCount() override {
    MIP_PRINT;
    return 0; // 1
  }

  //----------

  // Gets for a given index the Program List Info.

  /*
  struct ProgramListInfo {
    ProgramListID id;     // program list identifier
    String128 name;       // name of program list
    int32 programCount;   // number of programs in this list
  };
  */

  tresult PLUGIN_API getProgramListInfo(int32 listIndex, ProgramListInfo& info) override {
    MIP_PRINT;
    if (listIndex == 0) {
      info.id = 0;
      VST3_CharToUtf16("program",info.name);
      info.programCount = 1;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  // Gets for a given program list ID and program index its program name.

  tresult PLUGIN_API getProgramName(ProgramListID listId, int32 programIndex, String128 name) override {
    MIP_PRINT;
    if ((listId == 0) && (programIndex == 0)) {
      VST3_CharToUtf16("program",name);
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    Gets for a given program list ID, program index and attributeId the
    associated attribute value.
  */

  //CString attributeId, String128 attributeValue) {
  tresult PLUGIN_API getProgramInfo(ProgramListID listId, int32 programIndex, Steinberg::Vst::CString attributeId, String128 attributeValue) override {
    MIP_PRINT;
    ////attributeId = "";
    //if ((listId == 0) && (programIndex == 0) /* attributeId */) {
    //  char_to_utf16("",attributeValue);
    //  return kResultOk;
    //}
    return kResultFalse;
  }

  //----------

  // Returns kResultTrue if the given program index of a given program list ID supports PitchNames.

  tresult PLUGIN_API hasProgramPitchNames(ProgramListID listId, int32 programIndex) override {
    MIP_PRINT;
    return kResultFalse;
  }

  //----------

  /*
    Gets the PitchName for a given program list ID, program index and pitch.
    If PitchNames are changed the Plug-in should inform the host with
    IUnitHandler::notifyProgramListChange.
  */

  tresult PLUGIN_API getProgramPitchName(ProgramListID listId, int32 programIndex, int16 midiPitch, String128 name) override {
    MIP_PRINT;
    //char_to_utf16("pitch",name);
    return kResultFalse;
  }

  //----------

  // Gets the current selected unit.

  UnitID PLUGIN_API getSelectedUnit() override {
    MIP_PRINT;
    return 0;
  }

  //----------

  // Sets a new selected unit.

  tresult PLUGIN_API selectUnit(UnitID unitId) override {
    MIP_PRINT;
    return kResultOk;
  }

  //----------

  /*
    Gets the according unit if there is an unambiguous relation between a
    channel or a bus and a unit.
    This method mainly is intended to find out which unit is related to a
    given MIDI input channel.
  */

  tresult PLUGIN_API getUnitByBus(MediaType type, BusDirection dir, int32 busIndex, int32 channel, UnitID& unitId) override {
    MIP_PRINT;
    unitId = 0;
    return kResultOk;//False;
  }

  //----------

  /*
    Receives a preset data stream.
    - If the component supports program list data (IProgramListData), the destination of the data
      stream is the program specified by list-Id and program index (first and second parameter)
    - If the component supports unit data (IUnitData), the destination is the unit specified by the first
      parameter - in this case parameter programIndex is < 0).
  */

  tresult PLUGIN_API setUnitProgramData(int32 listOrUnitId, int32 programIndex, IBStream* data) override {
    MIP_PRINT;
    return kResultFalse;
  }

  //--------------------
  // IEditController
  //--------------------

    // file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Automation.html

  //----------

  /*
    // https://forum.juce.com/t/automation-issue-in-cubase-nuendo/14566/5
    // Cubase and Nuendo need to inform the host of the current parameter values
    if (AudioProcessor* const pluginInstance = getPluginInstance()) {
      for (int i = 0; i < pluginInstance->getNumParameters(); ++i)
        setParamNormalized((Vst::ParamID) i, (double) pluginInstance->getParameter (i));
    }
    return kResultFalse; // false?
  */

  tresult PLUGIN_API setComponentState(IBStream* state) override {
    MIP_PRINT;
    /*
    // we receive the current state of the component (processor part)
    if (state == nullptr) return kResultFalse;
    // using helper to read the stream
    IBStreamer streamer(state, kLittleEndian);
    // ABSwitchParamID::kAudioSwitch
    float savedParam1 = 0.f;
    if (!streamer.readFloat(savedParam1)) return kResultFalse;
    setParamNormalized(ABSwitchParamID::kAudioSwitch, savedParam1);
    return kResultOk;
    */
    //return kResultOk;//False;

    /*
      https://forum.juce.com/t/possible-demo-plugin-bug/19131/9
      "..which in turn returns kNotImplemented, should you not be returning
      kResultOk here?"
      "That is intended. I can’t remember why but some DAWs needed that."

    */

    /*
    int num_read = 0;
    uint32_t test_int = 0;
    state->read(&test_int,4,&num_read);
    */

    // MEditor->updateParameterFromHost(param,index,value);
    return kResultOk;
  }

  //----------

  /*
    stores and restores any GUI settings that are not related to the processor
    (like scroll positions etc)

    When the states are restored, the host passes the processor state to both
    the processor and the controller (IEditController::setComponentState).
    A host must always pass that state to the processor first. The controller
    then has to synchronize its parameters to this state (but must not perform
    any IComponentHandler callbacks).
    After restoring a state, the host will rescan the parameters (asking the
    controller) in order to update its intern representation.
  */

  //tresult PLUGIN_API setState(IBStream* state) {
  tresult PLUGIN_API setEditorState(IBStream* state) override {
    MIP_PRINT;
    return kResultOk;
  }

  //----------

  //tresult PLUGIN_API getState(IBStream* state) {
  tresult PLUGIN_API getEditorState(IBStream* state) override {
    MIP_PRINT;
    return kResultOk;
  }

  //----------

  /*
    called repeatedly by bitwig, even if plugin is deactivated
  */

  int32 PLUGIN_API getParameterCount() override {
    MIP_PRINT;
    return MDescriptor->getNumParameters();
  }

  //----------

  tresult PLUGIN_API getParameterInfo(int32 paramIndex, ParameterInfo& info) override {
    MIP_Print("index %i\n",paramIndex);
    if (paramIndex < 0) return kResultFalse;

    int32_t num_params = MDescriptor->getNumParameters();
    if (paramIndex < num_params) {
      MIP_Parameter* param = MDescriptor->getParameter(paramIndex);
      if (param) {
        memcpy(&info,&MParamInfos[paramIndex],sizeof(ParameterInfo));
        return kResultOk;
      }
    } // index < numparams

    else { // > # params
      //MIP_Trace("paramIndex (%08x) >= num.params\n",paramIndex);
      switch (paramIndex) {
        case kAfterTouch: // 128
          break;
        case kPitchBend: // 129
          break;
        case kCtrlFilterResonance: // cc 74 (slide)
          break;
      }
      switch (paramIndex & 0xffff0000) {
        case MIP_VST3_PARAM_AFTERTOUCH:
          break;
        case MIP_VST3_PARAM_PITCHBEND:
          break;
        case MIP_VST3_PARAM_BRIGHTNESS:
          break;
      }
      //return kResultOk;
      return kResultFalse;
    } // > #params

    return kResultFalse;

  }

  //----------

  tresult PLUGIN_API getParamStringByValue(ParamID id, ParamValue valueNormalized, String128 string) override {
    MIP_PRINT;
    if (id < MDescriptor->getNumParameters()) {
      MIP_Parameter* param = MDescriptor->getParameter(id);
      char buffer[32]; // ???
//      param->getDisplayText(valueNormalized,buffer);
      param->displayText(buffer,valueNormalized);
      VST3_CharToUtf16(buffer,string);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  tresult PLUGIN_API getParamValueByString(ParamID id, TChar* string, ParamValue& valueNormalized) override {
    if (id < MDescriptor->getNumParameters()) {
      MIP_Parameter* param = MDescriptor->getParameter(id);
      char temp[129];
      VST3_CharToUtf16(string,temp);
      float v = atoi(temp);
      float v2 = param->to01(v);
      valueNormalized = v2;
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  ParamValue PLUGIN_API normalizedParamToPlain(ParamID id, ParamValue valueNormalized) override {
    MIP_PRINT;
    if (id < MDescriptor->getNumParameters()) {
      MIP_Parameter* param = MDescriptor->getParameter(id);
      float v = param->from01(valueNormalized);
      return v;
    }
    else {
      return valueNormalized;
    }
  }

  //----------

  ParamValue PLUGIN_API plainParamToNormalized(ParamID id, ParamValue plainValue) override {
    MIP_PRINT;
    if (id < MDescriptor->getNumParameters()) {
      MIP_Parameter* param = MDescriptor->getParameter(id);
      float v = param->to01(plainValue);
      return v;
    }
    else {
      return plainValue;
    }
  }

  //----------

  ParamValue PLUGIN_API getParamNormalized(ParamID id) override {
    MIP_PRINT;
    if (id < MDescriptor->getNumParameters()) {
//      float v = MPlugin->getParamValue(id);
      float v = 0;
      return v;
    }
    else {
      return 0;
    }
  }

  //----------

  /*
    The host is responsible for updating the Plug-in GUI when automation data
    is transmitted to the processor. This is realized by frequent calls of
    Steinberg::Vst::IEditController::setParamNormalized.
  */

  /*
    Sets the normalized value to the parameter associated to the paramID.
    The controller must never pass this value-change back to the host via the
    IComponentHandler. It should update the according GUI element(s) only!
  */

  // bitwig sends a ParamID = 0x3f800000

  tresult PLUGIN_API setParamNormalized(ParamID id, ParamValue value) override {
    MIP_PRINT;
    if (id >= MDescriptor->getNumParameters()) {
      return kResultFalse; // ???
    }
//    MPlugin->setParamValue(id,value);
    //MEditorValues[id] = value;
    #ifndef MIP_NO_GUI
    if (MEditor) {
      MEditor->setParameterValueAndRedraw(id,value);
    }
    #endif // MIP_NO_GUI
    //MIP_Parameter* parameter = MDescriptor->getParameter(id);
    //float v = parameter->from01(value);
    //on_updateParameter(id,v);
    return kResultOk;
  }

  //----------

  /*
    Gets from host a handler.
    aka host.. beginEdit, performEdit, endEdit, restartComponent
  */

  /*
  class IComponentHandler: public FUnknown {
  public:
    virtual tresult PLUGIN_API beginEdit (ParamID id) = 0;
    virtual tresult PLUGIN_API performEdit (ParamID id, ParamValue valueNormalized) = 0;
    virtual tresult PLUGIN_API endEdit (ParamID id) = 0;
    virtual tresult PLUGIN_API restartComponent (int32 flags) = 0;
    static const FUID iid;
  };

  notifyHostParameterChanged
    MComponentHandler->begin/perform/endEdit

  */

  tresult PLUGIN_API setComponentHandler(IComponentHandler* handler) override {
    MIP_PRINT;
    MComponentHandler = handler;
    //handler->queryInterface(IComponentHandler2::iid,(void**)&MComponentHandler2);
    //MIP_Vst3Host* host = (MIP_Vst3Host*)MPlugin->getHost();
    //if (host) host->setComponentHandler(MComponentHandler,MComponentHandler2);
    return kResultOk;
  }

  //----------

  /*
    Creates the editor view of the Plug-in, currently only "editor" is
    supported, see ViewType.
    The life time of the editor view will never exceed the life time of
    this controller instance.
  */

  IPlugView* PLUGIN_API createView(FIDString name) override {
    MIP_PRINT;
    if (MDescriptor->hasEditor()) {
      if (name && (strcmp(name,ViewType::kEditor) == 0)) {
        addRef();
        return (IPlugView*)this;
      }
    }
    return nullptr;
  }

  //--------------------
  // IEditController2
  //--------------------

  tresult PLUGIN_API setKnobMode(KnobMode mode) override {
    MIP_PRINT;
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API openHelp(TBool onlyCheck) override {
    MIP_PRINT;
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API openAboutBox(TBool onlyCheck) override {
    MIP_PRINT;
    return kResultFalse;
  }

  //--------------------
  // IPlugView
  //--------------------

  tresult PLUGIN_API isPlatformTypeSupported(FIDString type) override {
    MIP_PRINT;
    // "X11EmbedWindowID"
    if (type && strcmp(type,kPlatformTypeX11EmbedWindowID) == 0) {
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    The parent window of the view has been created, the (platform)
    representation of the view should now be created as well.

    Note that the parent is owned by the caller and you are not allowed
    to alter it in any way other than adding your own views. Note that
    in this call the Plug-in could call a IPlugFrame::resizeView ()!

    Parameters
      parent  : platform handle of the parent window or view
      type    : Platform UI Types which should be created
  */

  tresult PLUGIN_API attached(void* parent, FIDString type) override {
    MIP_PRINT;

    #ifndef MIP_NO_GUI
      if (MDescriptor->hasFlag(MIP_PLUGIN_HAS_EDITOR)) {
        if (MPlugFrame) {
          uint32_t w = MDescriptor->getEditorDefaultWidth();
          uint32_t h = MDescriptor->getEditorDefaultHeight();
          //if (w == 0) w = MPlugin->getDefaultEditorWidth();
          //if (h == 0) h = MPlugin->getDefaultEditorHeight();
          ViewRect r;
          r.left    = 0;
          r.top     = 0;
          r.right   = w;
          r.bottom  = h;
          MPlugFrame->resizeView(this,&r);
        }
        MEditor = MPlugin->openEditor(parent);
        MEditor->open();
        //if (MRunLoop)
        MRunLoop->registerTimer(this,MIP_PLUGIN_VST3_TIMER_MS);
        return kResultOk;
      }
    #endif // MIP_NO_GUI
    return kResultFalse;
  }

  //----------

  /*
    The parent window of the view is about to be destroyed.
    You have to remove all your own views from the parent window or view.
  */

  tresult PLUGIN_API removed() override {
    MIP_PRINT;

      #ifndef MIP_NO_GUI
      if (MDescriptor->hasFlag(MIP_PLUGIN_HAS_EDITOR)) {
        //if (MRunLoop)
        MRunLoop->unregisterTimer(this);
        MEditor->close();
        MPlugin->closeEditor(MEditor);
        MEditor = nullptr;
        return kResultOk;
      }
      #endif // MIP_NO_GUI
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API onWheel(float distance) override {
    MIP_PRINT;
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API onKeyDown(char16 key, int16 keyCode, int16 modifiers) override {
    MIP_PRINT;
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API onKeyUp(char16 key, int16 keyCode, int16 modifiers) override {
    MIP_PRINT;
    return kResultOk;
  }

  //----------

  /*
    Returns the size of the platform representation of the view.
  */

  tresult PLUGIN_API getSize(ViewRect* size) override {
    MIP_PRINT;
    if (MDescriptor->hasEditor()) {
      size->left    = 0;
      size->top     = 0;
      size->right   = MDescriptor->getEditorWidth();
      size->bottom  = MDescriptor->getEditorHeight();
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    Resizes the platform representation of the view to the given rect.
    Note that if the Plug-in requests a resize (IPlugFrame::resizeView())
    onSize has to be called afterward.
  */

  tresult PLUGIN_API onSize(ViewRect* newSize) override {
    MIP_PRINT;
    if (MDescriptor->hasEditor()) {
      //MEditorWidth = newSize->getWidth();
      //MEditorHeight = newSize->getHeight();
      //TODO: resize/redraw editor
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  /*
    Focus changed message.
  */

  tresult PLUGIN_API onFocus(TBool state) override {
    MIP_PRINT;
    return kResultOk;
  }

  //----------

  /*
    Sets IPlugFrame object to allow the Plug-in to inform the host
    about resizing.
  */

  tresult PLUGIN_API setFrame(IPlugFrame* frame) override {
    MIP_PRINT;

    if (MDescriptor->hasEditor()) {
      MPlugFrame = frame;
      //tresult res =
      MPlugFrame->queryInterface(IRunLoop_iid, (void**)&MRunLoop);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  /*
    Is view sizable by user?
  */

  tresult PLUGIN_API canResize() override {
    MIP_PRINT;
    return kResultFalse;
  }

  //----------

  /*
    On live resize this is called to check if the view can be resized to the
    given rect, if not adjust the rect to the allowed size.
  */

  tresult PLUGIN_API checkSizeConstraint(ViewRect* rect) override {
    MIP_PRINT;
    return kResultOk;//False;
  }

  //--------------------
  // ITimerHandler
  //--------------------

  // tid = same as setComponentHandler..

  /*
    will be called afte we regisdter the timer:
      MRunLoop->registerTimer(this,MIP_PLUGIN_VST3_TIMER_MS);
    (in IEditView.attached)
  */

  void onTimer() override {
    //MIP_PRINT;

    #ifndef MIP_NO_GUI
      MPlugin->on_updateEditor(MEditor);
      MPlugin->flushParamsToHost();
    #endif // MIP_NO_GUI
  }

};

//----------------------------------------------------------------------
#endif
