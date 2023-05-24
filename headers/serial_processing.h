#include <stdlib.h>
#include "matrix_utils.h"

void alloc_matrix_single_threaded(struct matrix* m) {
  m->matrix = (double **)malloc(m->num_rows * sizeof(double *));
  for (int i = 0; i < m->num_rows; i++) {
        m->matrix[i] = (double *)malloc(m->num_cols * sizeof(double));
  }
}

void freealloc_single_threaded(struct matrix* m) {
  for (int i = 0; i < m->num_rows; i++) {
        free(m->matrix[i]);
  }
  free(m->matrix);
}

// Performing a matrix multiplication takes a dot product among the row values
// from matrix A and column values from matrix B:
// 1 x 1
// [1]      1 x 3
// [2] * [4][5][6] = [1*4] [2*5] [3*6] = [4][10][18] 
// [3]
// 
// By transposing matrix B we can avoid an access cost on each row/column for
// matrix b and make a row by row access.
struct matrix compute_matrix_single_threaded(struct matrix* a, struct matrix* b_transposed) {
  double* resulting_vector = (double*)malloc(sizeof(double) * a->num_rows * b_transposed->num_rows);

  clock_gettime(CLOCK_REALTIME, &start);
  for (int i = 0; i < a->num_rows; ++i) {
    for (int j = 0; j < b_transposed->num_rows; ++j) {
      double dot_prod = 0.0;
      for (int k = 0; k < a->num_cols; ++k) {
        dot_prod += a->matrix[i][k] * b_transposed->matrix[j][k];
      }
      resulting_vector[i * b_transposed->num_rows + j] = dot_prod;
    }
  }
  clock_gettime(CLOCK_REALTIME, &end);
  printf("Time = (%f)\n", (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0);

  struct matrix res_matrix;
  res_matrix.num_rows = a->num_rows;
  res_matrix.num_cols = b_transposed->num_rows;
  
  alloc_matrix_single_threaded(&res_matrix);
  fill_resulting_matrix(resulting_vector, &res_matrix);
  return res_matrix;
}

void run_matrix_mult_single_threaded(
  struct matrix* a, struct matrix* b, struct matrix* b_transposed) {
  alloc_matrix_single_threaded(a);
  alloc_matrix_single_threaded(b);

  // Fill the matrix values with either 2500 or 1048576 sizes.
  fill_matrices(a, b);

  // Make the transposition of matrix b since it's accessing will be more efficient
  // on the multiplication algorithm.
  b_transposed->num_rows = b->num_cols;
  b_transposed->num_cols = b->num_rows;
  
  alloc_matrix_single_threaded(b_transposed);  
  tranpose_matrix(b, b_transposed);

  struct matrix res_matrix = compute_matrix_single_threaded(a, b_transposed);
  //print_matrix(&res_matrix);

  freealloc_single_threaded(a);
  freealloc_single_threaded(b);
  freealloc_single_threaded(b_transposed);
  freealloc_single_threaded(&res_matrix);
}
