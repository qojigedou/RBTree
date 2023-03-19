MYTREE_NAME = main
MYTREE_SOURCE = $(MYTREE_NAME).cpp
MYTREE_TARGET = $(MYTREE_NAME).out


TARGETS = $(MYTREE_TARGET)

CC = g++
CFLAGS = -w -std=c++11

%.out: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

all: $(TARGETS)

.PHONY: clean

clean:
	$(RM) $(TARGETS) *.o core