#include "graph.h"

void exact_coloring(int k_colors) {
  uint8_t used[MAX_DIM];
  int node, neighbor, color;

  for (node = MAX_DIM - 1; node >= 0; node--) {
    if (node > num_nodes) { break; }
    if (colors[node] > 0)
      continue;

    for (color = 0; color < MAX_DIM; color++) {
      if (color > num_nodes) { break; }
      used[color] = 0;
    }

    for (neighbor = MAX_DIM; neighbor >= 0; neighbor--) {
      if (neighbor > num_nodes) { break; }
      if (neighbor != node && adj_matrix[node][neighbor] &&
          colors[neighbor] > 0)
        used[colors[neighbor]] = 1;
    }

    for (color = 1; color < MAX_DIM; color++) {
      if (color > k_colors) { break; }
      if (used[color] == 0) {
        colors[node] = color;
        break;
      } else if (color == k_colors) {
        printf("[error] FAILED!\n");
      }
    }
  }
}
