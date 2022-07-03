#ifndef mip_descriptor_included
#define mip_descriptor_included
//----------------------------------------------------------------------

// not used?

#include "mip.h"
#include "extern/clap/clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Descriptor {

//------------------------------
private:
//------------------------------

  clap_plugin_descriptor_t MDescriptor = {
    .clap_version = CLAP_VERSION_INIT,
    .id           = "MIP3",
    .name         = "MIP3",
    .vendor       = "skei.audio",
    .url          = "",
    .manual_url   = "",
    .support_url  = "",
    .version      = "0.0.0",
    .description  = "",
    .features     = (const char*[]){"audio_effect"},
  };

//------------------------------
public:
//------------------------------

  MIP_Descriptor() {
    MIP_PRINT;
  }

  //----------

  MIP_Descriptor(clap_plugin_descriptor_t* ADescriptor) {
    MIP_PRINT;
    MDescriptor.clap_version = ADescriptor->clap_version;
    MDescriptor.id           = ADescriptor->id;
    MDescriptor.name         = ADescriptor->name;
    MDescriptor.vendor       = ADescriptor->vendor;
    MDescriptor.url          = ADescriptor->url;
    MDescriptor.manual_url   = ADescriptor->manual_url;
    MDescriptor.support_url  = ADescriptor->support_url;
    MDescriptor.version      = ADescriptor->version;
    MDescriptor.description  = ADescriptor->description;
    MDescriptor.features     = ADescriptor->features;
  }

  virtual ~MIP_Descriptor() {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  void setId(const char* AId)                   { MDescriptor.id = AId; }
  void setName(const char* AName)               { MDescriptor.name = AName; }
  void setVentor(const char* AVendor)           { MDescriptor.vendor = AVendor; }
  void setUrl(const char* AUrl)                 { MDescriptor.url = AUrl; }
  void setManualUrl(const char* AManualUrl)     { MDescriptor.manual_url = AManualUrl; }
  void setSupportUrl(const char* ASupportUrl)   { MDescriptor.support_url = ASupportUrl; }
  void setVersion(const char* AVersion)         { MDescriptor.version = AVersion; }
  void setDescription(const char* ADescription) { MDescriptor.description = ADescription; }
  void setFeatures(const char** AFeatures)      { MDescriptor.features = AFeatures; }

  //----------

  const clap_plugin_descriptor_t* getDescriptor() {
    return &MDescriptor;
  }

};

//----------------------------------------------------------------------
#endif
