a simple and minimal command-line clap plugin host.. one cpp file, doesn't depend on any other files/libs (except, it uses libdl for loading the plugin, and libsndfile for handling wav files).. just copy the entire directory somewhere, and compile..

g++ -Wall -g -c main.cpp -o main.o
g++ -o clap-host main.o -ldl -lsndfile

or ./compile.sh

