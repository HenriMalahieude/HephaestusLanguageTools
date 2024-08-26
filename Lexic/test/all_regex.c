#include <stdio.h>
#include <stdbool.h>

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

//including all the other tests
bool DirectTest();
bool EscapedTest();
bool BracketTest();
bool QualifierTest();
bool GroupTest();
bool OrTest();

int main(void) {
	printf("[!] Testing all Regexes\n");
	
	printf("[.] Direct Regex");
	if (!DirectTest()) return 1;
	test_count = 1;

	printf(" passed. \n[.] Escaped Regexes");
	if (!EscapedTest()) return 1;
	test_count = 1;

	printf(" passed. \n[.] Bracket Regexes");
	if (!BracketTest()) return 1;
	test_count = 1;

	printf(" passed. \n[.] Qualifier Regexes");
	if (!QualifierTest()) return 1;
	test_count = 1;

	printf(" passed. \n[.] Group Regexes");
	if (!GroupTest()) return 1;
	test_count = 1;

	printf(" passed. \n[.] Or Regexes");
	if (!OrTest()) return 1;
	test_count = 1;

	printf(" passed. \n[!] Finished testing all Regexes\n");

	return 0;
}

