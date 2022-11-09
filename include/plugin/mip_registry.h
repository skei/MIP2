#ifndef mip_registry_included
#define mip_registry_included
//----------------------------------------------------------------------



#include "mip.h"
#include "base/types/mip_array.h"
#include "plugin/mip_plugin.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/clap/mip_clap_factory.h"

//----------------------------------------------------------------------

class MIP_Registry;
extern void MIP_Register() __MIP_WEAK;
extern void MIP_Unregister() __MIP_WEAK;
extern MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __MIP_WEAK;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Registry {

//------------------------------
private:
//------------------------------

  bool                                        MInitialized  = false;
  char                                        MPath[1024]   = {0};  // plugin path
  MIP_Array<const char*>                      MFactoryIds   = {};   // factory id's
  MIP_Array<const clap_plugin_factory_t*>     MFactories    = {};   // factories
  MIP_Array<const clap_plugin_descriptor_t*>  MDescriptors  = {};   // plugin descriptors

//------------------------------
public:
//------------------------------

  MIP_Registry() {
    //LOG.print("REGISTRY: ctor\n");
    //MIP_PRINT;
    //if (MIP_Register) MIP_Register(this);
    initialize();
  }

  //----------

  ~MIP_Registry() {
    //LOG.print("REGISTRY: dtor\n");
    //MIP_PRINT;
    if (MIP_Unregister) MIP_Unregister();
    #ifndef MIP_NO_AUTODELETE
      deleteFactories();
      //deleteDescriptors();  // must be done manually, since we have many ways
      //freeDescriptors();    // to define and register descriptors
    #endif
  }

//------------------------------
public:
//------------------------------

 void setPath(const char* path) {
    //LOG.print("REGISTRY: setPath '%s'\n",path);
   strcpy(MPath,path);
   //MIniFile.load(path);
 }

 const char* getPath() {
    //LOG.print("REGISTRY: getPath -> '%s'\n",MPath);
   return MPath;
 }

  void initialize() {
    //LOG.print("REGISTRY: initialize\n");
    if (!MInitialized) {
      if (MIP_Register) MIP_Register();
    }
    MInitialized = true;
  }


//------------------------------
public: // factories
//------------------------------

  bool appendFactory(const char* id, const clap_plugin_factory_t* factory) {
    //LOG.print("REGISTRY: appendFactory '%s' = %p\n",id,factory);
    //MIP_Print("id: %s\n",id);
    MFactoryIds.append(id);
    MFactories.append(factory);
    return true;
  }

  //----------

  void deleteFactories() {
    //LOG.print("REGISTRY: deleteFactories\n");
    //MIP_Print("\n");
    uint32_t num = MFactories.size();
    for (uint32_t i=0; i<num; i++) {
      delete MFactoryIds[i];
      delete MFactories[i];
    }
  }

  //----------

  int32_t getNumFactories() {
    int32_t num = MFactories.size();
    //LOG.print("REGISTRY: getNumFactories -> %i\n",num);
    //MIP_Print("-> %i\n",MFactories.size());
    return num;
  }

  //----------

  const void* findFactoryById(const char* factory_id) {
    //MIP_Print("id: %s\n",factory_id);
    //LOG.print("REGISTRY: findFactoryById '%s'\n",factory_id);
    uint32_t num = MFactories.size();
    for (uint32_t i=0; i<num; i++) {
      if (strcmp(factory_id,MFactoryIds[i]) == 0) {
        return MFactories[i];
      }
    }
    return nullptr;
  }

//------------------------------
public: // descriptors
//------------------------------

  bool appendDescriptor(const clap_plugin_descriptor_t* descriptor) {
    //LOG.print_if(!descriptor,"REGISTRY: 'descriptor' is null\n");
    //LOG.print("REGISTRY: Appending descriptor '%s' : index %i\n",descriptor->name,MDescriptors.size());
    //MIP_Print("%i. %s\n",MDescriptors.size(),descriptor->name);
    MDescriptors.append(descriptor);
    return true;

  }

  //----------

  void deleteDescriptors() {
    //MIP_Print("\n");
    uint32_t num = MDescriptors.size();
    //LOG.print("REGISTRY: delete %i descriptors\n",num);
    for (uint32_t i=0; i<num; i++) {
      delete MDescriptors[i];
      MDescriptors[i] = nullptr;
    }
  }

  //----------

  void freeDescriptors() {
    //MIP_Print("\n");
    uint32_t num = MDescriptors.size();
    //LOG.print("REGISTRY: free %i descriptors\n",num);
    for (uint32_t i=0; i<num; i++) {
      free( (void*)MDescriptors[i] );
      MDescriptors[i] = nullptr;
    }
  }

  //----------

  int32_t getNumDescriptors() {
    //MIP_Print("-> %i\n",MDescriptors.size());
    uint32_t num = MDescriptors.size();
    //LOG.print("REGISTRY: getNumDescriptors -> %i\n",num);
    return num;
  }

  //----------

  const clap_plugin_descriptor_t* getDescriptor(int32_t index) {
    //MIP_Print("%i\n",index);
    //LOG.print("REGISTRY: getDescriptor %i -> %p\n",index,MDescriptors[index]);
    return MDescriptors[index];
  }

  //----------

  int32_t findDescriptorById(const char* plugin_id) {
    //LOG.print("REGISTRY: findDescriptorById '%s'\n",plugin_id);
    //MIP_Print("id %s\n",plugin_id);
    uint32_t num = MDescriptors.size();
    for (uint32_t i=0; i<num; i++) {
      if (strcmp(plugin_id,MDescriptors[i]->id) == 0) {
        return i;
      }
    }
    return -1;
  }

};

//----------------------------------------------------------------------
//
// global registry object
//
//----------------------------------------------------------------------

MIP_Registry MIP_REGISTRY = {};

//----------------------------------------------------------------------
//----------------------------------------------------------------------

#define MIP_DEFAULT_ENTRY(DESC,PLUG)                                                                                      \
                                                                                                                          \
  void MIP_Register() {                                                                                                   \
    MIP_REGISTRY.appendDescriptor(&DESC);                                                                                 \
  };                                                                                                                      \
                                                                                                                          \
  MIP_Plugin* MIP_CreatePlugin( uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) { \
    if (AIndex == 0) return new PLUG(ADescriptor,AHost);                                                                  \
    return nullptr;                                                                                                       \
  }                                                                                                                       \

//----------------------------------------------------------------------
#endif
