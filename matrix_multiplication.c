// A00824335 - Ricardo Chapa
// A01280562 - Miguel Elizondo
// Final Project
// Compilation: /usr/local/gcc9.3/bin/gcc -mavx512f -std=c99 matrix_multiplication.c -o matrix_multiplication
#include "headers/matrix_utils.h"
#include "headers/serial_processing.h"
#include "headers/intrinsics_processing.h"

double* A = NULL;
double* B = NULL;
int size_of_A = 0;
int size_of_B = 0;

double* open_matrix_file(char* filename) {
  FILE *file = fopen(filename, "r"); // read mode
  if (file == NULL) {
    printf("Could not open file %s.\n", filename);
    return NULL;
  }
  
  double value = 0.0;
  int i = 0;
  // Iterate over the file a first time to get the amount
  // of values we'll store in our matrices.
  while (fscanf(file, "%lf", &value) != EOF) {
    i++;
  }
  fclose(file);

  if (size_of_A == 0) {
    size_of_A = i;
  } else {
    size_of_B = i;
  }

  double* value_array = (double*)malloc(sizeof(double) * i);
  if (value_array == NULL) {
    printf("There's not enough memory to read from file %s\n", filename);
    return NULL;
  }

  // Iterate over the file a second time to store the
  // values into a temp value array.
  value = 0.0;
  i = 0;
  file = fopen(filename, "r");
  while (fscanf(file, "%lf", &value) != EOF) {
   value_array[i] = value;
   i++;
  }

  fclose(file);
  return value_array;
}

int load_matrices_values() {
  A = open_matrix_file("./matrizA.txt");
  if (A == NULL) {
    printf("An error occured when attempting to read file A.\n");
    return 1;
  }

  B = open_matrix_file("./matrizB.txt");
  if (B == NULL) {
    printf("An error occured when attempting to read file B.\n");
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
  int size_of_mat_a = *A_m * *A_n;
  if (size_of_mat_a != size_of_A) {
    printf("Number of row/col for matrix A do not match with size of file: %d\n", size_of_A);
    return 1;
  }
  int size_of_mat_b = *B_m * *B_n;
  if (size_of_mat_b != size_of_B) {
    printf("Number of row/col for matrix B do not match with size of file: %d\n", size_of_B);
    return 1;
  }
  
  if (*A_n != *B_m) {
    printf("Number of columns for matrix A are not equal number of rows for matrix B: \n");
    printf("Matrix A: %d x %d. Matrix B: %d x %d\n", *A_m, *A_n, *B_m, *B_n);
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

  struct matrix single_thread_res = 
    run_matrix_mult_single_threaded(&a, &b, &b_transposed, A, B);

  freealloc_single_threaded(&a);
  freealloc_single_threaded(&b);
  freealloc_single_threaded(&b_transposed);

  struct matrix intrinsic_proc_res = 
    run_matrix_mult_intrinsic_processed(&a, &b, &b_transposed, A, B);

  int is_different = 0;
  for (int i = 0; i < single_thread_res.num_rows; ++i) {
    for (int j = 0; j < single_thread_res.num_cols; ++j) { 
      if (fabs(single_thread_res.matrix[i][j] - intrinsic_proc_res.matrix[i][j]) > 1e-10) {
        printf("%.13g %.13g\n", single_thread_res.matrix[i][j], intrinsic_proc_res.matrix[i][j]);
        is_different++;
        break;
      }
    }
    if (is_different) break;
  }
  printf("Matrix are different: %d\n", is_different);

  freealloc_intrinsic_processed(&a);
  freealloc_intrinsic_processed(&b);
  freealloc_intrinsic_processed(&b_transposed);

  freealloc_single_threaded(&single_thread_res);
  freealloc_intrinsic_processed(&intrinsic_proc_res);

  return 0;
}