


all: link clear

link: cmpl
	g++ main.o Mandel.o -lsfml-graphics -lsfml-window -lsfml-system -o Mandel.exe

cmpl:
	g++ -c main.cpp Mandel.cpp

clear: link
	rm *.o