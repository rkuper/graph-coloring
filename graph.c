#include "graph.h"

// Graph info
uint32_t  degrees[MAX_DIM];
uint8_t   adj_matrix[MAX_DIM][MAX_DIM];
int       colors[MAX_DIM];
int       num_nodes;
int       num_edges;



int cmp_deg (const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}



void add_edge(int edge_v1, int edge_v2, int inc, int triangle) {
	if (triangle == 1) {
    if (edge_v1 < edge_v2) {
      adj_matrix[edge_v2][edge_v1] = 1;
      degrees[edge_v2] += inc;
      degrees[edge_v1] += inc;
    } else {
      adj_matrix[edge_v1][edge_v2] = 1;
      degrees[edge_v2] += inc;
      degrees[edge_v1] += inc;
    }
  } else {
    adj_matrix[edge_v1][edge_v2] = 1;
    adj_matrix[edge_v2][edge_v1] = 1;
    degrees[edge_v2] += inc;
    degrees[edge_v1] += inc;
  }
  return;
}



void print_adj_matrix(int n) {
  printf("\nSorted vertices by degree (node, degree):");
  printf("\n-------------------------------------------");
  for (int i = 0; i < n; i++) {
    if (i % 8 == 0)
      printf("\n");
    printf("(%3d, %4d)   ", degrees[i] & 0x0000FFFF, degrees[i] >> (DEGREE_OFFSET + 1));
  }
  printf("\n");
}



int max_colors() {
	int max_color = 0;
	for (int i = 0; i < MAX_DIM; i++)
		if (colors[i] > max_color)
			max_color = colors[i];
	return max_color + 1;
}



void print_coloring() {
	int max_color = max_colors();
	printf("\nNumber of Colors: %3d\n", max_color - 1);
	printf("---------------------\n");
	for (int color = 1; color < max_color; color++) {
		printf("Color %3d:    ", color);
		for (int node = 0; node < MAX_DIM; node++) {
			if (node <= num_nodes && colors[node] == color) {
				printf("%d ", node);
			}
		}
		printf("\n");
	}
}



void test_coloring() {
  for (int u = 0; u < MAX_DIM; u++) {
    int test_color = colors[u];
    for (int v = 0; v < MAX_DIM; v++) {
      if ((u != v) && (adj_matrix[u][v] == 1)) {
        if (test_color == colors[v]) {
          printf("[error] graph Coloring isn't correct - adjacent nodes have same color\n");
          return;
        }
      }
    }
  }
  printf("\n[ info] passed\n");
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
  int       edge_v1, edge_v2;
  FILE     *fp;
  char     *line_buf = NULL;
  size_t    buf_len = 32;
  int       print_sorted = 104;

  // Open graph file
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("[error] could not find file: %s\n", argv[1]);
    return 0;
  }

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
                                        &num_nodes,
                                        &num_edges);
    } else if (line_buf[0] == 'e') {
      sscanf(line_buf, "%s %d %d\n", line_type, &edge_v1, &edge_v2);
      add_edge(edge_v1, edge_v2, 1 << DEGREE_OFFSET, 0);
    } else if (line_buf[0] >= '0' && line_buf[0] <= '9') {
      sscanf(line_buf, "%d %d\n", &edge_v1, &edge_v2);
      num_nodes = (edge_v2 > num_nodes) ? edge_v2 :
                    ((edge_v1 > num_nodes) ? edge_v1 : num_nodes);
      add_edge(edge_v1, edge_v2, 1 << DEGREE_OFFSET, 0);
    } else {
      printf("[error] line with bad prefix: %s\n", line_buf);
    }
  }
  printf("[ info] porcessing file: %s\n", argv[1]);
  printf("[ info] num_nodes = %d\n", num_nodes);
  printf("[ info] num_edges = %d\n", num_edges);

  // Print the matrix out for debugging if need be
  print_adj_matrix(print_sorted);

  // 1 = greedy, 2 = largest_degree_first, 3 = smallest_degree_last
  /* qsort(degrees, MAX_DIM, sizeof(int), cmp_deg); */
	if (atoi(argv[2]) == 1) {
		greedy();
	} else if (atoi(argv[2]) == 2) {
    largest_degree_first();
  } else if (atoi(argv[2]) == 3) {
    smallest_degree_last();
  } else if (atoi(argv[2]) == 4) {
    recursive_largest_first();
  }

  // Print out the final results and confirm they are correct
	print_coloring();
  test_coloring();

  fclose(fp);
  return 0;
}
