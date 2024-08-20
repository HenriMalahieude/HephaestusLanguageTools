#include <stdio.h>
#include <stdbool.h>
int test_count = 1;

//Regex Tests
void DirectTest();
void EscapedTest();
void BracketTest();
void QualifierTest();
void GroupTest();
void OrTest();

//Lexic Whole Tests
bool VocabularyMakeTests();
bool TokenStreamTest();
bool TokenNameStreamTests();

int main(void) {
	printf("[!] Testing Lexic\n");

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
