#include "graph.h"

// Source largely from: https://www.codeproject.com/articles/88674/graph-coloring-using-recursive-large-first-rlf-alg
// Some info: https://github.com/victorvde/heuristic-graph-coloring

// this function finds the unprocessed vertex of which degree is maximum
int max_node_degree() {
  int max = 0;
  int max_i;
	for (int i = 0; i < MAX_DIM; i++) {
		if (i > num_nodes) { break; }
    if (colors[i] == 0) {
      if ((degrees[i] >> (DEGREE_OFFSET)) >= max) {
        max = (degrees[i] >> (DEGREE_OFFSET));
        max_i = i;
      }
    }
  }
  return max_i;
}

// this function updates nn array
void update_nn(int color_number, int *nn_count, int nn[MAX_DIM]) {
  *nn_count = 0;
  // firstly, add all the uncolored vertices into nn set
  for (int i = 0; i < MAX_DIM; i++) {
		if (i > num_nodes) { break; }
    if (colors[i] == 0) {
      nn[*nn_count] = i;
      (*nn_count)++; // when we add a vertex, increase the nn_count
    }
  }
  // then, remove all the vertices in nn that
  // is adjacent to the vertices colored color_number
  for (int i=0; i < MAX_DIM; i++) {
		if (i > num_nodes) { break; }
    if (colors[i] == color_number) { // find one vertex colored color_number
      for (int j=0; j < *nn_count; j++) {
        // remove vertex that adjacent to the found vertex
        while (adj_matrix[i][nn[j]] == 1) {
          nn[j] = nn[*nn_count - 1];
          (*nn_count)--; // decrease the nn_count
        }
      }
    }
  }
}

// this function will find suitable y from nn
int find_suitable_y(int color_number, int *vertices_in_common, int *nn_count, int nn[MAX_DIM]) {
  int temp,tmp_y,y;
  // array scanned stores uncolored vertices
  // except the vertex is being processing
  int scanned[MAX_DIM];
  *vertices_in_common = 0;
  for (int i=0; i < *nn_count; i++) { // check the i-th vertex in nn
    // tmp_y is the vertex we are processing
    tmp_y = nn[i];
    // temp is the neighbors in common of tmp_y
    // and the vertices colored color_number
    temp = 0;
    for (int i=0; i < MAX_DIM; i++) {
			if (i > num_nodes) { break; }
    	scanned[i] = 0;
		}
    //reset scanned array in order to check all
    //the vertices if they are adjacent to i-th vertex in nn
    for (int x=0; x < MAX_DIM; x++) {
			if (x > num_nodes) { break; }
      if (colors[x] == color_number) { // find one vertex colored color_number
        for (int k=0; k < MAX_DIM; k++) {
					if (k > num_nodes) { break; }
          if (colors[k] == 0 && scanned[k] == 0) {
            // if k is a neighbor in common of x and tmp_
            if (adj_matrix[x][k] == 1 && adj_matrix[tmp_y][k] == 1) {
              temp ++;
              scanned[k] = 1; // k is scanned
            }
          }
        }
      }
    }
    if (temp > *vertices_in_common) {
      *vertices_in_common = temp;
      y = tmp_y;
    }
  }
  return y;
}

// find the vertex in nn of which degree is maximum
int max_degree_in_nn(int *nn_count, int nn[MAX_DIM]) {
  int tmp_y; // the vertex has the current maximum degree
  int temp, max = 0;
  for (int i=0; i < *nn_count; i++) {
    temp = 0;
    for (int j=0; j < MAX_DIM; j++) {
			if (j > num_nodes) { break; }
      if (colors[j] == 0 && adj_matrix[nn[i]][j] == 1)
        temp ++;
		}
    if (temp>max) { // if the degree of vertex nn[i] is higher than tmp_y's one
      max = temp; // assignment nn[i] to tmp_y
      tmp_y = nn[i];
    }
  }
  if (max == 0) // so all the vertices have degree 0
    return nn[0];
  else // exist a maximum, return it
    return tmp_y;
}


// processing function
void recursive_largest_first() {
	int nn[MAX_DIM];
	int nn_count = 0;
  int unprocessed = num_nodes;
  int x,y;
  int color_number = 0;
  int vertices_in_common = 0;

  for (int i = 0; i < MAX_DIM; i++) {
    if (unprocessed <= 0)
      break;
    x = max_node_degree(); // find the one with maximum degree
    color_number++;
    colors[x] = color_number; // give it a new color
    unprocessed--;
    update_nn(color_number, &nn_count, nn); // find the set of non-neighbors of x
    for (int j = 0; j < MAX_DIM; j++) {
      if (nn_count <= 0)
      break;
    // find y, the vertex has the maximum neighbors in common with x
    // vertices_in_common is this maximum number
      y = find_suitable_y(color_number, &vertices_in_common, &nn_count, nn);
    // in case vertices_in_common = 0
    // y is determined that the vertex with max degree in nn
      if (vertices_in_common == 0)
        y = max_degree_in_nn(&nn_count, nn);
    // color y the same to x
      colors[y] = color_number;
      unprocessed--;
      update_nn(color_number, &nn_count, nn);
    // find the new set of non-neighbors of x
    }
  }
}
