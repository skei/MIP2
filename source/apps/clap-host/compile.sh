g++ -Wall -g -c main.cpp -o main.o -std=c++11
g++ -o clap-host main.o -ldl -lsndfile
rm main.o

