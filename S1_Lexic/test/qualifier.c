#define TEST_NAME "Qualifier"
#include "regex_helper.h"

void QualifierTest() {
#ifdef VERBOSE
	printf("[!] Regex Qualifier Test Start\n");
#endif

	char q1[] = "a*";
	TEST_REGEX(q1, "", true);
	TEST_REGEX(q1, "a", true);
	TEST_REGEX(q1, "aa", true);
	TEST_REGEX(q1, "aaaaaaa", true);
	TEST_REGEX(q1, "aaab", false);
	TEST_REGEX(q1, "b", false);

	char q2[] = "b?";
	TEST_REGEX(q2, "", true);
	TEST_REGEX(q2, "b", true);
	TEST_REGEX(q2, "bb", false);
	TEST_REGEX(q2, "a", false);
	TEST_REGEX(q2, "aaaaaaaaa", false);
	TEST_REGEX(q2, "ba", false);

	char q3[] = "a+";
	TEST_REGEX(q3, "", false);
	TEST_REGEX(q3, "a", true);
	TEST_REGEX(q3, "aaaaaa", true);
	TEST_REGEX(q3, "ababa", false);//*/

	char q4[] = "[ab]*";
	TEST_REGEX(q4, "", true);
	TEST_REGEX(q4, "ab", true);
	TEST_REGEX(q4, "ba", true);
	TEST_REGEX(q4, "abababababababababbbaaabababaaa", true);
	TEST_REGEX(q4, "abd", false);
	TEST_REGEX(q4, "d", false);//*/

	char q5[] = "\\s+";
	TEST_REGEX(q5, "", false);
	TEST_REGEX(q5, " ", true);
	TEST_REGEX(q5, "        \t ", true);
	TEST_REGEX(q5, "   b", false);//*/

#ifdef VERBOSE
	printf("[!] Regex Qualifier Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	warn_level = LWT_DEBUG;
	QualifierTest();
	return 0;
}
#endif
