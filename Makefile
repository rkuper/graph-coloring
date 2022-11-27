CC=gcc
CFLAGS=-I. -g
DEPS = graph.h
OBJ = graph.o greedy.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

graph: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(OBJ)

.PHONY: clean

clean:
	rm -f $(OBJ)
