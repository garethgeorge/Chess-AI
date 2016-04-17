CXX = g++ 
CFLAGS = -std=c++11 -Wdivision-by-zero
OBJECTS=main.o board.o
BINARY=program

all: program

program: $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(BINARY) $(OBJECTS)

main.o: main.cpp board.h
	$(CXX) $(CFLAGS) -c main.cpp -o main.o

board.o: board.cpp board.h
	$(CXX) $(CFLAGS) -c board.cpp -o board.o

clean:
	rm -f *.o $(BINARY)