#define TEST_NAME "Set"
#include "syn_helper.h"
#include "../private/sets.h"

bool SetTest() {
	test_count = 0;
	int valid = 0;
	
	print_test("Start");

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
	valid += TEST(SetContains(set, ""), 0);
	valid += TEST(SetContains(set, "b"), 1);
	valid += TEST(SetContains(set, "Cc"), 1);
	valid += TEST(SetContains(set, "item"), 0);
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
	valid += TEST(SetEquality(same_set, set), 1);
	SetFree(same_set); same_set = NULL;

	char **new_set = NULL;
	SetAdd(&new_set, "d");
	SetAdd(&new_set, "c");
	valid += TEST(SetCount(new_set), 2);

	char **union_set = SetUnion(set, new_set);
	valid += TEST(SetCount(union_set), 6);
	valid += TEST(SetContains(union_set, "c"), 1);
	valid += TEST(SetContains(union_set, "a"), 1);
	valid += TEST(SetContains(union_set, "cC"), 1);

	//Check Equality
	valid += TEST(SetEquality(NULL, NULL), 1);
	valid += TEST(SetEquality(union_set, new_set), 0);

	char **right_set = NULL;
	SetAdd(&right_set, "c");
	SetAdd(&right_set, "d");
	valid += TEST(SetCount(right_set), 2);
	
	valid += TEST(SetEquality(right_set, NULL), 0);
	valid += TEST(SetEquality(NULL, new_set), 0);
	valid += TEST(SetEquality(new_set, right_set), 1);
	
	SetFree(right_set); right_set = NULL;
	SetFree(new_set); new_set = NULL;
	SetFree(union_set); union_set = NULL;

	//Check creation
	char **variadic_set = SetCreate(5, "a", "b", "c", "dd", "eee");
	valid += TEST(SetCount(variadic_set), 5);

	valid += TEST(SetContains(variadic_set, "a"), 1);
	valid += TEST(SetContains(variadic_set, "dd"), 1);
	valid += TEST(SetContains(variadic_set, "e"), 0);

	char **set_set = SetCreate(1, "bruh");
	valid += TEST(SetCount(set_set), 1);
	valid += TEST_STRING(set_set[0], "bruh");
	valid += TEST(SetAdd(&set_set, "L"), 1);
	valid += TEST(SetContains(set_set, "L"), 1);
	valid += TEST(SetContains(set_set, "dd"), 0);
	valid += TEST(SetEquality(set_set, variadic_set), 0);

	SetFree(variadic_set); variadic_set = NULL;
	SetFree(set_set); set_set = NULL;

bottom:
	SetFree(set); set = NULL;
	print_test("Finished");

	return test_count == valid;
}

#ifndef ALL_TESTS
int test_count = 0;
int main(void) {
	//warn_level = HLT_DEBUG;
	return !SetTest();
}
#endif
