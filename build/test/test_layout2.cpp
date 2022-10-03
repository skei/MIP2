
// nc -U -l -k /tmp/mip.socket
#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
#endif

//-----

// MIP_LINUX is not defined yet (mip_defines.h)
#ifdef __gnu_linux__
  #define MIP_GUI_XCB
  //#define MIP_PAINTER_XCB
  //#define MIP_WINDOW_BUFFERED
  #define MIP_PAINTER_NANOVG
#else
  #define MIP_GUI_WIN32
  #define MIP_PAINTER_GDI
#endif

//-----

//#define MIP_PLUGIN_GENERIC_EDITOR

//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/widgets/mip_widgets.h"

//----------

//#define TEXTBOX1_LINE_SIZE    256
//#define TEXTBOX1_LINE_COUNT   256
//#define TEXTBOX1_BUFFER_SIZE  (TEXTBOX1_LINE_COUNT * TEXTBOX1_LINE_SIZE)

#define EDITOR_WIDTH  640
#define EDITOR_HEIGHT 480

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t template_descriptor = {
  .clap_version  = CLAP_VERSION,
  .id            = "me/test_layout2/0",
  #ifdef MIP_DEBUG
    #ifdef MIP_EXECUTABLE_SHARED_LIBRARY
      .name      = "test_layout2 (exec.so)",
    #else
      .name      = "test_layout2 (debug)",
    #endif
  #else
  .name          = "test_layout2",
  #endif
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

  //----------

  //const char* button_txt[5] = { "1", "2", "3", "4", "v" };
  //char textbox1_buffer[TEXTBOX1_BUFFER_SIZE] = {0};

//------------------------------
public:
//------------------------------

  test_layout_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    //MIP_Print("Creating plugin\n");
    MEditorWidth  = EDITOR_WIDTH;
    MEditorHeight = EDITOR_HEIGHT;
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    //MIP_Print("Initializing plugin\n");

    appendAudioInputPort( &myAudioInputPorts[0] );
    appendAudioOutputPort(&myAudioOutputPorts[0]);

    appendNoteInputPort(  &myNoteInputPorts[0]  );
    appendNoteOutputPort( &myNoteOutputPorts[0] );

    for (uint32_t i=0; i<PARAM_COUNT; i++) {
      appendParameter( new MIP_Parameter(&myParameters[i]) );
    }
    appendParameter( new MIP_Parameter(0,"param4","",-10,10,0,CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED) );

    return true;
  }

//------------------------------
public: // gui
//------------------------------

#ifndef MIP_PLUGIN_GENERIC_EDITOR

  bool gui_create(const char *api, bool is_floating) final {
    bool result = MIP_Plugin::gui_create(api,is_floating);
    if (result /*&& MEditor*/) {

      MIP_Window* editor_window = MEditor->getWindow();
      //editor_window->setWindowBackgroundColor(0.5);
      //editor_window->setWindowFillBackground(true);

      //----- background -----

      MIP_PanelWidget* background = new MIP_PanelWidget( MIP_DRect(EDITOR_WIDTH,EDITOR_HEIGHT) );
      //MIP_PanelWidget* background = new MIP_PanelWidget( MIP_DRect(0,0,1,1) );
      //background->Layout.rectMode = MIP_WIDGET_RECT_MODE_PARENT_RATIO;
      background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_PARENT;
      background->setFillBackground(true);
      background->setBackgroundColor(0.5);
      background->setDrawBorder(true);
      background->setBorderColor(0);
      background->Layout.aspectRatio = ((double)EDITOR_WIDTH / (double)EDITOR_HEIGHT);
      editor_window->appendChildWidget(background);

//#if 0

      //----------

      MIP_PanelWidget* p = new MIP_PanelWidget( MIP_DRect(50,50,540,380) );
      p->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      p->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      p->setFillBackground(true);
      p->setBackgroundColor( MIP_Color(0.6,0.5,0.5));
      p->setDrawBorder(true);
      p->setBorderColor(0.3);
      background->appendChildWidget(p);

      MIP_PanelWidget*  p1 = new MIP_PanelWidget( MIP_DRect(   0,  0, 80, 80 ) );
      p1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
      p1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      p1->setBackgroundColor( MIP_Color(0.5,0.6,0.5));
      p->appendChildWidget(p1);

      MIP_PanelWidget*  p2 = new MIP_PanelWidget( MIP_DRect( 100, 10, 80, 80 ) );
      p2->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      p2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      p2->setBackgroundColor( MIP_Color(0.5,0.6,0.5));
      p->appendChildWidget(p2);

      MIP_PanelWidget*  p3 = new MIP_PanelWidget( MIP_DRect( 200, 10, 80, 80 ) );
      p3->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      p3->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      p3->setBackgroundColor( MIP_Color(0.5,0.6,0.5));
      p->appendChildWidget(p3);

      MIP_PanelWidget*  p4 = new MIP_PanelWidget( MIP_DRect( 300, 10, 80, 80 ) );
      p4->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      p4->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      p4->setBackgroundColor( MIP_Color(0.5,0.6,0.5));
      p->appendChildWidget(p4);

      MIP_SymbolWidget* p5 = new MIP_SymbolWidget( MIP_DRect( 400, 10, 80, 80 ), MIP_SYMBOL_FILL_ELLIPSE );
      p5->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      p5->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      p5->setFillBackground(false);
      p5->setColor(MIP_COLOR_BLACK);
      p->appendChildWidget(p5);

//#endif // 0

    } // editor
    return result;
  }

#endif // MIP_PLUGIN_GENERIC_EDITOR

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

//----------

void MIP_Register() {
  MIP_REGISTRY.appendDescriptor(&template_descriptor);
};

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new test_layout_plugin(ADescriptor,AHost);
  }
  return nullptr;
}


