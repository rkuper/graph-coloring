#include "graph.h"



void set_main_matrix(uint8_t main_matrix[MAX_DIM][MAX_DIM]) {
  for (int i = 0; i < MAX_DIM; i++) {
    if (i > num_nodes) { break; }
    for (int j = 0; j < MAX_DIM; j++) {
      if (j > num_nodes) { break; }
      main_matrix[i][j] = 2;
    }
  }
}



void reset_sub_matrix(uint8_t sub_matrix[MAX_DIM][MAX_DIM]) {
  for (int i = 0; i < MAX_DIM; i++) {
    if (i > num_nodes) { break; }
    for (int j = 0; j < MAX_DIM; j++) {
      if (j > num_nodes) { break; }
      sub_matrix[i][j] = 2;
    }
  }
}



void fpga_implementation() {
  uint8_t  main_matrix[MAX_DIM][MAX_DIM];
  uint8_t  sub_matrix[MAX_DIM][MAX_DIM];
  uint8_t  active_main_row[MAX_DIM];
  uint8_t  active_sub_row[MAX_DIM];
  uint8_t  keep_color_row[MAX_DIM];
  uint16_t arbitrary_options = 20;

  for (int node = 0; node < MAX_DIM; node++) {
    if (node > num_nodes) { break; }
    active_main_row[node] = 1;
    active_sub_row[node] = 0;
    keep_color_row[node] = 0;
  }

  set_main_matrix(main_matrix);
  for (int option = 0; option < arbitrary_options; option++) {
    reset_sub_matrix(sub_matrix);
    for (int node = 0; node < MAX_DIM; node++) {
      if (node > num_nodes) { break; }
      if (active_main_row[node] == 0) { continue; }
      for (int neighbor = 0; neighbor < MAX_DIM; neighbor++) {
        if (neighbor > num_nodes) { break; }
      }
    }
  }
}
