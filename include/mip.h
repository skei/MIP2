#ifndef mip_included
#define mip_included
//----------------------------------------------------------------------

#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIP_Print   printf
#define MIP_DPrint  printf
#define MIP_PRINT   printf("\n")

#define MIP_MIDI_NOTE_OFF            0x80
#define MIP_MIDI_NOTE_ON             0x90
#define MIP_MIDI_POLY_AFTERTOUCH     0xA0
#define MIP_MIDI_CONTROL_CHANGE      0xB0
#define MIP_MIDI_PROGRAM_CHANGE      0xC0
#define MIP_MIDI_CHANNEL_AFTERTOUCH  0xD0
#define MIP_MIDI_PITCHBEND           0xE0
#define MIP_MIDI_SYS                 0xF0

//----------------------------------------------------------------------
#endif
