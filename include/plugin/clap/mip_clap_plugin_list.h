//#ifndef mip_clap_plugin_list_included
//#define mip_clap_plugin_list_included
////----------------------------------------------------------------------
//
////extern uint32_t MIP_GetNumPlugins();
////extern const clap_plugin_descriptor_t* MIP_GetDescriptor(uint32_t index);
////extern const clap_plugin_t* MIP_CreatePlugin(const clap_host_t *host, const char *plugin_id);
//
////----------------------------------------------------------------------
//
//typedef std::vector<const clap_plugin_descriptor_t*> MIP_PluginDescriptors;
//
////----------------------------------------------------------------------
//
//class MIP_ClapPluginList {
//
////------------------------------
//private:
////------------------------------
//
//  MIP_PluginDescriptors MDescriptors;
//
////------------------------------
//public:
////------------------------------
//
//  void appendDescriptor(const clap_plugin_descriptor_t* descriptor) {
//    MDescriptors.push_back(descriptor);
//  }
//
//  uint32_t getNumDescriptors() {
//    return MDescriptors.size();
//  }
//
//  void deleteDescriptors() {
//  }
//
//  int32_t findId(const char *plugin_id) {
//    return -1;
//  }
//
//};
//
////----------------------------------------------------------------------
//
//MIP_ClapPluginList MIP_GLOBAL_PLUGIN_LIST;
//
////----------------------------------------------------------------------
////
////
////
////----------------------------------------------------------------------
//
//void MIP_RegisterPlugin(const clap_plugin_descriptor_t* descriptor) {
//  MIP_GLOBAL_PLUGIN_LIST.appendDescriptor(descriptor);
//}
//
//uint32_t MIP_GetNumPlugins() {
//  return MIP_GLOBAL_PLUGIN_LIST.getNumDescriptors();
//}
//
//const clap_plugin_descriptor_t* MIP_GetDescriptor(uint32_t index) {
//  return MIP_GLOBAL_PLUGIN_LIST.getDescriptors(index);
//}
//
//const clap_plugin_t* MIP_CreatePlugin(const clap_host_t *host, const char *plugin_id) {
//  int32_t id = MIP_GLOBAL_PLUGIN_LIST.findId(plugin_id);
//  if (id >= 0) {
//    //return MIP_GLOBAL_PLUGIN_LIST.createPlugin(const clap_host_t *host, const char *plugin_id) {
//  }
//}
//
////----------------------------------------------------------------------
//#endif
