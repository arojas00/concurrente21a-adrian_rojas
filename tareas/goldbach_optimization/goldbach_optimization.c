#include "goldbach_optimization.h"

// SUMS_LEN is also defined in goldbach_calculator.c
#define SUMS_LEN 10000000 // this length for the sums array is needed for test case 21
//#define SUMS_LEN 400000 // this length for the sums array is enough for every test case except 21
#define NUMS_LEN 100

int main(int argc, char* argv[]) {
  FILE *input = stdin;
  int error = EXIT_SUCCESS;
  size_t numbers_length = NUMS_LEN;
  shared_data_t* shared_data = (shared_data_t*) calloc(1, sizeof(shared_data_t));
  sem_init(&shared_data->can_access_numbers_consumed, 0, 1);
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
      sem_destroy(&shared_data->can_access_numbers_consumed);
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
  // printf("execution time: %.9lfs\n", elapsed); //for time measuring
  return error;
}
/**
 * @brief run method for pthreads
 * @param data shared data for each thread
 */
void* run(void* data) {
	const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;
	while (true) {
    sem_wait(&shared_data->can_access_numbers_consumed);
      if (shared_data->numbers_consumed >= shared_data->number_count) {
        sem_post(&shared_data->can_access_numbers_consumed);
        break;
      }

      size_t index = shared_data->numbers_consumed;
      shared_data->numbers_consumed++;
    sem_post(&shared_data->can_access_numbers_consumed);

    process_number(shared_data->numbers[index], index, data);
  }
  return NULL;
}
/**
 * @brief processes the number assigned to the thread
 * @param number number to be processed
 * @param index index assigned to the given number
 * @param data shared data for each thread
 */
void process_number(long long int number, int index, void* data){
  const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;
  size_t sums_count;
  size_t sums_length = SUMS_LEN;
	char* calculated_sums = (char*) calloc(sums_length, sizeof(char));
  if(check_valid(number) == false){
    sprintf(shared_data->sums[index], "%lld: NA", number);
  }
  else{
    if(check_negative(number) == false){
      sums_count = find_sums(number, check_even(number), false,
       calculated_sums,sums_length);
      sprintf(shared_data->sums[index], "%lld: %zu sums"
      , number, sums_count);
    }else{
      sums_count = find_sums(number*(-1), check_even(number*(-1)), true,
       calculated_sums, sums_length);
      // if(sums_length > SUMS_LEN){
      //   increase_size(shared_data->sums[index], sums_length);
      // }
      sprintf(shared_data->sums[index], "%lld: %zu sums: %s"
      , number, sums_count, calculated_sums);
    }
  }
  free(calculated_sums);
}
void increase_size(char* array, size_t array_length){
  char* temp_ptr = (char*) realloc(array,array_length*sizeof(char));
  array = temp_ptr;
}