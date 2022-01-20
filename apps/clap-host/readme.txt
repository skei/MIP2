this one is doesn't depend on any other files/libs (?)
except is uses libdl for loading plugin, and libsndfile for loading/saving wav
clap files is included

g++ -Wall -g -c main.cpp -o main.o
g++ -o clap-host main.o -ldl -lsndfile

or ./compile.sh

