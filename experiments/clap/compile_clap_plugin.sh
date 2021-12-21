
g++ -Wall -std=c++14 -fPIC -g -DMIP_DEBUG -I../../src -c clap_plugin.cpp -o clap_plugin.o

g++ -shared clap_plugin.o -o plugin.clap -fPIE

rm ./clap_plugin.o

