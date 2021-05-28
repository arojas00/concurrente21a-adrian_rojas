#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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
int find_sums(long long number, _Bool even, _Bool print) {
	FILE *output = stdout;
	int count = 0;
	int index = 0;
	for (int i = 2; i < number; i++) {
		for (int j = i; j < number; j++) {
			if(even == true) {
				if ((check_prime(i) == true && check_prime(j) == true) && i + j == number) {
					count++;
					if(print == true) {
						if(index != 0) {
							fprintf(output, ",");
						}
						fprintf(output, " %d + %d", i, j);
						index = 1;
					}
				}
			}
			else{
				for (int k = j; k < number; k++) {
					if (check_prime(i) == true && check_prime(j) == true && check_prime(k) == true && i + j + k == number) {
						count++;
						if(print == true) {
							if(index != 0) {
								fprintf(output, ",");
							}
							fprintf(output, " %d + %d + %d", i, j, k);
							index = 1;
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
void check_negative(long long number) {
	FILE *output = stdout;
	_Bool negative = false;
	_Bool even = false;
	int count = 0;
	even = check_even(number);
	fprintf(output, "%lld: ", number);
	if (number < 0) {
		number *= (-1);
		negative = true;
	}
	if (negative == true) {
		count = find_sums(number,even,false);
		fprintf(output, "%d sums:", count);
		find_sums(number,even,true);
		fprintf(output, "\n");
	}
	else {
		count = find_sums(number,even,false);
		fprintf(output, "%d sums\n", count);
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
		check_negative(number);
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