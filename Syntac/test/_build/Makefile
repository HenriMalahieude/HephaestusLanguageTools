CC = gcc
FLAGS = -Wall -Wextra -g
XTRA_FLAGS ?=

INDV_TESTS = book_ruleadd book_fromx set

set: sets.o
	$(CC) -o $@ ../set.c $^ $(FLAGS) $(XTRA_FLAGS)

book_fromx: warn.o book.o file_to_str.o trim.o
	$(CC) -o $@ ../from.c $^ $(FLAGS) $(XTRA_FLAGS)

book_ruleadd: warn.o book.o file_to_str.o trim.o
	$(CC) -o $@ ../ruleadd.c $^ $(FLAGS) $(XTRA_FLAGS)

warn.o: ../../../helpers/log/warn.c
	$(CC) -c $< $(FLAGS)

file_to_str.o: ../../../helpers/string/file_to_str.c
	$(CC) -c $< $(FLAGS)

trim.o: ../../../helpers/string/trim.c
	$(CC) -c $< $(FLAGS)

%.o: ../../private/%.c
	$(CC) -c $< $(FLAGS)

clean:
	-@rm *.o 2>/dev/null || true
	-@rm $(INDV_TESTS) 2>/dev/null || true
