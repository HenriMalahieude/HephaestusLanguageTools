#define TEST_NAME "Set"
#include "syn_helper.h"
#include "../private/sets.h"

bool SetTest() {
	test_count = 0;
	int valid = 0;

#ifdef VERBOSE
	printf("[!] Set Test Start\n");
#endif

	char **set = NULL;
	
	//Edge-cases
	valid += TEST(SetCount(set), 0);
	valid += TEST(SetContains(set, "item"), 0);
	valid += TEST(SetContains(set, ""), 0); // */

	//Adding things
	valid += TEST(SetAdd(&set, "a"), 1);
	valid += TEST(SetAdd(&set, "b"), 1);
	valid += TEST(SetAdd(&set, "a"), 0);
	valid += TEST(SetCount(set), 2);
	valid += TEST(SetAdd(&set, "cC"), 1);
	valid += TEST(SetAdd(&set, "cC"), 0);
	valid += TEST(SetAdd(&set, "Cc"), 1);
	valid += TEST(SetAdd(&set, "b"), 0);
	valid += TEST(SetCount(set), 4);

	//Contains things
	valid += TEST(SetContains(set, "a"), 1);
	valid += TEST(SetContains(set, "b"), 1);
	valid += TEST(SetContains(set, "Cc"), 1);
	valid += TEST(SetContains(set, "cC"), 1);

	//Investigate the internals
	valid += TEST_STRING(set[0], "a");
	valid += TEST_STRING(set[1], "b");
	valid += TEST_STRING(set[2], "cC");
	valid += TEST_STRING(set[3], "Cc");
	valid += TEST_NULL(set[4]); // */

	SetFree(set);
	set = NULL;
#ifdef VERBOSE
	printf("[!] Set Test Finished\n");
#endif

	return test_count == valid;
}

#ifndef ALL_TESTS
int test_count = 0;
int main(void) {
	//warn_level = HLT_DEBUG;
	return !SetTest();
}
#endif
