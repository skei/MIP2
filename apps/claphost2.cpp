
// nc -U -l -k /tmp/mip.socket
//#define MIP_DEBUG_PRINT_SOCKET

#define MIP_NO_PLUGIN
#define MIP_NO_GUI

//#define MIP_GUI_XCB
//#define MIP_USE_CAIRO

//----------------------------------------------------------------------

#include "mip.h"
#include "base/utils/mip_arguments.h"
#include "plugin/clap/mip_clap_host.h"

//----------------------------------------------------------------------

MIP_Arguments arg   = {};
MIP_ClapHost  host  = {};

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

//----------------------------------------------------------------------

#include "claphost/claphost_process.h"

//----------------------------------------------------------------------

//----------------------------------------------------------------------

void print_help() {
  printf("usage: claphost plugin-path index [options]\n");
  printf("options:\n");
  printf("  -h  --help\n");
  printf("  -m  --midi-input    <path>\n");
  printf("  -a  --audio-input   <path>\n");
  printf("  -o  --audio-output  <path>\n");
  printf("  -s  --sample_rate   <samples>        (default 44100.0)\n");
  printf("  -b  --block_size    <samples>        (default 256)\n");
  printf("  -c  --channels      <inputs:outputs> (default 2:2)\n");
  printf("  -d  --decay-seconds <seconds>        (default 0.0)\n");
  printf("  -l  --list-plugins\n");
  printf("  -p  --print-descriptor\n");
  printf("  -f  --fuzz-block-size\n");
}

//----------------------------------------------------------------------

bool load_plugin(const char* path) {
  return host.loadPlugin(path);
}

//----------------------------------------------------------------------

void list_plugin() {
}

//----------------------------------------------------------------------

void print_descriptor(uint32_t index) {
}

//----------------------------------------------------------------------

void process() {
  //MIP_ClapPlugin* plugin = entry.createPlugin(GPluginPath,GPluginIndex);
  //plugin->activate(GSampleRate,1,GBlockSize);
  //plugin->start_processing();
  //process.process(plugin);
  //plugin->stop_processing();
  //plugin->deactivate();
  //entry.destroyPlugin(plugin);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

int main(int argc, char** argv) {
  arg.init(argc,argv);
  uint32_t num_args = arg.count();

  if ((num_args == 1) || arg.hasOption("-h","--help") || arg.hasOption("-?","--?")) {
    print_help();
    exit(0);
  }

  else if (num_args == 2) {
    arg_plugin_path = arg.getStr(1);
    arg_plugin_index = 0;
  }

  else {

    // -> arg struct ?

    arg_plugin_path = arg.getStr(1);
    arg_plugin_index = arg.getInt(2);

    arg_print_help        = arg.hasOption(                  "-h",   "--help");
    arg_print_help       |= arg.hasOption(                  "-?",   "--?");

    arg_midi_input_file    = arg.getArgStr(                  "-m",   "--midi-input");
    arg_audio_input_file   = arg.getArgStr(                  "-a",   "--audio-input");
    arg_audio_output_file  = arg.getArgStr(                  "-o",   "--audio-output");

    arg_sample_rate       = arg.getArgFloat(                "-s",   "--sample_rate");
    arg_block_size        = arg.getArgInt(                  "-b",   "--block_size");
    arg_num_audio_inputs   = arg.getArgInt(                  "-c",   "--channels");
    arg_num_audio_outputs  = arg.getArgIntAfterSymbol( ':',  "-c",   "--channels");
    arg_decay_seconds     = arg.getArgFloat(                "-d",   "--decay-seconds");

    arg_list_plugins      = arg.hasOption(                  "-l",   "--list-plugins");
    arg_print_descriptor  = arg.hasOption(                  "-p",   "--print-descriptor");
    arg_fuzz_block_size    = arg.hasOption(                  "-f",   "--fuzz-block-size");

    if (arg.hasOption("-r","--remap")) {
      arg_remap_cc        = arg.getArgInt(                  "-r",   "--remap");
      arg_remap_param     = arg.getArgIntAfterSymbol( ':',  "-r",   "--remap");
    }

  }

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

  load_plugin(arg_plugin_path);

  if (arg_list_plugins) {
    MIP_Print("list plugins:\n");

//    entry.listPlugins();

  }
  else if (arg_print_descriptor) {
    MIP_Print("print descriptor:\n");
//    entry.printDescriptor(GPluginIndex);
  }
  else {
    // process
    MIP_Print("process:\n");
//    MIP_ClapPlugin* plugin = entry.createPlugin(GPluginPath,GPluginIndex);
//    plugin->activate(GSampleRate,1,GBlockSize);
//    plugin->start_processing();
//    process.process(plugin);
//    plugin->stop_processing();
//    plugin->deactivate();
//    entry.destroyPlugin(plugin);
  }
//  entry.unload();
  return 0;
}

