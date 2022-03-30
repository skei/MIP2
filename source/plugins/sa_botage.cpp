#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"
#include "audio/mip_voice_manager.h"
#include "audio/mip_audio_math.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

#include "sa_botage/sa_botage_logo.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define NUM_PARAMS          4
//#define NUM_AUDIO_INPUTS    2
//#define NUM_AUDIO_OUTPUTS   2
//#define NUM_NOTE_INPUTS     2
//#define NUM_NOTE_OUTPUTS    2
//#define NUM_QUICK_CONTROLS  2
//#define SUPPORTED_DIALECTS  CLAP_NOTE_DIALECT_CLAP

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const char* myFeatures1[] = {
  "audio_effect",
  nullptr
};

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "torhelgeskei/sa_botage/v0.0.13",
  "sa_botage",
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.13",
  "glitch, stutter, fsu",
  myFeatures1
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor
: public MIP_Editor {

//------------------------------
private:
//------------------------------

  MIP_Bitmap*   MLogoBitmap  = nullptr;
  MIP_Surface*  MLogoSurface = nullptr;

//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded)
  : MIP_Editor(AListener,APlugin,AWidth,AHeight,AEmbedded) {

    MIP_Window* window = getWindow();

//    // can resize
//    setCanResize();
//
//    // menu
//
//    MIP_MenuWidget* menu1 = new MIP_MenuWidget( MIP_FRect(100,100) );
//    menu1->appendMenuItem("first");
//    menu1->appendMenuItem("item2");
//    menu1->appendMenuItem("item3");
//    menu1->appendMenuItem("4");
//    menu1->appendMenuItem("five");
//    menu1->setItemSize(90,20);
//    menu1->setItemLayout(1,5);
//    menu1->setMenuMirror(true,false);

    // panel

    MIP_PanelWidget* MEditorPanel = new MIP_PanelWidget(MIP_FRect());
    MEditorPanel->setBackgroundColor(0.6);
    MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    //MEditorPanel->layout.innerBorder = MIP_FRect(10,10,10,10);
    //MEditorPanel->layout.spacing = 5;

    // logo
    MLogoBitmap = new MIP_Bitmap(sa_botage_logo,sa_botage_logo_size);
    MLogoBitmap->premultAlpha(MIP_Color(0.6));

    MLogoSurface = new MIP_Surface(window,sa_botage_logo_width,sa_botage_logo_height);;
    MIP_Painter* painter = new MIP_Painter(MLogoSurface);
    painter->uploadBitmap(0,0,MLogoBitmap);
    painter->flush();
    delete painter;

    MIP_ImageWidget* logo = new MIP_ImageWidget(MIP_FRect(sa_botage_logo_width,sa_botage_logo_height),MLogoSurface);
    MEditorPanel->appendWidget(logo);

//    // knob 1
//
//    MIP_KnobWidget* knob1 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
//    MEditorPanel->appendWidget(knob1);
//    knob1->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(knob1,0);
//
//    // knob 2
//
//    MIP_KnobWidget* knob2 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
//    MEditorPanel->appendWidget(knob2);
//    knob2->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(knob2,1);
//
//    // knob 3
//
//    MIP_KnobWidget* knob3 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
//    MEditorPanel->appendWidget(knob3);
//    knob3->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(knob3,2);
//
//    // knob 4
//
//    MIP_KnobWidget* knob4 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
//    MEditorPanel->appendWidget(knob4);
//    knob4->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
//    connect(knob4,3);
//
//    // window sizer
//
//    MIP_SizerWidget* MSizer = new MIP_SizerWidget(MIP_FRect( 15,15),MIP_SIZER_WINDOW);
//    MSizer->layout.alignment = MIP_WIDGET_ALIGN_BOTTOM_RIGHT;
//    MEditorPanel->appendWidget(MSizer);
//
//    // button row
//
//    MIP_ButtonRowWidget* button_row = new MIP_ButtonRowWidget(MIP_FRect(230,20), 6, buttonrow_text, MIP_BUTTON_ROW_MULTI );
//    MEditorPanel->appendWidget(button_row);
//    button_row->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//
//    // slider
//
//    MIP_SliderWidget* slider = new MIP_SliderWidget(MIP_FRect(110,20), "Slider", 0.5 );
//    MEditorPanel->appendWidget(slider);
//    slider->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
//
//    // selector
//
//    MIP_SelectorWidget* selector = new MIP_SelectorWidget(MIP_FRect(110,20));
//    MEditorPanel->appendWidget(selector);
//    selector->setMenu(menu1);
//    selector->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
//
//    // append menu last (so it draws on top of everything else)
//
//    MEditorPanel->appendWidget(menu1);

    //

    window->appendWidget(MEditorPanel);

//    MSizer->setTarget(win);

  }

  //----------

  virtual ~myEditor() {
    delete MLogoBitmap;
    delete MLogoSurface;
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

  // todo: -> descriptor..
  uint32_t MDefaultEditorWidth  = 400;
  uint32_t MDefaultEditorHeight = 400;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public:
//------------------------------

  //void handle_parameter_event(const clap_event_param_value_t* param_value) final {
  //  MIP_Plugin::handle_parameter_event(param_value);
  //}

  //----------

  //void handle_modulation_event(const clap_event_param_mod_t* param_mod) final {
  //  MIP_Plugin::handle_modulation_event(param_mod);
  //}

  //----------

  //void handle_events_input(const clap_input_events_t* in_events, const clap_output_events_t* out_events) override {
  //  //MIP_Plugin::handle_events_input(in_events,out_events);
  //  uint32_t num_events = in_events->size(in_events);
  //  for (uint32_t i=0; i<num_events; i++) {
  //    const clap_event_header_t* header = in_events->get(in_events,i);
  //    if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
  //      switch (header->type) {
  //
  //        case CLAP_EVENT_NOTE_ON:
  //          MVoices.on_note_on((clap_event_note_t*)header);
  //          break;
  //
  //        case CLAP_EVENT_NOTE_OFF:
  //          MVoices.on_note_off((clap_event_note_t*)header);
  //          break;
  //
  //        case CLAP_EVENT_NOTE_END:
  //          MVoices.on_note_end((clap_event_note_t*)header);
  //          break;
  //
  //        case CLAP_EVENT_NOTE_CHOKE:
  //          MVoices.on_note_choke((clap_event_note_t*)header);
  //          break;
  //
  //        case CLAP_EVENT_PARAM_VALUE:
  //          handle_parameter_event((clap_event_param_value_t*)header);
  //          MVoices.on_parameter_value((clap_event_param_value_t*)header);
  //          break;
  //
  //        case CLAP_EVENT_PARAM_MOD:
  //          handle_modulation_event((clap_event_param_mod_t*)header);
  //          MVoices.on_parameter_modulation((clap_event_param_mod_t*)header);
  //          break;
  //
  //        //case CLAP_EVENT_MIDI:
  //        //case CLAP_EVENT_MIDI2:
  //        //case CLAP_EVENT_MIDI_SYSEX:
  //        //case CLAP_EVENT_TRANSPORT:
  //
  //        case CLAP_EVENT_NOTE_EXPRESSION:
  //          {
  //            clap_event_note_expression_t* event = (clap_event_note_expression_t*)header;
  //            switch (event->expression_id) {
  //              case CLAP_NOTE_EXPRESSION_VOLUME:     MVoices.on_note_volume_expression(event);     break;
  //              case CLAP_NOTE_EXPRESSION_PAN:        MVoices.on_note_pan_expression(event);        break;
  //              case CLAP_NOTE_EXPRESSION_TUNING:     MVoices.on_note_tuning_expression(event);     break;
  //              case CLAP_NOTE_EXPRESSION_VIBRATO:    MVoices.on_note_vibrato_expression(event);    break;
  //              case CLAP_NOTE_EXPRESSION_EXPRESSION: MVoices.on_note_expression_expression(event); break;
  //              case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MVoices.on_note_brightness_expression(event); break;
  //              case CLAP_NOTE_EXPRESSION_PRESSURE:   MVoices.on_note_pressure_expression(event);   break;
  //            }
  //          } // note expression
  //
  //        //default:
  //        //  break;
  //
  //      } // switch type
  //    } // CLAP_CORE_EVENT_SPACE_ID
  //  } // for events
  //}

//------------------------------
public:
//------------------------------

  //void handle_process(const clap_process_t *process) final {
  //  float** outputs = process->audio_outputs[0].data32;
  //  uint32_t length = process->frames_count;
  //  MIP_ClearStereoBuffer(outputs,length);
  //  MVoices.process(process);
  //  // post-process?
  //}

  //----------

  //void handle_events_output(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
  //  //float v0 = MParameterValues[0] + MParameterModulations[0];
  //  //v0 = MIP_Clamp(v0,0,1);
  //  //send_param_mod_event(0,v0,out_events);
  //  MIP_Plugin::handle_events_output(in_events,out_events);
  //}

//------------------------------
public: // plugin
//------------------------------

  //bool init() final {
  //  for (uint32_t i=0; i<NUM_PARAMS; i++) {
  //    appendParameter(new MIP_Parameter( &myParameters[i] ));
  //  }
  //  // do the same thing with the following too..
  //  setupAudioInputs(myAudioInputs,NUM_AUDIO_INPUTS);
  //  setupAudioOutputs(myAudioOutputs,NUM_AUDIO_OUTPUTS);
  //  setupNoteInputs(myNoteInputs,NUM_NOTE_INPUTS);
  //  setupNoteOutputs(myNoteOutputs,NUM_NOTE_OUTPUTS);
  //  setupQuickControls(myQuickControls,NUM_QUICK_CONTROLS);
  //  return MIP_Plugin::init();
  //}

  //----------

  //bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
  //  MVoices.prepare(sample_rate);
  //  return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  //}

  //----------

  const void* get_extension(const char *id) final {
    //const void* ext = MIP_Plugin::get_extension(id);
    //if (!ext) {
    //  if (strcmp(id,CLAP_EXT_GUI) == 0) return &MGui;
    //}
    //return ext;
    if (strcmp(id,CLAP_EXT_GUI) == 0) return &MGui;
    else return MIP_Plugin::get_extension(id);

  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) final {
    if (strcmp(api,CLAP_WINDOW_API_X11) != 0) {
      MIP_Print("not x11\n");
      return false;
    }
    if (is_floating) {
      MIP_Print("floating\n");
    return false;
    }
    MEditorIsOpen = false;
    MEditor = new myEditor(this,this,MDefaultEditorWidth,MDefaultEditorHeight,true);
    return (MEditor);
  }

};

//----------------------------------------------------------------------

//#undef NUM_PARAMS
//#undef NUM_AUDIO_INPUTS
//#undef NUM_AUDIO_OUTPUTS
//#undef NUM_NOTE_INPUTS
//#undef NUM_NOTE_OUTPUTS
//#undef NUM_QUICK_CONTROLS
//#undef ALL_DIALECTS

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendPlugin(&myDescriptor);
}

//----------

//void MIP_Unregister(MIP_ClapRegistry* ARegistry) {
//}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  MIP_PRINT;
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
