#include <stdio.h>
#include <string.h>

#include "../lexic.h"
#include "../private/lexic_internal.h"

bool CompareResults(char **output, char **expected, int *error_index) {
	int i = 0; *error_index = 0;
	while (output[i] != NULL && expected[i] != NULL) {
		char *cur_out = output[i];
		char *cur_exp = expected[i];
		if (cur_out == NULL){
			if (cur_exp == NULL) return true;
			else return false;
		} else if (cur_exp == NULL || strcmp(cur_out, cur_exp) != 0) return false;

		i++;
		*error_index = i;
	}

	return output[i] == NULL && expected[i] == NULL;
}

int test_count = 1;
void ReportResult(bool succ, char *output, char *expected) {
	char nil[] = "NULL!";
	if (output == NULL) output = nil;
	if (expected == NULL) expected = nil;

#ifndef VERBOSE
	if (!succ) printf("[X] Token Name Stream Test %d Fail: (Got '%s' vs Expected '%s')\n", test_count, output, expected);
#else 
	printf("[?] Token Name Stream Test %d ", test_count);
	if (succ) printf("Pass\n");
	else printf("Fail: (Got '%s' vs Expected '%s')\n", output, expected);
#endif
	test_count++;
}

bool Test(LexicVocabulary *vocab, char *stream, char **expected) {
	char **output = LexicTokenNamesFromString(stream, vocab);
	
	int error_index = 0;
	bool succ = CompareResults(output, expected, &error_index);

	if (succ) error_index = 0;
	ReportResult(succ, output[error_index], expected[error_index]);

	return succ;
}

void TokenNameStreamTests(){
#ifdef VERBOSE
	printf("[!] Token Name Stream Tests Start!\n");
#endif

	LexicVocabulary *vocab = LexicVocabularyFromFile("../example.vocab");
	
	char *names[6] = {
		"TYPE",
		"IDENTIFIER",
		"ASSIGN",
		"NUMBER",
		"SEMICOLON",
		NULL
	};

	if (!Test(vocab, "int _word_ = 500;", names)) return;
	if (!Test(vocab, "char abc = 42;", names)) return;
	if (!Test(vocab, "float pi_2 = -5.6;", names)) return;

	char *second_test[2] = {
		"FNUMBER",
		NULL
	};

	if (!Test(vocab, "504.f", second_test)) return;
	if (!Test(vocab, "-3.12345f", second_test)) return;

#ifdef VERBOSE
	printf("[!] Token Name Stream Tests Finished Successfully!\n");
#endif
}

int main(void) {
	TokenNameStreamTests();
	return 0;
}
