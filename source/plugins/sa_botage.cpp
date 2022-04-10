
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

#include "sa_botage/sa_botage_editor.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define EDITOR_WIDTH  546
#define EDITOR_HEIGHT 626

//#define NUM_GRAPH_MODULES 8
//const char* buttonrow_text[6] = { "1", "2", "3", "four", "5", "6" };

const char* txt_1to8[8] = {
  "1", "2", "3", "4", "5", "6", "7", "8"
};

const char* txt_filter[5] = {
  "Off",
  "Lowpass",
  "Highpass",
  "Bandpass",
  "Notch"
};

//----------

const char* myFeatures[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "skei.audio/sa_botage",
  "sa_botage",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.11",
  "glitch/fsu effect",
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

//  MIP_Widget*   MEditorWidget = nullptr;
//
//  //MIP_Bitmap*   MKnobBitmap   = nullptr;
//  //MIP_Surface*  MKnobSurface  = nullptr;
//
//  MIP_Bitmap*   MLogoBitmap   = nullptr;
//  MIP_Surface*  MLogoSurface  = nullptr;
//
//  MIP_Bitmap*   MMip2Bitmap   = nullptr;
//  MIP_Surface*  MMip2Surface  = nullptr;

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
    //for (uint32_t i=0; i<3; i++) {
    //  appendParameter(new MIP_Parameter( &myParameters[i] ));
    //}
    return MIP_Plugin::init();
  }

  //----------

  const void* get_extension(const char *id) final {
    if (strcmp(id,CLAP_EXT_GUI) == 0) return &MGui;
    return MIP_Plugin::get_extension(id);
  }

  //----------


  //----------

  bool gui_create(const char *api, bool is_floating) final {
    if (strcmp(api,CLAP_WINDOW_API_X11) != 0) { MIP_Print("not x11\n"); return false; }
    if (is_floating) { MIP_Print("floating\n"); return false; }
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
//    MIP_Plugin::handle_parameter_event(param_value);
//    uint32_t i = param_value->param_id;
//    if (i == 0) {
//      if (MEditor && MEditorIsOpen) {
//        float v = param_value->value;
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


  bool create_editor(bool is_floating) {
    MEditor = new sa_botage_editor(this,this,EDITOR_WIDTH,EDITOR_HEIGHT,!is_floating);
    if (MEditor) {
      return true;
    }
    return false;
  }

  //----------

  void destroy_editor() {
//    delete MKnobBitmap;
//    delete MKnobSurface;
//    delete MLogoBitmap;
//    delete MLogoSurface;
//    delete MMip2Bitmap;
//    delete MMip2Surface;
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

