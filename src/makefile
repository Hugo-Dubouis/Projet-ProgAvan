all :snake

snake: main.o
	g++ main.o -lmingw32 -lSDL2main -lSDL2 -o snake.exe

main.o: main.cpp
	g++ -c main.cpp 

clean:
	rm -rf *o snake
