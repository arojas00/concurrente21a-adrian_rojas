#include "matrix.h"

/**
 * @brief auxilar method for creating matrix
 * @param row_count number of rows in the matrix
 * @param col_count number of columns in the matrix
 * @param element_size the size of the elements in the matrix
 */
void** create_matrix(size_t row_count, size_t col_count, size_t element_size) {
  void** matrix = (void**) calloc(row_count, sizeof(void*));
  if ( matrix == NULL ) {
    return NULL;
  }

  for (size_t row = 0; row < row_count; ++row) {
    if ( (matrix[row] = calloc(col_count, element_size) ) == NULL ) {
      free_matrix(row_count, matrix);
      return NULL;
    }
  }

  return matrix;
}
/**
 * @brief auxilar method for freeing matrix
 * @param row_count number of rows in the matrix
 * @param matrix matrix to be freed
 */
void free_matrix(const size_t row_count, void** matrix) {
  if (matrix) {
    for (size_t row = 0; row < row_count; ++row) {
      free(matrix[row]);
    }
  }

  free(matrix);
}