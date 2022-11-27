#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "graph.h"



void greedy_coloring() {
  int used[MAX_DIM];    //Used to check whether color is used or not

  colors[0] = 0;    //Assign first color for the first node
  for(int i = 1; i < MAX_DIM; i++)
    colors[i] = -1;    //initialize all other vertices are unassigned

  for(int i = 0; i < MAX_DIM; i++)
    used[i] = 0;    //initially any colors are not chosen

  for(int u = 1; u < MAX_DIM; u++) {    //for all other MAX_DIM - 1 vertices
    for(int v = 0; v < MAX_DIM; v++) {
      if(adj_matrix[u][v] == 1) {
        if(colors[v] != -1)    //when one color is assigned, make it unavailable
          used[colors[v]] = 1;
      }
    }

    int col;
    for(col = 0; col < MAX_DIM; col++)
      if(used[col] == 0)    //find a color which is not assigned
        break;

    colors[u] = col;    //assign found color in the list

    for(int v = 0; v < MAX_DIM; v++) {    //for next iteration make color availability to 0
      if(adj_matrix[u][v] == 1) {
        if(colors[v] != -1)
          used[colors[v]] = 0;
      }
    }
  }
}
