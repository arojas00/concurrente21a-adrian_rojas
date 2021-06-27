#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>

void** create_matrix(size_t row_count, size_t col_count, size_t element_size);
void free_matrix(const size_t row_count, void** matrix);

#endif