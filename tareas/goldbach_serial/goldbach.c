#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
/**
 * @brief check if the number si prime
 * @param number the number to be checked
 * @return bool telling if prime number
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
 * @brief print the two numbers composing the sum
 * @param number the number to chech goldbach
 * @param count the number of sums found
 */
void print_even_sums(long long number, int count) {
	FILE *output = stdout;
	fprintf(output, ": ");
	int index = 0;
	for (int i = 2; i < number; i++) {
		for (int j = i; j < number; j++) {
			if ((check_prime(i) == true && check_prime(j) == true) && i + j == number) {
				fprintf(output, "%d + %d", i, j);
				index++;
				if (index < count) {
					fprintf(output, ", ");
				}
			}
		}
	}
	fprintf(output, "\n");
}
/**
 * @brief find the amount of sums for the even number
 * @param number the number to check goldbach
 */
void sum_even(long long number) {
	FILE *output = stdout;
	int count = 0;
	_Bool negative = false;
	fprintf(output, "%lld: ", number);
	if (number < 0) {
		number *= (-1);
		negative = true;
	}
	for (int i = 2; i < number; i++) {
		for (int j = i; j < number; j++) {
			if ((check_prime(i) == true && check_prime(j) == true) && i + j == number) {
				count++;
			}
		}
	}
	fprintf(output, "%d sums", count);
	if (negative == true) {
		print_even_sums(number, count);
	}
	else {
		fprintf(output, "\n");
	}
}
/**
 * @brief print the three numbers composing the sum
 * @param number the number to chech goldbach
 * @param count the number of sums found
 */
void print_odd_sums(long long number, int count) {
	FILE *output = stdout;
	fprintf(output, ": ");
	int index = 0;
	for (int i = 2; i < number; i++) {
		for (int j = i; j < number; j++) {
			for (int k = j; k < number; k++) {
				if (check_prime(i) == true && check_prime(j) == true && check_prime(k) == true && i + j + k == number) {
					fprintf(output, "%d + %d + %d", i, j, k);
					index++;
					if (index < count) {
						fprintf(output, ", ");
					}
				}
			}
		}
	}
	fprintf(output, "\n");
}
/**
 * @brief find the amount of sums for the  odd number
 * @param number the number to check goldbach
 */
void sum_odd(long long number) {
	FILE *output = stdout;
	int count = 0;
	_Bool negative = false;
	fprintf(output, "%lld: ", number);
	if (number < 0) {
		number *= (-1);
		negative = true;
	}
	for (int i = 2; i < number; i++) {
		for (int j = i; j < number; j++) {
			for (int k = j; k < number; k++) {
				if (check_prime(i) == true && check_prime(j) == true && check_prime(k) == true && i + j + k == number) {
					count++;
				}
			}
		}
	}
	fprintf(output, "%d sums", count);
	if (negative == true) {
		print_odd_sums(number, count);
	}
	else {
		fprintf(output, "\n");
	}
}
/**
 * @brief check if the number si oven or odd
 * @param number the number to be checked
 */
void check_even(long long number) {
	if (number % 2 == 0) {
		sum_even(number);
	}
	else {
		sum_odd(number);
	}
}
/**
 * @brief check if the number si valid
 * @param number the number to be checked
 */
void check_valid(long long number) {
	FILE *output = stdout;
	if (number <= 5 && number >= 0) {
		fprintf(output, "%lld: NA\n", number);
	}
	else {
		check_even(number);
	}
}
/**
 * @brief stores the numbers from the input in an array
 */
void store_numbers() {
	FILE *input = stdin;
	long long number = 0ll;
	int size = 10;
	int index = 0;
	long long * array = (long long*)calloc(size, sizeof(long long));
	while (fscanf(input, "%lld", &number) == 1) {
		array[index] = number;
		index++;
		if(index >= size) {
			size = size + 10;
			array = realloc(array, size * sizeof(long long));
		}
	}
	for(int i = 0; i < index; i++) {
		number = array[i];
		check_valid(number);
	}
	free(array);
}

int main(void) {
	store_numbers();
	return 0;
}