snake: main.o game.o
	g++ -lmingw32 -lSDL2main -lSDL2 main.o game.o -o snake.exe

main.o: main.cpp main.h
	g++ -c main.cpp

game.o: game.cpp game.h
	g++ -c game.cpp

clean:
	rm *.o snake