#include "regex_helper.h"

#undef TEST_NAME
#define TEST_NAME "Brackets"

void BracketTest() {

#ifdef VERBOSE
	printf("[!] Regex Bracket Test Start\n");
#endif

	char list[4] = "abc";
	struct regex *brackets1 = Regex_New_Brackets(list);
	
#ifdef VERBOSE
	printf("[!] Initialized 2 Bracket Regexes\n");
#endif

	//Single characters
	TEST_REGEX_TRUE("a", brackets1);

	TEST_REGEX_TRUE("b", brackets1);

	TEST_REGEX_TRUE("c", brackets1);

	TEST_REGEX_FALSE("ad", brackets1);

	TEST_REGEX_FALSE("bd", brackets1);
	
	TEST_REGEX_FALSE("cd", brackets1);

	char seq[8] = "a-zA-Z_";
	struct regex *brackets2 = Regex_New_Brackets(seq);

	//Sequences
	TEST_REGEX_TRUE("_", brackets2);

	char send[2] = "a";
	for (int i = (int)'a'; i <= (int)'z'; i++) {
		send[0] = (char)i;
		TEST_REGEX_TRUE(send, brackets2); 
	}

	send[0] = 'A';
	for (int i = (int)'A'; i <= (int)'Z'; i++) {
		send[0] = (char)i;
		TEST_REGEX_TRUE(send, brackets2);
	}
	
	TEST_REGEX_FALSE("9", brackets2);
	TEST_REGEX_FALSE("2", brackets2);

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
