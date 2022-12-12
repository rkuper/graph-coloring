#include "graph.h"

int early_term_ldf(float percent_done) {
  uint8_t  used[MAX_DIM];
  uint16_t current_degree;
  uint16_t max_degree      = 0;
  uint16_t neighbor_color  = 0;
  uint16_t min_color       = 0;
  uint16_t colored_nodes   = 0;
  uint16_t max_color       = 0;

  // Get the maximum degree from graph
  for (int node = 0; node < MAX_DIM; node++) {
    if (node > num_nodes) { break; }
    current_degree = degrees[node] >> (DEGREE_OFFSET);
    if (current_degree > max_degree)
      max_degree = current_degree;
  }

  // Begin algorithm by first going from highest to lowest degree
  for (int degree = MAX_DIM - 1; degree >= 0; degree--) {
    if (degree > max_degree)
      continue;
    /* if (degree < (int)(max_degree*percent_done)) */
    /*   break; */
    if (colored_nodes > (int)(num_nodes * percent_done))
      break;

    // Through each node, get neighbors and get what colors they're using
    for (int node = 0; node < MAX_DIM; node++) {
      if (node > num_nodes) { break; }
      if ((degrees[node] >> DEGREE_OFFSET) == degree) {

        // Zero out used neighbor colors
        min_color = 1;
        for (int zeroed = 0; zeroed < MAX_DIM; zeroed++)
          used[zeroed] = 0;

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
          if (!used[color]) {
            min_color = color;
            break;
          }
        }

        // Assign the minimum color found
        colors[node] = min_color;
        if (max_color < min_color)
          max_color = min_color;
        colored_nodes++;
      }
    }
  }
  return max_color;
}
