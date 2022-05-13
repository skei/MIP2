this is NOT a 'proper' synth.. it's the thing i have been experimenting with, while i rebuilt the mip2 framework around clap.. so, this is just as much a test/beta for the framework itself.. it is used for most (if not all) my plugins (and related applications), so the more stable and compatible i can make it, the better..

the synth itself.. it is build using clap 0.25.0, so you need a recent host (currently the newest bitwig beta) to use it.. it has quite minimalistic audio capabilities - just enough for testing clap and the framework..

it has/uses/is:

* linux, 64bit, xcb/cairo
* minimalistic, simplistic, naive
* saw/pulse osc, lp filter, amp envelope
* 16-voice polyphonic
* note expressions (brightnes: filter, pressure: volume, tuning: pitch)
* polyphonic modulation (osc, filter, pitch)
* uses host thread-pool (if supported) for the (up to 16) voices
* sample accurate events
* no optimizations, simd, shortcuts, approximations, assembler, anything, except -O3
* no aesthetics, no parameter tuning, no usefulness, no guarantees, ..
..

youtube:
https://www.youtube.com/watch?v=LF5HGZcXIQk

[todo: more...]

