cody: parser.o CCScanner.o names.o devices.o network.o monitor.o
	g++ -g -o cody parser.o names.o devices.o network.o CCScanner.o monitor.o
parser.o: parser.cpp parser.h CCScanner.h names.h devices.h network.h monitor.h
	g++ -g -c parser.cpp
CCScanner.o: CCScanner.h CCScanner.cpp names.h
	g++ -g -c CCScanner.cpp
names.o: names.h
	g++ -g -c names.cc
network.o: network.cc network.h names.h
	g++ -g -c network.cc
devices.o: devices.cc devices.h network.h names.h
	g++ -g -c devices.cc
monitor.o: network.cc network.h names.h names.cc
	g++ -g -c monitor.cc
clean:
	rm *.o cody