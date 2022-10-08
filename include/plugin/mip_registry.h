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
    //MIP_PRINT;
    //if (MIP_Register) MIP_Register(this);
    initialize();
  }

  //----------

  ~MIP_Registry() {
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
   strcpy(MPath,path);
   //MIniFile.load(path);
 }

 const char* getPath() {
   return MPath;
 }

  void initialize() {
    if (!MInitialized) {
      if (MIP_Register) MIP_Register();
    }
    MInitialized = true;
  }


//------------------------------
public: // factories
//------------------------------

  bool appendFactory(const char* id, const clap_plugin_factory_t* factory) {
    //MIP_Print("id: %s\n",id);
    MFactoryIds.append(id);
    MFactories.append(factory);
    return true;
  }

  //----------

  void deleteFactories() {
    //MIP_Print("\n");
    uint32_t num = MFactories.size();
    for (uint32_t i=0; i<num; i++) {
      delete MFactoryIds[i];
      delete MFactories[i];
    }
  }

  //----------

  int32_t getNumFactories() {
    //MIP_Print("-> %i\n",MFactories.size());
    return MFactories.size();
  }

  //----------

  const void* findFactoryById(const char* factory_id) {
    //MIP_Print("id: %s\n",factory_id);
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
    //MIP_Print("%i. %s\n",MDescriptors.size(),descriptor->name);
    MDescriptors.append(descriptor);
    return true;

  }

  //----------

  void deleteDescriptors() {
    //MIP_Print("\n");
    uint32_t num = MDescriptors.size();
    for (uint32_t i=0; i<num; i++) {
      delete MDescriptors[i];
      MDescriptors[i] = nullptr;
    }
  }

  //----------

  void freeDescriptors() {
    //MIP_Print("\n");
    uint32_t num = MDescriptors.size();
    for (uint32_t i=0; i<num; i++) {
      free( (void*)MDescriptors[i] );
      MDescriptors[i] = nullptr;
    }
  }

  //----------

  int32_t getNumDescriptors() {
    //MIP_Print("-> %i\n",MDescriptors.size());
    return MDescriptors.size();
  }

  //----------

  const clap_plugin_descriptor_t* getDescriptor(int32_t index) {
    //MIP_Print("%i\n",index);
    return MDescriptors[index];
  }

  //----------

  int32_t findDescriptorById(const char* plugin_id) {
    //MIP_Print("id %s\n",plugin_id);
    uint32_t num = MDescriptors.size();
    for (uint32_t i=0; i<num; i++) {
      if (strcmp(plugin_id,MDescriptors[i]->id) == 0) return i;
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
#endif
