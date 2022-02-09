
#include "mip.h"
#include "base/utils/mip_json.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------

void json_param(MIP_Json* AJson, clap_param_info_t* param) {

  uint64_t  cookie            = (uint64_t)param->cookie;  // ouch !!!!!
  char*     name              = param->name;
  char*     module            = param->module;
  uint32_t  flags             = param->flags;

  bool      is_stepped        = flags & CLAP_PARAM_IS_STEPPED;
  bool      is_per_note       = flags & CLAP_PARAM_IS_PER_NOTE;
  bool      is_per_channel    = flags & CLAP_PARAM_IS_PER_CHANNEL;
  bool      is_per_port       = flags & CLAP_PARAM_IS_PER_PORT;
  bool      is_periodic       = flags & CLAP_PARAM_IS_PERIODIC;
  bool      is_hidden         = flags & CLAP_PARAM_IS_HIDDEN;
  bool      is_bypass         = flags & CLAP_PARAM_IS_BYPASS;
  bool      is_readonly       = flags & CLAP_PARAM_IS_READONLY;
  bool      is_modulatable    = flags & CLAP_PARAM_IS_MODULATABLE;
  bool      requires_process  = flags & CLAP_PARAM_REQUIRES_PROCESS;

  AJson->objectBegin(nullptr);
  AJson->u32("id",&param->id);
  AJson->u32("flags",&param->flags);
  AJson->arrayBegin("flags_ext");
  AJson->boolean("CLAP_PARAM_IS_STEPPED",&is_stepped);
  AJson->boolean("CLAP_PARAM_IS_PER_NOTE",&is_per_note);
  AJson->boolean("CLAP_PARAM_IS_PER_CHANNEL",&is_per_channel);
  AJson->boolean("CLAP_PARAM_IS_PER_PORT",&is_per_port);
  AJson->boolean("CLAP_PARAM_IS_PERIODIC",&is_periodic);
  AJson->boolean("CLAP_PARAM_IS_HIDDEN",&is_hidden);
  AJson->boolean("CLAP_PARAM_IS_BYPASS",&is_bypass);
  AJson->boolean("CLAP_PARAM_IS_READONLY",&is_readonly);
  AJson->boolean("CLAP_PARAM_IS_MODULATABLE",&is_modulatable);
  AJson->boolean("CLAP_PARAM_REQUIRES_PROCESS",&requires_process);
  AJson->arrayEnd();
  AJson->u64("cookie",&cookie);
  AJson->string("name",(const char**)&name);
  AJson->string("module",(const char**)&module);
  AJson->f64("min_value",&param->min_value);
  AJson->f64("max_value",&param->max_value);
  AJson->f64("default_value",&param->default_value);
  AJson->objectEnd();

  if (AJson->error()) {
    MIP_DPrint(AJson->errorMessage());
  }

}

//----------------------------------------------------------------------


clap_param_info_t param1 = {
  31,           // id
  85,           // flags
  (void*)666,   // cookie
  "myPlugin",   // name
  "myModule",   // module
  0.0,          // min
  1.0,          // max
  0.5           // def
};


//----------------------------------------------------------------------

int main() {
  char buffer[2048] = {0};
  MIP_Json json = MIP_Json();
  json.setupWrite(true,buffer,2048);
  json_param(&json,&param1);
  json.setupRead(buffer,2048);
  json_param(&json,&param1);
  MIP_DPrint(buffer);
  return 0;
}
