#include <stdio.h>
#include <stdbool.h>

_Bool check_prime(long long number);
int find_sums(long long number, _Bool even, _Bool print, char* str);
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
 * @param str reference to the string containing the sums for each number
 * @return int telling the amount of sums found
 */
int find_sums(long long number, _Bool even, _Bool print, char* str) {
	int count = 0;
	int flag = 0;
  char str2[100];
	for (int i = 2; i < number; i++) {
		for (int j = i; j < number; j++) {
			if(even == true) {
				if ((check_prime(i) == true && check_prime(j) == true) && i + j == number) {
					count++;
					if(print == true) {
						if(flag != 0) {
							strcat(str,", ");
						}
						sprintf(str2, "%d + %d", i, j); 
            strcat(str,str2);
						flag = 1;
					}
				}
			}
			else{
				for (int k = j; k < number; k++) {
					if (check_prime(i) == true && check_prime(j) == true 
						&& check_prime(k) == true && i + j + k == number) {
						count++;
						if(print == true) {
							if(flag != 0) {
								strcat(str,", ");
							}
							sprintf(str2, "%d + %d + %d", i, j, k);
              strcat(str,str2);
							flag = 1;
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