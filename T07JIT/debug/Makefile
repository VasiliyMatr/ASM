
INCLUDE_PATH_ = -Iheaders
SOURCE_PATH_  = source/

GPP_FLAGS_    = -g -O0
OUT_NAME_     = jit-debug

.PHONY: all build dummy

all: build

build: $(wildcard $(SOURCE_PATH_)*.cpp)
	g++ $(SOURCE_PATH_)*.cpp $(GPP_FLAGS_) $(INCLUDE_PATH_) -o $(OUT_NAME_)

dummy:
	nasm -f elf64 cmd/cmd-dummy.asm
	ld cmd/cmd-dummy.o -o cmd/cmd-dummy
	rm cmd/cmd-dummy.o

run:
	./jit-debug
	sudo chmod 755 jit-out