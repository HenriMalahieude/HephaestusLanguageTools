#include <stdio.h>

#include "../private/regex.h"

void TEST_VALIDATE(char *rgx, bool result_to) {
	bool val = (RegexValidate(rgx) == result_to);

#ifndef VERBOSE
	if (!val) printf("[X] Regex Validation Fail: %s\n", rgx);
#else
	printf("[?] Regex Validation Test: ");
	if (val) printf("[+]\n");
	else printf("[X] w/ %s\n", rgx);
#endif
}

void ValidationTest() {
#ifdef VERBOSE
	printf("[!] Regex Validation Test Start\n");
#endif
	TEST_VALIDATE("-?[0-9]+(\\.[0-9]+)?", true);
	TEST_VALIDATE("?", false);
	TEST_VALIDATE("a*?", false);
	TEST_VALIDATE("[]]", false);
	TEST_VALIDATE("[[]", false);
	TEST_VALIDATE("(()", false);
	TEST_VALIDATE("())", false);
	TEST_VALIDATE("|", false);
	TEST_VALIDATE("abc|(|)", false);
	TEST_VALIDATE("abc|*", false);
	TEST_VALIDATE("abc|", false);

#ifdef VERBOSE
	printf("[!] Regex Validation Test Finished\n");
#endif
}

#ifndef ALL_TESTS
#include "../private/warn.h"

int regex_line_no = 0;
int regex_colu_no = 0;

int main(void) {
	warn_level = LWT_SILENT;
	ValidationTest();
	return 0;
}
#endif
