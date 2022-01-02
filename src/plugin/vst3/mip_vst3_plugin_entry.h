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

  uint32_t        MRefCount     = 1;
  FUnknown*       MHostContext  = nullptr;

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
    MIP_PRINT;
    MRefCount++;
    return MRefCount;
  }

  //----------

  uint32 PLUGIN_API release() override {
    MIP_PRINT;
    const uint32_t r = --MRefCount;
    if (r == 0) delete this;
    return r;
  }

  //----------

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    MIP_PRINT;
    if (FUnknownPrivate::iidEqual(IPluginFactory2_iid,_iid)) {
      *obj = (IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    if (FUnknownPrivate::iidEqual(IPluginFactory3_iid,_iid)) {
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
    MIP_PRINT;
    strcpy(info->vendor,"<author>");
    strcpy(info->url,"<url>");
    strcpy(info->email,"<email>");
    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  //----------


  int32   PLUGIN_API countClasses() override {
    MIP_PRINT;
    //return 1;
    uint32_t num = MIP_GLOBAL_PLUGIN_LIST.getNumPlugins();
    MIP_Print("%i\n",num);
    return num;
  }

  //----------

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) override {
    MIP_PRINT;
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
    MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.findPluginByLongId(cid); // byLongId
    if (info) {
      MIP_Print("index %i\n",info->index);
      uint32_t index = info->index;
      MIP_Descriptor* desc = info->descriptor;
      if (desc) {
        MIP_Plugin* plugin = MIP_CreatePlugin(index,desc); // deleted in MIP_Vst3Plugin destructor
//        MIP_Vst3Plugin* vst3_plugin = new MIP_Vst3Plugin(plugin); // deleted where ?
        // plugin->setListener(vst3_instance);
        // plugin->on_open();
        // plugin->setDefaultParameterValues();
        // plugin->updateAllParameters();
//        *obj = (Vst::IComponent*)vst3_plugin;
//        return kResultOk;
      }
    }
    *obj = nullptr;
    MIP_Print("ok\n");;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

//    memcpy(info->cid,(const char*)plugin_desc->getLongId(),16);
//    info->cardinality = PClassInfo::kManyInstances;
//    strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
//    strncpy(info->name,plugin_desc->getName(),PClassInfo::kNameSize);
//    return kResultOk;


  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) override {
    MIP_PRINT;
    MIP_PluginInfo* plugin_info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(index);
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
    return kResultOk;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  tresult PLUGIN_API getClassInfoUnicode(int32 index, PClassInfoW* info) override {
    MIP_PRINT;
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API setHostContext(FUnknown* context) override {
    MIP_PRINT;
    MHostContext = context;
    return kResultOk;
  }


};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_VST3_ENTRY_SYMBOL asm ("GetPluginFactory");
IPluginFactory* PLUGIN_API mip_vst3_entry() MIP_VST3_ENTRY_SYMBOL

//----------

__MIP_EXPORT
IPluginFactory* PLUGIN_API mip_vst3_entry() {
  MIP_PRINT;
  return new MIP_Vst3PluginEntry();
}
//----------------------------------------------------------------------
#endif
