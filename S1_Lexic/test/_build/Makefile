CC = gcc

WARN_F = -Wall -Wextra -g

or_test: regex.o match.o
	@echo 'Regex Or Test'
	@$(CC) ../or.c regex.o match.o -D VERBOSE -o test_regex_or $(WARN_F)

direct_test: regex.o match.o
	@echo 'Regex Direct Test'
	@$(CC) ../direct.c regex.o match.o -D VERBOSE -o test_regex_direct $(WARN_F)

regex.o:
	@$(CC) ../../private/regex.c -c $(WARN_F)

match.o:
	@$(CC) ../../private/match.c -c $(WARN_F)

clean:
	-rm *.o 2>/dev/null || true
	-rm *.exe 2>/dev/null || true
