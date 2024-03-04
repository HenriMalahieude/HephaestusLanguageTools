#include "regex_helper.h"

#undef TEST_NAME
#define TEST_NAME "Escaped"

void EscapedTest() {
#ifdef VERBOSE
	printf("[!] Regex Escaped Test Start\n");
#endif

	char nl[3] = "\\n";
	char ws[3] = "\\s";
	char nws[3] = "\\S";

	struct regex *new_line = Regex_New_Escaped(nl);
	struct regex *white_space = Regex_New_Escaped(ws);
	struct regex *not_white_space = Regex_New_Escaped(nws);

#ifdef VERBOSE
	printf("[.] Initialized 3 Escaped Regexes\n");
#endif

	TEST_REGEX_TRUE("\n", new_line);

	TEST_REGEX_FALSE("a", new_line);

	TEST_REGEX_TRUE(" ", white_space);

	TEST_REGEX_TRUE("\t", white_space);

	TEST_REGEX_FALSE("a", white_space);

	TEST_REGEX_FALSE("A", white_space);

	TEST_REGEX_FALSE("9", white_space);

	TEST_REGEX_TRUE("a", not_white_space);

	TEST_REGEX_TRUE("A", not_white_space);

	TEST_REGEX_TRUE("0", not_white_space);

	TEST_REGEX_FALSE("\t", not_white_space);

	TEST_REGEX_FALSE(" ", not_white_space);

#ifdef VERBOSE
	printf("[!] Regex Escaped Test Finished\n");
#endif
}

#ifndef ALL_TESTS
int main(void) {
	EscapedTest();
	return 0;
}
#endif
