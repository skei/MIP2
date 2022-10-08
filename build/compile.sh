#!/bin/bash

# --------------------------------------------------

#g++ -Wextra -Wall -std=gnu++14 -Wl,--as-needed -g -fno-pie -rdynamic -Wl,-Bsymbolic -DMIP_EXE -DMIP_DEBUG -I../include -I../include/extern/vst3 -c /DISKS/sda2/code/git/MIP2/build/build.cpp -o obj/debug/build/build.o

#g++  -o ../bin/build_linux_debug obj/debug/build/build.o  -Wl,--as-needed -no-pie  -lpthread -lrt -lm -ldl -lxcb -lxcb-util -lxcb-image -lxcb-cursor -lxcb-keysyms -lxkbcommon -lX11 -lX11-xcb -lGL -lGLX -lGLU -lglut -lcairo -ljack -lsndfile

# --------------------------------------------------
# --------------------------------------------------
# --------------------------------------------------

#ALL_C_FLAGS="-std=c++11 -Wall -Wl,--as-needed, -faligned-new, -fexceptions "
#DEBUG_C_FLAGS="-g, -Wl,-Bsymbolic, -rdynamic -DKODE_DEBUG "
#RELEASE_C_FLAGS="-O3, -s, -msse4, -mfpmath=sse "
#EXE_C_FLAGS="-fno-pie "
#VST_C_FLAGS="-fPIE "

#ALL_L_FLAGS="-lrt, -lpthread, -lxcb, -lxcb-util, -lxcb-image, -lxcb-cursor, -lxcb-keysyms, -lcairo, -lportaudio, -lSDL2, -lSDL2_image, -lSDL2_mixer "
#EXE_L_FLAGS="-no-pie "
#VST_L_FLAGS="-fPIE, -fPIC "

# --------------------------------------------------
# default values
# --------------------------------------------------

#ARCH=64
#DEBUG="off"
#GPL="off"
#FORMAT="vst3"
#GUI="nogui"
#PAINTER="nopainter"
#FLAGS=""
#LIBS=""

FLAGS="-Wall "
DEF=""
INC="-I../include "
LIB="-lpthread -lrt -lm"
#POST="-s -lstdc++ -lm "

# --------------------------------------------------
# help
# --------------------------------------------------

function print_help {
  echo "usage: ./compile <flags>"
  echo "  -i input file name/path"
  echo "  -o output file name/path"
  echo "  -f format (CLAP, vst2, vst3, ladspa, dssi, lv2, exe)"
  echo "  -g gui (NOGUI,xcb,win32)"
  echo "  -p painter (NOPAINTER,xcb,cairo,opengl,nanovg)"
#  echo "  -a architecture (32/64)"
#  echo "  -v version (v1)"
  echo "  -d debug"
  echo "  -h this help"
  echo "  -F additional gcc flags"
#  echo "  -G use gpl"
#  echo "  -D additional defines"
#  echo "  -I additional include directories"
#  echo "  -L additional linked libraries"
  exit 1
}

# --------------------------------------------------
# parse command line
# --------------------------------------------------

if [ "$#" -eq 0 ]; then
  print_help
fi

#while getopts i:o:t:g:a:v:dhF:GD:I:L: opt
while getopts i:o:f:g:p:a:dhG opt
do
  case "$opt" in
    i) INPUT="$OPTARG";;
    o) OUTPUT="$OPTARG";;
    f) FORMAT="$OPTARG";;
    g) GUI="$OPTARG";;
    p) PAINTER="$OPTARG";;
#    a) ARCH="$OPTARG";;
    d) DEBUG="on";;
    h) print_help;;
#    G) GPL="on";;
#    v) VERSION="$OPTARG";;
    F) FLAGS+="$OPTARG ";;
#    D) DEF+="-D$OPTARG ";;
#    I) INC+="-I$OPTARG ";;
#    L) LIB+="-l$OPTARG ";;
  esac
done

# --------------------------------------------------
# paths..
# --------------------------------------------------

INPUT_FILE=${INPUT##*/}
INPUT_BASE=${INPUT_FILE%%.*}
INPUT_EXT=${INPUT_FILE##*.}
INPUT_DIR=${INPUT%$INPUT_FILE}

echo ""
echo "INPUT     : $INPUT"
echo "INPUT_FILE: $INPUT_FILE"
echo "INPUT_BASE: $INPUT_BASE"
echo "INPUT_EXT : $INPUT_EXT"
echo "INPUT_DIR : $INPUT_DIR"

# input = ../plugins/fx_pitch.cpp"
# input_file = fx_pitch.cpp
# input_base = fx_pitch
# input_ext = cpp
# input_dir = ../plugins/

OUTPUT_FILE=${OUTPUT##*/}
OUTPUT_BASE=${OUTPUT_FILE%%.*}
OUTPUT_EXT=${OUTPUT_FILE##*.}
OUTPUT_DIR=${OUTPUT%$OUTPUT_FILE}

echo ""
echo "OUTPUT     : $OUTPUT"
echo "OUTPUT_FILE: $OUTPUT_FILE"
echo "OUTPUT_BASE: $OUTPUT_BASE"
echo "OUTPUT_EXT : $OUTPUT_EXT"
echo "OUTPUT_DIR : $OUTPUT_DIR"

# --------------------------------------------------
#
# --------------------------------------------------

#if [ "$OUTPUT" = "" ]; then
#  OUTPUT=$INPUT_BASE
#fi

# ----------

#if [ "$DEBUG" = "on" ]; then
#  C_FLAGS+=""
#else
#  FLAGS=""
#fi

# ----------

#if [ "$GPL" = "on" ]; then
#  INC+="-I../plugins/gpl "
#fi

# ----------

#if [ "$TYPE" = "exe" ]; then
#  FLAGS+="-DKODE_PLUGIN_EXE "
#fi

#if [ "$TYPE" = "ladspa" ]; then
#  FLAGS+="-DKODE_PLUGIN_LADSPA "
#fi

#if [ "$TYPE" = "dssi" ]; then
#  FLAGS+="-DKODE_PLUGIN_DSSI "
#fi

#if [ "$TYPE" = "lv2" ]; then
#  FLAGS+="-DKODE_PLUGIN_LV2 "
#fi

#if [ "$TYPE" = "vst2" ]; then
#  FLAGS+="-DKODE_PLUGIN_VST2 "
#fi

#if [ "$TYPE" = "vst3" ]; then
#  FLAGS+="-DKODE_PLUGIN_VST3 "
#fi

# ----------

#if [ "$ARCH" = "32" ]; then
#  FLAGS+="-m32 "
#fi

#if [ "$ARCH" = "64" ]; then
#  FLAGS+="-m64 "
#fi

# ----------

#if [ "$GUI" = "nogui" ]; then
#  DEF+="-DKODE_NO_GUI "
#fi

#if [ "$GUI" = "xcb" ]; then
#  FLAGS+="-DKODE_GUI_XCB "
#fi

# ----------

#if [ "$PAINTER" = "nopainter" ]; then
#  FLAGS+="-DKODE_NO_PAINTER "
#fi

#if [ "$PAINTER" = "cairo" ]; then
#  FLAGS+="-DKODE_PAINTER_CAIRO "
#fi

#if [ "$PAINTER" = "xcb" ]; then
#  FLAGS+="-DKODE_PAINTER_XCB "
#fi

# ----------

#INC+="-I"
#INC+=$INPUT_DIR

# --------------------------------------------------
# printout
# --------------------------------------------------

#echo "----------"
#echo "input:" $INPUT
#echo "output:" $OUTPUT
#echo "debug:" $DEBUG
#echo "type:" $TYPE
#echo "gui:" $GUI
#echo "flags:" $FLAGS
#echo "def:" $DEF
#echo "inc:" $INC
#echo "lib:" $LIB

# --------------------------------------------------
# create compile.cpp
# --------------------------------------------------

# echo 'compile.cpp:'
#echo '#include "base/kode.h"' >> compile.cpp
#echo '#include "'$INPUT_FILE'"' >> compile.cpp

# --------------------------------------------------
# finalize command line
# --------------------------------------------------

CMDLINE=""
CMDLINE+=$FLAGS
CMDLINE+=$DEF
CMDLINE+=$INC
CMDLINE+=$LIB

echo ""
echo "compiling $INPUT_BASE"
echo "gcc" $CMDLINE $INPUT -o $OUTPUT $POST
# gcc $CMDLINE compile.cpp -o $OUTPUT.so $POST

##rm compile.cpp


