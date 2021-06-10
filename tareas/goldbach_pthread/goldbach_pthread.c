// el numero de sumas que realiza es igual al numero de hilos
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define SUMS_LEN 1000

typedef struct {
  size_t thread_count;
  size_t position;
  char** sums;
} shared_data_t;

typedef struct  {
  size_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

_Bool check_prime(long long number);
int find_sums(long long number, _Bool even, _Bool print, char* str);
_Bool check_even(long long number);
_Bool check_negative(long long number);
_Bool check_valid(long long number);
void store_numbers();
int create_threads(shared_data_t* shared_data);
int print_goldbach(shared_data_t* shared_data);
void* run(void* data);
void** create_matrix(size_t row_count, size_t col_count, size_t element_size);
void free_matrix(const size_t row_count, void** matrix);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->position = 0;
    shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc == 2) {
      if (sscanf(argv[1], "%zu", &shared_data->thread_count) != 1 || errno) {
        fprintf(stderr, "error: invalid thread count\n");
        error = 1;
      }
    }
    if (error == EXIT_SUCCESS) {
      shared_data->sums = (char**) create_matrix(shared_data->thread_count
        , SUMS_LEN, sizeof(char));
      if (shared_data->sums) {
        error = print_goldbach(shared_data);
        free_matrix(shared_data->thread_count, (void**)shared_data->sums);
      } else {
        fprintf(stderr, "error: could not allocate semaphores\n");
        error = 3;
      }
      free(shared_data);
    } else {
      fprintf(stderr, "error: could not allocate shared memory\n");
      error = 2;
    }
  }

  return error;
}
/**
 * @brief check if the number is prime
 * @param number the number to be checked
 * @return bool telling if the number is prime
 */
_Bool check_prime(long long number) {
	_Bool is_prime = true;
	for (long long i = 2; i <= number / 2; i++) {
		if (number % i == 0) {
			is_prime = false;
			break;
		}
	}
	return is_prime;
}
/**
 * @brief finds the sums for each number and prints them if needed
 * @param number the number to be checked
 * @param _Bool tells if the numer es even
 * @param _Bool tells if it should print the numbers
 * @return int telling the amount of sums found
 */
int find_sums(long long number, _Bool even, _Bool print, char* str) {
	int count = 0;
	int flag = 0;
  char str2[100];
	for (int i = 2; i < number; i++) {
		for (int j = i; j < number; j++) {
			if(even == true) {
				if ((check_prime(i) == true && check_prime(j) == true) && i + j == number) {
					count++;
					if(print == true) {
						if(flag != 0) {
							strcat(str,", ");
						}
						sprintf(str2, "%d + %d", i, j); 
            strcat(str,str2);
						flag = 1;
					}
				}
			}
			else{
				for (int k = j; k < number; k++) {
					if (check_prime(i) == true && check_prime(j) == true 
						&& check_prime(k) == true && i + j + k == number) {
						count++;
						if(print == true) {
							if(flag != 0) {
								strcat(str,", ");
							}
							sprintf(str2, "%d + %d + %d", i, j, k);
              strcat(str,str2);
							flag = 1;
						}
					}
				}
			}
		}
	}
	return count;
}
/**
 * @brief check if the number is even
 * @param number the number to be checked
 * @return bool telling if the number is even
 */
_Bool check_even(long long number) {
	if (number % 2 == 0) {
		return true;
	}
	else {
		return false;
	}
}
/**
 * @brief determines if the number is negative and calls find_sums
 * @param number the number to be checked
 */
_Bool check_negative(long long number) {
	if (number < 0) {
		return true;
	}
	else{
		return false;
	}
}
/**
 * @brief check if the number si valid
 * @param number the number to be checked
 */
_Bool check_valid(long long number) {
	if (number <= 5 && number >= -5) {
		return false;
	}
	else {
		return true;
	}
}
/**
 * @brief check if the number si valid
 * @param shared_data carries the sums to be printed
 */
int print_goldbach(shared_data_t* shared_data) {
  struct timespec start_time;
  clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

  int error = create_threads(shared_data);

  struct timespec finish_time;
  clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

  for (size_t index = 0; index < shared_data->thread_count; ++index) {
    printf("%s\n", shared_data->sums[index]);
  }

  double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
    (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
  printf("execution time: %.9lfs\n", elapsed);
  return error;
}
/**
 * @brief creates the pthreads
 * @param shared_data shared data for each thread
 */
int create_threads(shared_data_t* shared_data) {
  assert(shared_data);
  int error = EXIT_SUCCESS;
  pthread_t* threads = (pthread_t*) calloc(shared_data->thread_count
    , sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (size_t index = 0; index < shared_data->thread_count; ++index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      if (error == EXIT_SUCCESS) {
        if (pthread_create(&threads[index], /*attr*/ NULL, run
          , &private_data[index]) == EXIT_SUCCESS) {
        } else {
          fprintf(stderr, "error: could not create thread %zu\n", index);
          error = 21;
          shared_data->thread_count = index;
          break;
        }
      } else {
        fprintf(stderr, "error: could not init semaphore %zu\n", index);
        error = 22;
        shared_data->thread_count = index;
        break;
      }
    }
    for (size_t index = 0; index < shared_data->thread_count; ++index) {
      pthread_join(threads[index], /*value_ptr*/ NULL);
    }
    free(threads);
    free(private_data);
  } else {
    fprintf(stderr, "error: could not allocate memory for %zu threads\n"
      , shared_data->thread_count);
    error = 22;
  }
  return error;
}

void* run(void* data) {
  FILE *input = stdin;
	const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;
  const size_t my_thread_id = private_data->thread_number;
  //const size_t thread_count = shared_data->thread_count;
	long long my_thread_number;
	size_t my_thread_sums;
	char sums[1000];

	if(fscanf(input, "%lld", &my_thread_number) == 1){
		if(check_valid(my_thread_number) == false){
			sprintf(shared_data->sums[my_thread_id], "%lld: NA"
				, my_thread_number);
		}
		else{
			if(check_negative(my_thread_number) == false){
				my_thread_sums = find_sums(my_thread_number, check_even(my_thread_number), false, sums);
				sprintf(shared_data->sums[my_thread_id], "%lld: %zu sums"
				, my_thread_number, my_thread_sums);
			}else{
				my_thread_sums = find_sums(my_thread_number*(-1), check_even(my_thread_number*(-1)), true, sums);
				sprintf(shared_data->sums[my_thread_id], "%lld: %zu sums: %s"
				, my_thread_number, my_thread_sums, sums);
			}
		}
	}
  return NULL;
}
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