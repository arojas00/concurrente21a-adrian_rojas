#include "goldbach_optimization.h"

#define NUMS_LEN 100

int main(int argc, char* argv[]) {
  FILE *input = stdin;
  int error = EXIT_SUCCESS;
  size_t numbers_length = NUMS_LEN;
  shared_data_t* shared_data = (shared_data_t*) calloc(1, sizeof(shared_data_t));
  queue_init(&shared_data->queue);
  sem_init(&shared_data->can_consume, 0, 0);
  sem_init(&shared_data->can_access_units_consumed, 0, 1);
  sem_init(&shared_data->can_access_units_produced, 0, 1);
  shared_data->numbers = (long long int*) calloc(numbers_length, 
    sizeof(long long int));
  
  shared_data->number_count = 0;
  while(fscanf(input, "%lld", &shared_data->numbers[shared_data->number_count]) == 1){
    shared_data->number_count++;
    if(shared_data->number_count+1 >= numbers_length){
      numbers_length += NUMS_LEN;
      shared_data->numbers = (long long int*) realloc(shared_data->numbers, 
        numbers_length*sizeof(long long int));
    }
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
      sem_destroy(&shared_data->can_consume);
      sem_destroy(&shared_data->can_access_units_consumed);
      sem_destroy(&shared_data->can_access_units_produced);
      queue_free(&shared_data->queue);
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

  double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
    (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
  printf("execution time: %.9lfs\n", elapsed);
  return error;
}
void* run(void* data) {
	const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;
  const size_t my_thread_id = private_data->thread_number;
	
  for (size_t i = my_thread_id; i < shared_data->number_count;
   i+=shared_data->thread_count){
    process_number(shared_data->numbers[i], i, data);
  }
  return NULL;
}
void process_number(long long int number, int index, void* data){
  const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;
  size_t sums_count;
  //size_t sums_length = SUMS_LEN;
	char* calculated_sums = (char*) calloc(SUMS_LEN, sizeof(char));
  if(check_valid(number) == false){
    sprintf(shared_data->sums[index], "%lld: NA", number);
  }
  else{
    if(check_negative(number) == false){
      sums_count = find_sums(number, check_even(number), false,
       calculated_sums);//,sums_length,shared_data->sums[index]);
      sprintf(shared_data->sums[index], "%lld: %zu sums"
      , number, sums_count);
    }else{
      sums_count = find_sums(number*(-1), check_even(number*(-1)), true,
       calculated_sums);//, sums_length,shared_data->sums[index]);
      sprintf(shared_data->sums[index], "%lld: %zu sums: %s"
      , number, sums_count, calculated_sums);
    }
  }
  free(calculated_sums);

}