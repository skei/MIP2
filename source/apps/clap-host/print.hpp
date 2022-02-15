#ifndef print_included
#define print_included
//----------------------------------------------------------------------

#include "json.hpp"

Json json = {};

//----------

void print_plugin_list(Host* host, const char* json_file=nullptr) {
  if (json_file) {
  }
  else {
    printf("plugins:\n");
    const clap_plugin_factory* factory = host->getClapFactory();
    uint32_t num = factory->get_plugin_count(factory);
    for (uint32_t i=0; i<num; i++) {
      const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,i);
      printf("  %i. %s (%s)\n",i,descriptor->name,descriptor->description);
    }
  }
}

//----------

void print_plugin_descriptor(Host* host, uint32_t index, const char* json_file=nullptr) {
  if (json_file) {
  }
  else {
    printf("descriptor (%i):\n",index);
    const clap_plugin_factory* factory = host->getClapFactory();
    const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,index);
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
}


//----------------------------------------------------------------------
#endif
