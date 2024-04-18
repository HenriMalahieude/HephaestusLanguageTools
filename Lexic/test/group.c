#include <stdlib.h>

#define TEST_NAME "Group"
#include "regex_helper.h"

void GroupTest(){
#ifdef VERBOSE
	printf("[!] Regex Group Test Start\n");
#endif

	char manual_construct[] = "(\\.[0-9]+)*";
	TEST_REGEX(manual_construct, "", true);
	TEST_REGEX(manual_construct, ".0", true);
	TEST_REGEX(manual_construct, ".00.000.01238.1239.487", true);
	TEST_REGEX(manual_construct, "092", false);
	TEST_REGEX(manual_construct, ".12309a098", false);
	TEST_REGEX(manual_construct, ".01.a", false);

	char built[] = "-?[0-9]+(\\.[0-9]+)?";
	TEST_REGEX(built, "9", true);
	TEST_REGEX(built, "-0", true);
	TEST_REGEX(built, "09.123", true);
	TEST_REGEX(built, "-10.005", true);
	TEST_REGEX(built, "10023213854.901293847", true);
	TEST_REGEX(built, "-123.123.123", false);
	TEST_REGEX(built, "-1.ab", false);
	TEST_REGEX(built, "asdf", false);
	TEST_REGEX(built, "--1-1", false);// */

	char nested[] = "ab(cd*(a*f*)?)+";
	TEST_REGEX(nested, "abcddddddd", true);
	TEST_REGEX(nested, "abcdaaa", true);
	TEST_REGEX(nested, "abcddfff", true);
	TEST_REGEX(nested, "abcdafcdafccd", true);
	TEST_REGEX(nested, "abcddfa", false);
	TEST_REGEX(nested, "abc", true);// */

#ifdef VERBOSE
	printf("[!] Regex Group Test Finished\n");
#endif
}

#ifndef ALL_TESTS
int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;
int main(void) {
	//warn_level = LWT_DEBUG;
	GroupTest();
	return 0;
}
#endif
