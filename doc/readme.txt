todo.. preliminary .. sketch.. 

examine the /build/mip2.cbp file (code::blocks project file), especially the start of it, and you will easily find the settings for the various compile targets..

linked libraries:
pthread, m, rt, dl
sndfile, jack
cairo
xcb, xcb-util, xcb-image, xcb-cursor, xcb-keysyms, xkbcommon
X11, X11-xcb
GL, GLU, GLX, glut

--as-needed gets rid of those you don't actually use.. i'm lazy, and just add everything there, and gcc takes care of whetever it needs to do.. but if you don't have any of these, and don't plan to use tnything that uses them, get rid of it.. various parts of the toolkit needs different libraries, so you can just add stoff ftrom the list until the compiles stops complaining..

everythig else is already included
(as .h files, so you don't need to compile, or link anything)

when using the codeblocks project, i set it up to compile the /build/build.cpp file, and in there, i have just a list of the other test/plugins/projects, that i comment and uncomment when needed.. lazy and hackish, but it makes switching between tests and project real quick.. also, i have setup bitwig (or any other clap-host) to scan the /bin folder for clap plugins, so it will automatically find the compiled plugins..

i will make scripts to simplify compilation..

------------------------------

notes to self:


C = Compile
L = link(
----------)

all:
C -Wall
  -std=c++14
  -Wl,--as-needed

-I../include/extern/clap

exe:
C -fno-pie
L -no-pie -s

plugin:
C -fPIC
L -fPIE

debug:
C -g
  -rdynamic
  -Wl,-Bsymbolic

release:
C -O2

libs:
  pthread, m, rt, dl
  sndfile, jack, cairo
  xcb, xcb-util, xcb-image, xcb-cursor, xcb-keysyms
