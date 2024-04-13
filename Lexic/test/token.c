#include "../lexic.h"
#include "../private/lexic_internal.h"
#include "warn.h"

int test_count = 1;
void ReportResult(int errCode, struct lexic_token out, struct lexic_token exp) {
	if (errCode > 0) {
		printf("[X] Token Stream Test (%d): ", test_count);
		switch (errCode) {
			case 1:
				printf("def (out '%s' vs '%s')\n", out.definition_name, exp.definition_name);
				break;
			case 2:
				printf("mch (out '%s' vs '%s')\n", out.matching_input, exp.matching_input);
				break;
			case 3:
				printf("lne (out %d vs %d)\n", out.line, exp.line);
				break;
			case 4:
				printf("col (out %d vs %d)\n", out.col, exp.col);
				break;
			default:
				break;
		}
	} 
#ifdef VERBOSE
	else {
		printf("[.] Token Stream Test (%d) Pass\n", test_count);
	}
#endif

	test_count++;
}

int CompareTokens(struct lexic_token streamout, struct lexic_token expected) {
	if (strcmp(streamout.definition_name, expected.definition_name) != 0) return 1;
	if (expected.definition_name != NULL && strcmp(streamout.matching_input, expected.matching_input) != 0) return 2;
	if (expected.line >= 0 && streamout.line != expected.line) return 3;
	if (expected.col >= 0 && streamout.col != expected.col) return 4;

	return 0;
}

void TokenStreamTest() {
#ifdef VERBOSE
	printf("[!] Token Stream Test Start!\n");
#endif

	LexicVocabulary *vocab = Lexic_Vocabulary_Allocate();
	Lexic_Vocabulary_Add_Definition(vocab, "digit", "[0-9]*");
	Lexic_Vocabulary_Add_Definition(vocab, "alpha", "[a-z]*");

	LexicToken niltok = {.definition_name = NULL, .matching_input = NULL, .line = -1, .col = -1};	
	int result = 0; LexicToken expected;

	//one token type
	LexicToken *tstream = Lexic_Token_Stream_Make_From_String("a", vocab);
	expected =  {.definition_name = "alpha", .matching_input = "a", .line = 1, .col = 1};
	result = CompareTokens(tstream[0], expected);
	ReportResult(result, tstream[0], expected); if (result != 0) return;

	result = CompareTokens(tstream[1], niltok);
	if (result != 0) return;
	Lexic_Token_Stream_Free(tstream);

	//one token type
	tstream = Lexic_Token_Stream_Make_From_String("0", vocab);
	expected =  {.definition_name = "digit", .matching_input = "0", .line = 1, .col = 1};
	result = CompareTokens(tstream[0], expected);
	ReportResult(result, tstream[0], expected); if (result != 0) return;
	Lexic_Token_Stream_Free(tstream);

	//multi stream
	tstream = Lexic_Token_Stream_Make_From_String("ab c", vocab);
	expected =  {.definition_name = "alpha", .matching_input = "ab", .line = 1, .col = 2};
	result = CompareTokens(tstream[0], expected);
	ReportResult(result, tstream[0], expected); if (result != 0) return;

	expected =  {.definition_name = "alpha", .matching_input = "c", .line = 1, .col = 4};
	result = CompareTokens(tstream[1], expected);
	ReportResult(result, tstream[1], expected); if (result != 0) return;
	Lexic_Token_Stream_Free(tstream);

	//multi stream
	tstream = Lexic_Token_Stream_Make_From_String("01 123", vocab);
	expected =  {.definition_name = "digit", .matching_input = "01", .line = 1, .col = 2};
	result = CompareTokens(tstream[0], expected);
	ReportResult(result, tstream[0], expected); if (result != 0) return;

	expected =  {.definition_name = "digit", .matching_input = "123", .line = 1, .col = 6};
	result = CompareTokens(tstream[1], expected);
	ReportResult(result, tstream[1], expected); if (result != 0) return;
	Lexic_Token_Stream_Free(tstream);

	tstream = Lexic_Token_Stream_Make_From_String("0a\nasdfbz123", vocab);
	expected =  {.definition_name = "digit", .matching_input = "0", .line = 1, .col = 1};
	result = CompareTokens(tstream[0], expected);
	ReportResult(result, tstream[0], expected); if (result != 0) return;

	expected =  {.definition_name = "alpha", .matching_input = "a", .line = 1, .col = 2};
	result = CompareTokens(tstream[1], expected);
	ReportResult(result, tstream[1], expected); if (result != 0) return;

	expected =  {.definition_name = "alpha", .matching_input = "asdfbz", .line = 2, .col = 6};
	result = CompareTokens(tstream[2], expected);
	ReportResult(result, tstream[2], expected); if (result != 0) return;

	expected =  {.definition_name = "digit", .matching_input = "123", .line = 2, .col = 9};
	result = CompareTokens(tstream[3], expected);
	ReportResult(result, tstream[3], expected); if (result != 0) return;

	result = CompareTokens(tstream[4], NULL);
	if (result != 0) return;
	Lexic_Token_Stream_Free(tstream);

#ifdef VERBOSE
	printf("[!] Token Stream Test Finished!\n");
#endif
}

int main(void) {
	TokenStreamTest();
	return 0;
}
