#ifndef mip_vst3_plugin_entry_included
#define mip_vst3_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_plugin.h"
#include "plugin/vst3/mip_vst3.h"
#include "plugin/vst3/mip_vst3_plugin.h"
#include "plugin/vst3/mip_vst3_utils.h"

//#ifndef MIP_NO_GUI
#include "plugin/mip_editor.h"
//#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//template <class DESC, class PLUG>
class MIP_Vst3PluginEntry
: public IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t  MRefCount     = 1;
  FUnknown* MHostContext  = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Vst3PluginEntry() {
    MIP_PRINT;
    MRefCount = 1;
  }

  //----------

  virtual ~MIP_Vst3PluginEntry() {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  //--------------------
  // FUnknown
  //--------------------

  uint32 PLUGIN_API addRef() override {
    MRefCount++;
    MIP_Print("-> %i\n",MRefCount);
    return MRefCount;
  }

  //----------

  uint32 PLUGIN_API release() override {
    MIP_PRINT;
    const uint32_t r = --MRefCount;
    MIP_Print("-> %i\n",r);
    if (r == 0) delete this;
    return r;
  }

  //----------

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    //if (FUnknownPrivate::iidEqual(IPluginFactory2_iid,_iid)) {
    if (VST3_iidEqual(_iid,IPluginFactory2_iid)) {
      MIP_Print("");
      VST3_PrintIID(_iid);
      MIP_DPrint(" (IPluginFactory2) -> kResultOk\n");
      *obj = (IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    //if (FUnknownPrivate::iidEqual(IPluginFactory3_iid,_iid)) {
    if (VST3_iidEqual(_iid,IPluginFactory3_iid)) {
      MIP_Print("");
      VST3_PrintIID(_iid);
      MIP_DPrint(" (IPluginFactory3) -> kResultOk\n");
      *obj = (IPluginFactory3*)this;
      addRef();
      return kResultOk;
    }
      MIP_Print("");
      VST3_PrintIID(_iid);
      MIP_DPrint("(unknown) -> kNoInterface\n");
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
    MIP_Print("-> kResultOk\n");
    strcpy(info->vendor,"<factory author>");
    strcpy(info->url,"<factory url>");
    strcpy(info->email,"<factory email>");
    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  //----------


  int32   PLUGIN_API countClasses() override {
    uint32_t num = MIP_GLOBAL_PLUGIN_LIST.getNumPlugins();
    MIP_Print("-> %i\n",num);
    return num;
  }

  //----------

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) override {
    MIP_Print("index %i -> kResultOk\n",index);
    MIP_PluginInfo* plugin_info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(index);
    MIP_Descriptor* plugin_desc = plugin_info->descriptor;
    memcpy(info->cid,(const char*)plugin_desc->getLongId(),16);
    info->cardinality = PClassInfo::kManyInstances;
    strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
    strncpy(info->name,plugin_desc->getName(),PClassInfo::kNameSize);
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) override {
    MIP_Print("");
    VST3_PrintIID(cid);
    //MIP_DPrint("\n");
    MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.findPluginByLongId(cid);
    if (info) {
      MIP_DPrint(" (index %i) -> kResultOk\n",info->index);
      uint32_t index = info->index;
      MIP_Descriptor* desc = info->descriptor;
      MIP_Plugin* plugin = MIP_CreatePlugin(index,desc); // deleted in MIP_Vst3Plugin destructor
      MIP_Vst3Plugin* vst3_plugin = new MIP_Vst3Plugin(plugin); // deleted where ?
//      plugin->setListener(vst3_instance);
//      plugin->on_plugin_open();
//      plugin->setDefaultParameterValues();
//      plugin->updateAllParameters();
      *obj = (Vst::IComponent*)vst3_plugin;
      return kResultOk;
    }
    MIP_DPrint(" -> kNotImplemented\n");
    //*obj = nullptr;
    //MIP_Print("ok\n");;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) override {
    MIP_Print("index %i",index);
    MIP_PluginInfo* plugin_info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(index);
    if (info) {
      MIP_Descriptor* plugin_desc = plugin_info->descriptor;
      memcpy(info->cid,(const char*)plugin_desc->getLongId(),16);
      info->cardinality = PClassInfo::kManyInstances;
      strcpy(info->category,kVstAudioEffectClass);
      strcpy(info->name,plugin_desc->getName());
      info->classFlags = 0;
      if (plugin_desc->isSynth()) strcpy(info->subCategories,Vst::PlugType::kInstrument);
      else strcpy(info->subCategories,Vst::PlugType::kFx);
      strcpy(info->vendor,plugin_desc->getAuthor());
      strcpy(info->version,plugin_desc->getVersionText());
      strcpy(info->sdkVersion,kVstVersionString);
      MIP_DPrint(" -> kResultOk\n");
      return kResultOk;
    }
    MIP_DPrint(" -> kResultFalse\n");
    return kResultFalse;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  tresult PLUGIN_API getClassInfoUnicode(int32 index, PClassInfoW* info) override {
    MIP_Print("index %i -> kResultFalse\n",index);
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API setHostContext(FUnknown* context) override {
    MIP_Print("-> kResultOk\n");
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

__MIP_EXPORT
IPluginFactory* PLUGIN_API mip_vst3_entry() {
  MIP_PRINT;
  return new MIP_Vst3PluginEntry();
}

//----------

//void* moduleHandle = nullptr;
//static int counter {0};

__MIP_EXPORT
bool vst3_module_entry(void* sharedLibraryHandle)  {
  MIP_PRINT;
  //if (++counter == 1) {
  //  moduleHandle = sharedLibraryHandle;
  //  return true;
  //}
  return true;
}

__MIP_EXPORT
bool vst3_module_exit(void) {
  MIP_PRINT;
  //if (--counter == 0) {
  //  moduleHandle = nullptr;
  //  return true;
  //}
  return true;
}

//----------------------------------------------------------------------
#endif
