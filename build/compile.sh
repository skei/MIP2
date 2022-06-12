#!/bin/bash

# TODO!

# --------------------------------------------------
# default values
# --------------------------------------------------

INPUT_PATH="./"
OUTPUT_PATH="./"
PLUGIN_NAME=""

# --------------------------------------------------
# help
# --------------------------------------------------

function print_help {
  echo "usage: ./compile <flags>"
  echo "  -i input path"
  echo "  -o output path"
  echo "  -p plugin file (without path or ext)"
  echo "example: ./compile -i ../plugins ../bin appleffilter"
  exit 1
}


# --------------------------------------------------
# parse command line
# --------------------------------------------------

if [ "$#" -eq 0 ]; then
  print_help
fi

#if [ "$#" -ne 3 ]; then
#  print_help
#fi

while getopts i:o:p: opt
do
  case "$opt" in
    i) INPUT_PATH="$OPTARG";;
    o) OUTPUT_PATH="$OPTARG";;
    p) PLUGIN_NAME="$OPTARG";;
  esac
done

# INPUT_FILE=${INPUT##*/}
# INPUT_BASE=${INPUT_FILE%%.*}
# INPUT_EXT=${INPUT_FILE##*.}
# INPUT_DIR=${INPUT%$INPUT_FILE}

# echo "INPUT_PATH =  " $INPUT_PATH
# echo "OUTPUT_PATH = " $OUTPUT_PATH
# echo "PLUGIN NAME = " $PLUGIN_NAME

# --------------------------------------------------
#
# --------------------------------------------------

#for f in $INPUT_PATH
#do
#  echo "compiling " "$INPUT_PATH/$f"
#done

# echo "compiling $INPUT_PATH/$PLUGIN_NAME.h"

FILE_IN="$INPUT_PATH/$PLUGIN_NAME.h"
FILE_OUT="$OUTPUT_PATH/$PLUGIN_NAME.so"

echo "compiling $FILE_IN > $FILE_OUT"
echo "./compile -i $FILE_IN -o $FILE_OUT"
