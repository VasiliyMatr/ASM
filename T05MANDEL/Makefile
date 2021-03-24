


all: link clear

link: cmpl
	g++ main.o Mandel.o -o3 -lsfml-graphics -lsfml-window -lsfml-system -o Mandel.exe

cmpl:
	g++ -c -msse4.2 main.cpp Mandel.cpp

clear: link
	rm *.o