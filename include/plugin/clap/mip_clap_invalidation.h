//#ifndef mip_clap_invalidation_included
//#define mip_clap_invalidation_included
////----------------------------------------------------------------------
//
//#include "mip.h"
//#include "extern/clap/clap.h"
//#include "plugin/clap/mip_clap_list.h"
//
////----------------------------------------------------------------------
////
////
////
////----------------------------------------------------------------------
//
//#ifdef MIP_PLUGIN_USE_INVALIDATION
//
//uint32_t clap_invalidation_count_callback(const struct clap_plugin_invalidation_factory *factory) {
//  return MIP_GLOBAL_CLAP_LIST.getNumInvalidationSources();
//}
//
//const clap_plugin_invalidation_source_t* clap_invalidation_get_callback(const struct clap_plugin_invalidation_factory *factory, uint32_t index) {
//  return MIP_GLOBAL_CLAP_LIST.getInvalidationSource(index);
//}
//
//bool clap_invalidation_refresh_callback(const struct clap_plugin_invalidation_factory *factory) {
//  return false;
//}
//
//#endif // MIP_PLUGIN_USE_INVALIDATION
//
////----------------------------------------------------------------------
////
////
////
////----------------------------------------------------------------------
//
//#ifdef MIP_PLUGIN_USE_INVALIDATION
//
////static
////constexpr
//const clap_plugin_invalidation_factory MIP_GLOBAL_CLAP_INVALIDATION = {
//  clap_invalidation_count_callback,
//  clap_invalidation_get_callback,
//  clap_invalidation_refresh_callback
//};
//
//#endif // MIP_PLUGIN_USE_INVALIDATION
//
////----------------------------------------------------------------------
//#endif
