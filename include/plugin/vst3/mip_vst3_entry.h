#ifndef mip_vst3_entry_included
#define mip_vst3_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/vst3/mip_vst3.h"
#include "plugin/vst3/mip_vst3_host_implementation.h"
#include "plugin/vst3/mip_vst3_plugin.h"
#include "plugin/vst3/mip_vst3_utils.h"

#include "plugin/mip_plugin.h"

//#ifndef MIP_NO_GUI
//#include "plugin/mip_editor.h"
//#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Vst3Entry
: public IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t      MRefCount     = 1;
  FUnknown*     MHostContext  = nullptr;
  char          MPluginId[16] = {0};
  char          MEditorId[16] = {0};

//------------------------------
public:
//------------------------------

  MIP_Vst3Entry() {
    MIP_Print("MIP_Vst3Entry\n");
    MRefCount = 1;
  }

  //----------

  virtual ~MIP_Vst3Entry() {
    MIP_Print("~MIP_Vst3Entry\n");
  }

//------------------------------
private:
//------------------------------


  //#define MIP_MAGIC_M_PL   0x4d5f504c    // M_PL   // plugin
  //#define MIP_MAGIC_M_ED   0x4d5f4544    // M_ED   // editor

  //----------

  const char* getLongId(const clap_plugin_descriptor_t* descriptor) {
    uint32_t* id = (uint32_t*)MPluginId;
    id[0] = MIP_MAGIC_M_PL;
    id[1] = MIP_HashString(descriptor->name);
    id[2] = MIP_HashString(descriptor->vendor);
    id[3] = MIP_HashString(descriptor->version);
    return (const char*)id;
  }

  //----------

  int32_t findPluginIndex(const char* cid) {
    uint32_t num = MIP_REGISTRY.getNumDescriptors();
    for (uint32_t i=0; i<num; i++) {
      const clap_plugin_descriptor_t* desc = MIP_REGISTRY.getDescriptor(i);
      const char* id = getLongId(desc);
      if (VST3_iidEqual(cid,id)) { return i; }
    }
    return -1;
  }

  //----------

  bool isInstrument(const clap_plugin_descriptor_t* descriptor) {
    const char** feature = descriptor->features;
    uint32_t index = 0;
    while (feature[index]) {
      if (strcmp(feature[index], "instrument") == 0) return true;
      index++;
    }
    return false;
  }


//------------------------------
public:
//------------------------------

  //--------------------
  // FUnknown
  //--------------------

  uint32 PLUGIN_API addRef() override {
    MIP_Print("MIP_Vst3Entry.addRef\n");
    MRefCount++;
    return MRefCount;
  }

  //----------

  uint32 PLUGIN_API release() override {
    MIP_Print("MIP_Vst3Entry.release\n");
    const uint32_t r = --MRefCount;
    if (r == 0) delete this;
    return r;
  }

  //----------

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    MIP_Print("MIP_Vst3Entry.queryInterface\n");
    if (VST3_iidEqual(_iid,IPluginFactory2_iid)) {
      *obj = (IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    if (VST3_iidEqual(_iid,IPluginFactory3_iid)) {
      *obj = (IPluginFactory3*)this;
      addRef();
      return kResultOk;
    }
    *obj = nullptr;
    return kNoInterface;
  }

  //--------------------
  // IPluginFactory
  //--------------------

  tresult PLUGIN_API getFactoryInfo(PFactoryInfo* info) override {
    MIP_Print("MIP_Vst3Entry.getFactoryInfo\n");
    strcpy(info->vendor,"<factory author>");
    strcpy(info->url,"<factory url>");
    strcpy(info->email,"<factory email>");
    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  //----------


  int32   PLUGIN_API countClasses() override {
    MIP_Print("MIP_Vst3Entry.countClasses\n");
    return MIP_REGISTRY.getNumDescriptors();
  }

  //----------

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) override {
    MIP_Print("MIP_Vst3Entry.getClassInfo\n");
    const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);
    const char* long_id = getLongId(descriptor);
    memcpy(info->cid,long_id,16);
    info->cardinality = PClassInfo::kManyInstances;
    strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
    strncpy(info->name,descriptor->name,PClassInfo::kNameSize);
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) override {
    int32_t index = findPluginIndex(cid);
    if (index < 0) return kNotImplemented;
    const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);
    //MIP_Vst3Host* vst3_host = new MIP_Vst3Host();
    MIP_Vst3HostImplementation* vst3_host = new MIP_Vst3HostImplementation();
    //MIP_Print("vst3_host: %p\n",vst3_host);
    //MIP_Print("vst3_host->getHost(): %p\n",vst3_host->getHost());
    MIP_ClapPlugin* plugin = MIP_CreatePlugin(index,descriptor,vst3_host->getHost());
    plugin->init();
    MIP_Vst3Plugin* vst3plugin = new MIP_Vst3Plugin(plugin,vst3_host);
    MIP_Plugin* pl = (MIP_Plugin*)plugin;
    MIP_ParameterArray* pa = pl->getParameters();
    vst3plugin->setParameters(pa);
//      plugin->setListener(vst3_instance);
//      plugin->on_plugin_open();
//      plugin->setDefaultParameterValues();
//      plugin->updateAllParameters();
      *obj = (Vst::IComponent*)vst3plugin;
      return kResultOk;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) override {
    MIP_Print("MIP_Vst3Entry.getClassInfo2\n");
    const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);
    const char* long_id = getLongId(descriptor);
    memcpy(info->cid,long_id,16);
    info->cardinality = PClassInfo::kManyInstances;
    strcpy(info->category,kVstAudioEffectClass);
    strcpy(info->name,descriptor->name);
    info->classFlags = 0;
    if (isInstrument(descriptor)) strcpy(info->subCategories,Vst::PlugType::kInstrument);
    else strcpy(info->subCategories,Vst::PlugType::kFx);
    strcpy(info->vendor,descriptor->vendor);
    strcpy(info->version,descriptor->version);
    strcpy(info->sdkVersion,kVstVersionString);
    return kResultOk;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  tresult PLUGIN_API getClassInfoUnicode(int32 index, PClassInfoW* info) override {
    MIP_Print("MIP_Vst3Entry.getClassInfoUnicode\n");
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API setHostContext(FUnknown* context) override {
    MIP_Print("MIP_Vst3Entry.setHostContext\n");
    MHostContext = context;
    return kResultOk;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define MIP_VST3_ENTRY_SYMBOL asm ("GetPluginFactory");
//IPluginFactory* PLUGIN_API mip_vst3_entry() MIP_VST3_ENTRY_SYMBOL

IPluginFactory* PLUGIN_API mip_vst3_entry() asm ("GetPluginFactory");
bool vst3_module_entry(void* sharedLibraryHandle) asm ("ModuleEntry");
bool vst3_module_exit(void) asm ("ModuleExit");

//----------

//__MIP_EXPORT
__attribute__ ((visibility ("default")))
IPluginFactory* PLUGIN_API mip_vst3_entry() {
    MIP_Print("GetPluginFactory\n");
  return new MIP_Vst3Entry();
}

//----------

//void* moduleHandle = nullptr;
//static int counter {0};

//__MIP_EXPORT
__attribute__ ((visibility ("default")))
bool vst3_module_entry(void* sharedLibraryHandle)  {
  MIP_Print("ModuleEntry\n");
  //if (++counter == 1) {
  //  moduleHandle = sharedLibraryHandle;
  //  return true;
  //}
  return true;
}

//__MIP_EXPORT
__attribute__ ((visibility ("default")))
bool vst3_module_exit(void) {
  MIP_Print("ModuleExit\n");
  //if (--counter == 0) {
  //  moduleHandle = nullptr;
  //  return true;
  //}
  return true;
}

//----------------------------------------------------------------------
#endif
