CC = gcc
FLAGS = -Wall -Wextra -g

#Ordered in how foundational it is

token: warn.o
	@echo 'Token Stream Tests'
	@$(CC) ../token.c ../../private/token.c ../../private/vocab.c ../../private/file_to_str.c warn.o -D VERBOSE -o test_token_stream $(FLAGS)

vocab: warn.o validate.o
	@echo 'Vocab Make Tests'
	@$(CC) ../vocab.c ../../private/vocab.c ../../private/file_to_str.c warn.o validate.o -D VERBOSE -o test_vocab_make $(FLAGS)

rgx_all: match.o warn.o
	@echo 'All Regex Tests'
	@$(CC) ../all_regex.c warn.o match.o ../group.c ../qualifier.c ../escaped.c ../brackets.c ../or.c ../direct.c -D ALL_TESTS -o test_regex_all $(FLAGS)

rgx_or: match.o warn.o
	@echo 'Regex Or Test'
	@$(CC) ../or.c warn.o match.o -D VERBOSE -o test_regex_or $(FLAGS)

rgx_group: match.o warn.o
	@echo 'Regex Group Test'
	@$(CC) ../group.c match.o warn.o -D VERBOSE -o test_regex_group $(FLAGS)

rgx_qualifier: match.o warn.o
	@echo 'Regex Qualifier Test'
	@$(CC) ../qualifier.c match.o warn.o -D VERBOSE -o test_regex_qualifier $(FLAGS)

rgx_brackets: match.o warn.o
	@echo 'Regex Brackets Test'
	@$(CC) ../brackets.c warn.o match.o -D VERBOSE -o test_regex_brackets $(FLAGS)

rgx_escaped: match.o warn.o
	@echo 'Regex Escaped Test'
	@$(CC) ../escaped.c warn.o match.o -D VERBOSE -o test_regex_escaped $(FLAGS)

rgx_direct: match.o warn.o
	@echo 'Regex Direct Test'
	@$(CC) ../direct.c warn.o match.o -D VERBOSE -o test_regex_direct $(FLAGS)

rgx_validate: validate.o warn.o
	@echo 'Regex Validation Test'
	@$(CC) ../validation.c validate.o warn.o -D VERBOSE -o test_regex_validation $(FLAGS)

match.o: ../../lexic.h ../../private/regex.h ../../private/match.c
	@echo 'match.c -> .o'
	@$(CC) ../../private/match.c -c $(FLAGS)

validate.o: ../../lexic.h ../../private/regex.h ../../private/validate.c
	@echo 'validate.c -> .o'
	@$(CC) ../../private/validate.c -c $(FLAGS)

warn.o: ../../lexic.h ../../private/warn.c
	@echo 'warn.c -> .o'
	@$(CC) ../../private/warn.c -c $(FLAGS)

clean:
	@echo 'Removing all object and executables'
	@rm *.o 2>/dev/null || true
	@rm *.exe 2>/dev/null || true
	@rm *.out 2>/dev/null || true
