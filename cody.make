cody: parser.o CCScanner.o
	g++ -g -o cody parser.o CCScanner.o
parser.o: parser.cpp parser.h CCScanner.h
	g++ -g -c parser.cpp
CCScanner.o: CCScanner.h CCScanner.cpp
	g++ -g -c CCScanner.cpp
clean:
	rm *.o cody