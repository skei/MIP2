/*
  Look at this later..
  needs more preparations (hosting)..
*/


#if 0

#define MIP_NO_GUI
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket


//#define MIP_USE_INVALIDATION

//----------

#include <dirent.h>
#include <sys/stat.h>

#include "mip.h"
#include "plugin/clap/mip_clap.h"

// hack to define our own custom entry/factory

//#define MIP_NO_DEFAULT_PLUGIN_ENTRY
//extern CLAP_EXPORT const clap_plugin_entry clap_entry;
#define MIP_NO_DEFAULT_PLUGIN_FACTORY
extern const clap_plugin_factory MIP_GLOBAL_CLAP_FACTORY;

//----------

//#include "mip.h"
#include "plugin/mip_plugin.h"

#include "plugin/ladspa/mip_ladspa.h"
#include "plugin/ladspa/mip_ladspa_hosted_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* myFeatures[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "torhelgeskei/ladspa_wrapper/v0.0.1",
  "ladspa_wrapper",
  "torhelgeskei",
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

  #define NUM_PARAMS 1

  clap_param_info_t myParameters[NUM_PARAMS] = {
    { 0, CLAP_PARAM_IS_MODULATABLE, nullptr, "Gain", "Params", 0.0, 1.0, 0.5 }
  };

  MIP_Library             MLibrary = {};
  MIP_LadspaHostedPlugin* MLadspa = nullptr;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {

    MLadspa = new MIP_LadspaHostedPlugin();
    MLadspa->load("/usr/lib/ladspa/guitarix.so");
    MLadspa->initDescriptor(0);
    MLadspa->initPorts();
    MLadspa->dumpLadspaDescriptor();
  }

  //----------

  virtual ~myPlugin() {
    MLadspa->unload();
  }

//------------------------------
private:
//------------------------------

  //void handle_parameter_event(const clap_event_param_value_t* param_value) final {
  //  MIP_Plugin::handle_parameter_event(param_value);
  //  uint32_t index = param_value->param_id;
  //  double value = param_value->value;
  //}

  //----------

  //void handle_modulation_event(const clap_event_param_mod_t* param_mod) final {
  //  MIP_Plugin::handle_modulation_event(param_mod);
  //}

  //----------

  void handle_process(const clap_process_t *process) final {
    float**  inputs  = process->audio_inputs[0].data32;
    float**  outputs = process->audio_outputs[0].data32;
    uint32_t length  = process->frames_count;
    float    scale   = getParameterValue(0) + getParameterModulation(0);
    MIP_CopyStereoBuffer(outputs,inputs,length);
    MIP_ScaleStereoBuffer(outputs,scale,length);
  }

  //----------

//  void handle_output_events(const clap_output_events_t* out_events) final {
//    if (MEditor && MIsEditorOpen) {
//      float v0 = MParamVal[0] + MParamMod[0];
//      v0 = MIP_Clamp(v0,0,1);
//      MEditor->send_param_mod(0,v0,out_events);
//    }
//    MIP_Plugin::handle_output_events(out_events);
//  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {

    //setupParameters(myParameters,NUM_PARAMS);
    //for (uint32_t i=0; i<num; i++) {
    //  const clap_param_info_t* info = &params[i];
    //  MParameters.append(info);
    //}

    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues(myParameters,NUM_PARAMS);
    }
    return result;
  }

  //activate() {
  //  MLadspa->instantiate(44100);
  //}

  //----------

//  const void* get_extension(const char *id) final {
//    const void* ext = MIP_Plugin::get_extension(id);
//    return ext;
//    //return nullptr;
//  }

};

//----------------------------------------------------------------------
//
// ladspa
//
//----------------------------------------------------------------------

bool HaveEnumeratedLadspaPlugins = false;

//----------

//void enumLadspaPlugins(const char* path/*, int depth */) {
//  DIR *dp;
//  struct dirent *entry;
//  //if ((dp = opendir(dir)) == NULL) {
//  //  MIP_Print("Can`t open directory %s\n", dir);
//  //  return;
//  //}
//  //chdir(dir);
//  dp = opendir(path);
//  if (dp) {
//    while ((entry = readdir(dp)) != NULL) {
//      //struct stat statbuf;
//      //stat(entry->d_name, &statbuf);
//      //if (S_ISDIR(statbuf.st_mode)) {
//      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ) continue;
//      //  MIP_Print("DIR %s\n",entry->d_name);
//      //  //printdir(entry->d_name, depth+4);
//      //} else {
//      //  //printf("%*s%s\n", depth, "", entry->d_name);
//      MIP_Print("FILE %s\n",entry->d_name);
//      //}
//    }
//    //chdir("..");
//    closedir(dp);
//  }
//}

const char* ladspa_features[] = {
  "audio_effect",
  nullptr
};

//----------

void appendLadspaPlugin(const char* name) {
  clap_plugin_descriptor_t* desc = (clap_plugin_descriptor_t*)malloc(sizeof(clap_plugin_descriptor_t));
  desc->clap_version  = CLAP_VERSION;
  desc->id            = "";
  desc->name          = "";
  desc->vendor        = "";
  desc->url           = "";
  desc->manual_url    = "";
  desc->support_url   = "";
  desc->version       = "";
  desc->description   = "";
  desc->features      = ladspa_features;;
  //MIP_GLOBAL_CLAP_LIST.appendPlugin(desc);
}

void cleanupLadspaPlugins() {
}

//----------

void enumLadspaPlugins(const char* path/*, int depth */) {
  DIR *dp;
  struct dirent *entry;
  dp = opendir(path);
  if (dp) {
    while ((entry = readdir(dp))) {
      MIP_Print("FILE %s\n",entry->d_name);
      appendLadspaPlugin(const char* name);
    }
    closedir(dp);
  }
}


//----------



//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

//bool clap_entry_init_callback(const char *plugin_path) {
//  return true;
//}
//
////----------
//
//void clap_entry_deinit_callback() {
//}
//
////----------
//
//const void* clap_entry_get_factory_callback(const char *factory_id) {
//  #ifdef MIP_PLUGIN_USE_INVALIDATION
//  if (strcmp(factory_id,CLAP_PLUGIN_INVALIDATION_FACTORY_ID) == 0) {
//    return &MIP_GLOBAL_CLAP_INVALIDATION;
//  }
//  #endif
//  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
//    return &MIP_GLOBAL_CLAP_FACTORY;
//  }
//  return nullptr;
//}
//
////----------------------------------------------------------------------
//
//// get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wattributes"
//
//// MIP_GLOBAL_CLAP_ENTRY
//CLAP_EXPORT const clap_plugin_entry clap_entry = {
//  CLAP_VERSION,
//  clap_entry_init_callback,
//  clap_entry_deinit_callback,
//  clap_entry_get_factory_callback,
//};
//
//#pragma GCC diagnostic pop

//----------------------------------------------------------------------
//
// factory
//
//----------------------------------------------------------------------

uint32_t clap_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  //if (!HaveEnumeratedLadspaPlugins) {
  //  enumLadspaPlugins("/usr/lib/ladspa");
  //  HaveEnumeratedLadspaPlugins = true;
  //}
  return MIP_GLOBAL_CLAP_LIST.getNumPlugins();
}

//----------

const clap_plugin_descriptor_t* clap_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  return MIP_GLOBAL_CLAP_LIST.getPlugin(index);
}

//----------

const clap_plugin_t* clap_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  int32_t index = MIP_GLOBAL_CLAP_LIST.findPluginById(plugin_id);
  //MIP_ClapHostProxy* hostproxy = new MIP_ClapHostProxy(host);
  const clap_plugin_descriptor_t* descriptor = MIP_GLOBAL_CLAP_LIST.getPlugin(index);
  MIP_ClapPlugin* plugin = MIP_CreatePlugin(index,descriptor,host); // deleted in MIP_ClapPlugin.clap_plugin_destroy_callback()

//  MIP_GLOBAL_CLAP_LIST.appendInstance(plugin);

  return plugin->ptr();
}

//----------------------------------------------------------------------

//static
//constexpr
const clap_plugin_factory MIP_GLOBAL_CLAP_FACTORY = {
  clap_factory_get_plugin_count_callback,
  clap_factory_get_plugin_descriptor_callback,
  clap_factory_create_plugin_callback
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins(MIP_ClapList* AList) {
  AList->appendPlugin(&myDescriptor);
}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}

#endif // 0