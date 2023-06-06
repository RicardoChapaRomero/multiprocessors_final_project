#include <immintrin.h>

// In order to store double precision elements the alginment
// of the array must be of 64 bytes.
const size_t alignment = 64; //[bytes]
void alloc_matrix_intrinsic_processed(struct matrix* m) {
  m->matrix =
    (double**)_mm_malloc(m->num_rows * sizeof(double*), alignment);
  for (int i = 0; i < m->num_rows; i++) {
        m->matrix[i] =
          (double*)_mm_malloc(m->num_cols * sizeof(double), alignment);
  }
}

void freealloc_intrinsic_processed(struct matrix* m) {
  for (int i = 0; i < m->num_rows; i++) {
        _mm_free(m->matrix[i]);
  }
  _mm_free(m->matrix);
}

// For Intel Intrinsic SSE2 processing we can load both our matrices a and b_transposed and
// compute our resulting matrix with a dot product using _mm_mul_pd (which is used for double
// precision floating point numbers). NOTE: All our arrays must be 16-byte aligned in order for
// loading and computing operations to work.
struct matrix compute_matrix_intrinsic_processed(struct matrix* a, struct matrix* b_transposed) {
  double* resulting_vector =
    (double*)malloc(sizeof(double) * a->num_rows * b_transposed->num_rows);

  clock_gettime(CLOCK_REALTIME, &start);
  for (int i = 0; i < a->num_rows; ++i) {
    for (int j = 0; j < b_transposed->num_rows; ++j) {
      __m512d dot_prod_vec = _mm512_setzero_pd();
      int k = 0;
      for (; k < a->num_cols - (a->num_cols % 8); k += 8) {
        __m512d a_intrinsics = _mm512_load_pd(&a->matrix[i][k]);
        __m512d bt_intrinsics = _mm512_load_pd(&b_transposed->matrix[j][k]);
        dot_prod_vec = _mm512_fmadd_pd(a_intrinsics, bt_intrinsics, dot_prod_vec);
      }

      double dot_prod = 0.0;
      for (; k < a->num_cols; ++k) {
        dot_prod += a->matrix[i][k] * b_transposed->matrix[j][k];
      } 
      dot_prod += _mm512_reduce_add_pd(dot_prod_vec);
      resulting_vector[(i) * b_transposed->num_rows + j] = dot_prod;
    }
  }
  clock_gettime(CLOCK_REALTIME, &end);
  printf("Time = (%f)\n", (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0);

  struct matrix res_matrix;
  res_matrix.num_rows = a->num_rows;
  res_matrix.num_cols = b_transposed->num_rows;
  
  alloc_matrix_intrinsic_processed(&res_matrix);
  fill_resulting_matrix(resulting_vector, &res_matrix);
  return res_matrix;
}

struct matrix run_matrix_mult_intrinsic_processed(
  struct matrix* a, struct matrix* b, struct matrix* b_transposed,
  double* A, double* B) {
  alloc_matrix_intrinsic_processed(a);
  alloc_matrix_intrinsic_processed(b);
  // Fill the matrix values with either 2500 or 1048576 sizes.
  fill_matrices(a, b, A, B);

  // Make the transposition of matrix b since it's accessing will be more efficient
  // on the multiplication algorithm.
  b_transposed->num_rows = b->num_cols;
  b_transposed->num_cols = b->num_rows;
  
  alloc_matrix_intrinsic_processed(b_transposed);  
  tranpose_matrix(b, b_transposed);

  return compute_matrix_intrinsic_processed(a, b_transposed);
}
