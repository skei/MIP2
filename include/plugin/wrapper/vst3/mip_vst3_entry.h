#ifndef mip_vst3_entry_included
#define mip_vst3_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/vst3/mip_vst3.h"
#include "plugin/vst3/mip_vst3_plugin.h"
#include "plugin/vst3/mip_vst3_utils.h"

//#include "plugin/mip_descriptor.h"
//#include "plugin/mip_plugin.h"
//#include "plugin/vst3/mip_vst3_host_proxy.h"

//#ifndef MIP_NO_GUI
//#include "plugin/mip_editor.h"
//#endif

//----------------------------------------------------------------------

//const char test_long_id[16] = {
//  0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
//};

//const char test_editor_id[16] = {
//  0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//template <class DESC, class PLUG>
class MIP_Vst3Entry
: public IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t      MRefCount     = 1;
  FUnknown*     MHostContext  = nullptr;

  char          MPluginId[16] = {0};
  char          MEditorId[16] = {0};

  MIP_ClapHost  MHost = {};

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

  //TODO: -> MIP_StrUtils.h
  // https://stackoverflow.com/questions/7666509/hash-function-for-string

  uint32_t MIP_HashString(const char* buffer) {
    char* ptr = (char*)buffer;
    unsigned long h = 5381;
    int c;
    while ((c = *ptr++)) {
      h = ((h << 5) + h) + c; // h * 33 + c
    }
    return h;
  }

  //----------

//  #define MIP_MAGIC_M_PL   0x4d5f504c    // M_PL   // plugin
//  #define MIP_MAGIC_M_ED   0x4d5f4544    // M_ED   // editor

  const char* getLongId(const clap_plugin_descriptor_t* descriptor) {
    uint32_t* id = (uint32_t*)MPluginId;
    id[0] = MIP_MAGIC_M_PL;
    id[1] = MIP_HashString(descriptor->name);
    id[2] = MIP_HashString(descriptor->vendor);
    id[3] = MIP_HashString(descriptor->version);
    return (const char*)id;
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
      //MIP_Print("queryInterface\n");
      //VST3_PrintIID(_iid);
      //MIP_DPrint(" (IPluginFactory2) -> kResultOk\n");
      *obj = (IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    if (VST3_iidEqual(_iid,IPluginFactory3_iid)) {
      //MIP_Print("\n");
      //VST3_PrintIID(_iid);
      //MIP_DPrint(" (IPluginFactory3) -> kResultOk\n");
      *obj = (IPluginFactory3*)this;
      addRef();
      return kResultOk;
    }
    //MIP_Print("\n");
    //VST3_PrintIID(_iid);
    //MIP_DPrint("(unknown) -> kNoInterface\n");
    *obj = nullptr;
    return kNoInterface;
  }

  //--------------------
  // IPluginFactory
  //--------------------

  /*
    Class factory that any Plug-in defines for creating class instances.
    From the host's point of view a Plug-in module is a factory which can
    create a certain kind of object(s). The interface IPluginFactory provides
    methods to get information about the classes exported by the Plug-in and a
    mechanism to create instances of these classes (that usually define the
    IPluginBase interface)
  */

  //----------

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
    return MIP_GetNumPlugins();
  }

  //----------

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) override {
    MIP_Print("MIP_Vst3Entry.getClassInfo\n");
//    MIP_PluginInfo* plugin_info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(index);
//    MIP_Descriptor* plugin_desc = plugin_info->descriptor;
    const clap_plugin_descriptor_t* descriptor = MIP_GetDescriptor(index);
    const char* long_id = getLongId(descriptor);
    memcpy(info->cid,long_id,16);
    info->cardinality = PClassInfo::kManyInstances;
    strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
    strncpy(info->name,descriptor->name,PClassInfo::kNameSize);
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) override {
    MIP_Print("MIP_Vst3Entry.createInstance\n");
//    VST3_PrintIID(cid);
//    //MIP_DPrint("\n");
//    MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.findPluginByLongId(cid);
//    if (info) {
//      MIP_DPrint(" (index %i) -> kResultOk\n",info->index);
//      uint32_t index = info->index;
//      MIP_Descriptor* desc = info->descriptor;
//      MIP_Vst3HostProxy* hostproxy = new MIP_Vst3HostProxy();       // deleted in MIP_Plugin() destructor
//      MIP_Plugin* plugin = MIP_CreatePlugin(index,desc,hostproxy);  // deleted in MIP_Vst3Plugin destructor
//      MIP_Vst3Plugin* vst3_plugin = new MIP_Vst3Plugin(plugin);     // deleted in MIP_Vst3Plugin.release()

    //TODO: cid -> index

    const clap_plugin_descriptor_t* descriptor  = MIP_GetDescriptor(0);
    const clap_plugin_t*            plugin      = MIP_CreatePlugin(MHost.getPtr(),descriptor->id); // deleted in ? (MIP_Vst3Plugin destructor)
    MIP_Vst3Plugin*                 vst3plugin  = new MIP_Vst3Plugin(plugin);                       // deleted in ?

////      plugin->setListener(vst3_instance);
////      plugin->on_plugin_open();
////      plugin->setDefaultParameterValues();
////      plugin->updateAllParameters();

      *obj = (Vst::IComponent*)vst3plugin;
      return kResultOk;

//    }
    //MIP_DPrint(" -> kNotImplemented\n");
    //*obj = nullptr;
    //MIP_Print("ok\n");;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) override {
    MIP_Print("MIP_Vst3Entry.getClassInfo2\n");
//    MIP_PluginInfo* plugin_info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(index);
//    if (info) {
//      MIP_Descriptor* plugin_desc = plugin_info->descriptor;
//      memcpy(info->cid,(const char*)plugin_desc->getLongId(),16);
    const clap_plugin_descriptor_t* descriptor = MIP_GetDescriptor(0);
    const char* long_id = getLongId(descriptor);
    memcpy(info->cid,long_id,16);
    info->cardinality = PClassInfo::kManyInstances;
    strcpy(info->category,kVstAudioEffectClass);
    strcpy(info->name,descriptor->name);
    info->classFlags = 0;

//    if (strstr(descriptor->features,"instrument")) strcpy(info->subCategories,Vst::PlugType::kInstrument);
//    else if (strstr(descriptor->features,"audio_effect")) strcpy(info->subCategories,Vst::PlugType::kFx);

    strcpy(info->vendor,descriptor->vendor);
    strcpy(info->version,descriptor->version);
    strcpy(info->sdkVersion,kVstVersionString);
    return kResultOk;
//    }
//    MIP_DPrint(" -> kResultFalse\n");
//    return kResultFalse;
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
