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
								
	if (test_count != valid) {
#ifdef VERBOSE
		printf("[/] Failed before checkpoint.\n");
#endif
		goto bottom;
	}
	
	//Check Unions
	valid += TEST_NULL(SetUnion(NULL, NULL));

	char **same_set = SetUnion(NULL, set);
	valid += TEST(SetCount(same_set), 4);
	SetFree(same_set);

	char **new_set = NULL;
	SetAdd(&new_set, "d");
	SetAdd(&new_set, "c");
	valid += TEST(SetCount(new_set), 2);

	char **union_set = SetUnion(set, new_set);
	valid += TEST(SetCount(union_set), 6);
	valid += TEST(SetContains(union_set, "c"), 1);
	valid += TEST(SetContains(union_set, "a"), 1);
	valid += TEST(SetContains(union_set, "cC"), 1);
	
	SetFree(new_set); new_set = NULL;
	SetFree(union_set); union_set = NULL;

bottom:
	SetFree(set); set = NULL;
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
