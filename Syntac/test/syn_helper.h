#ifndef __HEPHAESTUS_SYNTAC_TESTER_HELPER__
#define __HEPHAESTUS_SYNTAC_TESTER_HELPER__

#include <stdio.h>
#include <string.h>

#include "../private/syntac_internal.h"
#include "../../helpers/log/warn.h"

#ifndef TEST_NAME
# define TEST_NAME "UNDEFINED"
#endif

extern int test_count;

static bool TEST(int input, int shouldbe) {
	bool val = (input == shouldbe);
#ifndef VERBOSE
	if (!val) printf("[X] %s Test %d Fail\n", TEST_NAME, test_count);
#else
	printf("[?] %s Test %d: ", TEST_NAME, test_count);
	if (!val) printf("[X] (got %d, should be %d)\n", input, shouldbe);
	else printf("[+]\n");
#endif
	test_count++;
	return input == shouldbe;
}

static bool TEST_STRING(char *input, char *shouldbe) {
	bool val = (strcmp(input, shouldbe) == 0);
#ifndef VERBOSE
	if (!val) printf("[X] %s Test %d Fail: Got '%s' vs Expected '%s'\n", TEST_NAME, test_count, input, shouldbe);
#else
	printf("[?] %s Test %d: ", TEST_NAME, test_count);
	if (!val) printf("[X] (got '%s', expected '%s')\n", input, shouldbe);
	else printf("[+]\n");
#endif
	test_count++;
	return val;
}

static bool TEST_NULL(void *input) {
	bool val = (input == NULL);
#ifndef VERBOSE
	if (!val) printf("[X] %s Test %d Fail: Got %p vs Expected NULL\n", TEST_NAME, test_count, input);
#else
	printf("[?] %s Test %d: ", TEST_NAME, test_count);
	if (!val) printf("[X] (got %p, expected NULL)\n", input);
	else printf("[+]\n");
#endif
	test_count++;
	return val;
}

#endif
