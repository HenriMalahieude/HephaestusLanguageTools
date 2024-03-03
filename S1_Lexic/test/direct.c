#include "regex_helper.h"

#undef TEST_NAME
#define TEST_NAME "Direct"

int main(void) {
	#ifdef VERBOSE
	printf("[!] Regex Direct Test\n");
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
