#include <stdio.h>
int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

//including all the other tests
void DirectTest();
void EscapedTest();
void BracketTest();
void QualifierTest();
void GroupTest();
void OrTest();

int main(void) {
	printf("[!] Testing all Regexes\n");

	DirectTest();
	test_count = 1;

	EscapedTest();
	test_count = 1;

	BracketTest();
	test_count = 1;

	QualifierTest();
	test_count = 1;

	GroupTest();
	test_count = 1;

	OrTest();
	test_count = 1;

	printf("[!] Finished testing all Regexes\n");

	return 0;
}

