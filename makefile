river: main.o river.o
	g++ -Wall -g main.o river.o -o river

main.o: main.cpp river.h
	g++ -Wall -c main.cpp

river.o: river.cpp river.h
	g++ -Wall -c river.cpp