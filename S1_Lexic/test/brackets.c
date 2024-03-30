#define TEST_NAME "Brackets"
#include "regex_helper.h"

void BracketTest() {

#ifdef VERBOSE
	printf("[!] Regex Bracket Test Start\n");
#endif

	char list[] = "[abc]";
	char seq[] = "[a-zA-Z_]";

	//Single characters
	TEST_REGEX(list, "a", true);
	TEST_REGEX(list, "b", true);
	TEST_REGEX(list, "c", true);
	TEST_REGEX(list, "ad", false);
	TEST_REGEX(list, "bd", false);
	TEST_REGEX(list, "cd", false);

	//Sequences
	TEST_REGEX(seq, "_", true);
	char send[2] = "a";
	for (int i = (int)'a'; i <= (int)'z'; i++) {
		send[0] = (char)i;
		TEST_REGEX(seq, send, true); 
	}

	send[0] = 'A';
	for (int i = (int)'A'; i <= (int)'Z'; i++) {
		send[0] = (char)i;
		TEST_REGEX(seq, send, true);
	}
	
	TEST_REGEX(seq, "9", false);
	TEST_REGEX(seq, "2", false); //*/

#ifdef VERBOSE
	printf("[!] Regex Bracket Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	warn_level = LWT_STDWRN;
	BracketTest();
	return 0;
}
#endif
