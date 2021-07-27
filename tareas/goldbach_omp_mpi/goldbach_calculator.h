#ifndef GOLDBACH_CALCULATOR_H
#define GOLDBACH_CALCULATOR_H

// #include <stdio.h>
// #include <stdbool.h>
// #include <math.h>
// #include <string.h>
// #include <stdlib.h>
#include "goldbach_optimization.h"

#define FIRST_PRIMES ((1ULL <<  2) | (1ULL <<  3) | (1ULL <<  5) | \
                    	(1ULL <<  7) | (1ULL << 11) | (1ULL << 13) | \
                    	(1ULL << 17) | (1ULL << 19) | (1ULL << 23) | \
                    	(1ULL << 29) | (1ULL << 31) | (1ULL << 37) | \
                    	(1ULL << 41) | (1ULL << 43) | (1ULL << 47) | \
											(1ULL << 53) | (1ULL << 59) | (1ULL << 61))

bool check_prime(long long number);
int find_sums(long long number, bool even, bool print
 , char* calculated_sums, size_t sums_length);
bool check_even(long long number);
bool check_negative(long long number);
bool check_valid(long long number);
void store_sums(bool even, bool first,
 int i, int j, int k, char* calculated_sums);

#endif