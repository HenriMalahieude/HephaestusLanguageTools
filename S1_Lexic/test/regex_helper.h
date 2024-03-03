#include <stdio.h>
#include <string.h>

//Test Regex Section
#include "../private/regex.h"

int regex_line_no = 0;
int regex_colu_no = 0;

#ifndef VERBOSE

#define TEST_REGEX_FALSE(name, num, strvar, strconst, rgx) ({\
		strcpy(strvar, strconst);\
		if (rgx->match_function(strvar, rgx->attached_data)) {\
			printf("[X] Regex %s Test %d Fail w/ %s\n", name, num, strvar);\
		}\
		})

#define TEST_REGEX_TRUE(name, num, strvar, strconst, rgx) ({\
		strcpy(strvar, strconst);\
		if (!rgx->match_function(strvar, rgx->attached_data)) {\
			printf("[X] Regex %s Test %d Fail w/ %s\n", name, num, strvar);\
		}\
		})

#endif

//--------------------------------------------------------------------------------------------

#ifdef VERBOSE

#define TEST_REGEX_FALSE(name, num, strvar, strconst, rgx) ({\
		printf("[?] Regex %s Test %d Result: ", name, num);\
		strcpy(strvar, strconst);\
		if (rgx->match_function(strvar, rgx->attached_data)) {\
			printf("[X] w/ %s\n", strvar);\
		} else {\
			printf("[+]\n");\
		}\
		})

#define TEST_REGEX_TRUE(name, num, strvar, strconst, rgx) ({\
		printf("[?] Regex %s Test %d Result: ", name, num);\
		strcpy(strvar, strconst);\
		if (!rgx->match_function(strvar, rgx->attached_data)) {\
			printf("[X] w/ %s\n", strvar);\
		} else {\
			printf("[+]\n");\
		}\
		})

#endif

