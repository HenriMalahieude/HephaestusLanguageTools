#define TEST_NAME "Or"
#include "regex_helper.h"

void OrTest() {

#ifdef VERBOSE
	printf("[!] Regex Or Test Start\n");
#endif


	//Direct Or
	char or1[] = "int|double";
	TEST_REGEX(or1, "int", true);
	TEST_REGEX(or1, "double", true);
	TEST_REGEX(or1, "integer", false);
	TEST_REGEX(or1, "doubley", false);
	TEST_REGEX(or1, "not", false);

	//Triple Or
	char or2[] = "ab|cd|ef";
	TEST_REGEX(or2, "ab", true);
	TEST_REGEX(or2, "cd", true);
	TEST_REGEX(or2, "ef", true);
	TEST_REGEX(or2, "in", false);
	TEST_REGEX(or2, "oubl", false);
	TEST_REGEX(or2, "har", false);
	TEST_REGEX(or2, "bruh", false);

	//Escaped Or
	char or4[] = "\\d|a";
	TEST_REGEX(or4, "0", true);
	TEST_REGEX(or4, "5", true);
	TEST_REGEX(or4, "9", true);
	TEST_REGEX(or4, "a", true);
	TEST_REGEX(or4, "b", false);
	TEST_REGEX(or4, "01", false);
	TEST_REGEX(or4, "ab", false);

	//Qualified Or
	char or3[] = "0+|1";
	TEST_REGEX(or3, "", false);
	TEST_REGEX(or3, "1", true);
	TEST_REGEX(or3, "0001", false);
	TEST_REGEX(or3, "111", false);
	TEST_REGEX(or3, "10", false);//*/

	//Group Or
	char or5[] = "(abc|def)*";
	TEST_REGEX(or5, "", true);
	TEST_REGEX(or5, "a", false);
	TEST_REGEX(or5, "abc", true);
	TEST_REGEX(or5, "acb", false);
	TEST_REGEX(or5, "def", true);
	TEST_REGEX(or5, "dfe", false);
	TEST_REGEX(or5, "abcabc", true);
	TEST_REGEX(or5, "abccba", false);
	TEST_REGEX(or5, "defdef", true);
	TEST_REGEX(or5, "defdfa", false);
	TEST_REGEX(or5, "abcdefdefabc", true);
	TEST_REGEX(or5, "abcdfadfaabc", false); //*/

	//Nested Group Or
	char or6[] = "abc|(def+|(ghi|jkl+)|mno)";
	TEST_REGEX(or6, "abc", true);
	TEST_REGEX(or6, "defff", true);
	TEST_REGEX(or6, "ghi", true);
	TEST_REGEX(or6, "jklll", true);
	TEST_REGEX(or6, "mno", true);
	TEST_REGEX(or6, "jk", false);//*/

#ifdef VERBOSE
	printf("[!] Regex Or Test Finished\n");
#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	//warn_level = LWT_DEBUG;
	OrTest();
	return 0;
}
#endif
