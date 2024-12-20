##
# Kcreme
#
# @file
# @version 0.1

all: parser

parser: main.cpp tokens.cpp parser.cpp
	g++ -o parser parser.cpp tokens.cpp main.cpp

tokens.cpp: kcreme.l
	lex -o tokens.cpp kcreme.l

parser.cpp: kcreme.y
	bison -Dparse.trace -d -o parser.cpp kcreme.y

# end
