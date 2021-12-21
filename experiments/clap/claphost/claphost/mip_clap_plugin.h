#ifndef mip_clap_plugin_included
#define mip_clap_plugin_included
//----------------------------------------------------------------------

/*
  clap_plugin* 'wrapper'
  HostedPluginInstance
*/

//----------------------------------------------------------------------

#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class MIP_ClapPlugin {

//------------------------------
private:
//------------------------------

  const clap_plugin*                    MClapPlugin;

  const clap_plugin_audio_ports_config* MClapExtAudioPortsConfig;
  const clap_plugin_audio_ports*        MClapExtAudioPorts;
  const clap_plugin_event_filter*       MClapExtEventFilter;
  const clap_plugin_fd_support*         MClapExtFdSupport;
  const clap_plugin_gui*                MClapExtGui;
  const clap_plugin_gui_x11*            MClapExtGuiX11;
  const clap_plugin_latency*            MClapExtLatency;
  const clap_plugin_note_name*          MClapExtNoteName;
  const clap_plugin_params*             MClapExtParams;
  const clap_plugin_render*             MClapExtRender;
  const clap_plugin_state*              MClapExtState;
  const clap_plugin_vst2_convert*       MClapExtVst2Convert;
  const clap_plugin_vst3_convert*       MClapExtVst3Convert;

//------------------------------
public:
//------------------------------

  MIP_ClapPlugin(const clap_plugin* AClapPlugin) {
    MClapPlugin = AClapPlugin;
    init_extensions(AClapPlugin);
  }

  //----------

  ~MIP_ClapPlugin() {
  }

//------------------------------
public:
//------------------------------

  const clap_plugin* getClapPlugin() {
    return MClapPlugin;
  }

  bool getParam(uint32_t index, clap_param_info* info) {
    return MClapExtParams->get_info(MClapPlugin,index,info);
  }

//------------------------------
public:
//------------------------------

  // ask plugin (instance) about extensions..
  // TODO: check more extensions

  void init_extensions(const clap_plugin* plugin) {
    MClapExtAudioPortsConfig = NULL;// (const clap_plugin_audio_ports_config*)plugin->get_extension(plugin,CLAP_EXT_AUDIO_PORTS_CONFIG);
    MClapExtAudioPorts       = NULL;// (const clap_plugin_audio_ports*)plugin->get_extension(plugin,CLAP_EXT_AUDIO_PORTS);
    MClapExtEventFilter      = NULL;// (const clap_plugin_event_filter*)plugin->get_extension(plugin,CLAP_EXT_EVENT_FILTER);
    MClapExtFdSupport        = NULL;// (const clap_plugin_fd_support*)plugin->get_extension(plugin,CLAP_EXT_FD_SUPPORT);
    MClapExtGui              = NULL;// (const clap_plugin_gui*)plugin->get_extension(plugin,CLAP_EXT_GUI);
    MClapExtGuiX11           = NULL;// (const clap_plugin_gui_x11*)plugin->get_extension(plugin,CLAP_EXT_GUI_X11);
    MClapExtLatency          = NULL;// (const clap_plugin_latency*)plugin->get_extension(plugin,CLAP_EXT_LATENCY);
    MClapExtNoteName         = NULL;// (const clap_plugin_note_name*)plugin->get_extension(plugin,CLAP_EXT_NOTE_NAME);
    MClapExtParams           = NULL;// (const clap_plugin_params*)plugin->get_extension(plugin,CLAP_EXT_PARAMS);
    MClapExtRender           = NULL;// (const clap_plugin_render*)plugin->get_extension(plugin,CLAP_EXT_RENDER);
    MClapExtState            = NULL;// (const clap_plugin_state*)plugin->get_extension(plugin,CLAP_EXT_STATE);
    MClapExtVst2Convert      = NULL;// (const clap_plugin_vst2_convert*)plugin->get_extension(plugin,CLAP_EXT_VST2_CONVERT);
    MClapExtVst3Convert      = NULL;// (const clap_plugin_vst3_convert*)plugin->get_extension(plugin,CLAP_EXT_VST3_CONVERT);
  }

  //----------

  bool activate(float ASampleRate, uint32_t AMinFrames, uint32_t AMaxFrames) {
    return MClapPlugin->activate(MClapPlugin,ASampleRate,AMinFrames,AMaxFrames);
  }

  //----------

  void deactivate() {
    MClapPlugin->deactivate(MClapPlugin);
  }

  //----------

  bool start_processing() {
    return MClapPlugin->start_processing(MClapPlugin);
  }

  //----------

  void stop_processing() {
    MClapPlugin->stop_processing(MClapPlugin);
  }

  //----------

  // print port info (see printInfo)

  void printPortInfo(clap_audio_port_info* info) {
    printf("    name          %s\n",info->name);
    printf("    id            %i\n",info->id);
    printf("    channel count %i\n",info->channel_count);
    printf("    channel map   %i\n",info->channel_map);
    printf("    sample-size   %i\n",info->sample_size);
    printf("    is-main       %s\n",info->is_main ? "true" : "false");
    printf("    in-place      %s\n",info->in_place ? "true" : "false");
    printf("    is-cv         %s\n",info->is_cv ? "true" : "false");
  }

  //----------

  // print some info
  // TODO

  void printInfo() {
    int i,num;
    clap_audio_port_info info;
    printf("extensions:\n");

    printf("# audio-ports-config  (%s)\n", (MClapExtAudioPortsConfig) ? "yes" : "no"  );

//    if (MClapExtAudioPortsConfig) {
//      printf("### CLAP_EXT_AUDIO_PORTS_CONFIG\n");
//      clap_audio_ports_config config;
//      uint32_t num = MClapExtAudioPortsConfig->count(MClapPlugin);
//      printf("    count: %i\n",num);
//      for (uint32_t i=0; i<num; i++) {
//        bool result = MClapExtAudioPortsConfig->get(MClapPlugin,i,&config);
//        if (result) {
//          printf("    - id:   %i\n",config.id);
//          printf("    - name: %s\n",config.name);
//          printf("    - inputs: %i\n",config.input_channel_count);
//          printf("    - input map: %i\n",config.input_channel_map);
//          printf("    - outputs: %i\n",config.output_channel_count);
//          printf("    - output map: %i\n",config.output_channel_map);
//        }
//      }
//    }

    printf("# audio-ports         (%s)\n", (MClapExtAudioPorts) ? "yes" : "no"  );
    if (MClapExtAudioPorts) {
      num = MClapExtAudioPorts->count(MClapPlugin,true);
      printf("  %i audio input ports\n",num);
      for (i=0; i<num; i++) {
        MClapExtAudioPorts->get(MClapPlugin,i,true,&info);
        printPortInfo(&info);
      }
      num = MClapExtAudioPorts->count(MClapPlugin,false);
      printf("  %i audio output ports\n",num);
      for (i=0; i<num; i++) {
        MClapExtAudioPorts->get(MClapPlugin,i,false,&info);
        printPortInfo(&info);
      }
    }

    printf("# event-filter  (%s)\n", (MClapExtEventFilter) ? "yes" : "no"  );
    printf("# fd-support    (%s)\n", (MClapExtFdSupport) ? "yes" : "no"  );
    printf("# gui           (%s)\n", (MClapExtGui) ? "yes" : "no"  );
    printf("# gui-x11       (%s)\n", (MClapExtGuiX11) ? "yes" : "no"  );
    printf("# latency       (%s)\n", (MClapExtLatency) ? "yes" : "no"  );
    printf("# note-name     (%s)\n", (MClapExtNoteName) ? "yes" : "no"  );

    printf("# params        (%s)\n", (MClapExtParams) ? "yes" : "no"  );
    if (MClapExtParams) {
      num = MClapExtParams->count(MClapPlugin);
      printf("  %i params\n",num);
    }

    printf("# render        (%s)\n", (MClapExtRender) ? "yes" : "no"  );
    printf("# state         (%s)\n", (MClapExtState) ? "yes" : "no"  );
    printf("# vst2-convert  (%s)\n", (MClapExtVst2Convert) ? "yes" : "no"  );
    printf("# vst3-convert  (%s)\n", (MClapExtVst3Convert) ? "yes" : "no"  );

  }

  //----------

};

//----------------------------------------------------------------------
#endif

