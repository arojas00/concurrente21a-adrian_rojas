#target: prerequisites
#	rule to boild target

goldbach: goldbach.c
	clang -g -Wall -Wextra goldbach.c -o goldbach -pthread

.PHONY: clean
clean:
	rm -rf goldbach

#-fsanitize=thread -fsanitize=address 
