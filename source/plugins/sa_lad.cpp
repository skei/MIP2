
/*
  TODO:
  - config file in same dir as plugin..
  - MIP_LadspaFactory, move discovery to factory..
*/

#define MIP_NO_GUI
#define MIP_NO_PAINTER

#ifndef MIP_NO_GUI
  #define MIP_GUI_XCB
  #define MIP_PAINTER_NANOVG
  #define MIP_PLUGIN_GENERIC_EDITOR
#endif

//----------

#define MIP_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/mip.socket

//----------

#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap_entry.h"
//#include "plugin/exe/mip_exe_entry.h"
//#include "plugin/vst2/mip_vst2_entry.h"
#include "plugin/vst3/mip_vst3_entry.h"

//----------------------------------------------------------------------

#include "mip.h"
#include "base/utils/mip_strutils.h"
#include "plugin/mip_plugin.h"

#include "plugin/ladspa//mip_ladspa.h"
#include "plugin/ladspa//mip_ladspa_hosted_plugin.h"


//----------------------------------------------------------------------
//
// ladspa
//
//----------------------------------------------------------------------

const char* MIP_LadspaPluginFeatures[] = {
  CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
  //CLAP_PLUGIN_FEATURE_INSTRUMENT,
  //CLAP_PLUGIN_FEATURE_STEREO,
  nullptr
};

//----------

char* MIP_StrCopyAllc(const char* src) {
  char* dst = (char*)malloc(strlen(src)+1);
  strcpy(dst,src);
  return dst;
}

//----------

// /usr/lib/ladspa/test.so#0000


void MIP_RegisterLadspa(const LADSPA_Descriptor* ladspa_descriptor, const char* path, uint32_t index) {
  clap_plugin_descriptor_t* clap_descriptor = (clap_plugin_descriptor_t*)malloc(sizeof(clap_plugin_descriptor_t));
  char id[512] = {0};
  char temp[6];
  temp[0] = '#';
  temp[1] = MIP_HEX_TABLE[(index & 0xf000) >> 12];
  temp[2] = MIP_HEX_TABLE[(index & 0x0f00) >> 8];
  temp[3] = MIP_HEX_TABLE[(index & 0x00f0) >> 4];
  temp[4] = MIP_HEX_TABLE[(index & 0x000f) >> 0];
  temp[5] = 0;
  strncpy(id,path,511);
  strcat(id,temp);
  clap_descriptor->clap_version = CLAP_VERSION;
  clap_descriptor->id           = MIP_StrCopyAllc(id);
  clap_descriptor->name         = MIP_StrCopyAllc(ladspa_descriptor->Name);
  clap_descriptor->vendor       = MIP_StrCopyAllc(ladspa_descriptor->Maker);
  clap_descriptor->url          = "";
  clap_descriptor->manual_url   = "";
  clap_descriptor->support_url  = "";
  clap_descriptor->version      = "0.0.1";
  clap_descriptor->description  = "";
  clap_descriptor->features   = MIP_LadspaPluginFeatures;
  MIP_REGISTRY.appendDescriptor(clap_descriptor);
}

//----------

void MIP_RegisterLadspaDescriptors(const char* APath) {
  DIR* d = opendir(APath);
  if (d) {
    struct dirent* dir;
    while ((dir = readdir(d)) != NULL) {
      char fullpath[512];
      strcpy(fullpath,APath);
      strcat(fullpath,dir->d_name);
      const char* ext = MIP_GetFileExt(dir->d_name);
      if (strcmp(ext,"so") == 0) {
        void* lib = dlopen(fullpath,RTLD_LAZY|RTLD_LOCAL); // RTLD_NOW, RTLD_LAZY
        if (lib) {
          LADSPA_Descriptor_Function get_ladspa_descriptor = (LADSPA_Descriptor_Function)dlsym(lib,"ladspa_descriptor");
          int index = 0;
          const LADSPA_Descriptor* ladspa_descriptor;
          ladspa_descriptor = get_ladspa_descriptor(index);
          while (ladspa_descriptor) {
            MIP_RegisterLadspa(ladspa_descriptor,fullpath,index);
            index += 1;
            ladspa_descriptor = get_ladspa_descriptor(index);
          }
        }
        dlclose(lib);
      } // .so
    }
    closedir(d);
  }

}

//----------

MIP_Plugin* MIP_CreateLadspaPlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  MIP_Print("path: %s\n",MIP_REGISTRY.getPath());
  return new MIP_LadspaHostedPlugin(ADescriptor,AHost);
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_Register() {
  MIP_Print("\n");
  MIP_RegisterLadspaDescriptors("/usr/lib/ladspa/");
};

//----------

void MIP_Unregister() {
  MIP_Print("\n");
  MIP_REGISTRY.freeDescriptors();
}

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  MIP_Print("\n");
  return MIP_CreateLadspaPlugin(AIndex,ADescriptor,AHost);
}
