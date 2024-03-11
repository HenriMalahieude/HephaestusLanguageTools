#include <stdio.h>
#include <string.h>
#include "../private/regex.h"

#ifndef __HEPHAESTUS_REGEX_TESTER_HELPER__
#define __HEPHAESTUS_REGEX_TESTER_HELPER__

#ifndef TEST_NAME
	#define TEST_NAME "UNDEFINED"
#endif

extern int test_count;

void TEST_REGEX(struct regex rgx, char *str, bool resolve_to){
	bool val = (Regex_Match(rgx, str) == resolve_to);
	
#ifndef VERBOSE
	if (!val) printf("[X] Regex %s Test %d Fail w/ %s\n", TEST_NAME, test_count++, str);
#else
	printf("[?] Regex %s Test %d Result: ", TEST_NAME, test_count++);
	if (!val) printf("[X] w/ %s\n", str);
	if (val) printf("[+]\n");
#endif
}

#endif
