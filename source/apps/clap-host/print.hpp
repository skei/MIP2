#ifndef print_included
#define print_included
//----------------------------------------------------------------------

#define JSON_BUFFER_SIZE (1024*1024)

//----------------------------------------------------------------------
//
// json
//
//----------------------------------------------------------------------

#include "json.hpp"

Json  json                    = {};
char  json_buffer[1024*1024]  = {0};

//----------

void json_begin() {
  json_buffer[0] = 0;
  json.setupWrite(true,json_buffer,JSON_BUFFER_SIZE);
  json.objectBegin(nullptr);
}

//----------

void json_end(const char* filename) {
  json.objectEnd();
  if (json.error()) {
    printf("%s\n",json.errorMessage());
  }
  uint32_t json_size = strlen(json_buffer);
  FILE* fp = fopen(filename,"wt");
  fwrite(json_buffer,1,json_size,fp);
  fclose(fp);
}

//----------------------------------------------------------------------

void print_json_plugin_list(Host* host) {
  json.arrayBegin("plugin list");
    const clap_plugin_factory* factory = host->getClapFactory();
    uint32_t num = factory->get_plugin_count(factory);
    for (uint32_t i=0; i<num; i++) {
        json.objectBegin(nullptr);
        const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,i);
        json.i32("index",(int32_t *)&i);
        json.string("name",(const char **)&descriptor->name);
        json.string("description",(const char **)&descriptor->description);
      json.objectEnd();
    }
  json.arrayEnd();
}

//----------

void print_json_descriptor(const clap_plugin_descriptor_t* desc) {
  char version_string[32] = {0};
  sprintf(version_string,"%i.%i.%i",desc->clap_version.major,desc->clap_version.minor,desc->clap_version.revision);
  const char* version_ptr = (const char*)version_string;
  json.objectBegin("descriptor");
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
        json.string(nullptr,(const char**)&desc->features[i]);
        i++;
      }
    json.arrayEnd();
  json.objectEnd();
}

//----------

//   CLAP_PARAM_IS_STEPPED
//   CLAP_PARAM_IS_PERIODIC
//   CLAP_PARAM_IS_HIDDEN
//   CLAP_PARAM_IS_READONLY
//   CLAP_PARAM_IS_BYPASS
//   CLAP_PARAM_IS_AUTOMATABLE
//   CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE
//   CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL
//   CLAP_PARAM_IS_AUTOMATABLE_PER_PORT
//   CLAP_PARAM_IS_MODULATABLE
//   CLAP_PARAM_IS_MODULATABLE_PER_NOTE
//   CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
//   CLAP_PARAM_IS_MODULATABLE_PER_PORT
//   CLAP_PARAM_REQUIRES_PROCESS

void print_json_parameter(uint32_t index, const clap_param_info_t* param) {
  const char* name                        = param->name;
  const char* module                      = param->module;
  uint32_t    flags                       = param->flags;
  bool        is_stepped                  = flags & CLAP_PARAM_IS_STEPPED;
  bool        is_periodic                 = flags & CLAP_PARAM_IS_PERIODIC;
  bool        is_hidden                   = flags & CLAP_PARAM_IS_HIDDEN;
  bool        is_readonly                 = flags & CLAP_PARAM_IS_READONLY;
  bool        is_bypass                   = flags & CLAP_PARAM_IS_BYPASS;
  bool        is_automatable              = flags & CLAP_PARAM_IS_AUTOMATABLE;
  bool        is_automatable_per_note_id     = flags & CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE_ID;
  bool        is_automatable_per_channel  = flags & CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL;
  bool        is_automatable_per_port     = flags & CLAP_PARAM_IS_AUTOMATABLE_PER_PORT;
  bool        is_modulatable              = flags & CLAP_PARAM_IS_MODULATABLE;
  bool        is_modulatable_per_note_id  = flags & CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID;
  bool        is_modulatable_per_channel  = flags & CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL;
  bool        is_modulatable_per_port     = flags & CLAP_PARAM_IS_MODULATABLE_PER_PORT;
  bool        requires_process            = flags & CLAP_PARAM_REQUIRES_PROCESS;

  json.objectBegin(nullptr);
    json.u32("id",(uint32_t*)&param->id);
    //json.u32("flags",(uint32_t*)&param->flags);
    json.objectBegin("flags");
      json.boolean("CLAP_PARAM_IS_STEPPED",&is_stepped);
      json.boolean("CLAP_PARAM_IS_PERIODIC",&is_periodic);
      json.boolean("CLAP_PARAM_IS_HIDDEN",&is_hidden);
      json.boolean("CLAP_PARAM_IS_READONLY",&is_readonly);
      json.boolean("CLAP_PARAM_IS_BYPASS",&is_bypass);
      json.boolean("CLAP_PARAM_IS_AUTOMATABLE",&is_automatable);
      json.boolean("CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE_ID",&is_automatable_per_note_id);
      json.boolean("CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL",&is_automatable_per_channel);
      json.boolean("CLAP_PARAM_IS_AUTOMATABLE_PER_PORT",&is_automatable_per_port);
      json.boolean("CLAP_PARAM_IS_MODULATABLE",&is_modulatable);
      json.boolean("CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID",&is_modulatable_per_note_id);
      json.boolean("CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL",&is_modulatable_per_channel);
      json.boolean("CLAP_PARAM_IS_MODULATABLE_PER_PORT",&is_modulatable_per_port);
      json.boolean("CLAP_PARAM_REQUIRES_PROCESS",&requires_process);
    json.objectEnd();
    json.string("name",(const char**)&name);
    json.string("module",(const char**)&module);
    json.f64("min_value",(double*)&param->min_value);
    json.f64("max_value",(double*)&param->max_value);
    json.f64("default_value",(double*)&param->default_value);
  json.objectEnd();
}

//----------------------------------------------------------------------
//
// stdout
//
//----------------------------------------------------------------------

void print_stdout_plugin_list(Host* host) {
  printf("plugins:\n");
  const clap_plugin_factory* factory = host->getClapFactory();
  uint32_t num = factory->get_plugin_count(factory);
  for (uint32_t i=0; i<num; i++) {
    const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,i);
    printf("  %i. %s (%s)\n",i,descriptor->name,descriptor->description);
  }
}

//----------

void print_stdout_descriptor(const clap_plugin_descriptor_t* descriptor) {
  printf("  clap_version: %i.%i.%i\n",descriptor->clap_version.major,descriptor->clap_version.minor,descriptor->clap_version.revision);
  printf("  id:           %s\n",descriptor->id);
  printf("  name:         %s\n",descriptor->name);
  printf("  vendor:       %s\n",descriptor->vendor);
  printf("  url:          %s\n",descriptor->url);
  printf("  manual_url:   %s\n",descriptor->manual_url);
  printf("  support_url:  %s\n",descriptor->support_url);
  printf("  version:      %s\n",descriptor->version);
  printf("  description:  %s\n",descriptor->description);
  printf("  features:     ");
  int i = 0;
  while ( descriptor->features[i] ) {
    printf("%s ",descriptor->features[i]);
    i++;
  }
  printf("\n");

}

//----------

void print_stdout_parameter(uint32_t index, const clap_param_info_t* param) {
  const char* name              = param->name;
  const char* module            = param->module;
  uint32_t    flags             = param->flags;

  bool        is_stepped                  = flags & CLAP_PARAM_IS_STEPPED;
  bool        is_periodic                 = flags & CLAP_PARAM_IS_PERIODIC;
  bool        is_hidden                   = flags & CLAP_PARAM_IS_HIDDEN;
  bool        is_readonly                 = flags & CLAP_PARAM_IS_READONLY;
  bool        is_bypass                   = flags & CLAP_PARAM_IS_BYPASS;
  bool        is_automatable              = flags & CLAP_PARAM_IS_AUTOMATABLE;
  bool        is_automatable_per_note_id  = flags & CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE_ID;
  bool        is_automatable_per_channel  = flags & CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL;
  bool        is_automatable_per_port     = flags & CLAP_PARAM_IS_AUTOMATABLE_PER_PORT;
  bool        is_modulatable              = flags & CLAP_PARAM_IS_MODULATABLE;
  bool        is_modulatable_per_note_id  = flags & CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID;
  bool        is_modulatable_per_channel  = flags & CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL;
  bool        is_modulatable_per_port     = flags & CLAP_PARAM_IS_MODULATABLE_PER_PORT;
  bool        requires_process            = flags & CLAP_PARAM_REQUIRES_PROCESS;

  printf("\n");
  printf("  id: %i\n",param->id);
  printf("  name:          %s\n",name);
  printf("  module:        %s\n",module);
  printf("  min_value:     %.3f\n",param->min_value);
  printf("  max_value:     %.3f\n",param->max_value);
  printf("  default_value: %.3f\n",param->default_value);
  printf("  flags:         0x%04X\n",param->flags);

  if (is_stepped)                 printf("    CLAP_PARAM_IS_STEPPED\n");
  if (is_periodic)                printf("    CLAP_PARAM_IS_PERIODIC\n");
  if (is_hidden)                  printf("    CLAP_PARAM_IS_HIDDEN\n");
  if (is_readonly)                printf("    CLAP_PARAM_IS_READONLY\n");
  if (is_bypass)                  printf("    CLAP_PARAM_IS_BYPASS\n");
  if (is_automatable)             printf("    CLAP_PARAM_IS_AUTOMATABLE");
  if (is_automatable_per_note_id) printf("    CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE_ID\n");
  if (is_automatable_per_channel) printf("    CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL\n");
  if (is_automatable_per_port)    printf("    CLAP_PARAM_IS_AUTOMATABLE_PER_PORT\n");
  if (is_modulatable)             printf("    CLAP_PARAM_IS_MODULATABLE\n");
  if (is_modulatable_per_note_id) printf("    CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID\n");
  if (is_modulatable_per_channel) printf("    CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL\n");
  if (is_modulatable_per_port)    printf("    CLAP_PARAM_IS_MODULATABLE_PER_PORT\n");
  if (requires_process)           printf("    CLAP_PARAM_REQUIRES_PROCESS\n");

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
    json.arrayBegin("parameter list");
    for (uint32_t i=0; i<num; i++) {
      clap_param_info_t param_info;
      if (params->get_info(plugin,i,&param_info)) {
        if (json_file) print_json_parameter(i,&param_info);
        else print_stdout_parameter(i,&param_info);
      }
    }
    json.arrayEnd();
  }
}

//----------------------------------------------------------------------
#endif
