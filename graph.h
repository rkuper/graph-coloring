#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <stdint.h>

#define MAX_DIM 1000

// Graph info
uint32_t  degrees[MAX_DIM];
uint8_t   adj_matrix[MAX_DIM][MAX_DIM];
uint8_t   colors[MAX_DIM];

// Graph coloring solutions
void greedy_coloring();

// Useful helpers
int max_colors();
void print_coloring();

#endif // __GRAPH_H__
