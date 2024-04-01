#ifndef __HEPHAESTUS_REGEX_TESTER_HELPER__
#define __HEPHAESTUS_REGEX_TESTER_HELPER__

#include <stdio.h>
#include <string.h>

#include "../lexic.h"
#include "../private/regex.h"

#ifndef TEST_NAME
	#define TEST_NAME "UNDEFINED"
#endif

extern int test_count;

static void TEST_REGEX(char *rgx, char *str, bool resolve_to){
	bool val = (Regex_Match(rgx, str) == resolve_to);
	
#ifndef VERBOSE
	if (!val) printf("[X] %s Test %d (%s) Fail w/ %s\n", TEST_NAME, test_count++, rgx, str);
#else
	printf("[?] Regex %s Test %d Result: ", TEST_NAME, test_count++);
	if (!val) printf("[X] w/ '%s' -> '%s'\n", rgx, str);
	if (val) printf("[+]\n");
#endif
}

#endif
