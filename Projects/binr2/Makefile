TARGET  = binr2
CC      = g++
FLAGS   = -Wall -std=c++11 -I inc
SRC     = src/main.cpp src/binr2.cpp
  
all:
	$(CC) -o $(TARGET) $(FLAGS) $(SRC)

debug:
	$(CC) -o $(TARGET) $(FLAGS) -Ddebug $(SRC)

clean:
	rm -rf $(TARGET)