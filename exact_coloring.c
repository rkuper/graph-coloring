#include "graph.h"

void exact_coloring(int k_colors) {
  uint8_t used[MAX_DIM];
  int node, neighbor, color;

  for (node = MAX_DIM - 1; node >= 0; node--) {
    if (colors[node] > 0)
      continue;

    for (color = 0; color < MAX_DIM; color++) {
      used[color] = 0;
    }

    for (neighbor = 0; neighbor < MAX_DIM; neighbor++) {
      if (neighbor != node && adj_matrix[node][neighbor] &&
          colors[neighbor] > 0)
        used[colors[neighbor]] = 1;
    }

    for (color = 1; color <= k_colors; color++) {
      if (used[color] == 0) {
        colors[node] = color;
        break;
      } else if (color == k_colors) {
        printf("[error] FAILED!\n");
      }
    }
  }
}
