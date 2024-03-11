#define TEST_NAME "Brackets"
#include "regex_helper.h"

void BracketTest() {

#ifdef VERBOSE
	printf("[!] Regex Bracket Test Start\n");
#endif

	char list[4] = "abc";
	struct regex brackets1 = Regex_New_Brackets(list);
	
	char seq[8] = "a-zA-Z_";
	struct regex brackets2 = Regex_New_Brackets(seq);

#ifdef VERBOSE
	printf("[!] Initialized 2 Bracket Regexes\n");
#endif

	//Single characters
	TEST_REGEX(brackets1, "a", true);

	TEST_REGEX(brackets1, "b", true);

	TEST_REGEX(brackets1, "c", true);

	TEST_REGEX(brackets1, "ad", false);

	TEST_REGEX(brackets1, "bd", false);
	
	TEST_REGEX(brackets1, "cd", false);

	//Sequences
	TEST_REGEX(brackets2, "_", true);

	char send[2] = "a";
	for (int i = (int)'a'; i <= (int)'z'; i++) {
		send[0] = (char)i;
		TEST_REGEX(brackets2, send, true); 
	}

	send[0] = 'A';
	for (int i = (int)'A'; i <= (int)'Z'; i++) {
		send[0] = (char)i;
		TEST_REGEX(brackets2, send, true);
	}
	
	TEST_REGEX(brackets2, "9", false);
	TEST_REGEX(brackets2, "2", false);

#ifdef VERBOSE
	printf("[!] Regex Bracket Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	BracketTest();
	return 0;
}
#endif
