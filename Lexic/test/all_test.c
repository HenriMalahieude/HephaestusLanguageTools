#include <stdio.h>
#include <stdbool.h>
int test_count = 1;

//Regex Tests
bool DirectTest();
bool EscapedTest();
bool BracketTest();
bool QualifierTest();
bool GroupTest();
bool OrTest();

//Lexic Whole Tests
bool VocabularyMakeTests();
bool TokenStreamTest();
bool TokenNameStreamTests();

int main(void) {
	printf("[!] Testing Lexic\n");

	if (!DirectTest()) return 1;
	test_count = 1;

	if (!EscapedTest()) return 1;
	test_count = 1;

	if (!BracketTest()) return 1;
	test_count = 1;

	if (!QualifierTest()) return 1;
	test_count = 1;

	if (!GroupTest()) return 1;
	test_count = 1;

	if (!OrTest()) return 1;
	test_count = 1;

	bool succ = VocabularyMakeTests();
	if (!succ) {
		printf("[X] Vocabulary Make Test Failed at Test %d\n", test_count);
		return 1;
	}

	test_count = 1;
	succ = TokenStreamTest();
	if (!succ) {
		printf("[X] Token Stream Test Failed at Test %d\n", test_count);
		return 1;
	}

	test_count = 1;
	succ = TokenNameStreamTests();
	if (!succ) {
		printf("[X] Token Name Stream Test Failed at Test %d\n", test_count);
		return 1;
	}

	printf("[!] All Lexic Test Ran (No Output is Good Output)\n");
	
	return 0;
}
