#define TEST_NAME "Escaped"
#include "regex_helper.h"

void EscapedTest() {
#ifdef VERBOSE
	printf("[!] Regex Escaped Test Start\n");
#endif

	char nl[3] = "\\n";
	char ws[3] = "\\s";
	char nws[3] = "\\S";

	TEST_REGEX(nl, "\n", true);
	TEST_REGEX(nl, "a", false);

	TEST_REGEX(ws, " ", true);
	TEST_REGEX(ws, "\t", true);
	TEST_REGEX(ws, "a", false);
	TEST_REGEX(ws, "A", false);
	TEST_REGEX(ws, "9", false);

	TEST_REGEX(nws, "a", true);
	TEST_REGEX(nws, "A", true);
	TEST_REGEX(nws, "0", true);
	TEST_REGEX(nws, "\t", false);
	TEST_REGEX(nws, " ", false);

#ifdef VERBOSE
	printf("[!] Regex Escaped Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	//warn_level = LWT_DEBUG;
	EscapedTest();
	return 0;
}
#endif
