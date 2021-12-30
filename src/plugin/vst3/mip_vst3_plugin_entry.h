#ifndef mip_vst3_plugin_entry_included
#define mip_vst3_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "plugin/vst3/mip_vst3.h"

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
//  MIP_Descriptor* MDescriptor   = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Vst3PluginEntry() {
    MIP_PRINT;
    MRefCount = 1;
//    MDescriptor = new DESC();
  }

  //----------

  virtual ~MIP_Vst3PluginEntry() {
    MIP_PRINT;
//    if (MDescriptor) delete MDescriptor;
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
//    strcpy(info->vendor,MDescriptor->getAuthor());
//    strcpy(info->url,MDescriptor->getUrl());
//    strcpy(info->email,MDescriptor->getEmail());
//    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  //----------

  int32   PLUGIN_API countClasses() override {
    MIP_PRINT;
    return 1;
  }

  //----------

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) override {
    MIP_PRINT;
    switch (index) {
      case 0:
//        memcpy(info->cid,(const char*)MDescriptor->getLongId(),16);
//        info->cardinality = PClassInfo::kManyInstances;
//        strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
//        strncpy(info->name,MDescriptor->getName(),PClassInfo::kNameSize);
        return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) override {
    MIP_PRINT;
//    if (FUnknownPrivate::iidEqual(MDescriptor->getLongId(),cid)) {
//      INST* instance = new INST(MDescriptor);
//      KODE_Vst3Instance* vst3_instance = KODE_New KODE_Vst3Instance(instance);
//      instance->setListener(vst3_instance);
//      instance->on_open();
//      instance->setDefaultParameterValues();
//      instance->updateAllParameters();
//      *obj = (Vst::IComponent*)vst3_instance;
//      return kResultOk;
//    }
    *obj = nullptr;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) override {
    MIP_PRINT;
//    switch (index) {
//      case 0:
//        KODE_Memcpy(info->cid,(const char*)MDescriptor->getLongId(),16);
//        info->cardinality = PClassInfo::kManyInstances;
//        KODE_Strcpy(info->category,kVstAudioEffectClass);
//        KODE_Strcpy(info->name,MDescriptor->getName());
//        info->classFlags = 0;
//        if (MDescriptor->hasFlag(KODE_PLUGIN_IS_SYNTH)) {
//          KODE_Strcpy(info->subCategories,Vst::PlugType::kInstrument);
//        }
//        else {
//          KODE_Strcpy(info->subCategories,Vst::PlugType::kFx);
//        }
//        KODE_Strcpy(info->vendor,MDescriptor->getAuthor());
//        KODE_Strcpy(info->version,MDescriptor->getVersionText());
//        KODE_Strcpy(info->sdkVersion,kVstVersionString);
//        return kResultOk;
//    }
    return kResultFalse;
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
