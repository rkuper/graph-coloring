#include "graph.h"

// Source largely from: https://www.codeproject.com/articles/88674/graph-coloring-using-recursive-large-first-rlf-alg
// Some info: https://github.com/victorvde/heuristic-graph-coloring

// processing function
void recursive_largest_first() {
  int nn[MAX_DIM];
  int nn_count = 0;
  int unprocessed = num_nodes;
  int x = 0, y = 0;
  int color_number = 0;
  int vertices_in_common = 0;
  int max = 0, tmp_y = 0, temp = 0;
  int i, j, k, l, m;
  int scanned[MAX_DIM];

  for (i = 0; i < MAX_DIM; i++)
    nn[i] = 0;


  for (i = 0; i < MAX_DIM; i++) {
    if (unprocessed < 0) { break; }

    //////////////////////////////////////////////////////////////////////////
    // max_node_degree()
    max = 0;
    x = 0;
    for (j = 0; j < MAX_DIM; j++) {
      if (j <= num_nodes) {
        if (colors[j] == 0) {
          if ((degrees[j] >> (DEGREE_OFFSET)) >= max) {
            max = (degrees[j] >> (DEGREE_OFFSET));
            x = j;
          }
        }
      }
    }
    //////////////////////////////////////////////////////////////////////////

    color_number++;
    colors[x] = color_number; // give it a new color
    unprocessed--;

    //////////////////////////////////////////////////////////////////////////
    // update_nn(color_number, &nn_count, nn)
    nn_count = 0;
    // firstly, add all the uncolored vertices into nn set
    for (j = 0; j < MAX_DIM; j++) {
      if (j <= num_nodes) {
        if (colors[j] == 0) {
          nn[nn_count] = j;
          nn_count++;
        }
      }
    }
    // then, remove all the vertices in nn that
    // is adjacent to the vertices colored color_number
    for (j = 0; j < MAX_DIM; j++) {
      if (j <= num_nodes) {
        if (colors[j] == color_number) { // find one vertex colored color_number
          for (k = 0; k < MAX_DIM; k++) {
            if (k < nn_count) {
              // remove vertex that adjacent to the found vertex
              for (l = 0; l < MAX_DIM; l++) {
                if (adj_matrix[j][nn[k]] == 1) {
                  nn[k] = nn[nn_count - 1];
                  nn_count--; // decrease the nn_count
                }
              }
            }
          }
        }
      }
    }
    //////////////////////////////////////////////////////////////////////////

    for (j = 0; j < MAX_DIM; j++) {
      if (nn_count <= 0) { break; }
      if (nn_count > 0) {
        // find y, the vertex has the maximum neighbors in common with x
        // array scanned stores uncolored vertices
        // except the vertex is being processing

        //////////////////////////////////////////////////////////////////////////
        // y = find_suitable_y(color_number, &vertices_in_common, &nn_count, nn)
        vertices_in_common = 0;
        for (k = 0; k < MAX_DIM; k++) { // check the i-th vertex in nn
          if (k >= nn_count) { break; }
          // tmp_y is the vertex we are processing
          tmp_y = nn[k];
          // temp is the neighbors in common of tmp_y
          // and the vertices colored color_number
          temp = 0;
          for (l = 0; l < MAX_DIM; l++) {
            if (l <= num_nodes)
              scanned[l] = 0;
          }
          //reset scanned array in order to check all
          //the vertices if they are adjacent to i-th vertex in nn
          for (x = 0; x < MAX_DIM; x++) {
            /* if (x > num_nodes) { break; } */
            if (colors[x] == color_number) { // find one vertex colored color_number
              for (m = 0; m < MAX_DIM; m++) {
                /* if (m > num_nodes) { break; } */
                if (colors[m] == 0 && scanned[m] == 0) {
                  // if k is a neighbor in common of x and tmp_
                  if (adj_matrix[x][m] == 1 && adj_matrix[tmp_y][m] == 1) {
                    temp++;
                    scanned[m] = 1; // k is scanned
                  }
                }
              }
            }
          }
          if (temp > vertices_in_common) {
            vertices_in_common = temp;
            y = tmp_y;
          }
        }
        //////////////////////////////////////////////////////////////////////////

        // in case vertices_in_common = 0
        // y is determined that the vertex with max degree in nn
        if (vertices_in_common == 0) {
          //////////////////////////////////////////////////////////////////////////
          // y = max_degree_in_nn(&nn_count, nn)
          max = 0;
          for (k = 0; k < MAX_DIM; k++) {
            /* if (k >= nn_count) { break; } */
            if (k < nn_count) {
              temp = 0;
              for (l = 0; l < MAX_DIM; l++) {
                if (l <= num_nodes)
                  if (colors[l] == 0 && adj_matrix[nn[k]][l] == 1)
                    temp++;
              }
              if (temp > max) { // if the degree of vertex nn[i] is higher than tmp_y's one
                max = temp; // assignment nn[i] to tmp_y
                tmp_y = nn[k];
              }
            }
          }
          if (max == 0) // so all the vertices have degree 0
            y = nn[0];
          else // exist a maximum, return it
            y = tmp_y;
          //////////////////////////////////////////////////////////////////////////
        }

        // color y the same to x
        colors[y] = color_number;
        unprocessed--;

        //////////////////////////////////////////////////////////////////////////
        // update_nn(color_number, &nn_count, nn)
        nn_count = 0;
        // firstly, add all the uncolored vertices into nn set
        for (k = 0; k < MAX_DIM; k++) {
          if (k <= num_nodes) {
            if (colors[k] == 0) {
              nn[nn_count] = k;
              nn_count++; // when we add a vertex, increase the nn_count
            }
          }
        }
        // then, remove all the vertices in nn that
        // is adjacent to the vertices colored color_number
        for (k = 0; k < MAX_DIM; k++) {
          if (k <= num_nodes) {
            if (colors[k] == color_number) { // find one vertex colored color_number
              for (l = 0; l < MAX_DIM; l++) {
                if (l >= nn_count) { break; }
                // remove vertex that adjacent to the found vertex
                for (m = 0; m < MAX_DIM; m++) {
                  if (adj_matrix[k][nn[l]] == 1) {
                    nn[l] = nn[nn_count - 1];
                    nn_count--; // decrease the nn_count
                  }
                }
              }
            }
          }
        }
        //////////////////////////////////////////////////////////////////////////
      }
    }
  }
}
