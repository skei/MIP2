
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
   .id            = "me/test_editor4/0",
   .name          = "test_editor4",
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
// plugin
//
//----------------------------------------------------------------------

class test_editor4_plugin
: public MIP_Plugin
, public MIP_MenuListener {

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

  const char* button_txt[5] = {
    "1", "2", "3", "4", "v"
  };

//------------------------------
public:
//------------------------------

  test_editor4_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    //MIP_PRINT;
    MEditorWidth  = 800;
    MEditorHeight = 600;
  }

  //----------

  //virtual ~test_editor4_plugin() {
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
    //MEditor = new test_editor4_editor(this,MEditorWidth,MEditorHeight);
    bool result = MIP_Plugin::gui_create(api,is_floating);
    if (result) {

      MEditor->setWindowFillBackground(false);
      MEditor->setWidgetName("MEditor");

      // menu

      MIP_MenuWidget* menu1 = new MIP_MenuWidget( MIP_DRect( 170, 150, 160,100), this );
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,160,20), "item1" ));
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,20,160,20), "item2" ));
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,40,160,20), "item3" ));
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,60,160,20), "item4" ));
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,80,160,20), "item5" ));

      //----- panels -----

      MIP_PanelWidget* content;

      MIP_ScrollBoxWidget* left_scrollbox = new MIP_ScrollBoxWidget(MIP_DRect(200),true,true);
      left_scrollbox->setWidgetName("left_scrollbox");
      left_scrollbox->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
      MEditor->appendChildWidget(left_scrollbox);
        content = left_scrollbox->getContentWidget();
        content->Layout.border = MIP_DRect(10,10,10,10);
        content->Layout.spacing = MIP_DPoint(5,5);

      MIP_SizerWidget* left_sizer = new MIP_SizerWidget(5,MIP_SIZER_LEFT,left_scrollbox);
      left_sizer->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
      MEditor->appendChildWidget(left_sizer);

//      MIP_PanelWidget* bottom_panel = new MIP_PanelWidget(150);
//      bottom_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
//      MEditor->appendChildWidget(bottom_panel);

      MIP_ScrollBoxWidget* bottom_scrollbox = new MIP_ScrollBoxWidget(MIP_DRect(150),true,true);
      bottom_scrollbox->setWidgetName("bottom_scrollbox");
      bottom_scrollbox->Layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      MEditor->appendChildWidget(bottom_scrollbox);
        content = bottom_scrollbox->getContentWidget();
        content->Layout.border = MIP_DRect(10,10,10,10);
        content->Layout.spacing = MIP_DPoint(5,5);

      MIP_SizerWidget* bottom_sizer = new MIP_SizerWidget(5,MIP_SIZER_BOTTOM,bottom_scrollbox);
      bottom_sizer->Layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      MEditor->appendChildWidget(bottom_sizer);

//      MIP_PanelWidget* right_panel = new MIP_PanelWidget(150);
//      right_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_RIGHT;
//      MEditor->appendChildWidget(right_panel);

//      MIP_SizerWidget* right_sizer = new MIP_SizerWidget(5,MIP_SIZER_RIGHT,right_panel);
//      right_sizer->Layout.alignment = MIP_WIDGET_ALIGN_FILL_RIGHT;
//      MEditor->appendChildWidget(right_sizer);

//      MIP_PanelWidget* top_panel = new MIP_PanelWidget(100);
//      top_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//      MEditor->appendChildWidget(top_panel);

//      MIP_SizerWidget* top_sizer = new MIP_SizerWidget(5,MIP_SIZER_TOP,top_panel);
//      top_sizer->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//      MEditor->appendChildWidget(top_sizer);

      MIP_PanelWidget* center_panel = new MIP_PanelWidget(0);
      center_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      MEditor->appendChildWidget(center_panel);

      //----- left panel -----

      // color

      MIP_ColorWidget* color1 = new MIP_ColorWidget(20,MIP_COLOR_RED);
      color1->setWidgetName("color1");
      color1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //left_panel->appendChildWidget(color1);
      left_scrollbox->appendChildWidget(color1);

      // panel1

      MIP_PanelWidget* panel1 = new MIP_PanelWidget(20);
      panel1->setWidgetName("panel1");
      panel1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      panel1->setDrawBorder(true);
      panel1->setBorderColor(0.2);
      panel1->setFillBackground(false);
      panel1->setDrawRoundedCorners(false);
      //left_panel->appendChildWidget(panel1);
      left_scrollbox->appendChildWidget(panel1);

      // panel2

      MIP_PanelWidget* panel2 = new MIP_PanelWidget(20);
      panel2->setWidgetName("panel2");
      panel2->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      panel2->setDrawBorder(true);
      panel2->setBorderColor(0.2);
      panel2->setFillBackground(true);
      panel2->setBackgroundColor(MIP_COLOR_GREEN);
      panel2->setFillGradient(true);
      panel2->setGradientColor1(0.6);
      panel2->setGradientColor2(0.4);
      panel2->setDrawRoundedCorners(true);
      panel2->setCornerRadius(5,5,5,5);
      //left_panel->appendChildWidget(panel2);
      left_scrollbox->appendChildWidget(panel2);

      // panel3

      MIP_PanelWidget* panel3 = new MIP_PanelWidget(20);
      panel3->setWidgetName("panel3");
      panel3->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      panel3->setDrawBorder(true);
      panel3->setBorderColor(0.2);
      panel3->setFillBackground(true);
      panel3->setBackgroundColor(MIP_COLOR_GREEN);
      panel3->setFillGradient(true);
      panel3->setGradientColor2(0.6);
      panel3->setGradientColor1(0.4);
      panel3->setDrawRoundedCorners(true);
      panel3->setCornerRadius(0,10,0,0);
      //left_panel->appendChildWidget(panel3);
      left_scrollbox->appendChildWidget(panel3);

      //

      MIP_TextWidget* text1 = new MIP_TextWidget(MIP_DRect(50),"text");
      text1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //text1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      text1->setTextSize(-1);
      //text1->setTextOffset(MIP_DPoint(0,1));
      left_scrollbox->appendChildWidget(text1);

      MIP_ValueWidget* value1 = new MIP_ValueWidget(MIP_DRect(20),"value",0.3);
      value1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //value1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_scrollbox->appendChildWidget(value1);

      MIP_DragValueWidget* dragvalue1  = new MIP_DragValueWidget(MIP_DRect(20),"drag_value",0.3);
      dragvalue1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //dragvalue1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_scrollbox->appendChildWidget(dragvalue1);

      MIP_SliderWidget* slider1 = new MIP_SliderWidget(MIP_DRect(20),"slider",0.3);
      slider1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //slider1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      slider1->setMinValue(-5);
      slider1->setMaxValue(5);
      slider1->scaleDragSensitivity(10);
      left_scrollbox->appendChildWidget(slider1);

      MIP_ButtonWidget* button1 = new MIP_ButtonWidget(MIP_DRect(20),"on","off",0);
      button1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //button1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      button1->setTextColor(MIP_COLOR_BLACK);
      button1->setTextSize(-1);
      left_scrollbox->appendChildWidget(button1);

      MIP_ButtonWidget* button2 = new MIP_ButtonWidget(MIP_DRect(20),"on","off",0);
      button2->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //button2->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      button2->setTextColor(MIP_COLOR_RED);
      button2->setTextSize(-1);
      button2->setIsToggle(false);
      left_scrollbox->appendChildWidget(button2);

      MIP_MenuItemWidget* menuitem1 = new MIP_MenuItemWidget(MIP_DRect(20),"menu_item");
      menuitem1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //menuitem1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_scrollbox->appendChildWidget(menuitem1);

      MIP_SelectorWidget* selector1 = new MIP_SelectorWidget(MIP_DRect(20),"selector",menu1);
      selector1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //selector1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_scrollbox->appendChildWidget(selector1);

      MIP_ButtonRowWidget* buttonrow1 = new MIP_ButtonRowWidget(MIP_DRect(20),5,button_txt,MIP_BUTTON_ROW_MULTI);
      buttonrow1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //buttonrow1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      buttonrow1->setButtonState(0,true);
      left_scrollbox->appendChildWidget(buttonrow1);

      MIP_ScrollBarWidget* scrollbar1 = new MIP_ScrollBarWidget(MIP_DRect(20));
      scrollbar1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //scrollbar1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_scrollbox->appendChildWidget(scrollbar1);

      MIP_GroupBoxWidget* groupbox1 = new MIP_GroupBoxWidget(MIP_DRect(100),false);
      groupbox1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      //groupbox1->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_SPREAD;
      left_scrollbox->appendChildWidget(groupbox1);

        MIP_ColorWidget* color2 = new MIP_ColorWidget(MIP_DRect(100),MIP_COLOR_RED);
        color2->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
        groupbox1->appendChildWidget(color2);

      //

      MEditor->appendChildWidget(menu1);

    }
    return result;
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
    return new test_editor4_plugin(ADescriptor,AHost);
  }
  return nullptr;
}


