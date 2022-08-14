
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

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t template_descriptor = {
  .clap_version  = CLAP_VERSION,
  .id            = "me/test_layout/0",
  .name          = "test_layout",
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

class test_layout_plugin
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

  test_layout_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MEditorWidth  = 800;
    MEditorHeight = 600;
  }

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
    bool result = MIP_Plugin::gui_create(api,is_floating);
    if (result) {
      MEditor->setWidgetName("MEditor");
      MEditor->setWindowFillBackground(false);

      //----- menu-----

      MIP_MenuWidget* menu1 = new MIP_MenuWidget( MIP_DRect( 170, 150, 160,100), this );
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,160,20), "item1" ));
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,20,160,20), "item2" ));
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,40,160,20), "item3" ));
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,60,160,20), "item4" ));
      menu1->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,80,160,20), "item5" ));

      //----- background -----

      MIP_TextWidget* background = new MIP_TextWidget(0,"background");
      background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      background->setFillBackground(true);
      background->setDrawBorder(false);
      background->setDrawText(false);
      background->setBackgroundColor(0.55);
      MEditor->appendChildWidget(background);

      //----- sa header -----

      MIP_SAHeaderWidget* saheader = new MIP_SAHeaderWidget(60,MEditor);
      saheader->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      saheader->setPluginName("test_layout");
      saheader->setPluginVersion("v0.0.0");
      background->appendChildWidget(saheader);

      //----- left panel -----

      MIP_ScrollBoxWidget* left_scrollbox = new MIP_ScrollBoxWidget(150,true,true);
      left_scrollbox->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
      left_scrollbox->getContentWidget()->Layout.border = MIP_DRect(10,10,10,10);
      left_scrollbox->getContentWidget()->Layout.spacing = MIP_DPoint(5,5);
      background->appendChildWidget(left_scrollbox);

      {

        //--- group box

        MIP_GroupBoxWidget* groupbox1 = new MIP_GroupBoxWidget(100,true);
        groupbox1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        left_scrollbox->appendChildWidget(groupbox1);
        {
          MIP_PanelWidget* groupbox1_panel = new MIP_PanelWidget(100);
          groupbox1_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          groupbox1_panel->setFillBackground(true);
          groupbox1_panel->setDrawBorder(false);
          groupbox1_panel->setBackgroundColor(MIP_COLOR_RED);
          groupbox1->appendChildWidget(groupbox1_panel);
        }

        // color

        MIP_ColorWidget* color1 = new MIP_ColorWidget(20,MIP_COLOR_GREEN);
        color1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        left_scrollbox->appendChildWidget(color1);

        // panel

        MIP_PanelWidget* panel1 = new MIP_PanelWidget(20);
        panel1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        panel1->setFillBackground(true);
        panel1->setFillGradient(false);
        panel1->setDrawBorder(true);
        panel1->setDrawRoundedCorners(false);
        panel1->setCornerRadius(5);
        left_scrollbox->appendChildWidget(panel1);

        // toggle button

        MIP_ButtonWidget* button1 = new MIP_ButtonWidget(20,"On","Off",0);
        button1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        button1->setFillBackground(true);
        button1->setFillGradient(false);
        button1->setDrawBorder(true);
        button1->setDrawRoundedCorners(true);
        button1->setCornerRadius(5);
        button1->setIsToggle(true);
        left_scrollbox->appendChildWidget(button1);

        // switch button

        MIP_ButtonWidget* button2 = new MIP_ButtonWidget(20,"On","Off",0);
        button2->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        button2->setFillBackground(true);
        button2->setFillGradient(false);
        button2->setDrawBorder(true);
        button2->setDrawRoundedCorners(false);
        button2->setCornerRadius(5);
        button2->setIsToggle(false);
        left_scrollbox->appendChildWidget(button2);

        // text

        MIP_TextWidget* text1 = new MIP_TextWidget(20,"text");
        text1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        text1->setFillBackground(true);
        text1->setDrawBorder(false);
        text1->setBackgroundColor(0.55);
        left_scrollbox->appendChildWidget(text1);

        // value

        MIP_ValueWidget* value1 = new MIP_ValueWidget(20,"value",0.0);
        value1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        value1->setFillBackground(true);
        value1->setDrawBorder(false);
        value1->setBackgroundColor(0.55);
        left_scrollbox->appendChildWidget(value1);

        // drag_value

        MIP_DragValueWidget* dragvalue1 = new MIP_DragValueWidget(20,"drag_value",0.0);
        dragvalue1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        dragvalue1->setFillBackground(true);
        dragvalue1->setDrawBorder(false);
        dragvalue1->setBackgroundColor(0.55);
        left_scrollbox->appendChildWidget(dragvalue1);

        // slider

        MIP_SliderWidget* slider1 = new MIP_SliderWidget(20,"slider",0.0);
        slider1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        slider1->setFillBackground(true);
        slider1->setDrawBorder(false);
        slider1->setBackgroundColor(0.55);
        left_scrollbox->appendChildWidget(slider1);

        // scroll bar

        MIP_ScrollBarWidget* scrollbar1 = new MIP_ScrollBarWidget(20);
        scrollbar1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        scrollbar1->setFillBackground(true);
        scrollbar1->setDrawBorder(false);
        scrollbar1->setBackgroundColor(0.55);
        left_scrollbox->appendChildWidget(scrollbar1);

        // button row

        MIP_ButtonRowWidget* buttonrow1 = new MIP_ButtonRowWidget(20,5,button_txt,MIP_BUTTON_ROW_SINGLE);
        buttonrow1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        buttonrow1->setFillBackground(true);
        buttonrow1->setDrawBorder(false);
        buttonrow1->setBackgroundColor(0.55);
        left_scrollbox->appendChildWidget(buttonrow1);

        // menu item

        MIP_MenuItemWidget* menuitem1 = new MIP_MenuItemWidget(20,"menuitem");
        menuitem1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        //menuitem1->setFillBackground(true);
        //menuitem1->setDrawBorder(false);
        //menuitem1->setBackgroundColor(0.55);
        left_scrollbox->appendChildWidget(menuitem1);

        // selector

        MIP_SelectorWidget* selector1 = new MIP_SelectorWidget(20,"selector",menu1);
        selector1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        left_scrollbox->appendChildWidget(selector1);

        // keyboard

        MIP_KeyboardWidget* keyboard1 = new MIP_KeyboardWidget(40);
        keyboard1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        left_scrollbox->appendChildWidget(keyboard1);

      }

      //----- left sizer -----

      MIP_SizerWidget* left_sizer = new MIP_SizerWidget(5,MIP_SIZER_LEFT,left_scrollbox);
      left_sizer->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
      left_sizer->setFillColor(0.4);
      background->appendChildWidget(left_sizer);

      //----- right panel -----

      MIP_TextWidget* right_panel = new MIP_TextWidget(150,"right_panel");
      right_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_RIGHT;
      //right_panel->Layout.minSize.w = 100;
      //right_panel->Layout.maxSize.w = 500;
      right_panel->setFillBackground(false);
      right_panel->setDrawBorder(false);
      right_panel->setDrawText(false);
      right_panel->setBackgroundColor(0.45);
      background->appendChildWidget(right_panel);

      {

        //----- right top panel -----

      MIP_ScrollBoxWidget* right_top_scrollbox = new MIP_ScrollBoxWidget(100,true,true);
      right_top_scrollbox->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      right_top_scrollbox->getContentWidget()->Layout.border = MIP_DRect(10,10,10,10);
      right_top_scrollbox->getContentWidget()->Layout.spacing = MIP_DPoint(5,5);
      right_panel->appendChildWidget(right_top_scrollbox);

//      MIP_TextWidget* right_top_panel = new MIP_TextWidget(100,"right_top_panel");
//      right_top_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//      right_top_panel->Layout.border = MIP_DRect(10,10,10,10);
//      right_top_panel->Layout.spacing = MIP_DPoint(5,5);
//      right_top_panel->setFillBackground(false);
//      right_top_panel->setDrawBorder(false);
//      right_top_panel->setDrawText(true);
//      right_top_panel->setBackgroundColor(0.45);
//      right_panel->appendChildWidget(right_top_panel);

        {

          for (uint32_t i=0; i<32; i++) {
            MIP_ButtonWidget* button = new MIP_ButtonWidget(15,"","",0);
            button->Layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
            button->setDrawRoundedCorners(true);
            button->setFillBackground(true);
            float r = (double)i / 31.0;
            float g = (double)(31-i) / 31.0;
            float b = 0.0;
            button->setBackgroundColor(MIP_Color(r,g,b));
            right_top_scrollbox->appendChildWidget(button);
          }

          MIP_ColorWidget* color = new MIP_ColorWidget(5,MIP_COLOR_BLACK);
          color->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
          right_top_scrollbox->appendChildWidget(color);

          for (uint32_t i=0; i<32; i++) {
            MIP_ButtonWidget* button = new MIP_ButtonWidget(15,"","",0);
            button->Layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
            button->setDrawRoundedCorners(true);
            button->setFillBackground(true);
            float r = 0.0;
            float g = (double)i / 31.0;
            float b = (double)(31-i) / 31.0;
            button->setBackgroundColor(MIP_Color(r,g,b));
            right_top_scrollbox->appendChildWidget(button);
          }



        }

        //----- right top sizer -----

        MIP_SizerWidget* right_top_sizer = new MIP_SizerWidget(5,MIP_SIZER_TOP,right_top_scrollbox);
        right_top_sizer->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        right_top_sizer->setFillColor(0.4);
        right_panel->appendChildWidget(right_top_sizer);

        //----- right bottom panel -----

        MIP_TextWidget* right_bottom_panel = new MIP_TextWidget(0,"right_bottom_panel");
        right_bottom_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
        right_bottom_panel->setFillBackground(false);
        right_bottom_panel->setDrawBorder(false);
        right_bottom_panel->setDrawText(true);
        right_bottom_panel->setBackgroundColor(0.45);
        right_panel->appendChildWidget(right_bottom_panel);

        {
        }

      }

      //----- right sizer -----

      MIP_SizerWidget* right_sizer = new MIP_SizerWidget(5,MIP_SIZER_RIGHT,right_panel);
      right_sizer->Layout.alignment = MIP_WIDGET_ALIGN_FILL_RIGHT;
      right_sizer->setFillColor(0.4);
      background->appendChildWidget(right_sizer);

      //----- center bottom panel -----

      MIP_TextWidget* center_bottom_panel = new MIP_TextWidget(100,"center_bottom_panel");
      center_bottom_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      center_bottom_panel->setFillBackground(false);
      center_bottom_panel->setDrawBorder(false);
      center_bottom_panel->setDrawText(false);
      center_bottom_panel->setBackgroundColor(0.45);
      background->appendChildWidget(center_bottom_panel);

      {
        MIP_PanelWidget* aspect_rect1 = new MIP_PanelWidget(0);
        aspect_rect1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
        aspect_rect1->Layout.border = MIP_DRect(10,10,10,10);
        aspect_rect1->Layout.spacing = MIP_DPoint(5,5);
        aspect_rect1->Layout.aspectRatio = 4.0 / 3.0;
        aspect_rect1->setDrawBorder(false);
        aspect_rect1->setFillBackground(false);
        //aspect_rect1->setBackgroundColor(MIP_COLOR_BLUE2);
        center_bottom_panel->appendChildWidget(aspect_rect1);

        MIP_Knob2Widget* knob1 = new MIP_Knob2Widget(0.5,"knob",0.5);
        knob1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
        knob1->Layout.sizeModeX = MIP_WIDGET_SIZE_PARENT_RATIO;
        knob1->Layout.aspectRatio = 2.0 / 3.0;
        aspect_rect1->appendChildWidget(knob1);

        MIP_Knob2Widget* knob2 = new MIP_Knob2Widget(0.5,"knob",0.5);
        knob2->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
        knob2->Layout.sizeModeX = MIP_WIDGET_SIZE_PARENT_RATIO;
        knob2->Layout.aspectRatio = 2.0 / 3.0;
        aspect_rect1->appendChildWidget(knob2);

      }

      //----- center bottom sizer -----

      MIP_SizerWidget* center_bottom_sizer = new MIP_SizerWidget(5,MIP_SIZER_BOTTOM,center_bottom_panel);
      center_bottom_sizer->Layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      center_bottom_sizer->setFillColor(0.4);
      background->appendChildWidget(center_bottom_sizer);

      //----- center panel -----

      MIP_TextWidget* center_panel = new MIP_TextWidget(0,"center_panel");
      center_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      center_panel->Layout.border = MIP_DRect(10,10,10,10);
      center_panel->setFillBackground(false);
      center_panel->setDrawBorder(false);
      center_panel->setDrawText(false);
      center_panel->setBackgroundColor(0.45);
      background->appendChildWidget(center_panel);

      {

        MIP_TabsWidget* tabs1 = new MIP_TabsWidget(0,5);
        tabs1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
        center_panel->appendChildWidget(tabs1);

        {
          MIP_PanelWidget* tabs1_page1 = new MIP_PanelWidget(0);
          tabs1_page1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          tabs1_page1->setFillBackground(false);
          tabs1_page1->setDrawBorder(true);
          tabs1->appendPage("page1",tabs1_page1);

          MIP_PanelWidget* tabs1_page2 = new MIP_PanelWidget(0);
          tabs1_page2->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          tabs1_page2->setFillBackground(false);
          tabs1_page2->setDrawBorder(true);
          tabs1->appendPage("page2",tabs1_page2);

          MIP_PanelWidget* tabs1_page3 = new MIP_PanelWidget(0);
          tabs1_page3->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          tabs1_page3->setFillBackground(false);
          tabs1_page3->setDrawBorder(true);
          tabs1->appendPage("page1",tabs1_page3);

        }
        tabs1->selectPage(0);

      }

      //-----

      background->appendChildWidget(menu1);
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
    return new test_layout_plugin(ADescriptor,AHost);
  }
  return nullptr;
}


