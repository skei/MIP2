
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

//#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

#include "../data/img/knob4_60x60_131.h"
#include "../data/img/sa_logo_40_trans_black.h"
#include "../data/img/mip2_trans_129x34.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define NUM_GRAPH_MODULES 8
const char* buttonrow_text[6] = { "1", "2", "3", "four", "5", "6" };

//----------

const char* myFeatures[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "skei.audio/test_gui1",
  "test_gui1",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "simple mip2 test plugin",
  myFeatures
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

//  #define NUM_PARAMS 3
//

  clap_param_info_t myParameters[3] = {
    { 0,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Param1",
      "Params",
      0.1,
      4.0,
      1.0
    },
    { 1,
      CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE,
      nullptr,
      "Param2",
      "Params",
      0.0,
      1.0,
      0.5
    },
    { 2,
      CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE,
      nullptr,
      "Param3",
      "Params",
      0.0,
      1.0,
      0.5
    }
  };

  //----------

  MIP_Widget*   MEditorWidget = nullptr;

  MIP_Bitmap*   MKnobBitmap   = nullptr;
  MIP_Bitmap*   MLogoBitmap   = nullptr;
  MIP_Bitmap*   MMip2Bitmap   = nullptr;

  MIP_Surface*  MKnobSurface  = nullptr;
  MIP_Surface*  MLogoSurface  = nullptr;
  MIP_Surface*  MMip2Surface  = nullptr;

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
public: // plugin
//------------------------------

  bool init() final {
    for (uint32_t i=0; i<3; i++) {
      appendParameter(new MIP_Parameter( &myParameters[i] ));
    }
    return MIP_Plugin::init();
    //setDefaultParameterValues();
  }

  //----------

  const void* get_extension(const char *id) final {
    if (strcmp(id,CLAP_EXT_GUI) == 0) return &MGui;
    return MIP_Plugin::get_extension(id);
  }

  //----------


  //----------

  bool gui_create(const char *api, bool is_floating) final {
    if (strcmp(api,CLAP_WINDOW_API_X11) != 0) {
      MIP_Print("not x11\n");
      return false;
    }
    if (is_floating) {
      MIP_Print("floating\n");
      //return false;
    }
    return create_editor(is_floating);
  }

  //----------

  void gui_destroy() final {
    MIP_Plugin::gui_destroy();
    destroy_editor();
  }

//------------------------------
public:
//------------------------------

//  void handle_parameter_event(clap_event_param_value_t* param_value) final {
//    MIP_PRINT;
//    MIP_Plugin::handle_parameter_event(param_value);
//    uint32_t i = param_value->param_id;
//    if (i == 0) {
//      if (MEditor && MEditorIsOpen) {
//        float v = param_value->value;
//        v = MIP_Clamp(v,0.1,2);
//        MIP_Window* window = MEditor->getWindow();
//        window->setScale(v);
//        window->alignWidgets();
//        window->do_widget_redraw(window,window->getRect(),0);
//      }
//    }
//  }

  //----------

  //void handle_modulation_event(clap_event_param_mod_t* param_mod) final {
  //  MIP_Plugin::handle_modulation_event(param_mod);
  //}

  //----------

  void handle_process(const clap_process_t *process) final {
    float** inputs = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    MIP_CopyStereoBuffer(outputs,inputs,length);
  }

//------------------------------
public:
//------------------------------

  void add_modules(MIP_GraphWidget* AGraph) {
    for (uint32_t m=0; m<NUM_GRAPH_MODULES; m++) {
      uint32_t num_in = MIP_RandomRangeInt(0,5);
      uint32_t num_out = MIP_RandomRangeInt(0,5);
      MIP_GraphModule* module = new MIP_GraphModule();
      module->numInputs = num_in;
      module->numOutputs = num_out;
      for (uint32_t i=0; i<num_in; i++) {
        uint32_t typ = MIP_PIN_DATA;
        if (MIP_Random() >= 0.5) typ = MIP_PIN_SIGNAL;
        module->inputs[i] = typ;
      }
      for (uint32_t o=0; o<num_out; o++) {
        uint32_t typ = MIP_PIN_DATA;
        if (MIP_Random() >= 0.5) typ = MIP_PIN_SIGNAL;
        module->outputs[o] = typ;
      }
      uint32_t x = MIP_RandomRangeInt(0,255);
      uint32_t y = MIP_RandomRangeInt(0,255);
      AGraph->addModule( module, x,y, "Module");
    }
  }

  //----------

  bool create_editor(bool is_floating) {
    //MEditorIsOpen = false;
    MEditor = new MIP_Editor(this,this,800,600,!is_floating);

    if (MEditor) {

      MEditor->setCanResize();
      MIP_Window* window = MEditor->getWindow();

      // bitmaps
      MIP_Painter* painter;

      // knob
      MKnobBitmap = new MIP_Bitmap(knob4_60x60_131,knob4_60x60_131_size);
      //knob_bitmap->convertRgbaToBgra();
      MKnobBitmap->premultAlpha(0x999999);
      MKnobSurface = new MIP_Surface(window,MKnobBitmap->getWidth(),MKnobBitmap->getHeight());
      painter = new MIP_Painter(MKnobSurface);
      painter->uploadBitmap(0,0,MKnobBitmap);
      painter->flush();
      delete painter;

      // logo
      MLogoBitmap = new MIP_Bitmap(sa_logo_40_trans_black,sa_logo_40_trans_black_size);
      //knob_bitmap->convertRgbaToBgra();
      MLogoBitmap->premultAlpha(0x808080);
      //MLogoBitmap->fillLayer(3,255);
      MLogoSurface = new MIP_Surface(window,MLogoBitmap->getWidth(),MLogoBitmap->getHeight());
      painter = new MIP_Painter(MLogoSurface);
      painter->uploadBitmap(0,0,MLogoBitmap);
      painter->flush();
      delete painter;

      // mip2
      MMip2Bitmap = new MIP_Bitmap(mip2_trans_129x34,mip2_trans_129x34_size);
      //knob_bitmap->convertRgbaToBgra();
      MMip2Bitmap->scaleLayer(3,128);
      MMip2Bitmap->premultAlpha(0x808080);
      MMip2Surface = new MIP_Surface(window,MMip2Bitmap->getWidth(),MMip2Bitmap->getHeight());
      painter = new MIP_Painter(MMip2Surface);
      painter->uploadBitmap(0,0,MMip2Bitmap);
      painter->flush();
      delete painter;

      //

      // (selector) menu

      MIP_MenuWidget* selector_menu = new MIP_MenuWidget( MIP_FRect() );
      selector_menu->appendMenuItem("first");
      selector_menu->appendMenuItem("item2");
      selector_menu->appendMenuItem("item3");
      selector_menu->appendMenuItem("4");
      selector_menu->appendMenuItem("five");
      selector_menu->setItemSize(90,20);
      selector_menu->setItemLayout(1,5);
      //selector_menu->setMenuMirror(false,false);

      // editor widget

      MEditorWidget = new MIP_Widget(MIP_FRect());
      MEditorWidget->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

      /* */

      // sa header

      MIP_SAHeaderWidget* sa_header = new MIP_SAHeaderWidget(MIP_FRect(60),MLogoSurface,MMip2Surface);
      MEditorWidget->appendWidget(sa_header);

      MIP_PanelWidget* footer = new MIP_PanelWidget(MIP_FRect(20));
      MEditorWidget->appendWidget(footer);
      footer->layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      footer->setDrawBorder(false);

      MIP_SizerWidget* window_sizer = new MIP_SizerWidget(MIP_FRect( 15,15),MIP_SIZER_WINDOW);
      footer->appendWidget(window_sizer);
      window_sizer->layout.alignment = MIP_WIDGET_ALIGN_BOTTOM_RIGHT;

      /* */

//----------

      // left_panel

      MIP_PanelWidget* left_panel = new MIP_PanelWidget(MIP_FRect(250));
      MEditorWidget->appendWidget(left_panel);
      left_panel->setBackgroundColor(0.6);
      left_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
      left_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
      left_panel->layout.spacing = 5;

        //

        MIP_ScrollBoxWidget* left_scrollbox = new MIP_ScrollBoxWidget(MIP_FRect(),true,false);
        left_panel->appendWidget(left_scrollbox);
        left_scrollbox->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

        left_scrollbox->getContentWidget()->layout.innerBorder = MIP_FRect(5,5,5,5);
        left_scrollbox->getContentWidget()->layout.spacing = 5;

        //

        MIP_TextWidget* text = new MIP_TextWidget(20,"text");
        left_scrollbox->appendWidget(text);
        text->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        text->setFillBackground(true);
        text->setBackgroundColor(0.55);

        MIP_ValueWidget* value = new MIP_ValueWidget(20,"value");
        left_scrollbox->appendWidget(value);
        value->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        value->setFillBackground(true);
        value->setBackgroundColor(0.55);

        MIP_DragValueWidget* dragvalue = new MIP_DragValueWidget(MIP_FRect(20),"drag-value",0);
        left_scrollbox->appendWidget(dragvalue);
        dragvalue->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        dragvalue->setFillBackground(true);
        dragvalue->setBackgroundColor(0.55);

        MIP_SliderWidget* slider2 = new MIP_SliderWidget(MIP_FRect(110,20), "Slider2", 0.5 );
        left_scrollbox->appendWidget(slider2);
        slider2->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
        slider2->setDrawValueBarGradient(false);
        slider2->setDrawValueBarBorder(false);

        MIP_SliderWidget* slider = new MIP_SliderWidget(MIP_FRect(110,20), "Slider", 0.5 );
        left_scrollbox->appendWidget(slider);
        slider->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_ScrollBarWidget* scrollbar = new MIP_ScrollBarWidget(MIP_FRect(20));
        left_scrollbox->appendWidget(scrollbar);
        scrollbar->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_ButtonWidget* button = new MIP_ButtonWidget(20);
        left_scrollbox->appendWidget(button);
        button->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        button->setIsToggle(false);

        MIP_ButtonWidget* switch1 = new MIP_ButtonWidget(MIP_FRect(20));
        left_scrollbox->appendWidget(switch1);
        switch1->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        switch1->setIsToggle(true);

        MIP_RangeSliderWidget* rangeslider = new MIP_RangeSliderWidget(MIP_FRect(20));
        left_scrollbox->appendWidget(rangeslider);
        rangeslider->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        rangeslider->setValue(0.2);
        rangeslider->setValue2(0.7);

        MIP_TextEditWidget* textedit = new MIP_TextEditWidget(MIP_FRect(20));
        left_scrollbox->appendWidget(textedit);
        textedit->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_GroupBoxWidget* groupbox = new MIP_GroupBoxWidget(MIP_FRect(100),true);
        left_scrollbox->appendWidget(groupbox);
        groupbox->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
        groupbox->getTitleBar()->setDrawTriangle(true);
        groupbox->close();

          MIP_PanelWidget* group1 = new MIP_PanelWidget(MIP_FRect());
          groupbox->appendWidget(group1);
          group1->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          group1->setFillBackground(true);
          //group1->setDrawBorder(true);
          //group1->setRoundedCorners(MIP_CORNER_LEFT_BOTTOM+MIP_CORNER_RIGHT_BOTTOM);
          //group1->setRoundedRadius(8);
          group1->setBackgroundColor(MIP_COLOR_LIGHT_CYAN2);

        MIP_SelectorWidget* selector = new MIP_SelectorWidget(MIP_FRect(110,20));
        left_scrollbox->appendWidget(selector);
        selector->setMenu(selector_menu);
        selector->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_GridWidget* grid = new MIP_GridWidget(MIP_FRect(80),5,5);
        left_scrollbox->appendWidget(grid);
        grid->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_ButtonRowWidget* button_row = new MIP_ButtonRowWidget(MIP_FRect(20), 6, buttonrow_text, MIP_BUTTON_ROW_MULTI );
        left_scrollbox->appendWidget(button_row);
        button_row->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        button_row->setButtonState(0,true);
        button_row->setButtonState(2,true);

        MIP_ButtonRowWidget* button_row2 = new MIP_ButtonRowWidget(MIP_FRect(20), 6, buttonrow_text, MIP_BUTTON_ROW_SINGLE );
        left_scrollbox->appendWidget(button_row2);
        button_row2->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        button_row2->setButtonState(0,true);

        MIP_KeyboardWidget* keyboard = new MIP_KeyboardWidget(MIP_FRect(40));
        left_scrollbox->appendWidget(keyboard);
        keyboard->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_SliderBankWidget* sliderbank = new MIP_SliderBankWidget(MIP_FRect(40),16);
        left_scrollbox->appendWidget(sliderbank);
        sliderbank->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_ValueGraphWidget* valuegraph = new MIP_ValueGraphWidget(MIP_FRect(40),16);
        left_scrollbox->appendWidget(valuegraph);
        valuegraph->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        /*
        MIP_CircularWaveformWidget* circular = new MIP_CircularWaveformWidget(MIP_FRect(150));
        left_scrollbox->appendWidget(circular);
        circular->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        circular->setInnerRadius(0.5);
        circular->setOuterRadius(1);
        circular->setBipolar(true);
        circular->createBuffer(1024);
        for (uint32_t i=0; i<1024; i++) circular->setBuffer(i,MIP_RandomRange(-1,1));
        */

//----------

      // left_sizer

      MIP_SizerWidget* left_sizer = new MIP_SizerWidget(MIP_FRect(5),MIP_SIZER_LEFT);
      MEditorWidget->appendWidget(left_sizer);
      left_sizer->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
      left_sizer->setTarget(left_panel);

//----------

      // right top panel

      MIP_PanelWidget* right_top_panel = new MIP_PanelWidget(MIP_FRect(150));
      MEditorWidget->appendWidget(right_top_panel);
      right_top_panel->setBackgroundColor(0.6);
      right_top_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      right_top_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
      right_top_panel->layout.spacing = 5;

        MIP_KnobWidget* knob1 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
        right_top_panel->appendWidget(knob1);
        knob1->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;

        MIP_KnobWidget* knob2 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
        right_top_panel->appendWidget(knob2);
        knob2->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;

        MIP_KnobWidget* knob3 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
        right_top_panel->appendWidget(knob3);
        knob3->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;

        MEditor->connect(knob1,0);
        MEditor->connect(knob2,1);
        MEditor->connect(knob3,2);

        MIP_CurveWidget* curve1 = new MIP_CurveWidget( MIP_FRect( 50,50 ),false);
        right_top_panel->appendWidget(curve1);
        curve1->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
        curve1->setValue(0.5);

        MIP_CurveWidget* curve2 = new MIP_CurveWidget( MIP_FRect( 50,50 ),true);
        right_top_panel->appendWidget(curve2);
        curve2->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
        curve2->setValue(0.5);

        MIP_ImageStripWidget* imagestrip = new MIP_ImageStripWidget(MIP_FRect(60));
        right_top_panel->appendWidget(imagestrip);
        imagestrip->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
        imagestrip->setup(1,131,MKnobSurface);

        //

        MIP_Knob2Widget* k2 = new MIP_Knob2Widget(MIP_FRect(50,82),"knob2");
        right_top_panel->appendWidget(k2);
        k2->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;

        MIP_Knob3Widget* k3 = new MIP_Knob3Widget(MIP_FRect(100,25));
        right_top_panel->appendWidget(k3);
        k3->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;

        MIP_Knob4Widget* k4 = new MIP_Knob4Widget(MIP_FRect(150,25));
        right_top_panel->appendWidget(k4);
        k4->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;

        //

        MIP_Knob5Widget* k5 = new MIP_Knob5Widget(MIP_FRect(60,90),"Knob5");
        right_top_panel->appendWidget(k5);
        k5->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;

        k5 = new MIP_Knob5Widget(MIP_FRect(120,75),"Knob5b");
        right_top_panel->appendWidget(k5);
        k5->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;

        k5 = new MIP_Knob5Widget(MIP_FRect(60,120),"Knob5c");
        right_top_panel->appendWidget(k5);
        k5->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;

//----------

      // top_sizer

      MIP_SizerWidget* top_sizer = new MIP_SizerWidget(MIP_FRect(5),MIP_SIZER_TOP);
      MEditorWidget->appendWidget(top_sizer);
      top_sizer->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      top_sizer->setTarget(right_top_panel);

//----------

      // right bottom panel

      MIP_PanelWidget* right_bottom_panel = new MIP_PanelWidget(MIP_FRect(100));
      MEditorWidget->appendWidget(right_bottom_panel);
      right_bottom_panel->setBackgroundColor(0.6);
      right_bottom_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      right_bottom_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
      right_bottom_panel->layout.spacing = 5;

          MIP_PanelWidget* p;

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          p->setDrawBorder();
          p->setFillBackground(false);

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          p->setDrawBorder();
          p->setBorderEdges(MIP_EDGE_LEFT + MIP_EDGE_RIGHT);
          p->setRoundedCorners(MIP_CORNER_LEFT_TOP + MIP_CORNER_RIGHT_TOP + MIP_CORNER_RIGHT_BOTTOM);
          p->setFillBackground(false);

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          p->setFillBackground();

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          p->setFillGradient();

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          p->setFillGradient();
          p->setDrawBorder();

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          p->setFillGradient();
          p->setBackgroundColor(MIP_COLOR_LIGHT_BLUE1);
          p->setDrawBorder();
          p->setRoundedCorners(MIP_CORNER_ALL);
          p->setRoundedRadius(6);

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          p->setFillGradient();
          p->setBackgroundColor(MIP_COLOR_LIGHT_BLUE2);
          p->setDrawBorder();
          p->setRoundedCorners(MIP_CORNER_LEFT_TOP + MIP_CORNER_RIGHT_TOP);
          p->setRoundedRadius(6);

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          //p->setFillBackground();
          p->setFillGradient();
          p->setBackgroundColor(MIP_COLOR_LIGHT_GREEN);
          p->setDrawBorder();
          p->setBorderThickness(2);
          p->setBorderEdges(MIP_EDGE_LEFT + MIP_EDGE_TOP + MIP_EDGE_RIGHT);
          p->setRoundedCorners(MIP_CORNER_LEFT_TOP);
          p->setRoundedRadius(12);

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          p->setDrawBorder();
          p->setRoundedCorners(MIP_CORNER_ALL);
          p->setRoundedRadius(9);
          p->setBackgroundColor(MIP_COLOR_LIGHT_CYAN2);

          p = new MIP_PanelWidget(MIP_FRect(100,25));
          right_bottom_panel->appendWidget(p);
          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
          p->setFillGradient();
          p->setBackgroundColor(MIP_COLOR_LIGHT_ORANGE);
          p->setDrawBorder();
          p->setRoundedCorners(MIP_CORNER_ALL);
          p->setRoundedRadius(11);

//----------

      // bottom_sizer

      MIP_SizerWidget* bottom_sizer = new MIP_SizerWidget(MIP_FRect(5),MIP_SIZER_BOTTOM);
      MEditorWidget->appendWidget(bottom_sizer);
      bottom_sizer->layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      bottom_sizer->setTarget(right_bottom_panel);

//----------

      // right center panel

      MIP_PanelWidget* right_center_panel = new MIP_PanelWidget(MIP_FRect());
      MEditorWidget->appendWidget(right_center_panel);
      right_center_panel->setBackgroundColor(0.6);
      right_center_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      right_center_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
      right_center_panel->layout.spacing = 5;

        MIP_TabsWidget* tabs = new MIP_TabsWidget(MIP_FRect());
        right_center_panel->appendWidget(tabs);
        tabs->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

          // page 1

          MIP_PanelWidget* page1 = new MIP_PanelWidget(MIP_FRect());
          page1->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          page1->setFillBackground(false);
          //page1->setBackgroundColor(MIP_Color(0.65,0.6,0.6));
          //page1->layout.innerBorder = MIP_FRect(10,10,10,10);
          //page1->layout.spacing = 5;

            MIP_ImageWidget* image3 = new MIP_ImageWidget(MIP_FRect(50,50),MLogoSurface);
            page1->appendWidget(image3);
            image3->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

          // page 2

          MIP_PanelWidget* page2 = new MIP_PanelWidget(MIP_FRect());
          page2->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          page2->setFillBackground(false);
          page2->setBackgroundColor(MIP_Color(0.6,0.65,0.6));

            MIP_GraphWidget* graph = new MIP_GraphWidget(MIP_FRect());
            page2->appendWidget(graph);
            graph->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
            add_modules(graph);

          // page 3

          MIP_PanelWidget* page3 = new MIP_PanelWidget(MIP_FRect());
          page3->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          page3->setFillBackground(true);
          page3->setBackgroundColor(MIP_Color(0.6,0.6,0.65));

            MIP_ScrollBoxWidget* scrollbox1 = new MIP_ScrollBoxWidget(MIP_FRect(),true,false);
            page3->appendWidget(scrollbox1);
            scrollbox1->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
            //scrollbox1->getContentWidget()->setDrawBorder(true);
            scrollbox1->getContentWidget()->layout.innerBorder = MIP_FRect(5,5,5,5);
            scrollbox1->getContentWidget()->layout.spacing = 25;

            for (uint32_t i=0; i<100; i++) {
              //MIP_KnobWidget* knob = new MIP_KnobWidget( MIP_FRect( 32,32 ));
              //scrollbox1->appendWidget(knob);
              //knob->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
              MIP_PanelWidget* ppp = new MIP_PanelWidget(MIP_FRect(32,32));
              scrollbox1->appendWidget(ppp);
              ppp->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
            }

        tabs->appendPage("Page1",page1);
        tabs->appendPage("Page2",page2);
        tabs->appendPage("Page3",page3);
        tabs->selectPage(0);

//----------

      MEditorWidget->appendWidget(selector_menu);

      //MSizer->setTarget(win);

      window->appendWidget(MEditorWidget);
      window->setFillWindowBackground(false);
      return true;
    }
    return false;
  }

  //----------

  void destroy_editor() {
    delete MKnobBitmap;
    delete MLogoBitmap;
    delete MMip2Bitmap;
    delete MKnobSurface;
    delete MLogoSurface;
    delete MMip2Surface;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendPlugin(&myDescriptor);
}

//----------

//void MIP_Unregister(MIP_ClapRegistry* ARegistry) {
//  #ifdef MIP_DEBUG_MEMORY
//  MIP_GLOBAL_DEBUG.dumpMemoryNodes();
//  #endif
//}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
