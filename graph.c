#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "graph.h"

// Why header file, why?
extern uint32_t  degrees[MAX_DIM];
extern uint8_t   adj_matrix[MAX_DIM][MAX_DIM];
extern uint8_t   colors[MAX_DIM];



int cmp_deg (const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}



void add_edge(int edge_v1, int edge_v2, int inc) {
  if (edge_v1 < edge_v2) {
    adj_matrix[edge_v2][edge_v1] = 1;
    degrees[edge_v2] = degrees[edge_v2] + inc;
    degrees[edge_v1] = degrees[edge_v1] + inc;
  } else {
    adj_matrix[edge_v1][edge_v2] = 1;
    degrees[edge_v2] = degrees[edge_v2] + inc;
    degrees[edge_v1] = degrees[edge_v1] + inc;
  }
  return;
}



int max_colors() {
	int max_color = 0;
	for (int i = 0; i < MAX_DIM; i++)
		if (colors[i] > max_color)
			max_color = colors[i];
	return max_color;
}



void print_coloring() {
	int max_color = max_colors();
	printf("\nNumber of Colors: %d\n", max_color);
	printf("-------------------- %d\n", max_color);
	for (int color = 0; color < max_color; color++) {
		printf("Color %d:    ", color);
		for (int vertex = 0; vertex < MAX_DIM; vertex++) {
			if (colors[vertex] == color) {
				printf("%d ", vertex);
			}
		}
		printf("\n");
	}
}



int main (int argc, char *argv[]) {
  if( argc != 3 ) {
    printf("[error] incorrect usage of program.\n");
    printf("[ info] algorithm types:\n");
    printf("[ info]    - 1 = Greedy\n");
    printf("[ info]    - 2 = Largest Degree First\n");
    printf("-----------------------------------------------\n");
    printf("[ info] run using: ./graph <data-file> <algorithm-type>\n");
    return 1;
  }

  char      line_type[1], line_format[4];
  int       num_vertices, num_edges;
  int       edge_v1, edge_v2;
  FILE     *fp;
  char     *line_buf;
  size_t    buf_len = 32;
  int       degree_inc = 1 << 16;
  int       print_sorted = 104;

  // Open graph file
  fp = fopen(argv[1], "r");
  if (fp == NULL)
    return 0;

  // Reset adjacency matrix
  for (int i = 0; i < MAX_DIM; i++) {
    for (int j = 0; j < MAX_DIM; j++) {
      adj_matrix[i][j] = 0;
    }
    degrees[i] = i;
		colors[i] = 0;
  }

  // Parse graph file
  // Either through:
  //     - c <comment>
  //     - p <line_type> <edge1> <edge2>
  //     - e <edge1> <edge2>
  // or:
  //     - <edge1> <edge2>
  while (getline(&line_buf, &buf_len, fp) != -1) {
    if (line_buf[0] == '\0' || line_buf[0] == 'c') {
      continue;
    } else if (line_buf[0] == 'p') {
      sscanf(line_buf, "%s %s %d %d\n", line_type,
                                        line_format,
                                        &num_vertices,
                                        &num_edges);
    } else if (line_buf[0] == 'e') {
      sscanf(line_buf, "%s %d %d\n", line_type, &edge_v1, &edge_v2);
      add_edge(edge_v1, edge_v2, degree_inc);
    } else if (line_buf[0] >= '0' && line_buf[0] <= '9') {
      sscanf(line_buf, "%d %d\n", &edge_v1, &edge_v2);
      num_vertices = (edge_v2 > num_vertices) ? edge_v2 :
                    ((edge_v1 > num_vertices) ? edge_v1 : num_vertices);
      add_edge(edge_v1, edge_v2, degree_inc);
    } else {
      printf("Line with bad prefix: %s\n", line_buf);
    }
  }
  printf("Porcessing file %s:\n", argv[1]);
  printf("num_vertices = %d\n", num_vertices);
  printf("num_edges = %d\n", num_edges);

  // Sort the degrees combined array to get largest degree first
  qsort(degrees, MAX_DIM, sizeof(int), cmp_deg);

  // Print the first 300 sorted degrees matrix
  printf("\nSorted vertices by degree (vertex, degree):");
  for (int i = 0; i < print_sorted; i++) {
    if (i % 8 == 0)
      printf("\n");
    printf("(%3d, %4d)   ", degrees[i] & 0x0000FFFF, degrees[i] >> 16);
  }
  printf("\n");

	if (atoi(argv[2]) == 1) {
		greedy_coloring();
		print_coloring();
	}

  fclose(fp);
  return 0;
}
