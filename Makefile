OUTPUT = output.txt
CC = g++
EXE = crosswords

all: crosswords.cpp
	$(CC) crosswords.cpp -o $(EXE)

run: $(EXE)
	./$(EXE) > $(OUTPUT)
