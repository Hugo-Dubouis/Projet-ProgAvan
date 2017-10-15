all :snake

snake: main.o
	g++ main.o `sdl2-config --cflags --libs` -o snake

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm -rf *o snake
