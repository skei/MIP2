
g++ -Wall -std=c++14 -fPIC -g -DMIP_DEBUG -I../../src -c ../../build/build.cpp -o build.o

g++ -shared build.o -o test.clap -fPIE  -ldl -lrt -lpthread -lxcb -lxcb-util -lxcb-image -lxcb-cursor -lxcb-keysyms -lcairo -lsndfile

rm ./build.o

