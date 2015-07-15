LIBS=-lBox2D -lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ -c "main.cpp"
	g++ -c "classes/animation.cpp"
	g++ -c "classes/orb.cpp"
	g++ -c "classes/player.cpp"
	g++ -c "classes/blueguy.cpp"
	g++ -c "classes/hoop.cpp"
	g++ -o "Zeke's Puzzle.app" main.o orb.o hoop.o player.o blueguy.o animation.o $(LIBS)

clean:
	rm -f *.o
