#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define SUMS_LEN 400000

_Bool check_prime(long long number);
int find_sums(long long number, _Bool even, _Bool print
 , char* calculated_sums);//, size_t sums_length, char* sums);
_Bool check_even(long long number);
_Bool check_negative(long long number);
_Bool check_valid(long long number);
void store_sums(_Bool even,_Bool first,
 int i, int j, int k, char* calculated_sums);

/**
 * @brief check if the number is prime
 * @param number the number to be checked
 * @return bool telling if the number is prime
 */
_Bool check_prime(long long number) {
	for (long long i = 2; i <= sqrt(number); i++) {
		if (number % i == 0) {
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
 * @param sums reference to the string containing the sums for each number
 * @return int telling the amount of sums found
 */
int find_sums(long long number, _Bool even, _Bool print, char* calculated_sums){
	//, size_t sums_length, char* sums) {
	_Bool first = true;
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
							sprintf(calculated_sums, "error: not enough allocated memory");
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
									sprintf(calculated_sums, "error: not enough allocated memory");
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
void store_sums(_Bool even,_Bool first,
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
	//printf("strlen: %lu\n", strlen(calculated_sums)+strlen(str));
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
_Bool check_negative(long long number) {
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
_Bool check_valid(long long number) {
	if (number <= 5 && number >= -5) {
		return false;
	}
	else {
		return true;
	}
}