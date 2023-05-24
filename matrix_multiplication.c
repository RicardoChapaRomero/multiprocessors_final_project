// A00824335 - Ricardo Chapa
// A01280562 - Miguel Elizondo
// Final Project
// Compilation: gcc -std=c99 proyecto_final.c -o proyecto_final
#include "headers/serial_processing.h"

int open_matrix_file(char* filename, double* value_array) {
  FILE *file = fopen(filename, "r"); // read mode
  if (file == NULL) {
    printf("Could not open file.\n");
    return 1;
  }
  
  double value = 0.0;
  int i = 0;
  while (fscanf(file, "%lf", &value) != EOF) {
   value_array[i] = value;
   i++;
  }

  fclose(file);
  return 0;
}

// Pre-load the 2500 and 1048576 size A and B matrices
int load_matrices_values() {
  A_2500 = (double*)malloc(sizeof(double) * SIZE_2500);
  if (open_matrix_file("matrices_txts/matrixA2500.txt", A_2500) == -1) {
    printf("An error occured when attempting to read file A2500.\n");
    return 1;
  }
  B_2500 = (double*)malloc(sizeof(double) * SIZE_2500);
  if (open_matrix_file("matrices_txts/matrixB2500.txt", B_2500) == -1) {
    printf("An error occured when attempting to read file B2500.\n");
    return 1;
  }
  A_1048576 = (double*)malloc(sizeof(double) * SIZE_1048576);
  if (open_matrix_file("matrices_txts/matrixA1048576.txt", A_1048576) == -1) {
    printf("An error occured when attempting to read file A1048576.\n");
    return 1;
  }
  B_1048576 = (double*)malloc(sizeof(double) * SIZE_1048576);
  if (open_matrix_file("matrices_txts/matrixB1048576.txt", B_1048576) == -1) {
    printf("An error occured when attempting to read file B1048576.\n");
    return 1;
  }
  return 0;
}

int get_and_validate_rows_and_cols(int* A_m,int* A_n,int* B_m,int* B_n) {
  printf("Insert the number of rows for matrix A: ");
  scanf("%d", A_m);

  printf("Insert the number of columns for matrix A: ");
  scanf("%d", A_n);

  printf("Insert the number of rows for matrix B: ");
  scanf("%d", B_m);

  printf("Insert the number of columns for matrix B: ");
  scanf("%d", B_n);

  // Number of columns in matrix A should be the same as rows in matrix B.
  if (*A_n != *B_m) {
    printf("Number of columns for matrix A are not equal number of rows for matrix B: \n");
    printf("Matrix A: %d x %d. Matrix B: %d x %d\n", *A_m, *A_n, *B_m, *B_n);
    return 1;
  }

  // Check if the resulting size of the matrix does not exceed the size
  // from the maximum amount of data available: 1048576
  int res_matrix_size = (*A_m) * (*B_n);
  if (res_matrix_size > 1048576) {
    printf("There's not enough data to compute a matrix of size: %d \n", res_matrix_size);
    return 1;
  }
  return 0;
}

int main() {
  if (load_matrices_values()) {
    return 1;
  }

  struct matrix a;
  struct matrix b;
  struct matrix b_transposed;
  if (get_and_validate_rows_and_cols(&a.num_rows, &a.num_cols, &b.num_rows, &b.num_cols)) {
    return 1;
  }
  
  run_matrix_mult_single_threaded(&a, &b, &b_transposed);

  return 0;
}