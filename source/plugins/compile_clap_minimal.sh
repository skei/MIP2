g++ -Wall -std=c++14 -O2 -fPIC -c clap-minimal.cpp -o build.o
g++ -shared build.o -o clap-minimal.clap -s
rm build.o

