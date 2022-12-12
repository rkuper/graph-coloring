CC=gcc
CFLAGS=-I. -g
DEPS = graph.h
OBJ = exact_coloring.o early_term_rlf.o early_term_ldf.o fpga_implementation.o recursive_largest_first.o smallest_degree_last.o largest_degree_first.o greedy.o graph.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

graph: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(OBJ)

.PHONY: clean

clean:
	rm -f $(OBJ)
