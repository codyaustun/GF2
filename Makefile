CC = g++
OBJ = Parser.o CCScanner.o

parser: $(OBJ)
    $(CC) -g -o parser $(OBJ)
    
parser.o: parser.cpp
    $(CC) -g -c parser.cpp
        
CCScanner.o: CCScanner.h CCScanner.cpp
    $(CC) -g -c CCScanner.cpp