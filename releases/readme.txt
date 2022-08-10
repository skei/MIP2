
here's a set of clap plugins i ported during the development of the mip2 framework (or toolbox, as i like to call it).. they aren't really meant to be big and shiny plugins competing with the other great plugins out there.. i did them mainly to test mip2, and to see if i were on the right track regarding abstractions and simplifications and things.. if mip2 is actually usable for making proper plugins..
but then i thought it could be nice to have them available in linux and in both the hosts i use myself (bitwig/reaper).. and, maybe someone else might find a use for one or two of them.. :-)
when i finish the vst2/3 wrappers, they will all be available in these formats as well..
and automatically get a better 'generic' gui when that part is more fleshed out..

https://torhelgeskei.com/wp-content/uploads/2022/08/sa_clap_tests_9aug2022.zip

(130 kb, linux, 64bit, clap)

see also:
https://github.com/skei/MIP2
https://torhelgeskei.com
tor.helge.skei@gmail.com

----------------------------------------------------------------------


--------------------
sa_fibonacci
--------------------

fibonacci/pisano based note effect.. it doesn't generate notes, but
modifies incoming ones, and pitches them up or down according to the
'current number' in the fibonacci sequence..

i will add an editor, so you can see better what's going on and what the current state is.. but that have to wait until the gui stuff is more stable..

https://www.youtube.com/watch?v=o1eLKODSCqw

(there's also some other previews of upcoming things on my youtube channel)

--------------------
sa_ladspa_wrapper
--------------------

it looks in /usr/lib/ladspa, and wraps each plugin it finds as a clap plugin.. not all plugins make much sense in a regular daw setting, and not all audio ports map nicely with normal stereo channels and similar.. some hosts let you configure this.. but an idea i had is to add a .ini file thing, where you can route and override ports and channels.. and maybe also a double-mono version..

this could also be extended to make a dssi wrapper.. but there's not many interesting dssi synths around, i think.. earlier, i successfully wrapped whysynth to vst2, and that had some quite nice sounds.. but apart from that, i'm not sure if there's any point.. and then, the next step would be lv2.. but currently, i have very little motivation to do that.. but we'll see..

https://www.youtube.com/watch?v=jLj6-Z8tSdc

--------------------
sa_ports
--------------------

original authors:

  compciter - Michael Gruhn
  event_horizon - Thomas Scott Stillwell
  exciter - Michael Gruhn
  exciter2 - Thomas Scott Stillwell
  hall_reverb - Michael Gruhn
  hrtf - Benno Straub
  large_room_reverb - Michael Gruhn
  limiter - Michael Gruhn
  saturation - tale/loser
  small_room_reverb - Michael Gruhn
  sonic_maximizer - Dominique Guichaoua
  tilteq - Lubomir I. Ivanov
  transient_killer - Michael Gruhn

see individual source files for info, licenses, links, etc..
https://github.com/skei/MIP2/tree/master/source/plugins/sa_ports

