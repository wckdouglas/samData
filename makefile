cpp=g++

all: binDir collectDataFromSam mismatch

binDir:
	mkdir -p bin

collectDataFromSam:
	$(cpp) src/collectDataFromSam.cpp -o bin/collectDataFromSam

mismatch:
	$(cpp) src/collectMismatch.cpp -o bin/collectMismatch
