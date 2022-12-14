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



void add_edge(int edge_v1, int edge_v2, int triangle) {
	if (triangle == 1) {
    if (edge_v1 < edge_v2) {
      adj_matrix[edge_v2][edge_v1] = 1;
    } else {
      adj_matrix[edge_v1][edge_v2] = 1;
    }
  } else {
    adj_matrix[edge_v1][edge_v2] = 1;
    adj_matrix[edge_v2][edge_v1] = 1;
  }
  return;
}



void get_degrees(int inc) {
  for (int node = 0; node < MAX_DIM; node++) {
    if (node > num_nodes) { break; }
    for (int neighbor = 0; neighbor < MAX_DIM; neighbor++) {
      if (neighbor > num_nodes) { break; }
      if (node != neighbor && adj_matrix[node][neighbor] == 1) {
         degrees[node] += inc;
      }
    }
  }
}



void print_adj_matrix(int n) {
  printf("\nSorted vertices by degree (node, degree):");
  printf("\n-------------------------------------------");
  for (int i = 0; i < n; i++) {
    if (i % 8 == 0)
      printf("\n");
    printf("(%3d, %4d)   ", degrees[i] & 0x0000FFFF, degrees[i] >> (DEGREE_OFFSET));
  }
  printf("\n");
}



int max_colors() {
	int max_color = 0;
	for (int i = 1; i < MAX_DIM; i++)
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
		for (int node = 1; node < MAX_DIM; node++) {
			if (node <= num_nodes && colors[node] == color) {
				printf("%d ", node);
			}
		}
		printf("\n");
	}
}



void print_uncolored() {
  int uncolored = 0;
  for (int node = 1; node < MAX_DIM; node++) {
    if (node > num_nodes) { break; }
    if (colors[node] == 0)
      uncolored++;
  }
  printf("[ info] Number of uncolored nodes: %d/%d\n", uncolored, num_nodes);
  return;
}



void test_coloring() {
  for (int u = 1; u < MAX_DIM; u++) {
    int test_color = colors[u];
    if (u > num_nodes) { break; }
    if (test_color == 0) {
      printf("[error] graph Coloring isn't correct - adjacent nodes have same color\n");
      return;
    }
    for (int v = 1; v < MAX_DIM; v++) {
      if (v > num_nodes) { break; }
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
  if( argc > 4 ) {
    printf("[error] incorrect usage of program.\n");
    printf("[ info] algorithm types:\n");
    printf("[ info]    - 1 = Greedy\n");
    printf("[ info]    - 2 = Largest Degree First\n");
    printf("[ info]    - 3 = Smallest Degree Last\n");
    printf("[ info]    - 4 = Recursive Largest First\n");
    printf("[ info]    - 5 = FPGA Implementation\n");
    printf("[ info]    - 6 = Early Termination for LDF\n");
    printf("[ info]    - 7 = Early Termination for RLF\n");
    printf("----------------------------------------------------------------------\n");
    printf("[ info] run using: ./graph <data-file> <algorithm-type> <end-early-%%>\n");
    return 1;
  }

  char      line_type[1], line_format[4];
  int       edge_v1, edge_v2;
  FILE     *fp;
  char     *line_buf = NULL;
  size_t    buf_len = 32;
  int       print_sorted = 104;
  int       triangle = 0;
  int       percent_done = 0;
  int       max_color = 0;
  struct timespec start, stop;


  if (argc == 4)
    percent_done = atoi(argv[3]);

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

  if (atoi(argv[2]) == 5)
    triangle = 1;

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
      add_edge(edge_v1, edge_v2, triangle);
    } else if (line_buf[0] >= '0' && line_buf[0] <= '9') {
      sscanf(line_buf, "%d %d\n", &edge_v1, &edge_v2);
      num_nodes = (edge_v2 > num_nodes) ? edge_v2 :
                    ((edge_v1 > num_nodes) ? edge_v1 : num_nodes);
      add_edge(edge_v1, edge_v2, triangle);
    } else {
      printf("[error] line with bad prefix: %s\n", line_buf);
    }
  }
  get_degrees(1 << DEGREE_OFFSET);
  printf("[ info] porcessing file: %s\n", argv[1]);
  printf("[ info] num_nodes = %d\n", num_nodes);
  printf("[ info] num_edges = %d\n", num_edges);

  // Print the matrix out for debugging if need be
  print_adj_matrix(print_sorted);

  // 1 = greedy, 2 = largest_degree_first, 3 = smallest_degree_last
  // 4 = recurive_largest_fiest, 5 = fpga_implementation
  // 6 = early_term_ldf, 7 = early_term_rlf
  /* qsort(degrees, MAX_DIM, sizeof(int), cmp_deg); */
  clock_gettime( CLOCK_REALTIME, &start);
  switch (atoi(argv[2])) {
    case 1: greedy(); break;
    case 2: largest_degree_first(); break;
    case 3: smallest_degree_last(); break;
    case 4: recursive_largest_first(); break;
    case 5: fpga_implementation(); break;
    case 6:
      max_color = early_term_ldf((float)percent_done / 100.0);
      recursive_largest_first();
      /* exact_coloring(max_color); */
      printf("[ info] k_colors needed: %d\n", max_color);
      break;
    case 7:
      max_color = early_term_rlf((float)percent_done / 100.0);
      printf("[ info] k_colors needed: %d\n", max_color);
      largest_degree_first();
      /* exact_coloring(max_color); */
      break;
    default: break;
  }
  clock_gettime( CLOCK_REALTIME, &stop);

  // Print out the final results and confirm they are correct
	print_coloring();
  test_coloring();
  print_uncolored();

  printf("[ time] Time for coloring: %lluus\n", (long long unsigned int)(stop.tv_nsec - start.tv_nsec) / 1000);

  fclose(fp);
  return 0;
}
