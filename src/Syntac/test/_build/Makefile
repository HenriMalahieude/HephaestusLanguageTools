CC := gcc
FLAGS := -Wall -Wextra -g
XTRA_FLAGS ?=

HLP_DIR := ../../../helpers
SRC_DIR := ../../private

#In order of importance (foundational -> uses foundation)
DEPS := sets.o \
		warn.o \
		trim.o \
		file_to_str.o \
		terminal.o \
		book.o \
		firsts.o \
		follows.o

INDV_TESTS := set book_ruleadd book_fromx first follow

.PHONY: all
.SUFFIXES: 

all_indv: $(INDV_TESTS)

$(INDV_TESTS): $(DEPS)
	$(CC) -o $@ ../$@.c $^ $(FLAGS) -DVERBOSE $(XTRA_FLAGS)

warn.o: $(HLP_DIR)/log/warn.c
	$(CC) -c $< $(FLAGS)

file_to_str.o: $(HLP_DIR)/string/file_to_str.c
	$(CC) -c $< $(FLAGS)

trim.o: $(HLP_DIR)/string/trim.c
	$(CC) -c $< $(FLAGS)

%.o: $(SRC_DIR)/%.c
	$(CC) -c $< $(FLAGS)

clean:
	-@rm *.o 2>/dev/null || true
	-@rm $(INDV_TESTS) 2>/dev/null || true
