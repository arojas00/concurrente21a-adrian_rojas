#ifndef GOLDBACH_OPTIMIZATION_H
#define GOLDBACH_OPTIMIZATION_H

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
#include <math.h>

#include "matrix.h"
#include "goldbach_calculator.h"

typedef struct {
  sem_t can_access_numbers_consumed;

  size_t numbers_consumed;
  
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

#endif