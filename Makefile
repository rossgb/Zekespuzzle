LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ -c "main.cpp"
	g++ -o "Zeke's Puzzle" main.o $(LIBS)

clean:
	rm -f *.o
