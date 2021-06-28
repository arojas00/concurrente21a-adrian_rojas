#include "goldbach_calculator.h"

// SUMS_LEN is also defined in goldbach_optimization.c
#define SUMS_LEN 10000000 // this length for the sums array is needed for test case 21
//#define SUMS_LEN 400000 // this length for the sums array is enough for every test case except 21

/**
 * @brief check if the number is prime
 * @param number the number to be checked
 * @return bool telling if the number is prime
 */
bool check_prime(long long number) {
	if (number <= 63){
		return (FIRST_PRIMES >> number) & true;
	}
	if (number%2 == 0){
		return false;
	}
	for (long long i = 3; i*i <= number; i += 2) {
		if (number%i == 0){
			return false;
		}
	}
	return true;
}
/**
 * @brief finds the sums for each number and prints them if needed
 * @param number the number to be checked
 * @param even tells if the numer es even
 * @param print tells if it should print the numbers
 * @param calculated_sums reference to the string containing the sums for each number
 * @return int telling the amount of sums found
 */
int find_sums(long long number, bool even, bool print, char* calculated_sums){
	//, size_t sums_length, char* sums) {
	bool first = true;
	int count = 0;
	for (int i = 2; i <= number/2; i++) {
		if(check_prime(i)){
			if(even == true) {
				int j = number-i;
				if (check_prime(j) && j >= i) {
					count++;
					if(print == true) {
						store_sums(even, first, i, j, 0, calculated_sums);
						first = false;
						if(strlen(calculated_sums)+20>=SUMS_LEN){
							sprintf(calculated_sums, "error: not enough allocated memory for this number");
							return count;
						}
					}
				}
			}
			else{
				for (int j = i; j <= (number/3)*2; j++) {
					if(check_prime(j)){
						int k = number-(i+j);
						if(check_prime(k) && k >= j){
							count++;
							if(print == true) {
								store_sums(even, first, i, j, k, calculated_sums);
								first = false;
								if(strlen(calculated_sums)+20>=SUMS_LEN){
									sprintf(calculated_sums, "error: not enough allocated memory for this number");
									return count;
								}
							}
						}
					}
				}
			}
		}
	}
	return count;
}
/**
 * @brief concatenates the sums found into a string
 * @param even tells if the numer es even
 * @param first tells if its the first sum to determine format
 * @param i first number of the sum
 * @param j second number of the sum
 * @param k third number of the sum, if its an odd number
 * @param calculated_sums reference to the string containing the sums for each number
 */
void store_sums(bool even, bool first,
 int i, int j, int k, char* calculated_sums){
	char* str = (char*) calloc(100, sizeof(char));
	if(first == false) {
		strcat(calculated_sums,", ");
	}
	if(even){
		sprintf(str, "%d + %d", i, j); 
	}
	else{
		sprintf(str, "%d + %d + %d", i, j, k);
	}
	strcat(calculated_sums,str);
	// printf("strlen: %lu\n", strlen(calculated_sums)+strlen(str));
	// if(strlen(calculated_sums)+strlen(str)*2>=sums_length){
	// 	sums_length+=100;
	// 	char* copy1 = (char*) realloc(calculated_sums, sums_length*sizeof(char));
	// 	calculated_sums = copy1;
	// 	char* copy2 = (char*) realloc(sums, sums_length*sizeof(char));
	// 	sums = copy2;
	// }
	free(str);
}
/**
 * @brief check if the number is even
 * @param number the number to be checked
 * @return bool telling if the number is even
 */
bool check_even(long long number) {
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
bool check_negative(long long number) {
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
bool check_valid(long long number) {
	if (number <= 5 && number >= -5) {
		return false;
	}
	else {
		return true;
	}
}