#ifndef mip_clap_utils_included
#define mip_clap_utils_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------

void printParamInfo(const clap_param_info_t* info) {
  CLAP_DPrint("PARAMETER\n");
  CLAP_DPrint("  id:            %f\n",info->id);
  CLAP_DPrint("  flags:         %i\n",info->flags);
  CLAP_DPrint("  cookie:        %p\n",info->cookie);
  CLAP_DPrint("  name:          %s\n",info->name);
  CLAP_DPrint("  module:        %s\n",info->module);
  CLAP_DPrint("  min_value:     %.3f\n",info->min_value);
  CLAP_DPrint("  max_value:     %.3f\n",info->max_value);
  CLAP_DPrint("  default_value: %.3f\n",info->default_value);
}

void printTrackInfo(const clap_track_info_t* info) {
  CLAP_DPrint("id              %i\n",info->id);
  CLAP_DPrint("index           %i\n",info->index);
  CLAP_DPrint("name            %s\n",info->name);
  CLAP_DPrint("path            %s\n",info->path);
  CLAP_DPrint("channel_count   %i\n",info->channel_count);
  CLAP_DPrint("audio_port_type %s\n",info->audio_port_type);
  CLAP_DPrint("color           %02x,%02x,%02x\n",info->color.red,info->color.green,info->color.blue);
  CLAP_DPrint("is_return_track %i\n",info->is_return_track);
}


const char* MIP_ClapSpaceIdNames[] = {
  "CLAP_CORE_EVENT_SPACE_ID"
};

const char* MIP_ClapEventTypeNames[] = {
  "CLAP_EVENT_NOTE_ON",
  "CLAP_EVENT_NOTE_OFF",
  "CLAP_EVENT_NOTE_CHOKE",
  "CLAP_EVENT_NOTE_END",
  "CLAP_EVENT_NOTE_EXPRESSION",
  "CLAP_EVENT_PARAM_VALUE",
  "CLAP_EVENT_PARAM_MOD",
  "CLAP_EVENT_PARAM_GESTURE_BEGIN",
  "CLAP_EVENT_PARAM_GESTURE_END",
  "CLAP_EVENT_TRANSPORT",
  "CLAP_EVENT_MIDI",
  "CLAP_EVENT_MIDI_SYSEX",
  "CLAP_EVENT_MIDI2"
};

const char* MIP_ClapRenderModeNames[] = {
  "CLAP_RENDER_REALTIME",
  "CLAP_RENDER_OFFLINE"
};

//----------------------------------------------------------------------
#endif
