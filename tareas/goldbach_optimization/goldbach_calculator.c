#include <stdio.h>
#include <stdbool.h>

_Bool check_prime(long long number);
int find_sums(long long number, _Bool even, _Bool print
 , char* calculated_sums);//, size_t sums_length, char* sums);
_Bool check_even(long long number);
_Bool check_negative(long long number);
_Bool check_valid(long long number);
void store_numbers();

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
 * @param even tells if the numer es even
 * @param print tells if it should print the numbers
 * @param sums reference to the string containing the sums for each number
 * @return int telling the amount of sums found
 */
int find_sums(long long number, _Bool even, _Bool print
 , char* calculated_sums){//, size_t sums_length, char* sums) {
	int count = 0;
	_Bool first = true;
  char* str = (char*) calloc(100, sizeof(char));
	for (int i = 2; i < number; i++) {
		if(check_prime(i)){
			if(even == true) {
				int j = number-i;
				if (check_prime(j) && j >= i) {
					count++;
					if(print == true) {
						if(first == false) {
							strcat(calculated_sums,", ");
						}
						sprintf(str, "%d + %d", i, j); 
						strcat(calculated_sums,str);
						first = false;
						//printf("strlen: %lu\n", strlen(calculated_sums)+strlen(str));
						// if(strlen(calculated_sums)+strlen(str)*2>=sums_length){
						// 	sums_length+=100;
						// 	char* copy1 = (char*) realloc(calculated_sums, sums_length*sizeof(char));
						// 	calculated_sums = copy1;
						// 	char* copy2 = (char*) realloc(sums, sums_length*sizeof(char));
						// 	sums = copy2;
						// }
					}
				}
			}
			else{
				for (int j = i; j < number; j++) {
					if(check_prime(j)){
						int k = number-(i+j);
						if(check_prime(k) && k >= j){
							count++;
							if(print == true) {
								if(first == false) {
									strcat(calculated_sums,", ");
								}
								sprintf(str, "%d + %d + %d", i, j, k);
								strcat(calculated_sums,str);
								first = false;
								//printf("strlen: %lu\n", strlen(calculated_sums)+strlen(str));
								// if(strlen(calculated_sums)+strlen(str)*2>=sums_length){
								// 	//printf("entro\n");
								// 	sums_length+=100;
								// 	calculated_sums = (char*) realloc(calculated_sums, sums_length*sizeof(char));
								// 	sums = (char*) realloc(sums, sums_length*sizeof(char));
								// }
							}
						}
					}
				}
			}
		}
	}
	free(str);
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