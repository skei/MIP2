
// nc -U -l -k /tmp/mip.socket
//#define MIP_DEBUG_PRINT_SOCKET
#define MIP_NO_PLUGIN
#define MIP_NO_GUI
//#define MIP_GUI_XCB
//#define MIP_USE_CAIRO

//----------------------------------------------------------------------

#include "mip.h"
#include "base/utils/mip_arguments.h"
#include "claphost/mip_clap_plugin_entry.h"
#include "claphost/mip_clap_host.h"

//----------------------------------------------------------------------

  // -> arg.h ??

  bool                GPrintHelp        = false;
  const char*         GPluginPath       = "";
  uint32_t            GPluginIndex      = 0;
  const char*         GMidiInputFile    = "";
  const char*         GAudioInputFile   = "";
  const char*         GAudioOutputFile  = "";
  float               GSampleRate       = 0.0;
  uint32_t            GBlockSize        = 0;
  uint32_t            GNumAudioInputs   = 0;
  uint32_t            GNumAudioOutputs  = 0;
  bool                GListPlugins      = false;
  bool                GPrintDescriptor  = false;
  bool                GFuzzBlockSize    = false;
  float               GDecaySeconds     = 0.0;
  int32_t             GRemapCC          = -1;
  int32_t             GRemapParam       = -1;

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

int main(int argc, char** argv) {

  MIP_Arguments       arg     = {};
  MIP_ClapPluginEntry entry   = {};
  ClapHost_Process    process = {};

  arg.init(argc,argv);
  uint32_t num_args = arg.count();

  if ((num_args == 1) || arg.hasOption("-h","--help") || arg.hasOption("-?","--?")) {
    print_help();
    exit(0);
  }

  else if (num_args == 2) {
    GPluginPath = arg.getStr(1);
    GPluginIndex = 0;
  }

  else {

    // -> arg struct ?

    GPluginPath = arg.getStr(1);
    GPluginIndex = arg.getInt(2);

    GPrintHelp        = arg.hasOption(                  "-h",   "--help");
    GPrintHelp       |= arg.hasOption(                  "-?",   "--?");

    GMidiInputFile    = arg.getArgStr(                  "-m",   "--midi-input");
    GAudioInputFile   = arg.getArgStr(                  "-a",   "--audio-input");
    GAudioOutputFile  = arg.getArgStr(                  "-o",   "--audio-output");

    GSampleRate       = arg.getArgFloat(                "-s",   "--sample_rate");
    GBlockSize        = arg.getArgInt(                  "-b",   "--block_size");
    GNumAudioInputs   = arg.getArgInt(                  "-c",   "--channels");
    GNumAudioOutputs  = arg.getArgIntAfterSymbol( ':',  "-c",   "--channels");
    GDecaySeconds     = arg.getArgFloat(                "-d",   "--decay-seconds");

    GListPlugins      = arg.hasOption(                  "-l",   "--list-plugins");
    GPrintDescriptor  = arg.hasOption(                  "-p",   "--print-descriptor");
    GFuzzBlockSize    = arg.hasOption(                  "-f",   "--fuzz-block-size");

    if (arg.hasOption("-r","--remap")) {
      GRemapCC        = arg.getArgInt(                  "-r",   "--remap");
      GRemapParam     = arg.getArgIntAfterSymbol( ':',  "-r",   "--remap");
    }
  }

  printf("plugin_path       %s\n",GPluginPath);
  printf("plugin_index      %i\n",GPluginIndex);
  printf("midi_input file   %s\n",GMidiInputFile);
  printf("audio_input file  %s\n",GAudioInputFile);
  printf("audio_output file %s\n",GAudioOutputFile);
  printf("sample_rate       %.2f\n",GSampleRate);
  printf("block_size        %i\n",GBlockSize);
  printf("channels (in:out) %i:%i\n",GNumAudioInputs,GNumAudioOutputs);
  printf("decay seconds     %.2f\n",GDecaySeconds);
  printf("list_plugins      %s\n",GListPlugins?"yes":"no");
  printf("print_descriptor  %s\n",GPrintDescriptor?"yes":"no");
  printf("fuzz block size   %s\n",GFuzzBlockSize?"yes":"no");
  printf("remap (cc:param)  %i:%i\n",GRemapCC,GRemapParam);

  entry.load(GPluginPath);

  if (GListPlugins) {
    MIP_Print("list plugins:\n");
    entry.listPlugins();
  }
  else if (GPrintDescriptor) {
    MIP_Print("print descriptor:\n");
    entry.printDescriptor(GPluginIndex);
  }
  else {
    // process
    MIP_Print("process:\n");
    MIP_ClapPlugin* plugin = entry.createPlugin(GPluginPath,GPluginIndex);
    plugin->activate(GSampleRate,1,GBlockSize);
    plugin->start_processing();
    process.process(plugin);
    plugin->stop_processing();
    plugin->deactivate();
    entry.destroyPlugin(plugin);

  }

  entry.unload();

  return 0;
}

