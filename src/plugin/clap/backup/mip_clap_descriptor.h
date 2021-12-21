#ifndef mip_clap_descriptor_included
#define mip_clap_descriptor_included
//----------------------------------------------------------------------

class MIP_ClapDescriptor {

private:

  clap_plugin_descriptor  MClapDescriptor = {
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

public:

  void setId(const char* s)           { MClapDescriptor.id = s; }
  void setName(const char* s)         { MClapDescriptor.name = s; }
  void setVendor(const char* s)       { MClapDescriptor.vendor = s; }
  void setUrl(const char* s)          { MClapDescriptor.url = s; }
  void setManualUrl(const char* s)    { MClapDescriptor.manual_url = s; }
  void setSupportUrl(const char* s)   { MClapDescriptor.support_url = s; }
  void setVersion(const char* s)      { MClapDescriptor.version = s; }
  void setDescription(const char* s)  { MClapDescriptor.description = s; }
  void setKeywords(const char* s)     { MClapDescriptor.keywords = s; }
  void setKeywords(uint64_t t)        { MClapDescriptor.plugin_type = t; }

  const clap_plugin_descriptor* getClapDescriptor() { return &MClapDescriptor; }

public:

  MIP_ClapDescriptor() {}
  virtual ~MIP_ClapDescriptor() {}

};

//----------------------------------------------------------------------
#endif
