#ifndef mip_clap_plugin_entry_included
#define mip_clap_plugin_entry_included
//----------------------------------------------------------------------

/*
  clap_plugin_entry*
  HostedPluginEntry
*/

//----------------------------------------------------------------------

#include <dlfcn.h>
#include "plugin/clap/mip_clap.h"

//#include "system.h"
//#include "utils.h"

#include "mip_clap_host.h"
#include "mip_clap_plugin.h"

//----------------------------------------------------------------------

class MIP_ClapPluginEntry {

//------------------------------
private:
//------------------------------

  void*                     MLibHandle;
  const char*               MPluginPath;
  char                      MPathOnly[512];
  struct clap_plugin_entry* MClapEntry;
  MIP_ClapHost              MHost;

//------------------------------
public:
//------------------------------

  MIP_ClapPluginEntry() {
  }

  //----------

  ~MIP_ClapPluginEntry() {
  }

//------------------------------
private:
//------------------------------

  // these are a bit hacky..

  //----------

  // /home/skei/test.so -> test.so
  // returns ptr to first character after last /

  const char* get_filename_from_path(const char* path) {
    if (path) {
      const char* slash     = strrchr(path,'/');
      const char* backslash = strrchr(path,'\\');
      if (slash) {
        return slash + 1;
      }
      else if (backslash) {
        return backslash + 1;
      }
    }
    return NULL;
  }

  //----------

  // /home/skei/test.so -> /home/skei/
  // inserts a 0 after the last /

  char* get_path_only(char* dst, const char* src) {
    if (dst && src) {
      strcpy(dst,src);
      char* slash     = strrchr(dst,'/');
      char* backslash = strrchr(dst,'\\');
      if (slash) {
        slash[1] = 0;
        return dst;
      }
      else if (backslash) {
        backslash[1] = 0;
        return dst;
      }
    }
    return NULL;
  }

//------------------------------
public:
//------------------------------

  // load .so
  // entry.init

  bool load(const char* APath) {
    MPluginPath = APath;
    printf("Loading binary '%s'\n",APath);
    MLibHandle = dlopen(APath,RTLD_LAZY|RTLD_LOCAL); // RTLD_NOW
    if (!MLibHandle) {
      printf("* Error: Couldn't open '%s'\n", APath);
      return false;
    }
    printf("Binary loaded\n");
    printf("Getting 'clap_plugin_entry'\n");
    MClapEntry = (struct clap_plugin_entry*)dlsym(MLibHandle,"clap_plugin_entry");
    if (!MClapEntry) {
      printf("* Error: Couldn't find 'clap_plugin_entry'\n");
      dlclose(MLibHandle);
      return 0;
    }
    printf("Found 'clap_plugin_entry'\n");
    get_path_only(MPathOnly,APath);
    MClapEntry->init(MPathOnly);

    return 1;
  }

  //----------

  // entry.deinit
  // unload .so

  void unload() {
    printf("Unloading binary\n");
    MClapEntry->deinit();
    dlclose(MLibHandle);
    printf("Binary unloaded\n");
  }

  //----------

  // create and initialize a plugin instance

  MIP_ClapPlugin* createPlugin(const char* APath, uint32_t AIndex) {
    printf("Creating plugin (index %i)\n",AIndex);

    if (AIndex >= MClapEntry->get_plugin_count()) {
      printf("* Error: Index out of bounds\n");
      return NULL;
    }

    const clap_plugin_descriptor* descriptor = MClapEntry->get_plugin_descriptor(AIndex);
    if (!descriptor) {
      printf("* Error: Couldn't get descriptor\n");
      return NULL;
    }

    printf("Creating plugin instance\n");
    const clap_plugin* plugin = MClapEntry->create_plugin( MHost.getClapHost(), descriptor->id );
    if (!plugin) {
      printf("* Error: Couldn't create plugin instance\n");
      return NULL;
    }
    printf("Plugin instance created\n");

    bool result = plugin->init(plugin);
    if (!result) {
      printf("* Error: Couldn't initialize plugin instance\n");
      return NULL;
    }
    printf("Plugin instance initialized\n");

    return new MIP_ClapPlugin(plugin);

  }

  //----------

  // destroy instance

  void destroyPlugin(MIP_ClapPlugin* APlugin) {
    const clap_plugin* plugin = APlugin->getClapPlugin();
    printf("Destroying plugin instance\n");
    plugin->destroy(plugin);
    printf("Plugin instance destroyed\n");
  }

  //----------

  // list all (sub-) plugins in a binary .so

  void listPlugins() {
    uint32_t plugin_count = MClapEntry->get_plugin_count();
    printf("> found %i plugins\n",plugin_count);
    for (uint32_t i=0; i<plugin_count; i++) {
      const clap_plugin_descriptor* descriptor = MClapEntry->get_plugin_descriptor(i);
      printf("  %i. name '%s' id '%s'\n",i,descriptor->name,descriptor->id);
    }
  }

  //----------

  // print out descriptor of selected (sub-) plugin index

  void printDescriptor(uint32_t AIndex) {
    const clap_plugin_descriptor* descriptor = MClapEntry->get_plugin_descriptor(AIndex);
    if (descriptor) {
      printf("Descriptor:\n");
      printf("  clap_version: %i.%i.%i\n",descriptor->clap_version.major,descriptor->clap_version.minor,descriptor->clap_version.revision);
      printf("  id            %s\n",      descriptor->id);
      printf("  name          %s\n",      descriptor->name);
      printf("  vendor        %s\n",      descriptor->vendor);
      printf("  url           %s\n",      descriptor->url);
      printf("  manual_url    %s\n",      descriptor->manual_url);
      printf("  support_url   %s\n",      descriptor->support_url);
      printf("  version       %s\n",      descriptor->version);
      printf("  description   %s\n",      descriptor->description);
      printf("  plugin_type   %i\n", (int)descriptor->plugin_type);
    }
  }

  //----------

};

//----------------------------------------------------------------------
#endif
