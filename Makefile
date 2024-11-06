br: build run
bd: build debug

build:
	g++ -g -o main main.cpp Parser/Parser.cpp ValueInterface/ValueInterface.cpp -lreadline
#	g++ -g -std=c++17 -o test CommandLineTest.cpp -L ./MemoryManager -lMemoryManager

run:
	./main

debug:
	gdb ./main

mytest:
	g++ -g -std=c++17 -o testme test.cpp MemoryManager/MemoryManager.cpp
	gdb ./testme
