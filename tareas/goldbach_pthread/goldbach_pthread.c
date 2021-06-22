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
#include "matrix.c"
#include "goldbach_calculator.c"

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

int create_threads(shared_data_t* shared_data);
int print_goldbach(shared_data_t* shared_data);
void* run(void* data);

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
void* run(void* data) {
  FILE *input = stdin;
	const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;
  const size_t my_thread_id = private_data->thread_number;
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