
TABLE_SIZE_ = 1013
DATA_PATH_  = data/

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

plotting:
	python3 -W ignore plotting.py $(TABLE_SIZE_) $(DATA_PATH_)lettersAvgStat.txt
	python3 -W ignore plotting.py $(TABLE_SIZE_) $(DATA_PATH_)firstLetterStat.txt
	python3 -W ignore plotting.py $(TABLE_SIZE_) $(DATA_PATH_)oneValStat.txt
	python3 -W ignore plotting.py $(TABLE_SIZE_) $(DATA_PATH_)lettersSumStat.txt
	python3 -W ignore plotting.py $(TABLE_SIZE_) $(DATA_PATH_)myStat.txt
	python3 -W ignore plotting.py $(TABLE_SIZE_) $(DATA_PATH_)crc32Stat.txt
