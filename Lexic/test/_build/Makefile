CC = gcc
FLAGS = -Wall -Wextra -g

#Ordered in how foundational it is

all: warn.o match.o validate.o vocab.o file_to_str.o
	@echo 'Lexic Comprehensive Test'
	@$(CC) ../all_test.c ../name.c ../vocab.c ../token.c \
		../group.c ../qualifier.c ../escaped.c ../brackets.c ../or.c ../direct.c \
		../../private/name.c ../../private/token.c \
		warn.o match.o validate.o vocab.o file_to_str.o \
		-D ALL_TESTS -o test_all $(FLAGS)

lxc_name: warn.o match.o validate.o vocab.o file_to_str.o
	@echo 'Token Name Stream Tests'
	@$(CC) ../name.c ../../private/name.c ../../private/token.c vocab.o warn.o match.o validate.o file_to_str.o -D VERBOSE -o test_token_name $(FLAGS)

lxc_token: warn.o match.o validate.o vocab.o file_to_str.o
	@echo 'Token Stream Tests'
	@$(CC) ../token.c ../../private/token.c vocab.o warn.o match.o validate.o file_to_str.o -D VERBOSE -o test_token_stream $(FLAGS)

lxc_vocab: vocab.o warn.o validate.o file_to_str.o
	@echo 'Vocab Make Tests'
	@$(CC) ../vocab.c vocab.o warn.o validate.o file_to_str.o -D VERBOSE -o test_vocab_make $(FLAGS)

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

file_to_str.o: ../../private/file_to_str.c
	@echo 'file_to_str.o'
	@$(CC) ../../private/file_to_str.c -c $(FLAGS)

vocab.o: ../../lexic.h ../../private/lexic_internal.h ../../private/vocab.c
	@echo 'vocab.c -> .o'
	@$(CC) ../../private/vocab.c -c $(FLAGS)

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
