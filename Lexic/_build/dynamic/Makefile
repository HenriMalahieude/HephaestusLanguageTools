CC = gcc
AFLAGS = -Wall -fPIC

SRC_LOC = ../../private/

dynamic: file_to_str.o match.o validate.o name.o token.o vocab.o warn.o
	@echo 'Linking a dynamic library'
	@$(CC) -shared -o liblexic.so *.o $(AFLAGS)

file_to_str.o: ../../private/file_to_str.c
	@echo 'ftostr.o'
	@$(CC) $(SRC_LOC)file_to_str.c -c $(AFLAGS)

match.o: ../../private/match.c ../../private/*.h 
	@echo 'match.o'
	@$(CC) $(SRC_LOC)match.c -c $(AFLAGS)

validate.o: ../../private/validate.c ../../private/*.h 
	@echo 'validate.o'
	@$(CC) $(SRC_LOC)validate.c -c $(AFLAGS)

name.o: ../../private/name.c ../../private/*.h ../../lexic.h
	@echo 'name.o'
	@$(CC) $(SRC_LOC)name.c -c $(AFLAGS)

token.o: ../../private/token.c ../../private/*.h ../../lexic.h
	@echo 'token.o'
	@$(CC) $(SRC_LOC)token.c -c $(AFLAGS)

vocab.o: ../../private/vocab.c ../../private/*.h ../../lexic.h
	@echo 'vocab.o'
	@$(CC) $(SRC_LOC)vocab.c -c $(AFLAGS)

warn.o: ../../private/warn.c ../../private/warn.h
	@echo 'warn.o'
	@$(CC) $(SRC_LOC)warn.c -c $(AFLAGS)

clean:
	rm *.o 2>/dev/null || true
	rm *.so 2>/dev/null || true

