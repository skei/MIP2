this is NOT a 'proper' synth.. it's the thing i have been experimenting with, while i rebuilt the mip2 framework around clap.. so, this is just as much a test/beta for the framework itself.. it is used for most (if not all) my plugins (and related applications), so the more stable and compatible i can make it, the better..

the symth itself.. it is build using clap 0.25.0, so you need a recent host (currently the newest bitwig beta) to use it.. it is 16-voice polyphonic, quite minimalistic audio capabilities - just enough for testing clap and the framework) features.

it has/uses/is:

* linux, 64bit, xcb/cairo
* minimalistic, simplistic, naive
* saw/pulse osc, lp filter, amp envelope
* 16-voice polyphonic
* note expressions: brightnes:filter
* mono, and polyphonic modulation
* host thread-pool for the voices
* sample accurate events
* no optimizations, no simd, assembler, anything, except -O3
* no aesthetics, no parameter tuning, no usefulness, no guarantees, ..
..

[todo: more...]

