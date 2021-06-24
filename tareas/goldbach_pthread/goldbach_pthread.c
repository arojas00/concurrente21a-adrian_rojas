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

#define SUMS_LEN 100

typedef struct {
  size_t thread_count;
  size_t position;
  char** sums;
  long long int* numbers;
  size_t number_count;
} shared_data_t;

typedef struct  {
  size_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

int create_threads(shared_data_t* shared_data);
int print_goldbach(shared_data_t* shared_data);
void* run(void* data);
void process_number(long long int number,int index, void* data);

int main(int argc, char* argv[]) {
  FILE *input = stdin;
  int error = EXIT_SUCCESS;
  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));
  shared_data->numbers = (long long int*) calloc(SUMS_LEN, sizeof(long long int));
  shared_data->number_count = 0;
  while(fscanf(input, "%lld", &shared_data->numbers[shared_data->number_count]) == 1){
    shared_data->number_count++;
  }
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
      shared_data->sums = (char**) create_matrix(shared_data->number_count
        , SUMS_LEN, sizeof(char));
      if (shared_data->sums) {
        error = print_goldbach(shared_data);
        free_matrix(shared_data->number_count, (void**)shared_data->sums);
      } else {
        fprintf(stderr, "error: could not allocate semaphores\n");
        error = 3;
      }
      free(shared_data);
      free(shared_data->numbers);
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

  for (size_t index = 0; index < shared_data->number_count; ++index) {
    printf("%s\n" , shared_data->sums[index]);
  }

  // double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
  //   (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
  // printf("execution time: %.9lfs\n", elapsed);
  return error;
}
void* run(void* data) {
	const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;
  const size_t my_thread_id = private_data->thread_number;
	
  for (size_t i = my_thread_id; i < shared_data->number_count;
   i=i+shared_data->thread_count){
    process_number(shared_data->numbers[i], i, data);
  }
  return NULL;
}
void process_number(long long int number, int index, void* data){
  const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;
  size_t sums_count;
	char calculated_sums[100];
  if(check_valid(number) == false){
    sprintf(shared_data->sums[index], "%lld: NA"
      , number);
  }
  else{
    if(check_negative(number) == false){
      sums_count = find_sums(number, 
        check_even(number), false, calculated_sums);
      sprintf(shared_data->sums[index], "%lld: %zu sums"
      , number, sums_count);
    }else{
      sums_count = find_sums(number*(-1), 
        check_even(number*(-1)), true, calculated_sums);
      sprintf(shared_data->sums[index], "%lld: %zu sums: %s"
      , number, sums_count, calculated_sums);
    }
  }
}