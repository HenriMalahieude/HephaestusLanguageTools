CC = gcc

_BUILD = ../../_build/Lexic/object

BFLAGS = -I ../ -L $(_BUILD) -l lexic
WFLAGS = -Wall
FLAGS = $(WFLAGS) $(BFLAGS)

default: $(_BUILD)/liblexic.a
	$(CC) -o lexic_cli ./main.c $(FLAGS)

$(_BUILD)/liblexic.a:
	@$(MAKE) -s -C ../../ Lexic

clean: 
	-rm lexic_cli.exe
	@$(MAKE) -s -C ../../ clean
