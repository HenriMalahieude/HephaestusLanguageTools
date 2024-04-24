CC = gcc

BFLAGS = -I ../../ -L ../../_build/static/ -l lexic
WFLAGS = -Wall
FLAGS = $(WFLAGS) $(BFLAGS)

default: 
	$(CC) -o lexic_cli ../main.c $(FLAGS)

clean: 
	rm *.exe *.o *.out 2>/dev/null || true
