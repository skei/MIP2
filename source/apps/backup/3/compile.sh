g++ -Wall -g -c main.cpp -o main.o
g++ -o clap-host main.o -ldl -lsndfile
rm main.o

