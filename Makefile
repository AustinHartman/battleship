output: main.o Ship.o
	g++ main.o Ship.o -o output

main.o: main.cpp
	g++ -c main.cpp

Ship.o: Ship.cpp Ship.h
	g++ -c Ship.cpp

clean: 
	rm *.o output
