
#define MIP_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/mip.socket

//----------------------------------------------------------------------

#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap_entry.h"
//#include "plugin/vst2/mip_vst2_entry.h"
#include "base/utils/mip_vm.h"
#include "base/utils/mip_vm_compiler.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "skei.audio/sa_bel/0.0.1",
   .name          = "sa_bel",
   .vendor        = "skei.audio",
   .url           = "",
   .manual_url    = "",
   .support_url   = "",
   .version       = "0.0.1",
   .description   = "vm & compiler experiment",
   .features      =  (const char *[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, "Hello world!", nullptr }
};

//----------

const clap_param_info_t myParameters[] = {
  { 0, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Value 1", "", 0, 1, 0.3  },
  { 1, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Value 2", "", 0, 1, 0.6  }
};

const clap_audio_port_info_t myAudioInputPorts[] = {
  { 0, "input1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
};

const clap_audio_port_info_t myAudioOutputPorts[] = {
  { 0, "output1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
};

const clap_note_port_info_t myNoteInputPorts[] = {
  { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "input1" }
};

const clap_note_port_info_t myNoteOutputPorts[] = {
  { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "output1" }
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

  MIP_VirtualMachine  MVirtualMachine = {};
  MIP_VMCompiler      MCompiler       = {};

  uint32_t  MSourceSize         = 0;
  char      MSourceCode[65536]  = {0};

  double  par_value1  = 0;
  double  par_value2  = 1;

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

  static
  void vm_callback(MIP_VirtualMachine* vm) {
    MIP_DPrint("vm_callback\n");
  }

  //----------

  void test_vm() {

    const char* filepath = MIP_REGISTRY.getPath();
    const char* filename = MIP_GetFilenameFromPath(filepath);
    //MIP_Print("path '%s' filename '%s'\n",path,filename);
    char path[512] = {0};
    MIP_GetPathOnly(path,filepath);
    strcat(path,"data/");
    strcat(path,filename);
    MIP_StripFileExt(path);
    strcat(path,".script");

    MIP_DPrint("Loading script: %s\n",path);
    FILE* fp = fopen(path,"rb");
    fseek(fp,0,SEEK_END);
    MSourceSize = ftell(fp);
    fseek(fp,0,SEEK_SET);
    fread(MSourceCode,MSourceSize,1,fp);
    fclose(fp);

    MIP_DPrint("Compiling script\n");
    MCompiler.parse(MSourceCode,MSourceSize);
    MCompiler.compile();
    uint32_t start = MCompiler.findLabel((char*)"start");
    MIP_DPrint("Calling script 'start' function\n");
    MVirtualMachine.registerExternal(&vm_callback);
    MVirtualMachine.translate(MCompiler.getOpcodes(),MCompiler.getNumOpcodes());

    MVirtualMachine.execute(start);

    MIP_DPrint("All done!\n");
  }

  //----------

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendAudioInputPort(&myAudioInputPorts[0]);
    appendAudioOutputPort(&myAudioInputPorts[0]);
    appendNoteInputPort(&myNoteInputPorts[0]);
    appendNoteOutputPort(&myNoteOutputPorts[0]);
    appendParameter( new MIP_Parameter(&myParameters[0]) );
    appendParameter( new MIP_Parameter(&myParameters[1]) );
    test_vm();
    return true;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&myDescriptor);
};

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}

