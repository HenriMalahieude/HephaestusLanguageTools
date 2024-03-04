#include "regex_helper.h"

void QualifierTest() {
#ifdef VERBOSE
	printf("[!] Regex Qualifier Test Start\n");
#endif

	char dmatch[2] = "a";
	struct regex *direct1 = Regex_New_Direct(dmatch);

	char dmatch1[2] = "b";
	struct regex *direct2 = Regex_New_Direct(dmatch1);

	struct regex *or = Regex_New_Or(direct1, direct2);

	char espec[3] = "\\s";
	struct regex *escaped = Regex_New_Escaped(espec);

#ifdef VERBOSE
	printf("[!] Initialized 4 prototype regexes\n");
#endif
	struct regex *q1 = Regex_New_Qualifier(direct1, '?');

	TEST_REGEX_TRUE("", q1);

	TEST_REGEX_TRUE("a", q1);

	TEST_REGEX_FALSE("aa", q1);

	TEST_REGEX_FALSE("b", q1);

	struct regex *q2 = Regex_New_Qualifier(direct1, '*');

	TEST_REGEX_TRUE("", q2);

	TEST_REGEX_TRUE("a", q2);

	TEST_REGEX_TRUE("aa", q2);

	TEST_REGEX_TRUE("aaaaaaaaa", q2);

	TEST_REGEX_FALSE("aaabaaa", q2);

	TEST_REGEX_FALSE("aaab", q2);

	TEST_REGEX_FALSE("bbb", q2);

	struct regex *q3 = Regex_New_Qualifier(direct1, '+');

	TEST_REGEX_FALSE("", q3);

	TEST_REGEX_TRUE("a", q3);

	TEST_REGEX_TRUE("aaaaaa", q3);

	TEST_REGEX_FALSE("ababa", q3);

	struct regex *q4 = Regex_New_Qualifier(or, '*');

	TEST_REGEX_TRUE("", q4);

	TEST_REGEX_TRUE("ab", q4);

	TEST_REGEX_TRUE("ba", q4);

	TEST_REGEX_TRUE("abababababababababbbaaabababaaa", q4);

	TEST_REGEX_FALSE("abd", q4);

	TEST_REGEX_FALSE("d", q4);

	struct regex *q5 = Regex_New_Qualifier(escaped, '+');

	TEST_REGEX_FALSE("", q5);

	TEST_REGEX_TRUE(" ", q5);
	
	TEST_REGEX_TRUE("        \t ", q5);

	TEST_REGEX_FALSE("   b", q5);

#ifdef VERBOSE
	printf("[!] Regex Qualifier Test Finished\n");
#endif
}

#ifndef ALL_TESTS
int main(void) {
	QualifierTest();
	return 0;
}
#endif
