//
//#define MIP_GUI_XCB
//#define MIP_PAINTER_CAIRO
//
//#define MIP_DEBUG_PRINT_SOCKET
////nc -U -l -k /tmp/mip.socket
//
////----------
//
//#include "mip.h"
//#include "audio/mip_voice_manager.h"
//#include "audio/mip_audio_math.h"
//#include "audio/filters/mip_svf_filter.h"
//#include "plugin/mip_plugin.h"
//#include "plugin/mip_editor.h"
//#include "gui/mip_widgets.h"
//
//#define STB_HEXWAVE_IMPLEMENTATION
//#include "extern/stb/stb_hexwave.h"
//
////----------------------------------------------------------------------
////
////
////
////----------------------------------------------------------------------
//
//#define NUM_PARAMS 6
//
////#define SUPPORTED_DIALECTS (CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2)
////#define SUPPORTED_DIALECTS  CLAP_NOTE_DIALECT_CLAP
//
////----------------------------------------------------------------------
////
//// descriptor
////
////----------------------------------------------------------------------
//
//const char* myFeatures[] = {
//  "instrument",
//  nullptr
//};
//
////----------
//
//const clap_plugin_descriptor_t myDescriptor = {
//  CLAP_VERSION,
//  "skei.audio/sa_hexwave",
//  "sa_hexwave",
//  "skei.audio",
//  "https://torhelgeskei.com",
//  "",
//  "",
//  "0.0.1",
//  "simple mip2 test synth",
//  myFeatures
//};
//
////----------------------------------------------------------------------
////
//// voice
////
////----------------------------------------------------------------------
//
//class myVoice {
//
////------------------------------
//private:
////------------------------------
//
//  MIP_VoiceContext* context = nullptr;
//
//  //MIP_SvfFilter     filter  = {};
//  //float   ffreq   = 0.5;
//  //float   fres    = 0.5;
//  float   hz      = 0.0;  // note hz
//  //float   ph      = 0.0;  // phase
//  //float   phadd   = 0.0;  // phase add
//
//  int32_t   reflect     = 0;
//  float     peak_time   = 0;
//  float     half_height = 0;
//  float     zero_wait   = 0;
//
//  HexWave   hex;
//
//  int32_t   _key    = -1;
//  float     _onvel  = 0.0;
//  float     _offvel = 0.0;
//  float     _vol    = 0.0;
//  float     _pan    = 0.0;
//  float     _vibr   = 0.0;
//  float     _expr   = 0.0;
//  float     _bright = 0.0;
//  float     _press  = 0.0;
//
//
////------------------------------
//public:
////------------------------------
//
//  void prepare(MIP_VoiceContext* AContext) {
//    context = AContext;
//    hexwave_create(&hex,reflect,peak_time,(half_height*2)-1,zero_wait);
//
//
//  }
//
//  //----------
//
//  uint32_t note_on(int32_t key, float velocity) {
//    _key = key;
//    _onvel = velocity;
//    //ph = 0.0;
//    hz = MIP_NoteToHz(key);
//    //phadd = hz / context->samplerate;
//    return MIP_VOICE_PLAYING;
//  }
//
//  //----------
//
//  uint32_t note_off(float velocity) {
//    _offvel = velocity;
//    return MIP_VOICE_FINISHED;
//  }
//
//  //----------
//
//  void note_choke() {
//  }
//
//  //----------
//
//  void note_end() {
//  }
//
//  //----------
//
//  void tuning(float amount) {
//    hz = MIP_NoteToHz(_key + amount);
//    //phadd = hz / context->samplerate;
//  }
//
//  //----------
//
//  void volume(float amount) {
//    _vol = amount;
//  }
//
//  //----------
//
//  void pan(float amount) {
//    _pan = amount;
//  }
//
//  //----------
//
//  void vibrato(float amount) {
//    _vibr = amount;
//  }
//
//  //----------
//
//  void expression(float amount) {
//    _expr = amount;
//  }
//
//  //----------
//
//  void brightness(float amount) {
//    _bright = amount;
//  }
//
//  //----------
//
//  void pressure(float amount) {
//    _press = amount;
//  }
//
//  //----------
//
//  void parameter(uint32_t index, float value) {
//    switch (index) {
//      case 2: reflect = value; break;
//      case 3: peak_time = value; break;
//      case 4: half_height = value; break;
//      case 5: zero_wait = value; break;
//    }
//    hexwave_change(&hex,reflect,peak_time,(half_height*2)-1,zero_wait);
//  }
//
//  //----------
//
//  void modulation(uint32_t index, float value) {
//  }
//
//  //----------
//
//  uint32_t process(uint32_t AState) {
//    float* output0 = context->process->audio_outputs[0].data32[0];
//    float* output1 = context->process->audio_outputs[0].data32[1];
//    uint32_t length  = context->process->frames_count;
//
//    float freq = hz / context->samplerate;
//    hexwave_generate_samples(output0,length,&hex,freq);
//
//    // this glitches, because each buffer have a different scale/volume
//
//    float v = _onvel + _press;
//    v = MIP_Clamp(v,0,1);
//    MIP_ScaleMonoBuffer(output0,v,length);
//
//    MIP_AddMonoBuffer(output1,output0,length);
//
//    //for (uint32_t i=0; i<length; i++) {
//    //  filter.setMode(MIP_SVF_LP);
//    //  filter.setFreq(ffreq * ffreq);
//    //  filter.setBW(1.0 - fres);
//    //  float out = filter.process(ph);
//    //  ph += phadd;
//    //  ph = MIP_Fract(ph);
//    //  float v = _onvel + _press;
//    //  v = MIP_Clamp(v,0,1);
//    //  *output0++ += out * v;
//    //  *output1++ += out * v;
//    //}
//    return MIP_VOICE_PLAYING;
//  }
//
//};
//
////----------------------------------------------------------------------
////
//// editor
////
////----------------------------------------------------------------------
//
//class myEditor
//: public MIP_Editor {
//
////------------------------------
//private:
////------------------------------
//
//
//
////------------------------------
//public:
////------------------------------
//
//  myEditor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded)
//  : MIP_Editor(AListener,APlugin,AWidth,AHeight,AEmbedded) {
//    MIP_Window* window = getWindow();
//    // sa_header
//    // panel
//    MIP_PanelWidget* MEditorPanel = new MIP_PanelWidget(MIP_FRect());
//    MEditorPanel->setBackgroundColor(0.6);
//    MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
//    MEditorPanel->layout.innerBorder = MIP_FRect(10,10,10,10);
//    MEditorPanel->layout.spacing = 5;
//    // vol
//    MIP_Knob2Widget* vol_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Vol");
//    MEditorPanel->appendWidget(vol_knob);
//    vol_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(vol_knob,0);
//    // pan
//    MIP_Knob2Widget* pan_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Pan");
//    MEditorPanel->appendWidget(pan_knob);
//    pan_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(pan_knob,1);
//
//    // reflect
//    MIP_Knob2Widget* reflect_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Reflect");
//    MEditorPanel->appendWidget(reflect_knob);
//    reflect_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(reflect_knob,2);
//    // peak_time
//    MIP_Knob2Widget* peak_time_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Peak");
//    MEditorPanel->appendWidget(peak_time_knob);
//    peak_time_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(peak_time_knob,3);
//    // half_height
//    MIP_Knob2Widget* half_height_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Half H");
//    MEditorPanel->appendWidget(half_height_knob);
//    half_height_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(half_height_knob,4);
//    // zero_wwait
//    MIP_Knob2Widget* zero_wait_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Z Wait");
//    MEditorPanel->appendWidget(zero_wait_knob);
//    zero_wait_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(zero_wait_knob,5);
//    // test
//    //MIP_Knob2Widget* test_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Test");
//    //MEditorPanel->appendWidget(test_knob);
//    //test_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    //
//    window->appendWidget(MEditorPanel);
//  }
//
//  //----------
//
//  virtual ~myEditor() {
//  }
//
//};
//
////----------------------------------------------------------------------
////
//// plugin
////
////----------------------------------------------------------------------
//
//class myPlugin
//: public MIP_Plugin {
//
////------------------------------
//private:
////------------------------------
//
//// int reflect
//// float peak_time
//// float half_height
//// float zero_wait
//
//  clap_param_info_t myParameters[NUM_PARAMS] = {
//    { 0, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Vol",         "",  0.0, 1.0, 0.0 },
//    { 1, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Pan",         "",  0.0, 1.0, 0.5 },
//    { 2, CLAP_PARAM_IS_AUTOMATABLE |
//         CLAP_PARAM_IS_STEPPED,     nullptr, "Reflect",     "",  0.0, 1.0, 0.0 },
//    { 3, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Peak Time",   "",  0.0, 1.0, 0.0 },
//    { 4, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Half Height", "",  0.0, 1.0, 0.0 },
//    { 5, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Zero Wait",   "",  0.0, 1.0, 0.0 }
//  };
//
//  MIP_VoiceManager<myVoice,16>  MVoices = {};
//
//  uint32_t MDefaultEditorWidth  = 400;
//  uint32_t MDefaultEditorHeight = 200;
//
//  float user_buffer[1024] = {0};
//
////------------------------------
//public:
////------------------------------
//
//  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
//  : MIP_Plugin(ADescriptor,AHost) {
//  }
//
//  //----------
//
//  virtual ~myPlugin() {
//  }
//
////------------------------------
//public: // plugin
////------------------------------
//
////         width: size of BLEP, from 4..64, larger is slower & more memory but less aliasing
////    oversample: 2+, number of subsample positions, larger uses more memory but less noise
////   user_buffer: optional, if provided the library will perform no allocations.
////                16*width*(oversample+1) bytes, must stay allocated as long as library is used
////                technically it only needs:   8*( width * (oversample  + 1))
////                                           + 8*((width *  oversample) + 1)  bytes
////
//// width can be larger than 64 if you define STB_HEXWAVE_MAX_BLEP_LENGTH to a larger value
//
//  bool init() final {
//    setupParameters(myParameters,NUM_PARAMS);
//    hexwave_init(16,2,user_buffer);
//    return MIP_Plugin::init();
//  }
//
//  //----------
//
////       user_buffer: pass in same parameter as passed to hexwave_init
//
//  void destroy() final {
//    hexwave_shutdown(user_buffer);
//    MIP_Plugin::init();
//  }
//
//  //----------
//
//  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
//    MVoices.prepareVoices(sample_rate);
//    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
//  }
//
//  //----------
//
//  const void* get_extension(const char *id) final {
//    if (strcmp(id,CLAP_EXT_GUI) == 0) return &MGui;
//    return MIP_Plugin::get_extension(id);
//  }
//
////------------------------------
//public: // gui
////------------------------------
//
//  bool gui_create(const char *api, bool is_floating) final {
//    if (strcmp(api,CLAP_WINDOW_API_X11) != 0) {
//      MIP_Print("not x11\n");
//      return false;
//    }
//    if (is_floating) {
//      MIP_Print("floating\n");
//    return false;
//    }
//    MEditorIsOpen = false;
//    MEditor = new myEditor(this,this,MDefaultEditorWidth,MDefaultEditorHeight,true);
//    return (MEditor);
//  }
//
////------------------------------
//public:
////------------------------------
//
////  void handle_input_events(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
////    MIP_Plugin::handle_input_events(in_events,out_events);
////    uint32_t num_events = in_events->size(in_events);
////    for (uint32_t i=0; i<num_events; i++) {
////      const clap_event_header_t* header = in_events->get(in_events,i);
////      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
////        MVoices.on_event(header);
////      }
////    }
////  }
//
//  //----------
//
//  void handle_note_on_event(clap_event_note_t* event) final {
//    MVoices.handleNoteOnEvent(event);
//  }
//
//  void handle_note_off_event(clap_event_note_t* event) final {
//    MVoices.handleNoteOffEvent(event);
//  }
//
//  //void handle_note_end_event(clap_event_note_t* event) final {
//  //  MVoices.handleNoteEndEvent(event);
//  //}
//
//  void handle_note_choke_event(clap_event_note_t* event) final {
//    MVoices.handleNoteChokeEvent(event);
//  }
//
//  void handle_note_expression_event(clap_event_note_expression_t* event) final {
//    //switch (event->expression_id) {
//    //  case CLAP_NOTE_EXPRESSION_VOLUME:     MVoices.on_note_volume_expression(event);     break;
//    //  case CLAP_NOTE_EXPRESSION_PAN:        MVoices.on_note_pan_expression(event);        break;
//    //  case CLAP_NOTE_EXPRESSION_TUNING:     MVoices.on_note_tuning_expression(event);     break;
//    //  case CLAP_NOTE_EXPRESSION_VIBRATO:    MVoices.on_note_vibrato_expression(event);    break;
//    //  case CLAP_NOTE_EXPRESSION_EXPRESSION: MVoices.on_note_expression_expression(event); break;
//    //  case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MVoices.on_note_brightness_expression(event); break;
//    //  case CLAP_NOTE_EXPRESSION_PRESSURE:   MVoices.on_note_pressure_expression(event);   break;
//    //}
//    MVoices.handleNoteExpressionEvent(event);
//  }
//
//  void handle_midi_event(clap_event_midi_t* event) final {
//    MVoices.handleMidiEvent(event);
//  }
//
//  void handle_midi2_event(clap_event_midi2_t* event) final {
//    MVoices.handleMidi2Event(event);
//  }
//
//  //void handle_midi_sysex_event(clap_event_midi_sysex_t* event) final {
//  //  MVoices.handleMidiSysexEvent(event);
//  //}
//
//  void handle_transport_event(clap_event_transport_t* event) final {
//    //MVoices.handleTransportEvent(event);
//    }
//
//  void handle_parameter_event(clap_event_param_value_t* event) final {
//    MVoices.handleParamValueEvent(event);
//  }
//
//  void handle_modulation_event(clap_event_param_mod_t* event) final {
//    MVoices.handleParamModEvent(event);
//  }
//
//  //----------
//
//  void handle_process(const clap_process_t *process) final {
//
//    // send freq/res to voices..
//    // hmmm..
//    // todo: fix this..
////    MVoices.handle_master_param(2,MParameterValues[2]);
////    MVoices.handle_master_param(3,MParameterValues[3]);
////    MVoices.handle_master_param(4,MParameterValues[4]);
////    MVoices.handle_master_param(5,MParameterValues[5]);
//
//    float** outputs = process->audio_outputs[0].data32;
//    uint32_t length = process->frames_count;
//    MIP_ClearStereoBuffer(outputs,length);
//    MVoices.processVoices(0,length,process);
//
//    MIP_CopyMonoBuffer(outputs[0],outputs[1],length);
//
//    float v = MParameterValues[0];  // vol
//    float p = MParameterValues[1];  // pan
//    float l = v * (1.0 - p);        //
//    float r = v * (      p);
//    MIP_ScaleStereoBuffer(outputs,l,r,length);
//  }
//
//};
//
////----------------------------------------------------------------------
//
//#undef NUM_PARAMS
//#undef ALL_DIALECTS
//
////----------------------------------------------------------------------
////
//// register
////
////----------------------------------------------------------------------
//
//void MIP_Register(MIP_ClapRegistry* ARegistry) {
//  ARegistry->appendPlugin(&myDescriptor);
//}
//
////----------
//
//MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
//  switch (AIndex) {
//    case 0: return new myPlugin(ADescriptor,AHost);
//  }
//  return nullptr;
//}
