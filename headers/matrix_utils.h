#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int SIZE_2500 = 2500;
const int SIZE_1048576 = 1048576;
double* A_2500 = NULL;
double* B_2500 = NULL;
double* A_1048576 = NULL;
double* B_1048576 = NULL;

struct matrix {
  int num_rows;
  int num_cols;
  double** matrix;
};
struct timespec start, end;

void fill_matrices(struct matrix* a, struct matrix* b) {
  int res_matrix_size = a->num_rows * a->num_cols;
  // make this a switch case to handle intrinsics allocation if vec is wanted.
  if (res_matrix_size <= SIZE_2500) {
    for (int i = 0; i < a->num_rows; ++i) {
      for (int j = 0; j < a->num_cols; ++j) {
        a->matrix[i][j] = A_2500[i*a->num_cols + j];
      }
    }

     for (int i = 0; i < b->num_rows; ++i) {
      for (int j = 0; j < b->num_cols; ++j) {
        b->matrix[i][j] = B_2500[i*b->num_cols + j];
      }
    }
  }
  else if (res_matrix_size <= SIZE_1048576){
    for (int i = 0; i < a->num_rows; ++i) {
      for (int j = 0; j < a->num_cols; ++j) {
        a->matrix[i][j] = A_1048576[i*a->num_cols + j];
      }
    }

    for (int i = 0; i < b->num_rows; ++i) {
      for (int j = 0; j < b->num_cols; ++j) {
        b->matrix[i][j] = B_1048576[i*b->num_cols + j];
      }
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