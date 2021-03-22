
main: link clear

link: cmpl
	g++ MakePatch.o PatchMain.o -lsfml-graphics -lsfml-window -lsfml-system -o Patch.exe

cmpl:
	g++ -c MakePatch.cpp
	g++ -c PatchMain.cpp

clear: link
	rm *.o