#include "plugin/mip_plugin.h"

const clap_plugin_descriptor_t minimal_descriptor = {
  CLAP_VERSION, "MIP/minimal/0.0.0", "minimal", "MIP2", "", "", "", "0.0.0", "", (const char *[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

class minimal_plugin : public MIP_Plugin {
public:
  minimal_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) : MIP_Plugin(ADescriptor,AHost) {}
};

#include "plugin/clap/mip_clap_entry.h"
MIP_DEFAULT_ENTRY(minimal_descriptor,minimal_plugin);
