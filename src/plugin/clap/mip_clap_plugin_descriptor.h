#ifndef mip_clap_plugin_descriptor_included
#define mip_clap_plugin_descriptor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

class MIP_ClapPluginDescriptor {

//------------------------------
public:
//------------------------------

  MIP_ClapPluginDescriptor() {
  }

  //----------

  virtual ~MIP_ClapPluginDescriptor() {
  }

//------------------------------
public:
//------------------------------

  const clap_plugin_descriptor_t* getClapDescriptor() { return &MClapDescriptor; }
  const char*                     getId()             { return MClapDescriptor.id; }

//------------------------------
public:
//------------------------------

  void setId(const char* str)           { MClapDescriptor.id = str; }
  void setName(const char* str)         { MClapDescriptor.name = str; }
  void setVendor(const char* str)       { MClapDescriptor.vendor = str; }
  void setUrl(const char* str)          { MClapDescriptor.url = str; }
  void setManualUrl(const char* str)    { MClapDescriptor.manual_url = str; }
  void setSupportUrl(const char* str)   { MClapDescriptor.support_url = str; }
  void setVersion(const char* str)      { MClapDescriptor.version = str; }
  void setDescription(const char* str)  { MClapDescriptor.description = str; }
  void setKeywords(const char* str)     { MClapDescriptor.keywords = str; }
  void setType(uint32_t t)              { MClapDescriptor.plugin_type = t; }

//------------------------------
private:
//------------------------------

  clap_plugin_descriptor_t MClapDescriptor = {
    CLAP_VERSION,
    "id",
    "name",
    "vendor",
    "url",
    "manual_url",
    "support_url",
    "version",
    "description",
     "keywords",
     CLAP_PLUGIN_AUDIO_EFFECT
   };

};


//----------------------------------------------------------------------
#endif

