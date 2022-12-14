#include "graph.h"

void smallest_degree_last() {
  uint8_t  used[MAX_DIM];
  uint16_t neighbor_color  = 0;
  uint16_t min_color       = 0;


  //////////////////////////////
  //      WEIGHING PHASE      //
  //////////////////////////////
  uint16_t weights[MAX_DIM];
  uint16_t max_weight = 0;
  uint16_t current_degree;
  uint16_t minimum_degree = MAX_DIM;

  // Zero out weights
  for (int node = 0; node < MAX_DIM; node++) {
    if (node > num_nodes) { break; }
    weights[node] = 0;
  }

  // Get minimum degree
  for (int node = 0; node < MAX_DIM; node++) {
    if (node > num_nodes) { break; }
    current_degree = degrees[node] >> (DEGREE_OFFSET);
    if (current_degree < minimum_degree)
      minimum_degree = current_degree;
  }

  // Assign weights
  for (int weight = 1; weight < MAX_DIM; weight++) {
    if (weight > num_nodes) { break; }
    for (int node = 0; node < MAX_DIM; node++) {
      if (node > num_nodes) { break; }
      current_degree = degrees[node] >> (DEGREE_OFFSET);
      for (int neighbor = 0; neighbor < MAX_DIM; neighbor++) {
        if (neighbor > num_nodes) { break; }
        if (adj_matrix[node][neighbor] == 1 && weights[neighbor] > 0 && weights[neighbor] < weight) {
          current_degree--;
        }
      }
      if (weights[node] == 0 && current_degree <= weight) {
        weights[node] = weight;
        max_weight = weight;
      }
    }
  }
  printf("[ info] completed graph weighting\n");

  printf("\n\n\nWeights:\n-------\n");
  for (int i = 0; i < 30; i++) {
    if (i % 8 == 0)
      printf("\n");
    printf("(%3d, %4d)   ", i, weights[i]);
  }
  printf("\n");
  printf("\n\n");


  //////////////////////////////
  //      COLORING PHASE      //
  //////////////////////////////

  // Begin algorithm by first going from highest to lowest degree
  for (int weight = MAX_DIM - 1; weight >= 0; weight--) {
    if (weight > max_weight)
      continue;

    // Through each node, get neighbors and get what colors they're using
    for (int node = 0; node < MAX_DIM; node++) {
      if (node > num_nodes) { break; }
      if (weights[node] == weight) {

        // Zero out used neighbor colors
        min_color = 1;
        for (int zeroed = 0; zeroed < MAX_DIM; zeroed++) {
          if (zeroed > num_nodes) { break; }
          used[zeroed] = 0;
        }

        // Mark all used colors by adjacent vectors
        for (int neighbor = 0; neighbor < MAX_DIM; neighbor++) {
          if (neighbor > num_nodes) { break; }
          if (adj_matrix[node][neighbor] == 1) {
            neighbor_color = colors[neighbor];
            used[neighbor_color] = 1;
          }
        }

        // Find lowest color available
        for (int color = 1; color < MAX_DIM; color++) {
          if (color > num_nodes) { break; }
          if (!used[color]) {
            min_color = color;
            break;
          }
        }

        // Assign the minimum color found
        colors[node] = min_color;
      }
    }
  }
  return;
}
