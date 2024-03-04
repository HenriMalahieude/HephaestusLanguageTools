CC = gcc

WARN_F = -Wall -Wextra -g

escaped_test: regex.o match.o
	@echo 'Regex Escaped Test'
	@$(CC) ../escaped.c regex.o match.o -D VERBOSE -o test_regex_escaped $(WARN_F)

brackets_test: regex.o match.o
	@echo 'Regex Brackets Test'
	@$(CC) ../brackets.c regex.o match.o -D VERBOSE -o test_regex_brackets $(WARN_F)

sequence_test: regex.o match.o
	@echo 'Regex Sequence Test'
	@$(CC) ../sequence.c regex.o match.o -D VERBOSE -o test_regex_sequence $(WARN_F)

or_test: regex.o match.o
	@echo 'Regex Or Test'
	@$(CC) ../or.c regex.o match.o -D VERBOSE -o test_regex_or $(WARN_F)

direct_test: regex.o match.o
	@echo 'Regex Direct Test'
	@$(CC) ../direct.c regex.o match.o -D VERBOSE -o test_regex_direct $(WARN_F)

regex.o: ../../private/regex.h ../../private/regex.c
	@echo 'regex.c -> .o'
	@$(CC) ../../private/regex.c -c $(WARN_F)

match.o: ../../private/regex.h ../../private/match.h ../../private/match.c
	@echo 'match.c -> .o'
	@$(CC) ../../private/match.c -c $(WARN_F)

clean:
	-rm *.o 2>/dev/null || true
	-rm *.exe 2>/dev/null || true
