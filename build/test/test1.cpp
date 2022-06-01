
//----------------------------------------------------------------------

#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO
#define MIP_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/mip.socket

//#define MIP_DEBUG_CLAP

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
// exe
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
// descriptor, parameters
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "myPlugin/1234",
  .name         = "myPlugin",
  .vendor       = "skei.audio",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "description",
  .features     = (const char*[]){"audio_effect",nullptr}
};

//----------

const clap_param_info_t myParameters[] = {
  {0,CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,nullptr,"param1","", -2,2,0}
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MParameters.appendParameter(new MIP_Parameter("p1","",0.5));
    MParameters.appendParameter(new MIP_Parameter(&myParameters[0]));
    appendAudioInput(new MIP_AudioPort("ai1"));
    appendAudioOutput(new MIP_AudioPort("ao1"));
    appendAudioOutput(new MIP_AudioPort("ao2",0));
    appendNoteInput(new MIP_NotePort("ni1"));
    appendNoteOutput(new MIP_NotePort("no1"));
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public:
//------------------------------

  bool gui_create(const char *api, bool is_floating) override {
    //bool result = MIP_Plugin::gui_create(api,is_floating);
    MEditor = new MIP_Editor(this,this,320,240,true);
    MEditor->setCanResize(true);
    if (MEditor) {
      MIP_Window* window = MEditor->getWindow();
        MIP_PanelWidget* panel = new MIP_PanelWidget(MIP_FRect(200,100));
        window->appendWidget(panel);
        panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
        panel->setDrawBorder(false);

          MIP_SliderWidget* slider1 = new MIP_SliderWidget(MIP_FRect(10,10,200,20),"Slider1");
          panel->appendWidget(slider1);

          MIP_SliderWidget* slider2 = new MIP_SliderWidget(MIP_FRect(10,35,200,20),"Slider2");
          panel->appendWidget(slider2);

          MIP_SliderWidget* slider3 = new MIP_SliderWidget(MIP_FRect(10,60,200,20),"Slider3");
          panel->appendWidget(slider3);

          MEditor->connect(slider1,0);
          MEditor->connect(slider2,1);

      return true;
    }
    return false;
  }


};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&myDescriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    myPlugin* plugin = new myPlugin(ADescriptor,AHost);
    return plugin;
  }
  return nullptr;
}
