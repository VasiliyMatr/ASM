
INCLUDE_PATH_ = -Iheaders
SOURCE_PATH_  = source/

GPP_FLAGS_    = -g -O0
OUT_NAME_     = jit-debug

.PHONY: all build

all: build

build: $(wildcard $(SOURCE_PATH_)*.cpp)
	g++ $(SOURCE_PATH_)*.cpp $(GPP_FLAGS_) $(INCLUDE_PATH_) -o $(OUT_NAME_)