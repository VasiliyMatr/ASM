
all: build
build:
	g++ -O2 -g source/*.cpp -o hash

run:
	./hash

memCheck:
	valgrind --leak-check=full ./hash

profile:
	valgrind --tool=callgrind ./hash
	kcachegrind callgrind.out.*
	rm callgrind.out.*