#define TEST_NAME "Qualifier"
#include "regex_helper.h"

void QualifierTest() {
#ifdef VERBOSE
	printf("[!] Regex Qualifier Test Start\n");
#endif

	char dmatch[2] = "a";
	struct regex direct1 = Regex_New_Direct(dmatch);

	char dmatch1[2] = "b";
	struct regex direct2 = Regex_New_Direct(dmatch1);

	struct regex or = Regex_New_Or(direct1, direct2);

	char espec[3] = "\\s";
	struct regex escaped = Regex_New_Escaped(espec);

#ifdef VERBOSE
	printf("[!] Initialized 4 prototype regexes\n");
#endif
	struct regex q1 = Regex_New_Qualifier(direct1, '?');

	TEST_REGEX(q1, "", true);

	TEST_REGEX(q1, "a", true);

	TEST_REGEX(q1, "aa", false);

	TEST_REGEX(q1, "b", false);

	struct regex q2 = Regex_New_Qualifier(direct1, '*');

	TEST_REGEX(q2, "", true);

	TEST_REGEX(q2, "a", true);

	TEST_REGEX(q2, "aa", true);

	TEST_REGEX(q2, "aaaaaaaaa", true);

	TEST_REGEX(q2, "aaabaaa", false);

	TEST_REGEX(q2, "aaab", false);

	TEST_REGEX(q2, "bbb", false);

	struct regex q3 = Regex_New_Qualifier(direct1, '+');

	TEST_REGEX(q3, "", false);

	TEST_REGEX(q3, "a", true);

	TEST_REGEX(q3, "aaaaaa", true);

	TEST_REGEX(q3, "ababa", false);

	struct regex q4 = Regex_New_Qualifier(or, '*');

	TEST_REGEX(q4, "", true);

	TEST_REGEX(q4, "ab", true);

	TEST_REGEX(q4, "ba", true);

	TEST_REGEX(q4, "abababababababababbbaaabababaaa", true);

	TEST_REGEX(q4, "abd", false);

	TEST_REGEX(q4, "d", false);

	struct regex q5 = Regex_New_Qualifier(escaped, '+');

	TEST_REGEX(q5, "", false);

	TEST_REGEX(q5, " ", true);
	
	TEST_REGEX(q5, "        \t ", true);

	TEST_REGEX(q5, "   b", false);

#ifdef VERBOSE
	printf("[!] Regex Qualifier Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	QualifierTest();
	return 0;
}
#endif
