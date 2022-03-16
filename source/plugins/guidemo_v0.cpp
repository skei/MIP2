
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

#include "../data/img/knob4_60x60_131.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define NUM_GRAPH_MODULES 8
//----------

const char* myFeatures[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "torhelgeskei/guitest_v0/v0.0.0",
  "guitest_v0",
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
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
//  clap_param_info_t myParameters[NUM_PARAMS] = {
//    { 0, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE, nullptr, "Param1 AM", "Params", 0.0, 1.0, 0.5 },
//    { 1, CLAP_PARAM_IS_AUTOMATABLE,                             nullptr, "Param2 A",  "Params", 0.0, 1.0, 0.5 },
//    { 2, 0,                                                     nullptr, "Param3",    "Params", 0.0, 1.0, 0.5 }
//  };

  //----------

  MIP_Widget*  MEditorWidget = nullptr;

  MIP_Bitmap*   MKnobBitmap   = nullptr;
  MIP_Surface*  MKnobSurface  = nullptr;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {

    //MIP_FRect* rect = new MIP_FRect();

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
    MEditorIsOpen = false;

    MEditor = new MIP_Editor(this,this,800,600,!is_floating);

    if (MEditor) {
      MEditor->setCanResize();
      MIP_Window* window = MEditor->getWindow();

      // image strip (knob)

      MKnobBitmap = new MIP_Bitmap(knob4_60x60_131,knob4_60x60_131_size);
      //knob_bitmap->convertRgbaToBgra();
      MKnobBitmap->premultAlpha(0x999999);
      MKnobSurface = new MIP_Surface(window,MKnobBitmap->getWidth(),MKnobBitmap->getHeight());
      MIP_Painter* painter = new MIP_Painter(MKnobSurface);
      painter->uploadBitmap(0,0,MKnobBitmap);
      painter->flush();
      delete painter;

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

//----------

      // left_panel

      MIP_PanelWidget* left_panel = new MIP_PanelWidget(MIP_FRect(150));
      MEditorWidget->appendWidget(left_panel);
      left_panel->setBackgroundColor(0.6);
      left_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
      left_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
      left_panel->layout.spacing = 5;

        MIP_ButtonRowWidget* button_row = new MIP_ButtonRowWidget(MIP_FRect(20), 6, buttonrow_text, MIP_BUTTON_ROW_MULTI );
        left_panel->appendWidget(button_row);
        button_row->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        button_row->setButtonState(0,true);
        button_row->setButtonState(2,true);

        MIP_ButtonRowWidget* button_row2 = new MIP_ButtonRowWidget(MIP_FRect(20), 6, buttonrow_text, MIP_BUTTON_ROW_SINGLE );
        left_panel->appendWidget(button_row2);
        button_row2->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        button_row2->setButtonState(0,true);

        MIP_SliderWidget* slider = new MIP_SliderWidget(MIP_FRect(110,20), "Slider", 0.5 );
        left_panel->appendWidget(slider);
        slider->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_SelectorWidget* selector = new MIP_SelectorWidget(MIP_FRect(110,20));
        left_panel->appendWidget(selector);
        selector->setMenu(selector_menu);
        selector->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_GroupBoxWidget* groupbox = new MIP_GroupBoxWidget(MIP_FRect(100),true);
        left_panel->appendWidget(groupbox);
        groupbox->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
        groupbox->getTitleBar()->setDrawTriangle(true);
        groupbox->close();

          MIP_PanelWidget* group1 = new MIP_PanelWidget(MIP_FRect());
          groupbox->appendWidget(group1);
          group1->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          group1->setFillBackground(true);
          group1->setBackgroundColor(MIP_Color(0.6,0.6,0.65));

        MIP_KeyboardWidget* keyboard = new MIP_KeyboardWidget(MIP_FRect(40));
        left_panel->appendWidget(keyboard);
        keyboard->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_ButtonWidget* button = new MIP_ButtonWidget(MIP_FRect(20));
        left_panel->appendWidget(button);
        button->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        button->setIsToggle(false);

        MIP_ButtonWidget* switch1 = new MIP_ButtonWidget(MIP_FRect(20));
        left_panel->appendWidget(switch1);
        switch1->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        switch1->setIsToggle(true);

        MIP_RangeSliderWidget* rangeslider = new MIP_RangeSliderWidget(MIP_FRect(20));
        left_panel->appendWidget(rangeslider);
        rangeslider->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        rangeslider->setValue(0.2);
        rangeslider->setValue2(0.7);

        MIP_ScrollBarWidget* scrollbar = new MIP_ScrollBarWidget(MIP_FRect(20));
        left_panel->appendWidget(scrollbar);
        scrollbar->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_SliderBankWidget* sliderbank = new MIP_SliderBankWidget(MIP_FRect(40),16);
        left_panel->appendWidget(sliderbank);
        sliderbank->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_TextEditWidget* textedit = new MIP_TextEditWidget(MIP_FRect(20));
        left_panel->appendWidget(textedit);
        textedit->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        /*
        MIP_CircularWaveformWidget* circular = new MIP_CircularWaveformWidget(MIP_FRect(150));
        left_panel->appendWidget(circular);
        circular->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        circular->setInnerRadius(0.5);
        circular->setOuterRadius(1);
        circular->setBipolar(true);
        circular->createBuffer(1024);
        for (uint32_t i=0; i<1024; i++) circular->setBuffer(i,MIP_RandomRange(-1,1));
        */

        MIP_GridWidget* grid = new MIP_GridWidget(MIP_FRect(100),6,8);
        left_panel->appendWidget(grid);
        grid->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

        MIP_ValueGraphWidget* valuegraph = new MIP_ValueGraphWidget(MIP_FRect(40),16);
        left_panel->appendWidget(valuegraph);
        valuegraph->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;


//----------

      // left_sizer

      MIP_SizerWidget* left_sizer = new MIP_SizerWidget(MIP_FRect(5),MIP_SIZER_LEFT);
      MEditorWidget->appendWidget(left_sizer);
      left_sizer->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
      left_sizer->setTarget(left_panel);

//----------

      // right top panel

      MIP_PanelWidget* right_top_panel = new MIP_PanelWidget(MIP_FRect(100));
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

        MIP_ScrollBoxWidget* scrollbox1 = new MIP_ScrollBoxWidget(MIP_FRect(),true,false);
        right_bottom_panel->appendWidget(scrollbox1);
        scrollbox1->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

        scrollbox1->getContentWidget()->layout.innerBorder = MIP_FRect(5,5,5,5);
        scrollbox1->getContentWidget()->layout.spacing = 5;

        for (uint32_t i=0; i<100; i++) {
          MIP_KnobWidget* knob = new MIP_KnobWidget( MIP_FRect( 32,32 ));
          scrollbox1->appendWidget(knob);
          knob->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
        }

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

          MIP_PanelWidget* page1 = new MIP_PanelWidget(MIP_FRect());
          page1->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          page1->setFillBackground(true);
          page1->setBackgroundColor(MIP_Color(0.65,0.6,0.6));

          MIP_PanelWidget* page2 = new MIP_PanelWidget(MIP_FRect());
          page2->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          page2->setFillBackground(false);
          page2->setBackgroundColor(MIP_Color(0.6,0.65,0.6));

            MIP_GraphWidget* graph = new MIP_GraphWidget(MIP_FRect());
            page2->appendWidget(graph);
            graph->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
            add_modules(graph);

          MIP_PanelWidget* page3 = new MIP_PanelWidget(MIP_FRect());
          page3->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
          page3->setFillBackground(true);
          page3->setBackgroundColor(MIP_Color(0.6,0.6,0.65));

        tabs->appendPage("Page1",page1);
        tabs->appendPage("Page2",page2);
        tabs->appendPage("Page3",page3);
        tabs->selectPage(0);

//----------

      /*

      MSizer = new MIP_SizerWidget(MIP_FRect( 15,15),MIP_SIZER_WINDOW);
      MSizer->layout.alignment = MIP_WIDGET_ALIGN_BOTTOM_RIGHT;
      MEditorPanel->appendWidget(MSizer);

      */

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
    delete MKnobSurface;
    delete MKnobBitmap;
  }


//------------------------------
public:
//------------------------------

  void handle_process(const clap_process_t *process) final {
    float** inputs = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    MIP_CopyStereoBuffer(outputs,inputs,length);
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendParameter( new MIP_Parameter(0, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE,  "Param1 AM",  "Params", 0.0, 1.0, 0.4) );
    appendParameter( new MIP_Parameter(1, CLAP_PARAM_IS_AUTOMATABLE,                              "Param2 A",   "Params", 0.0, 1.0, 0.5) );
    appendParameter( new MIP_Parameter(2, 0,                                                      "Param3",     "Params", 0.0, 1.0, 0.6) );
    return MIP_Plugin::init();
  }

  //----------

  const void* get_extension(const char *id) final {
    if (strcmp(id,CLAP_EXT_GUI) == 0) return &MGui;
    return MIP_Plugin::get_extension(id);
  }

  //----------

  const char* buttonrow_text[6] = { "1", "2", "3", "four", "5", "6" };

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

  void gui_destroy() final {
    MIP_Plugin::gui_destroy();
    destroy_editor();
  }

  //----------

  // (should ideally have been be in MIP_Plugin, but MIP_Plugin doesn't know
  // about myParameters) (still true?)

  bool gui_show() final {
    //setEditorParameterValues(myParameters,NUM_PARAMS);
    setEditorParameterValues();
    MIP_Plugin::gui_show();
    return true;
  }

};

//----------------------------------------------------------------------

//#undef NUM_PARAMS

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendPlugin(&myDescriptor);
}

//----------

void MIP_Unregister(MIP_ClapRegistry* ARegistry) {
  #ifdef MIP_DEBUG_MEMORY
  MIP_GLOBAL_DEBUG.dumpMemoryNodes();
  #endif
}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
