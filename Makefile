CC      := g++

TARGETS:= main

.PHONY: all clean

all: $(TARGETS)

clean:
	-	rm -f $(TARGETS)

% : %.cpp
	$(CC) -std=c++17 -Wall -o $@ -I../AlgoraDynOptimization/src/ -I../AlgoraCore/src/ -L../AlgoraDynOptimization/build/Debug/ -L../AlgoraCore/build/Debug/ $^ -lAlgoraDyn -lAlgoraCore -g 
