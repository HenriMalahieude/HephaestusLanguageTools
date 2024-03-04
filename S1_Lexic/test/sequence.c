#define TEST_NAME "Sequence"
#include "regex_helper.h"

void SequenceTest() {
#ifdef VERBOSE
	printf("[!] Regex Sequence Test Start\n");
#endif

	char seq[4] = "0-9";
	struct regex *sequence = Regex_New_Sequence(seq);

#ifdef VERBOSE
	printf("[.] Initialized Sequence Regex\n");
#endif

	char send[2] = "0";
	for (int i = (int)'0'; i <= (int)'9'; i++) {
		send[0] = (char)i;
		TEST_REGEX_TRUE(send, sequence);
	}

	TEST_REGEX_FALSE("A", sequence);

	TEST_REGEX_FALSE("a", sequence);
	
	TEST_REGEX_FALSE("\n", sequence);

#ifdef VERBOSE
	printf("[!] Regex Sequence Test Finished\n");
#endif
}

#ifndef ALL_TESTS
int main(void) {
	SequenceTest();
	return 0;
}
#endif
