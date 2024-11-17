CC = gcc
FLAGS = -Wall -Wextra -g

INDV_TESTS = book_ruleadd

book_ruleadd: warn.o book.o file_to_str.o trim.o
	$(CC) -o $@ ../ruleadd.c $^ $(FLAGS)

book.o:
	$(CC) -c ../../private/book.c $(FLAGS)

warn.o:
	$(CC) -c ../../../helpers/log/warn.c $(FLAGS)

file_to_str.o:
	$(CC) -c ../../../helpers/string/file_to_str.c $(FLAGS)

trim.o:
	$(CC) -c ../../../helpers/string/trim.c $(FLAGS)

clean:
	-@rm *.o
	-@rm $(INDV_TESTS)
