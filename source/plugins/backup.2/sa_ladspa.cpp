
#define MIP_NO_GUI
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "mip.h"
#include "base/utils/mip_strutils.h"
#include "plugin/mip_plugin.h"
#include "plugin/ladspa/mip_ladspa_hosted_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

char* str_copy(const char* src) {
  char* dst = (char*)malloc(strlen(src)+1);
  strcpy(dst,src);
  return dst;
}

//----------

void register_ladspa_plugin(char* path, MIP_ClapRegistry* ARegistry) {
  const char* ext = MIP_GetFileExt(path);
  if (strcmp(ext,"so") == 0) {
    void* lib = dlopen(path,RTLD_LAZY | RTLD_LOCAL ); // RTLD_NOW, RTLD_LAZY
    if (lib) {
      //const char* filename = MIP_GetFilenameFromPath(path);
      //MIP_Print("%s\n",filename);
      LADSPA_Descriptor_Function get_ladspa_descriptor = (LADSPA_Descriptor_Function)dlsym(lib,"ladspa_descriptor");
      int index = 0;
      const LADSPA_Descriptor* ladspa_descriptor;
      ladspa_descriptor = get_ladspa_descriptor(index);
      while (ladspa_descriptor) {
        clap_plugin_descriptor_t* clap_descriptor = (clap_plugin_descriptor_t*)malloc(sizeof(clap_plugin_descriptor_t));
        char id[512] = {0};
        //char name[512] = {0};

        char temp[6];
        temp[0] = '#';
        temp[1] = MIP_HEX_TABLE[ (index & 0xf000) >> 12 ];
        temp[2] = MIP_HEX_TABLE[ (index & 0x0f00) >> 8 ];
        temp[3] = MIP_HEX_TABLE[ (index & 0x00f0) >> 4 ];
        temp[4] = MIP_HEX_TABLE[ (index & 0x000f) >> 0 ];
        temp[5] = 0;

        strncpy(id,path,511);
        strcat(id,temp);

        //strncpy(name,ladspa_descriptor->Name,511);
        //strcat(name,temp);

        //MIP_Print("%s : %s\n",id,name);
        MIP_Print("%s : %s\n",id,ladspa_descriptor->Name);

        clap_descriptor->clap_version = CLAP_VERSION;
        clap_descriptor->id           = str_copy( id );//path );//str_copy( ladspa_descriptor->Name );
        clap_descriptor->name         = str_copy( ladspa_descriptor->Name );
        clap_descriptor->vendor       = str_copy( ladspa_descriptor->Maker );
        clap_descriptor->url          = "";
        clap_descriptor->manual_url   = "";
        clap_descriptor->support_url  = "";
        clap_descriptor->version      = "0.0.0";
        clap_descriptor->description  = "";
        clap_descriptor->features     = MIP_LadspaHostedPlugin_features;

        ARegistry->appendPlugin(clap_descriptor);
        ARegistry->autoFreePlugin(clap_descriptor);

        index += 1;
        ladspa_descriptor = get_ladspa_descriptor(index);
      }

      dlclose(lib);
    }
    else { // !lib
      //MIP_Print("error opening %s\n",fullpath);
    } // lib
  } // .so
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  //ARegistry->appendFactory(CLAP_PLUGIN_FACTORY_ID,&ladspa_factory);
  //ARegistry->appendPlugin(&ladspa_wrapper_descriptor);
  char path[512];
  strcpy(path,"/usr/lib/ladspa/");
  DIR* d = opendir(path);
  if (d) {
    struct dirent* dir;
    while ((dir = readdir(d)) != NULL) {
      char fullpath[512];
      strcpy(fullpath,path);
      strcat(fullpath,dir->d_name);
      register_ladspa_plugin(fullpath,ARegistry);
    }
    closedir(d);
  }
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  MIP_Print("index: %i plugin: %s\n",AIndex,ADescriptor->id);
  const char* path = ADescriptor->id;
  //switch (AIndex) {
  //  case 0: return new ladspa_wrapper(ADescriptor,AHost);
  //}
  MIP_LadspaHostedPlugin* hosted_plugin = new MIP_LadspaHostedPlugin(path,ADescriptor,AHost);
  hosted_plugin->printLadspaDescriptor();
  return hosted_plugin;
}
