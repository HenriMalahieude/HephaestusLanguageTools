CC = gcc
AFLAGS = -Wall
DFLAGS = -fPIC $(AFLAGS)

SRC_LOC = ./private/
STATIC_LOC = ./bin/static/
DYNAMIC_LOC = ./bin/dynamic/

DSRC_LOC = ../../$(SRC_LOC)
DSRC_FILES = $(DSRC_LOC)file_to_str.c \
	     $(DSRC_LOC)match.c \
	     $(DSRC_LOC)name.c \
	     $(DSRC_LOC)token.c \
	     $(DSRC_LOC)validate.c \
	     $(DSRC_LOC)vocab.c \
	     $(DSRC_LOC)warn.c

static: s-location file_to_str.o match.o name.o token.o validate.o vocab.o warn.o lexic.h
	@echo 'Linking a static library into $(STATIC_LOC)'
	@ar rcs $(STATIC_LOC)lexic.a $(STATIC_LOC)*.o

dynamic: d-location
	@echo 'Making and Linking a dynamic library into $(DYNAMIC_LOC)'
	@cd $(DYNAMIC_LOC) && $(CC) $(DSRC_FILES) -c $(DFLAGS)
	@$(CC) -shared -o $(DYNAMIC_LOC)lexic.so $(DYNAMIC_LOC)*.o

#Objects
file_to_str.o: private/file_to_str.c
	@echo 'Making Static ftostr'
	@$(CC) -o $(STATIC_LOC)file_to_str.o $(SRC_LOC)file_to_str.c -c $(AFLAGS)

match.o: private/match.c private/*.h 
	@echo 'Making Static match'
	@$(CC) -o $(STATIC_LOC)match.o $(SRC_LOC)match.c -c $(AFLAGS)

validate.o: private/validate.c private/*.h 
	@echo 'Making Static validate'
	@$(CC) -o $(STATIC_LOC)validate.o $(SRC_LOC)validate.c -c $(AFLAGS)

name.o: private/name.c private/*.h lexic.h
	@echo 'Making Static name'
	@$(CC) -o $(STATIC_LOC)name.o $(SRC_LOC)name.c -c $(AFLAGS)

token.o: private/token.c private/*.h lexic.h
	@echo 'Making Static token'
	@$(CC) -o $(STATIC_LOC)token.o $(SRC_LOC)token.c -c $(AFLAGS)

vocab.o: private/vocab.c private/*.h lexic.h
	@echo 'Making Static vocab'
	@$(CC) -o $(STATIC_LOC)vocab.o $(SRC_LOC)vocab.c -c $(AFLAGS)

warn.o: private/warn.c private/warn.h
	@echo 'Making Static warn'
	@$(CC) -o $(STATIC_LOC)warn.o $(SRC_LOC)warn.c -c $(AFLAGS)

#Locations
s-location:
	@if [ ! -d $(STATIC_LOC) ]; then mkdir -p $(STATIC_LOC); fi

d-location:
	@if [ ! -d $(DYNAMIC_LOC) ]; then mkdir -p $(DYNAMIC_LOC); fi

#Clean up
clean: clean-all

clean-all: 
	rm -rf bin/ 2>/dev/null || true

clean-static:
	rm $(STATIC_LOC)*.o 2>/dev/null || true
	rm $(STATIC_LOC)*.a 2>/dev/null || true

clean-dynamic:
	rm $(DYNAMIC_LOC)*.o 2>/dev/null || true
	rm $(DYNAMIC_LOC)*.so 2>/dev/null || true
