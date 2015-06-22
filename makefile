cpp=g++

all: binDir collectDataFromSam  

binDir:
	mkdir -p bin

collectDataFromSam:
	$(cpp) src/collectDataFromSam.cpp -o bin/collectDataFromSam
