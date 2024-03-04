#include <stdio.h>
#include <string.h>

//Test Regex Section
#include "../private/regex.h"

int regex_line_no = 0;
int regex_colu_no = 0;

#ifndef TEST_NAME
	#define TEST_NAME "UNDEFINED"
#endif

int test_count = 1;

#ifndef VERBOSE

#define TEST_REGEX_FALSE(str, rgx) if (rgx->match_function(str, rgx->attached_data)) printf("[X] Regex %s False Test %d Fail w/ %s\n", TEST_NAME, test_count++, str)

#define TEST_REGEX_TRUE(str, rgx) if (!rgx->match_function(str, rgx->attached_data)) printf("[X] Regex %s Truth Test %d Fail w/ %s\n", TEST_NAME, test_count++, str)

#endif

//--------------------------------------------------------------------------------------------

#ifdef VERBOSE

#define TEST_REGEX_FALSE(str, rgx) ({\
		printf("[?] Regex %s False Test %d Result: ", TEST_NAME, test_count++);\
		if (rgx->match_function(str, rgx->attached_data)) {\
			printf("[X] w/ %s\n", str);\
		} else {\
			printf("[+]\n");\
		}\
		})

#define TEST_REGEX_TRUE(str, rgx) ({\
		printf("[?] Regex %s Truth Test %d Result: ", TEST_NAME, test_count++);\
		if (!rgx->match_function(str, rgx->attached_data)) {\
			printf("[X] w/ %s\n", str);\
		} else {\
			printf("[+]\n");\
		}\
		})

#endif

