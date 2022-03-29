

#include "clap/clap.h"
#include "midi_file.hpp"

int main() {
  MidiFile midi;
  midi.load("/DISKS/sda2/code/git/MIP2/bin/basic-ramp.MID");
  midi.print();
}
