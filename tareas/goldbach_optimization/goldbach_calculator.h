#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define SUMS_LEN 400000

bool check_prime(long long number);
int find_sums(long long number, bool even, bool print
 , char* calculated_sums);//, size_t sums_length, char* sums);
bool check_even(long long number);
bool check_negative(long long number);
bool check_valid(long long number);
void store_sums(bool even, bool first,
 int i, int j, int k, char* calculated_sums);

#endif