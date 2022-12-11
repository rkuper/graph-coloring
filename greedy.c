#include "graph.h"



void greedy() {
  int used[MAX_DIM];

  // First color is set to color 0
  colors[0] = 1;
  for(int node = 1; node < MAX_DIM; node++) {
    if (node > num_nodes) { break; }
    colors[node] = 0;
  }

  // used array starts with nothing
  for(int node = 0; node < MAX_DIM; node++) {
    if (node > num_nodes) { break; }
    used[node] = 0;
  }

  // Per node in graph
  for(int node = 1; node < MAX_DIM; node++) {
    // Mark all used colors
    if (node > num_nodes) { break; }
    for(int neighbor = 0; neighbor < MAX_DIM; neighbor++) {
      if (neighbor > num_nodes) { break; }
      if(adj_matrix[node][neighbor] == 1) {
        if(colors[neighbor] != 0)
          used[colors[neighbor]] = 1;
      }
    }

    // Find first color not used
    int color;
    for(color = 1; color < MAX_DIM; color++) {
      if (color > num_nodes || used[color] == 0) { break; }
    }

    // Assign color found
    colors[node] = color;

    // Reset colors used
    for(int neighbor = 0; neighbor < MAX_DIM; neighbor++) {
      if (neighbor > num_nodes) { break; }
      if(adj_matrix[node][neighbor] == 1) {
        if(colors[neighbor] != 0)
          used[colors[neighbor]] = 0;
      }
    }
  }
}
