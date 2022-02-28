#ifndef print_included
#define print_included
//----------------------------------------------------------------------



//----------------------------------------------------------------------
//
// json
//
//----------------------------------------------------------------------

#include "json.hpp"

Json  json                = {};
char  json_buffer[65536]  = {0};

//----------

void json_begin() {
  json_buffer[0] = 0;
  json.setupWrite(true,json_buffer,65536);
}

//----------

void json_end(const char* filename) {
  uint32_t json_size = strlen(json_buffer);
  FILE* fp = fopen(filename,"wt");
  fwrite(json_buffer,1,json_size,fp);
  fclose(fp);
}

//----------------------------------------------------------------------

void print_json_plugin_list(Host* host) {
}

//----------

//    Json json = {};
//    json_buffer[0] = 0;
//    json.setupWrite(true,json_buffer,65536);
//
//      const clap_plugin_factory* factory = host->getClapFactory();
//      const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,index);
//      print_json_descriptor(&json,descriptor);
//
//    uint32_t json_size = strlen(json_buffer);
//    FILE* fp = fopen(json_file,"wt");
//    fwrite(json_buffer,1,json_size,fp);
//    fclose(fp);

void print_json_descriptor(const clap_plugin_descriptor_t* desc) {
  json.objectBegin(nullptr);
    char version_string[32] = {0};
    sprintf(version_string,"%i.%i.%i",desc->clap_version.major,desc->clap_version.minor,desc->clap_version.revision);
    printf("version_string: '%s'\n",version_string);
    const char* version_ptr = (const char*)version_string;
    json.string("clap_version",&version_ptr);
    json.string("id",(const char**)&desc->id);
    json.string("name",(const char**)&desc->name);
    json.string("vendor",(const char**)&desc->vendor);
    json.string("url",(const char**)&desc->url);
    json.string("manual_url",(const char**)&desc->manual_url);
    json.string("support_url",(const char**)&desc->support_url);
    json.string("version",(const char**)&desc->version);
    json.string("description",(const char**)&desc->description);
    json.arrayBegin("features");
    int i = 0;
    while ( desc->features[i] ) {
      //json.string("...",(const char**)&desc->features[i]);
      json.string(nullptr,(const char**)&desc->features[i]);
      i++;
    }
    json.arrayEnd();
  json.objectEnd();
  if (json.error()) {
    printf("%s\n",json.errorMessage());
  }
  //printf("5\n");
  //printf("write_json_descriptor... ok\n");
}

//----------

void print_json_parameter(const clap_param_info_t* param) {
  uint64_t    cookie            = (uint64_t)param->cookie;  // ouch !!!!!
  const char* name              = param->name;
  const char* module            = param->module;
  uint32_t    flags             = param->flags;
  bool        is_stepped        = flags & CLAP_PARAM_IS_STEPPED;
  bool        is_per_note       = flags & CLAP_PARAM_IS_PER_NOTE;
  bool        is_per_channel    = flags & CLAP_PARAM_IS_PER_CHANNEL;
  bool        is_per_port       = flags & CLAP_PARAM_IS_PER_PORT;
  bool        is_periodic       = flags & CLAP_PARAM_IS_PERIODIC;
  bool        is_hidden         = flags & CLAP_PARAM_IS_HIDDEN;
  bool        is_bypass         = flags & CLAP_PARAM_IS_BYPASS;
  bool        is_readonly       = flags & CLAP_PARAM_IS_READONLY;
  bool        is_modulatable    = flags & CLAP_PARAM_IS_MODULATABLE;
  bool        requires_process  = flags & CLAP_PARAM_REQUIRES_PROCESS;
  json.objectBegin(nullptr);
  json.u32("id",(uint32_t*)&param->id);
  json.u32("flags",(uint32_t*)&param->flags);
  json.arrayBegin("flags_ext");
  json.boolean("CLAP_PARAM_IS_STEPPED",&is_stepped);
  json.boolean("CLAP_PARAM_IS_PER_NOTE",&is_per_note);
  json.boolean("CLAP_PARAM_IS_PER_CHANNEL",&is_per_channel);
  json.boolean("CLAP_PARAM_IS_PER_PORT",&is_per_port);
  json.boolean("CLAP_PARAM_IS_PERIODIC",&is_periodic);
  json.boolean("CLAP_PARAM_IS_HIDDEN",&is_hidden);
  json.boolean("CLAP_PARAM_IS_BYPASS",&is_bypass);
  json.boolean("CLAP_PARAM_IS_READONLY",&is_readonly);
  json.boolean("CLAP_PARAM_IS_MODULATABLE",&is_modulatable);
  json.boolean("CLAP_PARAM_REQUIRES_PROCESS",&requires_process);
  json.arrayEnd();
  json.u64("cookie",&cookie);
  json.string("name",(const char**)&name);
  json.string("module",(const char**)&module);
  json.f64("min_value",(double*)&param->min_value);
  json.f64("max_value",(double*)&param->max_value);
  json.f64("default_value",(double*)&param->default_value);
  json.objectEnd();
  if (json.error()) {
    printf("%s\n",json.errorMessage());
  }
}

//----------------------------------------------------------------------
//
// stdout
//
//----------------------------------------------------------------------

//    printf("plugins:\n");
//    const clap_plugin_factory* factory = host->getClapFactory();
//    uint32_t num = factory->get_plugin_count(factory);
//    for (uint32_t i=0; i<num; i++) {
//      const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,i);
//      printf("  %i. %s (%s)\n",i,descriptor->name,descriptor->description);
//    }

void print_stdout_plugin_list(Host* host) {
  //write_stdout_plugin_list(host)
  printf("plugins:\n");
  const clap_plugin_factory* factory = host->getClapFactory();
  uint32_t num = factory->get_plugin_count(factory);
  for (uint32_t i=0; i<num; i++) {
    const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,i);
    printf("  %i. %s (%s)\n",i,descriptor->name,descriptor->description);
  }
}

//----------

//    printf("descriptor (%i):\n",index);
//    const clap_plugin_factory* factory = host->getClapFactory();
//    const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,index);
//    printf("  clap_version: %i.%i.%i\n",descriptor->clap_version.major,descriptor->clap_version.minor,descriptor->clap_version.revision);
//    printf("  id:           %s\n",descriptor->id);
//    printf("  name:         %s\n",descriptor->name);
//    printf("  vendor:       %s\n",descriptor->vendor);
//    printf("  url:          %s\n",descriptor->url);
//    printf("  manual_url:   %s\n",descriptor->manual_url);
//    printf("  support_url:  %s\n",descriptor->support_url);
//    printf("  version:      %s\n",descriptor->version);
//    printf("  description:  %s\n",descriptor->description);
//    //printf("  features:     %s\n",descriptor->features);
//    printf("  features:     ");
//    int i = 0;
//    while ( descriptor->features[i] ) {
//      printf("%s ",descriptor->features[i]);
//      i++;
//    }

//void print_stdout_descriptor(Host* host, uint32_t index) {
void print_stdout_descriptor(const clap_plugin_descriptor_t* descriptor) {
  //printf("descriptor (%i):\n",index);
  //const clap_plugin_factory* factory = host->getClapFactory();
  //const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,index);
  printf("  clap_version: %i.%i.%i\n",descriptor->clap_version.major,descriptor->clap_version.minor,descriptor->clap_version.revision);
  printf("  id:           %s\n",descriptor->id);
  printf("  name:         %s\n",descriptor->name);
  printf("  vendor:       %s\n",descriptor->vendor);
  printf("  url:          %s\n",descriptor->url);
  printf("  manual_url:   %s\n",descriptor->manual_url);
  printf("  support_url:  %s\n",descriptor->support_url);
  printf("  version:      %s\n",descriptor->version);
  printf("  description:  %s\n",descriptor->description);
  //printf("  features:     %s\n",descriptor->features);
  printf("  features:     ");
  int i = 0;
  while ( descriptor->features[i] ) {
    printf("%s ",descriptor->features[i]);
    i++;
  }
}

//----------

void print_stdout_parameter(const clap_param_info_t* param) {
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void print_plugin_list(Host* host, const char* json_file=nullptr) {
  if (json_file) print_json_plugin_list(host);
  else print_stdout_plugin_list(host);
}

//----------

void print_plugin_descriptor(Host* host, uint32_t index, const char* json_file=nullptr) {
  const clap_plugin_factory* factory = host->getClapFactory();
  const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,index);
  if (json_file) print_json_descriptor(descriptor);
  else print_stdout_descriptor(descriptor);
}

//----------

void print_plugin_parameters(const clap_plugin_t* plugin, const char* json_file=nullptr) {
  const clap_plugin_params_t* params = (const clap_plugin_params_t*)plugin->get_extension(plugin,CLAP_EXT_PARAMS);
  if (params) {
    uint32_t num = params->count(plugin);
    for (uint32_t i=0; i<num; i++) {
      clap_param_info_t param_info;
      if (params->get_info(plugin,i,&param_info)) {
        if (json_file) print_json_parameter(&param_info);
        else print_stdout_parameter(&param_info);
      }
    }
  }
}

//----------------------------------------------------------------------
#endif
