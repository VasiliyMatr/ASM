
main: link clear

link: cmpl
	g++ MakePatch.o PatchMain.o -O3 -lsfml-graphics -lsfml-window -lsfml-system -o Patch.exe

cmpl:
	g++ -c -msse4.2 PatchMain.cpp MakePatch.cpp

clear: link
	rm *.o