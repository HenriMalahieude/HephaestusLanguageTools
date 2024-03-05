#include "regex_helper.h"

#undef TEST_NAME
#define TEST_NAME "Direct"

void DirectTest() {
	#ifdef VERBOSE
	printf("[!] Regex Direct Test Start\n");
	#endif

	char match[4] = "int";
	struct regex *direct = Regex_New_Direct(match);

	#ifdef VERBOSE
	printf("[.] Initialized Direct Regex\n");
	#endif


	TEST_REGEX_FALSE("\0", direct);
	
	TEST_REGEX_FALSE("i\0", direct);
	
	TEST_REGEX_FALSE("in\0", direct);

	TEST_REGEX_TRUE("int\0", direct);

	TEST_REGEX_FALSE("double\0", direct);

	TEST_REGEX_FALSE("dobby!!!!", direct);

	#ifdef VERBOSE
	printf("[!] Regex Direct Test Finished\n");
	#endif
}

#ifndef ALL_TESTS

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

int main(void) {
	DirectTest();
	return 0;
}
#endif
