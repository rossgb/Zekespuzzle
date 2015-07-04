LIBS=-lBox2D -lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ -c "main.cpp"
	g++ -o "Zeke's Puzzle.app" main.o $(LIBS)

clean:
	rm -f *.o
