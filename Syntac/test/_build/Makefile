CC = gcc
FLAGS = -Wall -Wextra -g
XTRA_FLAGS ?=

INDV_TESTS = book_ruleadd

book_ruleadd: warn.o book.o file_to_str.o trim.o
	$(CC) -o $@ ../ruleadd.c $^ $(FLAGS) $(XTRA_FLAGS)

book.o: ../../private/book.c
	$(CC) -c $< $(FLAGS)

warn.o: ../../../helpers/log/warn.c
	$(CC) -c $< $(FLAGS)

file_to_str.o: ../../../helpers/string/file_to_str.c
	$(CC) -c $< $(FLAGS)

trim.o: ../../../helpers/string/trim.c
	$(CC) -c $< $(FLAGS)

clean:
	-@rm *.o
	-@rm $(INDV_TESTS)
