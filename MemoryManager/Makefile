all: clean build link

build:
	g++ -g -c -std=c++17 -o MemoryManager.o MemoryManager.cpp

link:
	ar cr libMemoryManager.a MemoryManager.o

clean:
	rm -f libMemoryManager.a MemoryManager.o
