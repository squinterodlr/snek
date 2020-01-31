# Makefile for the console Snake game

CXX = g++
CXXFLAGS = -Wall -g -std=c++17
LDFLAGS = -lncurses

main: main.o Game.o
	$(CXX) $(CXXFLAGS) -o main.x main.o Game.o Snake.o $(LDFLAGS)

main.o: main.cpp Game.h
	$(CXX) $(CXXFLAGS) -c main.cpp	

Game.o: Snake.o Game.h Snake.h
	$(CXX) $(CXXFLAGS) -c Game.cpp

Snake.o: Snake.h
	$(CXX) $(CXXFLAGS) -c Snake.cpp

clean:
	rm -f *.o *.x