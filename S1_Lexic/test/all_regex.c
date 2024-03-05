#include <stdio.h>

int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;

void DirectTest();
void OrTest();
void SequenceTest();
void BracketTest();
void EscapedTest();
void QualifierTest();

int main(void) {
	printf("[!] Testing all Regexes\n");

	DirectTest();
	test_count = 0;

	OrTest();
	test_count = 0;

	SequenceTest();
	test_count = 0;

	EscapedTest();
	test_count = 0;

	//Because they require the other ones
	BracketTest();
	test_count = 0;

	QualifierTest();
	test_count = 0;

	printf("[!] Finished testing all Regexes\n");

	return 0;
}

