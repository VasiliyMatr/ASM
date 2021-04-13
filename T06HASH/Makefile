
TABLE_SIZE_ = 1013
DATA_PATH_  = data/

all: opt

opt:
	g++ -O2 -msse4.2 -g main.cpp optimized/*.cpp -o oHash

memCheck:
	valgrind --leak-check=full ./oHash

profile:
	valgrind --tool=callgrind ./oHash
	kcachegrind callgrind.out.*
	rm callgrind.out.*

plt = python3 -W ignore plotting.py $(TABLE_SIZE_) $(1)

plotting:
	$(foreach file, $(wildcard $(DATA_PATH_)*Stat.txt), $(call plt, $(file));)