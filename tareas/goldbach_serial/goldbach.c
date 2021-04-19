#include <stdio.h>
#include <stdbool.h>

_Bool checkPrime(long long number){
    _Bool isPrime = true;
    for (long long i = 2; i <= number/2; i++){
        if(number%i == 0){
            isPrime = false;
            break;
        }
    }
    return isPrime;
}

void printEvenSums(long long number, int count){
    FILE* output = stdout;
    fprintf(output, ": ");
    int index = 0;
    for(int i = 2; i < number; i++){
        for(int j = i; j < number; j++){
            if((checkPrime(i) == true && checkPrime(j) == true) && i+j == number){
                fprintf(output, "%d + %d", i, j);
                index++;
                if(index < count){
                    fprintf(output, ", ");
                }
            }
        }
    }
    fprintf(output, "\n");
}

void sumEven(long long number){
    FILE* output = stdout;
    int count = 0;
    _Bool negative = false;
    fprintf(output, "%lld: ", number);
    if(number < 0){
        number *= (-1);
        negative = true;
    }
    for(int i = 2; i < number; i++){
        for(int j = i; j < number; j++){
            if((checkPrime(i) == true && checkPrime(j) == true) && i+j == number){
                count++;
            }
        }
    }
    fprintf(output, "%d sums", count);
    if(negative == true){
        printEvenSums(number, count);
    }else{
        fprintf(output, "\n");
    }
}

void printOddSums(long long number, int count){
    FILE* output = stdout;
    fprintf(output, ": ");
    int index = 0;
    for(int i = 2; i < number; i++){
        for(int j = i; j < number; j++){
            for(int k = j; k < number; k++){
                if(checkPrime(i) == true && checkPrime(j) == true && checkPrime(k) == true && i+j+k == number){
                    fprintf(output, "%d + %d + %d", i, j, k);
                    index++;
                    if(index < count){
                        fprintf(output, ", ");
                    }
                }
            }
        }
    }
    fprintf(output, "\n");
}

void sumOdd(long long number){
    FILE* output = stdout;
    int count = 0;
    _Bool negative = false;
    fprintf(output, "%lld: ", number);
    if(number < 0){
        number *= (-1);
        negative = true;
    }
    for(int i = 2; i < number; i++){
        for(int j = i; j < number; j++){
            for(int k = j; k < number; k++){
                if(checkPrime(i) == true && checkPrime(j) == true && checkPrime(k) == true && i+j+k == number){
                    count++;
                }
            }
        }
    }
    fprintf(output, "%d sums", count);
    if(negative == true){
        printOddSums(number, count);
    }else{
        fprintf(output, "\n");
    }
}

void checkEven(long long number){
    if(number%2 == 0){
        sumEven(number);
    }else{
        sumOdd(number);
    }
}

void checkValid(long long number){
    FILE* output = stdout;
    if(number <= 5 && number >= 0){
        fprintf(output, "%lld: NA\n", number);
    }else{
        checkEven(number);
    }
}

int main(void){
    FILE* input = stdin;
    long long number = 0ll;
    while(fscanf(input, "%lld", &number) == 1){
        checkValid(number);
    }
}