#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

/*
  * parameters, ports: index == id
  * port_index: ignored (separate handlers for each port)
*/

//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_audio_port.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_entry.h"
#include "plugin/mip_host.h"
#include "plugin/mip_note_port.h"
#include "plugin/mip_parameter.h"
#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"
#include "plugin/clap/mip_clap_utils.h"

//----------

#ifdef MIP_EXE
  #include "plugin/exe/mip_exe_entry.h"
#endif

//#ifdef MIP_LV2
//  #include "plugin/lv2/mip_lv2_entry.h"
//#endif

#ifdef MIP_VST2
  #include "plugin/vst2/mip_vst2_entry.h"
#endif

#ifdef MIP_VST3
  #include "plugin/vst3/mip_vst3_entry.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  event -> note/param -> voice -> process
  event<note<voice>,param<voice>>
  process<voice>
*/

//class myVoice {};
//typedef MIP_VoiceHandler<myVoice,16>  myVoiceHandler;

#define MIP_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK  128

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Plugin
: public MIP_ClapPlugin
, public MIP_EditorListener {

//------------------------------
protected:
//------------------------------

  MIP_ParameterArray    MParameters               = {};
  MIP_Editor*           MEditor                   = nullptr;
  MIP_Host*             MHost                     = nullptr;
  const clap_host_t*    MClapHost                 = nullptr;

  bool                  MIsActivated              = false;
  bool                  MIsProcessing             = false;
  bool                  MIsEditorOpen             = false;

  uint32_t              MSelectedAudioPortsConfig = 0;
  uint32_t              MSelectedQuickControls    = 0;
  int32_t               MRenderMode               = 0;

  uint32_t              MLatency                  = 0;
  uint32_t              MTail                     = 0;

  MIP_AudioPortArray    MAudioInputs              = {};
  MIP_AudioPortArray    MAudioOutputs             = {};
  MIP_NotePortArray     MNoteInputs               = {};
  MIP_NotePortArray     MNoteOutputs              = {};

  double*               MAudioParamValues         = {0};
  double*               MHostParamValues          = {0};

  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK> MAudioParamQueue  = {};
  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK> MHostParamQueue   = {};

//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
    //MDescriptor = ADescriptor;

    MClapHost = AHost;
    //MHost = new MIP_Host(AHost);

  }

  //----------

  virtual ~MIP_Plugin() {
    #ifndef MIP_NO_AUTODELETE
      deleteAudioInputs();
      deleteAudioOutputs();
      deleteNoteInputs();
      deleteNoteOutputs();
      deleteParameters();
    #endif
    delete MHost;
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() override {
    CLAP_Print("\n");

    MHost = new MIP_Host(MClapHost);
    MHost->initExtensions();
    #ifdef MIP_DEBUG_CLAP
      MHost->printSupportedExtensions();
      CLAP_Print("host clap version: %i.%i.%i\n",AHost->clap_version.major,AHost->clap_version.minor,AHost->clap_version.revision);
      CLAP_Print("host name:         %s\n",AHost->name);
      CLAP_Print("host vendor:       %s\n",AHost->vendor);
      CLAP_Print("host url:          %s\n",AHost->url);
      CLAP_Print("host version:      %s\n",AHost->version);
    #endif

    // queues
    uint32_t num = MParameters.size();
    uint32_t size = num * sizeof(double);
    MAudioParamValues = (double*)malloc(size);
    MHostParamValues = (double*)malloc(size);
    memset(MAudioParamValues,0,size);
    memset(MHostParamValues,0,size);
    setDefaultParameterValues();
    return true;
  }

  //----------

  void destroy() override {
    CLAP_Print("\n");
    free(MAudioParamValues);
    free(MHostParamValues);
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    CLAP_Print("sample_rate %.3f min_frames_count %i max_frames_count %i\n",sample_rate,min_frames_count,max_frames_count);
    MIsActivated = true;
    //return MParameters.activate(sample_rate,min_frames_count,max_frames_count);
    return true;
  }

  //----------

  void deactivate() override {
    CLAP_Print("\n");
    MIsActivated = false;
  }

  //----------

  bool start_processing() override {
    CLAP_Print("\n");
    MIsProcessing = true;
    return true;
  }

  //----------

  void stop_processing() override {
    CLAP_Print("\n");
    MIsProcessing = false;
  }

  //----------

  void reset() override {
    CLAP_Print("\n");
  }

  //----------

  clap_process_status process(const clap_process_t *process) override {
    flushAudioParams();
    preProcessEvents(process->in_events,process->out_events);
    //..
    postProcessEvents(process->in_events,process->out_events);
    flushHostParams(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    CLAP_Print("id %s\n",id);
    //if (MIP_IsEqual(id,CLAP_EXT_AMBISONIC))           return &MAmbisonic;
    if (MIP_IsEqual(id,CLAP_EXT_AUDIO_PORTS))           return &MAudioPorts;
    //if (MIP_IsEqual(id,CLAP_EXT_AUDIO_PORTS_CONFIG))  return &MAudioPorts;
    ////if (MIP_IsEqual(id,CLAP_EXT_CHECK_FOR_UPDATE))  return &MCheckForUpdate;
    //if (MIP_IsEqual(id,CLAP_EXT_CV))                  return &MCv;
    //if (MIP_IsEqual(id,CLAP_EXT_FILE_REFERENCE))      return &MFileReference;
    if (MIP_IsEqual(id,CLAP_EXT_GUI))                   return &MGui;
    if (MIP_IsEqual(id,CLAP_EXT_LATENCY))               return &MLatency;
    //if (MIP_IsEqual(id,CLAP_EXT_MIDI_MAPPINGS))       return &MMidiMappings;
    //if (MIP_IsEqual(id,CLAP_EXT_NOTE_NAME))           return &MNoteName;
    if (MIP_IsEqual(id,CLAP_EXT_NOTE_PORTS))            return &MNotePorts;
    if (MIP_IsEqual(id,CLAP_EXT_PARAMS))                return &MParams;
    //if (MIP_IsEqual(id,CLAP_EXT_POSIX_FD_SUPPORT))    return &MPosixFdSupport;
    //if (MIP_IsEqual(id,CLAP_EXT_PRESET_LOAD))         return &MPresetLoad;
    //if (MIP_IsEqual(id,CLAP_EXT_QUICK_CONTROLS))      return &MQuickControls;
    //if (MIP_IsEqual(id,CLAP_EXT_RENDER))              return &MRender;
    if (MIP_IsEqual(id,CLAP_EXT_STATE))                 return &MState;
    //if (MIP_IsEqual(id,CLAP_EXT_SURROUND))            return &MSurround;
    if (MIP_IsEqual(id,CLAP_EXT_TAIL))                  return &MTail;
    //if (MIP_IsEqual(id,CLAP_EXT_THREAD_POOL))         return &MThreadPool;
    //if (MIP_IsEqual(id,CLAP_EXT_TIMER_SUPPORT))       return &MTimerSupport;
    //if (MIP_IsEqual(id,CLAP_EXT_TRACK_INFO))          return &MTrackInfo;
    //if (MIP_IsEqual(id,CLAP_EXT_TUNING))              return &MTuning;
    if (MIP_IsEqual(id,CLAP_EXT_VOICE_INFO))            return &MVoiceInfo;
    return nullptr;
  }

  //----------

  void on_main_thread() override {
    CLAP_Print("\n");
  }

//------------------------------
public: // ext - ambisonic
//------------------------------

  bool ambisonic_get_info(bool is_input, uint32_t port_index, clap_ambisonic_info_t *info) override {
    CLAP_Print("is_input %i port_index %i\n",is_input,port_index);
    //info->ordering      = 0;
    //info->normalization = 0;
    //return true;
    return false;
  }

//------------------------------
public: // ext - audio_ports_config
//------------------------------

  uint32_t audio_ports_config_count() override {
    CLAP_Print("\n");
    //return 1;
    return 0;
  }

  //----------

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t* config) override {
    CLAP_Print("index %i\n",index);
    //switch (index) {
    //  case 0:
    //    config->id                        = 0;
    //    strncpy(config->name,"config1",CLAP_NAME_SIZE-1);
    //    config->input_port_count          = MAudioInputs.size();
    //    config->output_port_count         = MAudioOutputs.size();
    //    config->has_main_input            = true;
    //    config->main_input_channel_count  = 2;
    //    config->main_input_port_type      = CLAP_PORT_STEREO;
    //    config->has_main_output           = true;
    //    config->main_output_channel_count = 2;
    //    config->main_output_port_type     = CLAP_PORT_STEREO;
    //    return true;
    //}
    return false;
  }

  //----------

  bool audio_ports_config_select(clap_id config_id) override {
    CLAP_Print("config_id %i\n",config_id);
    MSelectedAudioPortsConfig = config_id;
    return true;
  }

//------------------------------
public: // ext - audio_ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    CLAP_Print("is_input %i\n",is_input);
    if (is_input) return MAudioInputs.size();
    else return MAudioOutputs.size();
    return 0;
  }

  //----------

  bool audio_ports_get(uint32_t index,  bool is_input, clap_audio_port_info_t* info) override {
    CLAP_Print("index %i is_input %i\n",index,is_input);
    if (is_input) {
      MIP_AudioPort* audio_port = MAudioInputs[index];
      memcpy(info,audio_port->getInfo(),sizeof(clap_audio_port_info_t));
      return true;
    }
    else {
      MIP_AudioPort* audio_port = MAudioOutputs[index];
      memcpy(info,audio_port->getInfo(),sizeof(clap_audio_port_info_t));
      return true;
    }
    return false;
  }

//------------------------------
public: // ext - check for update
//------------------------------

  //void check_for_update_check(bool include_beta) override {
  //  CLAP_Print("\n");
  //}

//------------------------------
public: // ext - cv
//------------------------------

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t* channel_type) override {
    CLAP_Print("is_input %i port_index %i channel_index %i\n",is_input,port_index,channel_index);
    //return CLAP_CV_VALUE;
    return false;
  }

//------------------------------
public: // ext - posix_fd_support
//------------------------------

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) override {
    CLAP_Print("fd %i flags %i\n",fd,flags);
  }

//------------------------------
public: // file_reference
//------------------------------

  uint32_t file_reference_count() override {
    CLAP_Print("\n");
    return 0;
  }

  //----------

  bool file_reference_get(uint32_t index, clap_file_reference_t *file_reference) override {
    CLAP_Print("index %i\n",index);
    return false;
  }

  //----------

  bool file_reference_get_blake3_digest(clap_id resource_id, uint8_t *digest) override {
    CLAP_Print("resource_id %i\n",resource_id);
    return false;
  }

  //----------

  bool file_reference_get_file_size(clap_id resource_id, uint64_t *size) override {
    CLAP_Print("resource_id %i\n",resource_id);
    return false;
  }

  //----------

  bool file_reference_update_path(clap_id resource_id, const char *path) override {
    CLAP_Print("resource_id %i oath %s\n",resource_id,path);
    return false;
  }

  //----------

  bool file_reference_save_resources() override {
    CLAP_Print("\n");
    return false;
  }

//------------------------------
public: // ext - gui
//------------------------------

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    CLAP_Print("api %s is_floating %i\n",api,is_floating);
    if (is_floating) return false;
    if (strcmp(api,CLAP_WINDOW_API_X11) == 0) return true;
    return false;
  }

  //----------

  bool gui_get_preferred_api(const char **api, bool *is_floating) override {
    CLAP_Print("\n");
    //if (MEditor) return MEditor->getPreferredApi(api,is_floating);
    *api = CLAP_WINDOW_API_X11;
    *is_floating = false;
    return true;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    CLAP_Print("api %s is_floating %i\n",api,is_floating);
    MEditor = new MIP_Editor(this,this,256,256,true);
    return (MEditor);
  }

  //----------

  void gui_destroy() override {
    CLAP_Print("\n");
    if (MIsEditorOpen) {
      MIsEditorOpen = false;
      MEditor->hide();
    }
    delete MEditor;
    MEditor = nullptr;
  }

  //----------

  bool gui_set_scale(double scale) override {
    CLAP_Print("scale %.3f\n",scale);
    if (MEditor) return MEditor->setScale(scale);
    return false;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    CLAP_Print("*width %i *height %i\n",*width,*height);
    if (MEditor) return MEditor->getSize(width,height);
    return false;
  }

  //----------

  bool gui_can_resize() override {
    CLAP_Print("\n");
    if (MEditor) return MEditor->canResize();
    return true;
  }

  //----------

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) override {
    CLAP_Print("\n");
    if (MEditor) return MEditor->getResizeHints(hints);
    return false;
  }

  //----------

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    CLAP_Print("*width %i *height %i\n",*width,*height);
    if (MEditor) return MEditor->adjustSize(width,height);
    return false;
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    CLAP_Print("width %i height %i\n",width,height);
    if (MEditor) return MEditor->setSize(width,height);
    return true;
  }

  //----------

  bool gui_set_parent(const clap_window_t *window) override {
    CLAP_Print("api %s window %i\n",window->api,window->x11);
    if (MEditor) return MEditor->setParent(window);
    return true;
  }

  //----------

  bool gui_set_transient(const clap_window_t *window) override {
    CLAP_Print("api %s window %i\n",window->api,window->x11);
    if (MEditor) return MEditor->setTransient(window);
    return true;
  }

  //----------

  void gui_suggest_title(const char *title) override {
    CLAP_Print("title %s\n",title);
    if (MEditor) MEditor->suggestTitle(title);
  }

  //----------

  bool gui_show() override {
    CLAP_Print("\n");
    if (MEditor) {
      //MEditor->setEditorParameterValues(&MParameters,false);
      setEditorParameterValues(false);
      bool res = MEditor->show();
      MIsEditorOpen = res;
      return res;
    }
    return false;
  }

  //----------

  bool gui_hide() override {
    CLAP_Print("\n");
    if (MEditor) {
      MIsEditorOpen = false;
      return MEditor->hide();
    }
    return false;
  }

//------------------------------
public: // ext - latency
//------------------------------

  uint32_t latency_get() override {
    CLAP_Print("\n");
    return MLatency;
  }

//------------------------------
public: // ext - midi mappings
//------------------------------

  uint32_t midi_mappings_count() override {
    CLAP_Print("\n");
    return 0;
  }

  //----------

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t* mapping) override {
    CLAP_Print("index %i\n",index);
    //switch (index) {
    //  default:
    //    mapping->channel  = 0;
    //    mapping->number   = 0;
    //    mapping->param_id = 0;
    //}
    return false;
  }


//------------------------------
public: // ext - note name
//------------------------------

  uint32_t note_name_count() override {
    CLAP_Print("\n");
    return 0;
  }

  //----------

  bool note_name_get(uint32_t index, clap_note_name_t* note_name) override {
    CLAP_Print("index %i\n",index);
    //switch (index) {
    //  default:
    //    strncpy(note_name->name,"note_name",CLAP_NAME_SIZE-1);
    //    note_name->port     = 0;
    //    note_name->key      = 0;
    //    note_name->channel  = -1; // -1 for every channels
    //    return true;
    //}
    return false;
  }

//------------------------------
public: // note_ports
//------------------------------

  uint32_t note_ports_count(bool is_input) override {
    CLAP_Print("\n");
    if (is_input) return MNoteInputs.size();
    else return MNoteOutputs.size();
    return 0;
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) override {
    CLAP_Print("index %i is_input %i\n",index,is_input);
    if (is_input) {
      memcpy(info,MNoteInputs[index],sizeof(clap_note_port_info_t));
      return true;
    }
    else {
      memcpy(info,MNoteOutputs[index],sizeof(clap_note_port_info_t));
      return true;
    }
    return false;
  }

//------------------------------
public: // ext - params
//------------------------------

  uint32_t params_count() override {
    CLAP_Print("\n");
    return MParameters.size();
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) override {
    CLAP_Print("param_index %i\n",param_index);
    MIP_Parameter* parameter = MParameters[param_index];
    clap_param_info_t* info = parameter->getInfo();
    //printParamInfo(info);
    memcpy(param_info,info,sizeof(clap_param_info_t));
    return true;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) override {
    CLAP_Print("param_id %i\n",param_id);
    MIP_Parameter* parameter = MParameters[param_id];
    *value = parameter->getValue();
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) override {
    CLAP_Print("param_id %i value %.3f\n",param_id,value);
    MIP_Parameter* parameter = MParameters[param_id];
    return parameter->valueToText(value,display,size);
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char* display, double* value) override {
    CLAP_Print("param_id %i display %.3f\n",param_id,display);
    MIP_Parameter* parameter = MParameters[param_id];
    return parameter->textToValue(display,value);
  }

  //----------

  void params_flush(const clap_input_events_t  *in, const clap_output_events_t *out) override {
    CLAP_Print("\n");
    preProcessEvents(in,out);
    postProcessEvents(in,out);
    //handle_input_events(in,out);
    //handle_output_events(in,out);
  }

//------------------------------
public: // ext - preset load
//------------------------------

  bool preset_load_from_file(const char *path) override {
    CLAP_Print("path %s\n",path);
    return false;
  }

//------------------------------
public: // quick controls
//------------------------------

  uint32_t quick_controls_count() override {
    CLAP_Print("\n");
    return 0;
  }

  //----------

  bool quick_controls_get(uint32_t page_index, clap_quick_controls_page_t *page) override {
    CLAP_Print("page_index %i\n",page_index);
    //switch (page_index) {
    //  default:
    //    page->id = 0;
    //    strncpy(page->name, "name",CLAP_NAME_SIZE-1);
    //    strncpy(page->keywords, "",CLAP_KEYWORDS_SIZE-1);
    //    for (uint32_t i=0; i<CLAP_QUICK_CONTROLS_COUNT; i++) {
    //      page->param_ids[i] = 0;
    //    }
    //    return true;
    //}
    return false;
  }

  //----------

  void quick_controls_select(clap_id page_id) override {
    CLAP_Print("page_id %i\n",page_id);
    MSelectedQuickControls = page_id;
  }

  //----------

  clap_id quick_controls_get_selected() override {
    CLAP_Print("\n");
    return MSelectedQuickControls;
  }

//------------------------------
public: // ext - render
//------------------------------

  bool render_has_hard_realtime_requirement() override {
    CLAP_Print("\n");
    return false;
  }

  //----------

  bool render_set(clap_plugin_render_mode mode) override {
    CLAP_Print("mode %s\n",MIP_ClapRenderModeNames[mode]);
    MRenderMode = mode;
    return false;
  }

//------------------------------
public: // ext - state
//------------------------------

  bool state_save(const clap_ostream_t *stream) override {
    CLAP_Print("\n");
    uint32_t version = 0;
    stream->write(stream,&version,sizeof(uint32_t));
    uint32_t num = MParameters.size();
    stream->write(stream,&num,sizeof(uint32_t));
    for (uint32_t i=0; i<num; i++) {
      MIP_Parameter* param = MParameters[i];
      double value = param->getValue();
      stream->write(stream,&value,sizeof(double));
    }
    return true;
  }

  //----------

  bool state_load(const clap_istream_t *stream) override {
    CLAP_Print("\n");
    return true;
    uint32_t version = 0;
    uint32_t num = 0;
    uint32_t param_count = MParameters.size();
    stream->read(stream,&version,sizeof(uint32_t));
    stream->read(stream,&num,sizeof(uint32_t));
    //MIP_Assert( num == MParameters.parameterCount() );
    if (version == 0) {
      if (num == param_count) {
        for (uint32_t i=0; i<num; i++) {
          MIP_Parameter* param = MParameters[i];
          double value = 0.0;
          stream->read(stream,&value,sizeof(double));
          param->setValue(value);
        }
        return true;
      } // num
      else {
        CLAP_Print("Error: wrong number of values (expected %i, found %i\n",param_count,num);
        return false;
      }
    } // v0
    else {
      CLAP_Print("Error: version mismatch! (expected 0, found %i\n",version);
      return false;
    }
  }

//------------------------------
public: // ext - surround
//------------------------------

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t* channel_map, uint32_t channel_map_capacity) override {
    CLAP_Print("is_input %i port_index %i\n",is_input,port_index);
    return 0;
  }

  //----------

  void surround_changed() override {
    CLAP_Print("\n");
  }

//------------------------------
public: // ext - tail
//------------------------------

  uint32_t tail_get() override {
    CLAP_Print("\n");
    return MTail;
  }

//------------------------------
public: // ext - thread_pool
//------------------------------

  void thread_pool_exec(uint32_t task_index) override {
    //CLAP_Print("task_index %i\n",task_index);
  }

//------------------------------
public: // ext - timer_support
//------------------------------

  void timer_support_on_timer(clap_id timer_id) override {
    CLAP_Print("timer_id %i\n",timer_id);
    //if (MEditor) MEditor->on_timerCallback();
  }

//------------------------------
public: // ext - track_info
//------------------------------

  void track_info_changed() override {
    //clap_track_info_t info;
    //if (MHost) {
    //  if (MHost->track_info->get(MHost->host,&info)) {
    //    printTrackInfo(&info);
    //  }
    //}
  }

//------------------------------
public: // ext - tuning
//------------------------------

  void tuning_changed() override {
    CLAP_Print("\n");
  }

//------------------------------
public: // ext - voice_info
//------------------------------

  bool voice_info_get(clap_voice_info_t *info) override {
    CLAP_Print("\n");
    //info->voice_count     = NUM_VOICES;
    //info->voice_capacity  = NUM_VOICES;
    //info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    //return true;
    return false;
  }

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


//------------------------------
protected: // parameters
//------------------------------

  void setupParameters(clap_param_info_t* params, uint32_t num) {
    //MIP_PRINT;
    for (uint32_t i=0; i<num; i++) {
      MIP_Parameter* parameter = new MIP_Parameter(&params[i]);
      //parameter->setId(i);
      MParameters.append(parameter);
    }
  }

  //----------

  MIP_Parameter* appendParameter(MIP_Parameter* AParameter, bool AKeepIndex=false) {
    if (!AKeepIndex) {
      uint32_t index = MParameters.size();
      AParameter->setId(index);
    }
    MParameters.append(AParameter);
    return AParameter;
  }

  //----------

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      delete MParameters[i];
    }
  }


//------------------------------
public: // audio inputs
//------------------------------

  void setupAudioInputs(clap_audio_port_info_t* infos, uint32_t num) {
    //MIP_PRINT;
    for (uint32_t i=0; i<num; i++) {
      MIP_AudioPort* audio_port = new MIP_AudioPort(&infos[i]);
      MAudioInputs.append(audio_port);
    }
  }

  //----------

  MIP_AudioPort* appendAudioInput(MIP_AudioPort* AAudioPort, bool AKeepIndex=false) {
    if (!AKeepIndex) {
      uint32_t index = MAudioInputs.size();
      AAudioPort->setId(index);
    }
    MAudioInputs.append(AAudioPort);
    return AAudioPort;
  }

  //----------

  void deleteAudioInputs() {
    for (uint32_t i=0; i<MAudioInputs.size(); i++) {
      delete MAudioInputs[i];
      MAudioInputs[i] = nullptr;
    }
  }

//------------------------------
public: // audio outputs
//------------------------------

  void setupAudioOutputs(clap_audio_port_info_t* infos, uint32_t num) {
    //MIP_PRINT;
    for (uint32_t i=0; i<num; i++) {
      MIP_AudioPort* audio_port = new MIP_AudioPort(&infos[i]);
      MAudioOutputs.append(audio_port);
    }
  }

  //----------

  MIP_AudioPort* appendAudioOutput(MIP_AudioPort* AAudioPort, bool AKeepIndex=false) {
    if (!AKeepIndex) {
      uint32_t index = MAudioOutputs.size();
      AAudioPort->setId(index);
    }
    MAudioOutputs.append(AAudioPort);
    return AAudioPort;
  }

  //----------

  void deleteAudioOutputs() {
    for (uint32_t i=0; i<MAudioOutputs.size(); i++) {
      delete MAudioOutputs[i];
      MAudioOutputs[i] = nullptr;
    }
  }

//------------------------------
public: // note inputs
//------------------------------

  void setupNoteInputs(clap_note_port_info_t* infos, uint32_t num) {
    //MIP_PRINT;
    for (uint32_t i=0; i<num; i++) {
      MIP_NotePort* note_port = new MIP_NotePort(&infos[i]);
      MNoteInputs.append(note_port);
    }
  }

  //----------

  MIP_NotePort* appendNoteInput(MIP_NotePort* ANotePort, bool AKeepIndex=false) {
    if (!AKeepIndex) {
      uint32_t index = MNoteInputs.size();
      ANotePort->setId(index);
    }
    MNoteInputs.append(ANotePort);
    return ANotePort;
  }

  //----------

  void deleteNoteInputs() {
    for (uint32_t i=0; i<MNoteInputs.size(); i++) {
      delete MNoteInputs[i];
      MNoteInputs[i] = nullptr;
    }
  }

//------------------------------
public: // note outputs
//------------------------------

  void setupNoteOutputs(clap_note_port_info_t* infos, uint32_t num) {
    //MIP_PRINT;
    for (uint32_t i=0; i<num; i++) {
      MIP_NotePort* note_port = new MIP_NotePort(&infos[i]);
      MNoteOutputs.append(note_port);
    }
  }

  //----------

  MIP_NotePort* appendNoteOutput(MIP_NotePort* ANotePort, bool AKeepIndex=false) {
    if (!AKeepIndex) {
      uint32_t index = MNoteOutputs.size();
      ANotePort->setId(index);
    }
    MNoteOutputs.append(ANotePort);
    return ANotePort;
  }

  //----------

  void deleteNoteOutputs() {
    for (uint32_t i=0; i<MNoteOutputs.size(); i++) {
      delete MNoteOutputs[i];
      MNoteOutputs[i] = nullptr;
    }
  }

//------------------------------
public: // queues
//------------------------------

  /*
    called from
      on_editor_listener_parameter
  */

  void queueAudioParam(uint32_t AIndex, double AValue) {
    MAudioParamValues[AIndex] = AValue;
    MAudioParamQueue.write(AIndex);
  }

  //----------

  void flushAudioParams() {
    uint32_t index = 0;
    while (MAudioParamQueue.read(&index)) {
      MIP_Parameter* param = MParameters[index];
      double value = MAudioParamValues[index];
      // if we already set this, it should be (bit) identical?
      if (value != param->getValue()) {
        param->setValue(value);
      }
    }
  }

  //----------

  /*
    called from
      on_editor_listener_parameter
  */

  void queueHostParam(uint32_t AIndex, double AValue) {
    MHostParamValues[AIndex] = AValue;
    MHostParamQueue.write(AIndex);
  }

  //----------

  void flushHostParams(const clap_output_events_t* out_events) {
    uint32_t index = 0;
    while (MHostParamQueue.read(&index)) {
      double value = MHostParamValues[index];
      //MIP_Print("index %i value %f\n",index,value);
      //todo: check if value really changed (if multiple events)

      send_param_gesture_event(index,CLAP_EVENT_PARAM_GESTURE_BEGIN,out_events);
      send_param_value_event(index,value,out_events);
      send_param_gesture_event(index,CLAP_EVENT_PARAM_GESTURE_END,out_events);

    }
  }

//------------------------------
public: // editor listener
//------------------------------

//qwe

  void on_editor_listener_parameter(uint32_t AIndex, double AValue) override {
    queueAudioParam(AIndex,AValue);
    queueHostParam(AIndex,AValue);
    handle_parameter(AIndex,AValue);
  }

//------------------------------
public: // param / mod
//------------------------------

  /*
    called from:
      on_editor_listener_parameter
      handle_parameter_event
  */

  void handle_parameter(uint32_t AIndex, double AValue) {
    //MIP_Print("AIndex %i AValue %.3f\n",AIndex,AValue);
    MParameters[AIndex]->setValue(AValue);
  }

  //----------

  /*
    called from:
      handle_modulation_event
  */

  void handle_modulation(uint32_t AIndex, double AValue) {
    //MIP_Print("AIndex %i AValue %.3f\n",AIndex,AValue);
    MParameters[AIndex]->setModulation(AValue);
  }

//------------------------------
public: //events
//------------------------------

  virtual void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        handle_event(header);
      }
    }
  }

  //----------

  virtual void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

//------------------------------
public:
//------------------------------

  virtual void handle_event(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_PARAM_VALUE:      handle_parameter_event((clap_event_param_value_t*)header); break;
      case CLAP_EVENT_PARAM_MOD:        handle_modulation_event((clap_event_param_mod_t*)header); break;
      case CLAP_EVENT_TRANSPORT:        handle_transport_event((clap_event_transport_t*)header); break;
      case CLAP_EVENT_NOTE_ON:          handle_note_on_event((clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_OFF:         handle_note_off_event((clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_END:         handle_note_end_event((clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_CHOKE:       handle_note_choke_event((clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_EXPRESSION:  handle_note_expression_event((clap_event_note_expression_t*)header); break;
      case CLAP_EVENT_MIDI:             handle_midi_event((clap_event_midi_t*)header); break;
      case CLAP_EVENT_MIDI2:            handle_midi2_event((clap_event_midi2_t*)header); break;
      case CLAP_EVENT_MIDI_SYSEX:       handle_midi_sysex_event((clap_event_midi_sysex_t*)header); break;
    }
  };

  //----------

  virtual void handle_parameter_event(clap_event_param_value_t* event) {
    //MParameters.handle_value_event(event);
    handle_parameter(event->param_id,event->value);
    //if (MListener) MListener->on_event_listener_parameter(event->param_id,event->value);
    if (MEditor && MIsEditorOpen) {
      MEditor->queueGuiParam(event->param_id,event->value);
    }
  }

  virtual void handle_modulation_event(clap_event_param_mod_t* event) {
    //MParameters.handle_modulation_event(event);
    handle_modulation(event->param_id,event->amount);
    //if (MListener) MListener->on_event_listener_modulation(event->param_id,event->amount);
    if (MEditor && MIsEditorOpen) {
      MEditor->queueGuiMod(event->param_id,event->amount);
    }
  }

  virtual void handle_transport_event(clap_event_transport_t* event) {
    //MIP_Print("TODO\n");
  }

  virtual void handle_note_on_event(clap_event_note_t* event) {
    //MIP_Print("TODO\n");
    //if (MAudioProcessor) MAudioProcessor->handle_note_on_event(event);
  }

  virtual void handle_note_off_event(clap_event_note_t* event) {
    //MIP_Print("TODO\n");
    //if (MAudioProcessor) MAudioProcessor->handle_note_off_event(event);
  }

  virtual void handle_note_end_event(clap_event_note_t* event) {
    //MIP_Print("ERROR\n");
    //if (MAudioProcessor) MAudioProcessor->handle_note_end_event(event);
  }

  virtual void handle_note_choke_event(clap_event_note_t* event) {
    //MIP_Print("TODO\n");
    //if (MAudioProcessor) MAudioProcessor->handle_note_choke_event(event);
  }

  virtual void handle_note_expression_event(clap_event_note_expression_t* event) {
    //MIP_Print("TODO\n");
    //if (MAudioProcessor) MAudioProcessor->handle_note_expression_event(event);
  }

  //----------

  virtual void handle_midi_event(clap_event_midi_t* event) {
    //MIP_Print("TODO\n");
  }

  virtual void handle_midi2_event(clap_event_midi2_t* event) {
    //MIP_Print("TODO\n");
  }

  virtual void handle_midi_sysex_event(clap_event_midi_sysex_t* event) {
    //MIP_Print("TODO\n");
  }

//------------------------------
public: // send events
//------------------------------

  void send_event(const clap_event_header_t* header, const clap_output_events_t* out_events) {
    out_events->try_push(out_events,header);
  }

  //----------

  void send_param_value_event(uint32_t index, double value, const clap_output_events_t* out_events) {
    //MIP_Print("CLAP_EVENT_PARAM_VALUE: index %i value %f\n",index,value);
    clap_event_param_value_t param_value;
    param_value.header.size     = sizeof (clap_event_param_value_t);
    param_value.header.time     = 0;
    param_value.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    param_value.header.type     = CLAP_EVENT_PARAM_VALUE;
    param_value.header.flags    = 0;//CLAP_EVENT_DONT_RECORD;// | CLAP_EVENT_IS_LIVE;
    param_value.param_id        = index;
    param_value.cookie          = nullptr;
    param_value.port_index      = -1;
    param_value.key             = -1;
    param_value.channel         = -1;
    param_value.value           = value;
    clap_event_header_t* header = (clap_event_header_t*)&param_value;
    out_events->try_push(out_events,header);
  }

  //----------

  void send_param_mod_event(uint32_t index, double value, const clap_output_events_t* out_events) {
    clap_event_param_mod_t param_mod;
    param_mod.header.size     = sizeof (clap_event_param_mod_t);
    param_mod.header.time     = 0;
    param_mod.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    param_mod.header.type     = CLAP_EVENT_PARAM_MOD;
    param_mod.header.flags    = 0;//CLAP_EVENT_DONT_RECORD;// | CLAP_EVENT_IS_LIVE;
    param_mod.param_id        = index;
    param_mod.cookie          = nullptr;
    param_mod.port_index      = -1;
    param_mod.key             = -1;
    param_mod.channel         = -1;
    param_mod.amount          = value;
    clap_event_header_t* header = (clap_event_header_t*)&param_mod;
    out_events->try_push(out_events,header);
  }

  //----------

  void send_param_gesture_event(uint32_t index, int32_t param_gesture, const clap_output_events_t* out_events) {
    clap_event_param_gesture_t gesture;
    gesture.header.size     = sizeof (clap_event_param_gesture_t);
    gesture.header.time     = 0;
    gesture.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    gesture.header.type     = param_gesture;  //CLAP_EVENT_PARAM_GESTURE_BEGIN;
    gesture.header.flags    = 0;              //CLAP_EVENT_DONT_RECORD;// | CLAP_EVENT_IS_LIVE;
    gesture.param_id        = index;
    clap_event_header_t* header = (clap_event_header_t*)&gesture;
    out_events->try_push(out_events,header);
  }

  //----------

  void send_note_event(uint32_t type, int32_t port, int32_t key, int32_t channel, double velocity, const clap_output_events_t* out_events) {
    clap_event_note_t note;
    note.header.size      = sizeof(clap_event_note_t);
    note.header.time      = 0;
    note.header.space_id  = CLAP_CORE_EVENT_SPACE_ID;
    note.header.type      = type;
    note.header.flags     = 0; //CLAP_EVENT_DONT_RECORD;// | CLAP_EVENT_IS_LIVE;
    note.port_index       = port;
    note.channel          = channel;
    note.key              = key;
    note.velocity = velocity;
    clap_event_header_t* header = (clap_event_header_t*)&note;
    out_events->try_push(out_events,header);
  }

//------------------------------
public:
//------------------------------

  void setDefaultParameterValues() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      MIP_Parameter* param = MParameters[i];
      double value = param->getDefaultValue();
      //MIP_Print("%i. %f\n",i,value);
      param->setValue(value);
    }
  }

  //----------

  void setEditorParameterValues(bool ARedraw=false) {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      MIP_Parameter* param = MParameters[i];
      double value = param->getValue();
      MEditor->setParameterValue(i,value,ARedraw);
    }
  }

};

//----------------------------------------------------------------------
#endif
