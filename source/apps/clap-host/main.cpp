
/*

  COMPILE:

  g++ -Wall -g -c main.cpp -o main.o
  g++ -o clap-host main.o -ldl -lsndfile

  TODO / MAYBE / CONSIDER:

  * json config file (alternative to command line arguments)
  * printout: audio/note ports, note names, etc..
  * code cleanup, structure
  * error checking, safety
  * show gui (?)
  * more diagnostics
  * remove long command line argument variants?
  * save (potential) midi output to file

*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "clap/clap.h"
#include "audio_file.hpp"
#include "midi_file.hpp"
#include "midi_player.hpp"

#include "arguments.hpp"
#include "host.hpp"
#include "print.hpp"
#include "preset.hpp"

//----------------------------------------------------------------------

Arguments             arg_parser            = {};
Host                  host                  = {};
const clap_plugin_t*  plugin                = nullptr;

const char*           arg_plugin_path       = "";
uint32_t              arg_plugin_index      = 0;
uint32_t              arg_num_audio_inputs  = 2;
uint32_t              arg_num_audio_outputs = 2;
float                 arg_sample_rate       = 44100.0;
uint32_t              arg_block_size        = 256;
const char*           arg_midi_input_file   = "";
const char*           arg_midi_output_file  = "";
const char*           arg_audio_input_file  = "";
const char*           arg_audio_output_file = "";
bool                  arg_print_help        = false;
bool                  arg_fuzz_block_size   = false;
float                 arg_decay_seconds     = 0.0;
int32_t               arg_remap_cc          = -1;
int32_t               arg_remap_param       = -1;
const char*           arg_preset_file       = "";
const char*           arg_json_file         = "";
bool                  arg_list_plugins      = false;
bool                  arg_print_descriptor  = false;
bool                  arg_print_parameters  = false;

//----------------------------------------------------------------------

// uses a lot the above global variables..
#include "process.hpp"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void print_help() {
  printf("usage: claptesthost plugin-path index [options]\n");
  printf("options:\n");
  printf("  -h  --help\n");
  printf("  -mi --midi-input        <path>            \n");
  printf("  -mo --midi-output       <path>            \n");
  printf("  -mo --midi-output       <path>            \n");
  printf("  -ai --audio-input       <path>            \n");
  printf("  -ao --audio-output      <path>            \n");
  printf("  -s  --sample_rate       <samples>         \n");
  printf("  -b  --block_size        <samples>         \n");
  printf("  -c  --channels          <inputs:outputs>  \n");
  printf("  -d  --decay-seconds     <seconds>         \n");
  printf("  -r  --remap             <cc:param_id>     \n");
  printf("  -j  --json              <file>            \n");
  printf("  -p  --preset            <file>            \n");
  printf("  -f  --fuzz-block-size                     \n");
  printf("  -pl --list-plugins                        \n");
  printf("  -pd --print-descriptor                    \n");
  printf("  -pp --print-parameters                    \n");

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

  // help

  if ((num_args < 3) || arg_parser.hasOption("-h","--help") || arg_parser.hasOption("-?","--?")) {
    print_help();
    return 0;
  }

  // plugin, index

  if (!arg_parser.isOption(1)) arg_plugin_path = arg_parser.getStr(1);
  if (!arg_parser.isOption(2)) arg_plugin_index = arg_parser.getInt(2);
  else arg_plugin_index = 0;

  // arguments

  arg_midi_input_file   = arg_parser.getArgStr(                  "-mi",  "--midi-input");
  arg_midi_output_file  = arg_parser.getArgStr(                  "-mo",  "--midi-output");
  arg_audio_input_file  = arg_parser.getArgStr(                  "-ai",  "--audio-input");
  arg_audio_output_file = arg_parser.getArgStr(                  "-ao",  "--audio-output");

  arg_json_file         = arg_parser.getArgStr(                  "-j",   "--json");
  arg_preset_file       = arg_parser.getArgStr(                  "-p",   "--preset");

  arg_sample_rate       = arg_parser.getArgFloat(                "-s",   "--sample_rate");
  arg_block_size        = arg_parser.getArgInt(                  "-b",   "--block_size");
  arg_num_audio_inputs  = arg_parser.getArgInt(                  "-c",   "--channels");
  arg_num_audio_outputs = arg_parser.getArgIntAfterSymbol( ':',  "-c",   "--channels");
  arg_decay_seconds     = arg_parser.getArgFloat(                "-d",   "--decay-seconds");

  arg_list_plugins      = arg_parser.hasOption(                  "-pl",  "--list-plugins");
  arg_print_descriptor  = arg_parser.hasOption(                  "-pd",  "--print-descriptor");
  arg_print_parameters  = arg_parser.hasOption(                  "-pp",  "--print-parameters");
  arg_fuzz_block_size   = arg_parser.hasOption(                  "-f",   "--fuzz-block-size");

  if (arg_parser.hasOption("-r","--remap")) {
    arg_remap_cc        = arg_parser.getArgInt(                  "-r",   "--remap");
    arg_remap_param     = arg_parser.getArgIntAfterSymbol( ':',  "-r",   "--remap");
  }

  if (!host.loadPlugin(arg_plugin_path)) {
    printf("error loading plugin\n");
    return -1;
  }

  bool do_process = true;

  //----------

  if (arg_json_file) {
    json_begin();
  }

  if (arg_list_plugins) {
    print_plugin_list(&host,arg_json_file);
    do_process = false;
  }

  if (arg_print_descriptor) {
    print_plugin_descriptor(&host,arg_plugin_index,arg_json_file);
    do_process = false;
  }

  plugin = host.createPlugin(arg_plugin_path,arg_plugin_index);
  plugin->init(plugin);

  if (arg_print_parameters) {
    print_plugin_parameters(plugin,arg_json_file);
    do_process = false;
  }

  if (do_process) {
    if (arg_preset_file) {
      load_preset(plugin,arg_preset_file);
    }
    plugin->activate(plugin,arg_sample_rate,1,arg_block_size);
    plugin->start_processing(plugin);
    Process proc = Process(plugin);
    result = proc.process();
    plugin->stop_processing(plugin);
    plugin->deactivate(plugin);
  }

  plugin->destroy(plugin);

  if (arg_json_file) {
    json_end(arg_json_file);
  }

  //----------

  printf("unloading plugin\n");
  host.unloadPlugin();

  printf("ok\n");
  return result;

}


//----------------------------------------------------------------------
