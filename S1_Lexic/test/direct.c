#include "regex_helper.h"

int main(void) {
	#ifdef VERBOSE
	printf("[!] Regex Direct Test\n");
	#endif

	char match[4] = "int";
	struct regex *direct = Regex_New_Direct(match);

	#ifdef VERBOSE
	printf("[.] Initialized Direct Regex\n");
	#endif

	char test[100]; test[99] = '\0';

	TEST_REGEX_FALSE("Direct", 0, test, "\0", direct);
	
	TEST_REGEX_FALSE("Direct", 1, test, "i\0", direct);
	
	TEST_REGEX_FALSE("Direct", 2, test, "in\0", direct);

	TEST_REGEX_TRUE("Direct", 3, test, "int\0", direct);

	TEST_REGEX_FALSE("Direct", 4, test, "double\0", direct);

	TEST_REGEX_FALSE("Direct", 5, test, "dobby!!!!", direct);

	#ifdef VERBOSE
	printf("[!] Regex Direct Test Finished\n");
	#endif
}
