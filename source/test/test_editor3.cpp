
#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif // MIP_EXE

#define MIP_GUI_XCB
#define MIP_PAINTER_NANOVG

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"

#include "gui/widgets/mip_widgets.h"
#include "gui/mip_bitmap.h"

#include "base/system/mip_timer.h"


//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t template_descriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "me/test_editor3/0",
   .name          = "test_editor3",
   .vendor        = "me",
   .url           = "https://my_website.com",
   .manual_url    = "",
   .support_url   = "",
   .version       = "0.0.0",
   .description   = "",
   .features      =  (const char *[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class test_editor3_editor
: public MIP_Editor
, public MIP_MenuListener {

//------------------------------
private:
//------------------------------

  MIP_PanelWidget* left_panel;
  MIP_PanelWidget* top_panel;
  //MIP_PanelWidget* center_panel;
  MIP_ScrollBoxWidget* center_panel;

  MIP_ColorWidget*      background  = nullptr;
  MIP_ColorWidget*      color1      = nullptr;
  MIP_PanelWidget*      panel1      = nullptr;
  MIP_TextWidget*       text1       = nullptr;
  MIP_ValueWidget*      value1      = nullptr;
  MIP_DragValueWidget*  dragvalue1  = nullptr;
  MIP_SliderWidget*     slider1     = nullptr;
  MIP_ButtonWidget*     button1     = nullptr;
  MIP_ButtonWidget*     button2     = nullptr;
  MIP_MenuItemWidget*   menuitem1   = nullptr;
  MIP_SelectorWidget*   selector1   = nullptr;
  MIP_ButtonRowWidget*  buttonrow1  = nullptr;
  MIP_ScrollBarWidget*  scrollbar1  = nullptr;
  MIP_GroupBoxWidget*   groupbox1   = nullptr;

  MIP_MenuWidget*       menu1       = nullptr;
  MIP_KnobWidget*       knob1       = nullptr;
  MIP_Knob2Widget*      knob2       = nullptr;
  //MIP_ImageWidget*      image1      = nullptr;
  MIP_WaveformWidget*   waveform1   = nullptr;
  MIP_GridWidget*       grid1       = nullptr;

  float MWaveform[1024] = {0};
  const char* button_txt[5] = { "1", "2", "3", "4", "5" };

//------------------------------
public:
//------------------------------

  test_editor3_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Editor(AListener,AWidth,AHeight) {
    setup_widgets(AWidth,AHeight);
    setWindowFillBackground(false);
  }

  //----------

  virtual ~test_editor3_editor() {
    //MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  void setup_widgets(uint32_t AWidth, uint32_t AHeight) {

    for (uint32_t i=0; i<1024; i++) MWaveform[i] = MIP_RandomRange(-1,1);

    // background
    background = new MIP_ColorWidget( MIP_DRect( 0,0,500,400), MIP_COLOR_GRAY );
    appendChildWidget(background);
    background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

    //background->Layout.scale = 0.5;

    menu1 = new MIP_MenuWidget( MIP_DRect( 170, 150, 160,100), this );
    menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,160,20), "item1" ));
    menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,20,160,20), "item2" ));
    menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,40,160,20), "item3" ));
    menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,60,160,20), "item4" ));
    menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,80,160,20), "item5" ));

    //

    left_panel = new MIP_PanelWidget( MIP_DRect(0.25) );
    left_panel->Layout.sizeModeX = MIP_WIDGET_SIZE_MODE_RATIO;
    left_panel->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_RATIO;
    left_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
    left_panel->Layout.border = MIP_DRect(10,10,10,10);
    left_panel->Layout.spacing = MIP_DPoint(5,5);
    //left_panel->Layout.scale = 0.5;
    background->appendChildWidget(left_panel);

    top_panel = new MIP_PanelWidget( MIP_DRect(0.2) );
    top_panel->Layout.sizeModeX = MIP_WIDGET_SIZE_MODE_RATIO;
    top_panel->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_RATIO;
    top_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    background->appendChildWidget(top_panel);

    //center_panel = new MIP_PanelWidget( MIP_DRect(0) );
    center_panel = new MIP_ScrollBoxWidget( MIP_DRect(0), true, true );
    center_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    center_panel->getContentWidget()->Layout.border = MIP_DRect(10,10,10,10);
    background->appendChildWidget(center_panel);

    // left_panel

      color1 = new MIP_ColorWidget(MIP_DRect(20),MIP_COLOR_RED);
      color1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      color1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_panel->appendChildWidget(color1);

      panel1 = new MIP_PanelWidget(MIP_DRect(20));
      panel1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      panel1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_panel->appendChildWidget(panel1);

      text1 = new MIP_TextWidget(MIP_DRect(50),"text");
      text1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      text1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      text1->setTextSize(-1);
      //text1->setTextOffset(MIP_DPoint(0,1));
      left_panel->appendChildWidget(text1);

      value1 = new MIP_ValueWidget(MIP_DRect(20),"value",0.3);
      value1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      value1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_panel->appendChildWidget(value1);

      dragvalue1  = new MIP_DragValueWidget(MIP_DRect(20),"drag_value",0.3);
      dragvalue1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      dragvalue1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_panel->appendChildWidget(dragvalue1);

      slider1 = new MIP_SliderWidget(MIP_DRect(20),"slider",0.3);
      slider1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      slider1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      slider1->setMinValue(-5);
      slider1->setMaxValue(5);
      slider1->scaleDragSensitivity(10);
      left_panel->appendChildWidget(slider1);

      button1 = new MIP_ButtonWidget(MIP_DRect(20),"on","off",0);
      button1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      button1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      button1->setTextColor(MIP_COLOR_BLACK);
      button1->setTextSize(-1);
      left_panel->appendChildWidget(button1);

      button2 = new MIP_ButtonWidget(MIP_DRect(20),"on","off",0);
      button2->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      button2->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      button2->setTextColor(MIP_COLOR_RED);
      button2->setTextSize(-1);
      button2->setIsToggle(false);
      left_panel->appendChildWidget(button2);

      menuitem1 = new MIP_MenuItemWidget(MIP_DRect(20),"menu_item");
      menuitem1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      menuitem1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_panel->appendChildWidget(menuitem1);

      selector1 = new MIP_SelectorWidget(MIP_DRect(20),"selector",menu1);
      selector1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      selector1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_panel->appendChildWidget(selector1);

      buttonrow1 = new MIP_ButtonRowWidget(MIP_DRect(20),5,button_txt,MIP_BUTTON_ROW_MULTI);
      buttonrow1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      buttonrow1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      buttonrow1->setButtonState(0,true);
      left_panel->appendChildWidget(buttonrow1);

      scrollbar1 = new MIP_ScrollBarWidget(MIP_DRect(20));
      scrollbar1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      scrollbar1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_panel->appendChildWidget(scrollbar1);

      groupbox1 = new MIP_GroupBoxWidget(MIP_DRect(100),false);
      groupbox1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      groupbox1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_panel->appendChildWidget(groupbox1);

        MIP_ColorWidget* color2 = new MIP_ColorWidget(MIP_DRect(100),MIP_COLOR_RED);
        color2->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
        groupbox1->appendChildWidget(color2);


    // top

    waveform1 = new MIP_WaveformWidget(MIP_DRect());
    waveform1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    top_panel->appendChildWidget(waveform1);
    waveform1->setBuffer(MWaveform,1024);

    // center

    knob1 = new MIP_KnobWidget(MIP_DRect(0,0,40,40),"Knob",0.3);
    //waveform1->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
    knob1->setFillBackground(false);
    knob1->setDrawBorder(false);
    center_panel->appendChildWidget(knob1);

    knob2 = new MIP_Knob2Widget(MIP_DRect(0,50,42,75),"Knob",0.3);
    //waveform1->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
    knob2->setFillBackground(true);
    knob2->setFillGradient(true);
    knob2->setDrawBorder(true);
    center_panel->appendChildWidget(knob2);

    //image1      = new MIP_ImageWidget(      MIP_DRect( 170, 10, 320,240), voxelbuffer, 320,240);
    //background->appendChildWidget(image1);

    grid1 = new MIP_GridWidget(MIP_DRect( 0.25, 10, 0.5, 200 ),8,8);
    grid1->Layout.sizeModeX = MIP_WIDGET_SIZE_MODE_RATIO;
    grid1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_PIXELS;
    grid1->Layout.aspectRatio = 1.0;
    grid1->Layout.minSize.h = 100;
    center_panel->appendChildWidget(grid1);

    // menu

    background->appendChildWidget(menu1);

  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class test_editor3_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  enum myParameterEnums {
    PARAM1 = 0,
    PARAM2,
    PARAM3,
    PARAM_COUNT
  };

  const clap_param_info_t myParameters[PARAM_COUNT] = {
    { PARAM1, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param1", "", 0, 1, 0 },
    { PARAM2, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param2", "", -1, 1, 1 },
    { PARAM3, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED, nullptr, "param3", "", -12, 12, 0 }
  };

  const clap_audio_port_info_t myAudioInputPorts[1] = {
    { 0, "audio in 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_audio_port_info_t myAudioOutputPorts[1] = {
    { 0, "audio out 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_note_port_info_t myNoteInputPorts[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note in 1" }
  };

  const clap_note_port_info_t myNoteOutputPorts[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note out 1" }
  };

//------------------------------
public:
//------------------------------

  test_editor3_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    //MIP_PRINT;
    MEditorWidth = 500;
    MEditorHeight = 400;
  }

  //----------

  //virtual ~test_editor3_plugin() {
  //}

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendAudioInputPort( &myAudioInputPorts[0] );
    appendAudioOutputPort(&myAudioOutputPorts[0]);
    appendNoteInputPort(  &myNoteInputPorts[0]  );
    appendNoteOutputPort( &myNoteOutputPorts[0] );
    for (uint32_t i=0; i<PARAM_COUNT; i++) {
      appendParameter( new MIP_Parameter(&myParameters[i]) );
    }
    MIP_Parameter* param4 = new MIP_Parameter(0,"param4","",-1,1,0,CLAP_PARAM_IS_AUTOMATABLE);
    appendParameter(param4);
    return true;
  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) override {
    MEditor = new test_editor3_editor(this,MEditorWidth,MEditorHeight);
    return true;
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/clap/mip_clap_entry.h"
#include "plugin/exe/mip_exe_entry.h"
//#include "plugin/vst2/mip_vst2_entry.h"
//#include "plugin/vst3/mip_vst3_entry.h"

#include "plugin/mip_registry.h"

//----------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&template_descriptor);
};

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new test_editor3_plugin(ADescriptor,AHost);
  }
  return nullptr;
}

