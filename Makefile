LIBS=-lBox2D -lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ -c "main.cpp"
	g++ -c "classes/player.cpp"
	g++ -o "Zeke's Puzzle.app" main.o player.o $(LIBS)

clean:
	rm -f *.o
