CC=clang
CXX=clang++

IFLAGS=-I./include

MAIN=main

all: 
	$(CXX) $(MAIN).cpp $(IFLAGS) -o $(MAIN)
clean: 
	rm $(MAIN)