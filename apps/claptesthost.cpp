
/*
  TODO, consider, nloltes, etc..

  * find a proper name for this..
    (and not confuse it with regular clap names/structs,
    there's already clap_host, etc..))

  * cc -> parameter remapping
    (should work, already?)
    do we want more than one at a time?

  * audio processing (audio effect)
    (should more or less work, already?)

  * load preset from file
    (use clap.preset-load)

  * print more diagnostic info
    - list parameters
    - audio/event/note ports

  * update gui while processing?
    a bit messy, and unnecessary?
    need timer and event handling, etc..

  * add latency (clap.latency) to rendering total time..

  * notify pklugin about offline rendering?
    or 'simulare' realtime, like we do now?

  * is there problems with using libsndfile on win/mac?
    do we want to make our own wav import/exporter?
    (not too hard, but tedious..)

  * optimize?
    but is there any point?
    this isn't realtime, or performance-critical..
    - reading/writing audio from disk is sample by sample
    - we're converting and allocating buffers for midi events per block

*/

//----------------------------------------------------------------------

// nc -U -l -k /tmp/mip.socket
//#define MIP_DEBUG_PRINT_SOCKET

#define MIP_NO_PLUGIN

#define MIP_NO_GUI
//#define MIP_GUI_XCB

//#define MIP_GUI_XCB
//#define MIP_USE_CAIRO

//----------------------------------------------------------------------

#include "mip.h"
#include "base/utils/mip_arguments.h"
#include "plugin/clap/mip_clap_host.h"
#include "plugin/clap/mip_clap_hosted_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_Arguments         arg_parser  = {};
MIP_ClapHost          host        = {};
MIP_ClapHostedPlugin* plugin      = nullptr;

//----------

const char* arg_plugin_path       = "";
uint32_t    arg_plugin_index      = 0;
uint32_t    arg_num_audio_inputs  = 0;
uint32_t    arg_num_audio_outputs = 0;
float       arg_sample_rate       = 0.0;
uint32_t    arg_block_size        = 0;
const char* arg_midi_input_file   = "";
const char* arg_audio_input_file  = "";
const char* arg_audio_output_file = "";
bool        arg_print_help        = false;
bool        arg_list_plugins      = false;
bool        arg_print_descriptor  = false;
bool        arg_fuzz_block_size   = false;
float       arg_decay_seconds     = 0.0;
int32_t     arg_remap_cc          = -1;
int32_t     arg_remap_param       = -1;

//----------

#include "claptesthost_process.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void print_help() {
  printf("usage: claptesthost plugin-path index [options]\n");
  printf("options:\n");
  printf("  -h  --help\n");
  printf("  -m  --midi-input    <path>\n");
  printf("  -a  --audio-input   <path>\n");
  printf("  -o  --audio-output  <path>\n");
  printf("  -s  --sample_rate   <samples>        (default 44100.0)\n");
  printf("  -b  --block_size    <samples>        (default 256)\n");
  printf("  -c  --channels      <inputs:outputs> (default 2:2)\n");
  printf("  -d  --decay-seconds <seconds>        (default 0.0)\n");
  printf("  -r  --remap         <cc:param_id>\n");

  printf("  -l  --list-plugins\n");
  printf("  -p  --print-descriptor\n");
  printf("  -f  --fuzz-block-size\n");
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main(int argc, char** argv) {
  int result = 0;
  arg_parser.init(argc,argv);
  uint32_t num_args = arg_parser.count();

  if ((num_args < 3) || arg_parser.hasOption("-h","--help") || arg_parser.hasOption("-?","--?")) {
    print_help();
    return 0;
  }


  if (!arg_parser.isOption(1)) arg_plugin_path = arg_parser.getStr(1);

  if (!arg_parser.isOption(2)) arg_plugin_index = arg_parser.getInt(2);
  else arg_plugin_index = 0;

  //arg_plugin_path = arg_parser.getStr(1);
  //if (arg_parser.isOption(2)) arg_plugin_index = 0;
  //else arg_plugin_index = arg_parser.getInt(2);

  //arg_print_help        = arg_parser.hasOption(                  "-h",   "--help");
  //arg_print_help       |= arg_parser.hasOption(                  "-?",   "--?");

  arg_midi_input_file   = arg_parser.getArgStr(                  "-m",   "--midi-input");
  arg_audio_input_file  = arg_parser.getArgStr(                  "-a",   "--audio-input");
  arg_audio_output_file = arg_parser.getArgStr(                  "-o",   "--audio-output");

  arg_sample_rate       = arg_parser.getArgFloat(                "-s",   "--sample_rate");
  arg_block_size        = arg_parser.getArgInt(                  "-b",   "--block_size");
  arg_num_audio_inputs  = arg_parser.getArgInt(                  "-c",   "--channels");
  arg_num_audio_outputs = arg_parser.getArgIntAfterSymbol( ':',  "-c",   "--channels");
  arg_decay_seconds     = arg_parser.getArgFloat(                "-d",   "--decay-seconds");

  arg_list_plugins      = arg_parser.hasOption(                  "-l",   "--list-plugins");
  arg_print_descriptor  = arg_parser.hasOption(                  "-p",   "--print-descriptor");
  arg_fuzz_block_size   = arg_parser.hasOption(                  "-f",   "--fuzz-block-size");

  if (arg_parser.hasOption("-r","--remap")) {
    arg_remap_cc        = arg_parser.getArgInt(                  "-r",   "--remap");
    arg_remap_param     = arg_parser.getArgIntAfterSymbol( ':',  "-r",   "--remap");
  }

  printf("\n");
  printf("plugin_path       %s\n",arg_plugin_path);
  printf("plugin_index      %i\n",arg_plugin_index);
  printf("midi_input file   %s\n",arg_midi_input_file);
  printf("audio_input file  %s\n",arg_audio_input_file);
  printf("audio_output file %s\n",arg_audio_output_file);
  printf("sample_rate       %.2f\n",arg_sample_rate);
  printf("block_size        %i\n",arg_block_size);
  printf("channels (in:out) %i:%i\n",arg_num_audio_inputs,arg_num_audio_outputs);
  printf("decay seconds     %.2f\n",arg_decay_seconds);
  printf("list_plugins      %s\n",arg_list_plugins?"yes":"no");
  printf("print_descriptor  %s\n",arg_print_descriptor?"yes":"no");
  printf("fuzz block size   %s\n",arg_fuzz_block_size?"yes":"no");
  printf("remap (cc:param)  %i:%i\n",arg_remap_cc,arg_remap_param);
  printf("\n");

  if (!host.loadPlugin(arg_plugin_path)) {
    printf("error loading plugin\n");
    return -1;
  }

  if (arg_list_plugins) {
    printf("plugins:\n");
    const clap_plugin_factory* factory = host.getClapFactory();
    uint32_t num = factory->get_plugin_count(factory);
    for (uint32_t i=0; i<num; i++) {
      const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,i);
      printf("  %i. %s (%s)\n",i,descriptor->name,descriptor->description);
    }
    return 0;
  }

  else if (arg_print_descriptor) {
    printf("descriptor (%i):\n",arg_plugin_index);
    const clap_plugin_factory* factory = host.getClapFactory();
    const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,arg_plugin_index);
    printf("  clap_version: %i.%i.%i\n",descriptor->clap_version.major,descriptor->clap_version.minor,descriptor->clap_version.revision);
    printf("  id:           %s\n",descriptor->id);
    printf("  name:         %s\n",descriptor->name);
    printf("  vendor:       %s\n",descriptor->vendor);
    printf("  url:          %s\n",descriptor->url);
    printf("  manual_url:   %s\n",descriptor->manual_url);
    printf("  support_url:  %s\n",descriptor->support_url);
    printf("  version:      %s\n",descriptor->version);
    printf("  description:  %s\n",descriptor->description);
    printf("  features:     %s\n",descriptor->features);
    return 0;
  }

  else {
    printf("process\n");
    plugin = host.instantiatePlugin(arg_plugin_path,arg_plugin_index);
    //plugin->init();
    plugin->activate(arg_sample_rate,1,arg_block_size);
    plugin->start_processing();

    Process proc = Process(plugin);
    result = proc.process(plugin);

    plugin->stop_processing();
    plugin->deactivate();
    //plugin->destroy();
  }

  printf("unloading plugin\n");
  host.unloadPlugin();
  printf("unloading ok\n");
  return result;
}

