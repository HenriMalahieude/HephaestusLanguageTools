#define TEST_NAME "Sequence"
#include "regex_helper.h"

void SequenceTest() {
#ifdef VERBOSE
	printf("[!] Regex Sequence Test Start\n");
#endif

	char seq[4] = "0-9";
	struct regex sequence = Regex_New_Sequence(seq);

#ifdef VERBOSE
	printf("[.] Initialized Sequence Regex\n");
#endif

	char send[2] = "0";
	for (int i = (int)'0'; i <= (int)'9'; i++) {
		send[0] = (char)i;
		TEST_REGEX(sequence, send, true);
	}

	TEST_REGEX(sequence, "A", false);

	TEST_REGEX(sequence, "a", false);
	
	TEST_REGEX(sequence, "\n", false);

#ifdef VERBOSE
	printf("[!] Regex Sequence Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	SequenceTest();
	return 0;
}
#endif
