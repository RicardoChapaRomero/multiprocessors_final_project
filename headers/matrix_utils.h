#define _POSIX_C_SOURCE 199309L
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct matrix {
  int num_rows;
  int num_cols;
  double** matrix;
};
struct timespec start, end;

void fill_matrices(struct matrix* a, struct matrix* b, double* A, double* B) {
    for (int i = 0; i < a->num_rows; ++i) {
      for (int j = 0; j < a->num_cols; ++j) {
        a->matrix[i][j] = A[i*a->num_cols + j];
      }
    }

     for (int i = 0; i < b->num_rows; ++i) {
      for (int j = 0; j < b->num_cols; ++j) {
        b->matrix[i][j] = B[i*b->num_cols + j];
      }
    }
}

void print_matrix(struct matrix* m) {
  for (int i = 0; i < m->num_rows; ++i) {
    for (int j = 0; j < m->num_cols; ++j) {
      printf("%.13g ", m->matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

// Set the rows to columns and columns to rows to facilitate
// the resulting matrix computation.
void tranpose_matrix(struct matrix* b, struct matrix* b_transposed) {
  for (int i = 0; i < b->num_rows; i++) {
    for (int j = 0; j < b->num_cols; j++) {
      b_transposed->matrix[j][i] = b->matrix[i][j];
    }
  }
}

void fill_resulting_matrix(double* value_vec, struct matrix* res_matrix) {
  for (int i = 0; i < res_matrix->num_rows; ++i) {
    for (int j = 0; j < res_matrix->num_cols; ++j) {
      res_matrix->matrix[i][j] = value_vec[i*res_matrix->num_cols + j];
    }
  }
  free(value_vec);
}