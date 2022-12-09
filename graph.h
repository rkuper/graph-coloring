#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_DIM       10000
#define DEGREE_OFFSET 16

// Why header file, why?
extern uint32_t degrees[MAX_DIM];
extern uint8_t  adj_matrix[MAX_DIM][MAX_DIM];
extern int      colors[MAX_DIM];
extern int      degree_inc;
extern int      num_nodes;
extern int      num_edges;

// Graph coloring solutions
void greedy();
void largest_degree_first();
void smallest_degree_last();

// Useful helpers
void print_adj_matrix(int n);
int max_colors();
void print_coloring();

#endif // __GRAPH_H__
