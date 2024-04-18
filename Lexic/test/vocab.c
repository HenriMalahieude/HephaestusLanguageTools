#include <stdio.h>
#include <string.h>

#include "../lexic.h"
#include "../private/lexic_internal.h"
#include "../private/warn.h"

void ReportResult(char *msg, bool result) {
#ifdef VERBOSE
	printf("[?] Vocab Test (%s): %d\n", msg, result);
#else
	if (!result) printf("[X] Vocab Test (%s) failed\n", msg);
#endif
}

void VocabularyMakeTests(){
#ifdef VERBOSE
	printf("[!] Starting Vocabulary Make Tests\n");
#endif

	struct lxc_vocabulary *first = LexicVocabularyAllocate(); ReportResult("Allocation", first != NULL);
	
	bool succ = LexicVocabularyDefinitionAdd(first, "digit", "[0-9]"); 
	ReportResult("Add Def0", succ);
	ReportResult("Add Def1", first->def_count == 1);
	ReportResult("Add Def2", strcmp(first->definitions[0].name, "digit") == 0);
	ReportResult("Add Def3", strcmp(first->definitions[0].regex, "[0-9]") == 0);

	succ = LexicVocabularyDefinitionAdd(first, "abc", "abc");
	ReportResult("Add Def4", succ);
	ReportResult("Add Def5", first->def_count == 2);

	LexicVocabularyFree(first);
	
	//Note that this will be built in _build folder/directory
	first = LexicVocabularyFromFile("../example.vocab");
	ReportResult("File1", first->def_count == 6);
	ReportResult("File2", strcmp(first->definitions[0].name, "TYPE") == 0);
	ReportResult("File3", strcmp(first->definitions[3].name, "NUMBER") == 0);
	ReportResult("File4", strcmp(first->definitions[5].name, "SEMICOLON") == 0);
	ReportResult("File5", strcmp(first->definitions[5].regex, ";") == 0);
	ReportResult("File6", strcmp(first->definitions[2].regex, "=") == 0);
	ReportResult("File7", strcmp(first->definitions[3].regex, "-?[0-9]+(\\.[0-9]*)?") == 0);

	LexicVocabularyFree(first);
	first = LexicVocabularyFromString("THING: 00\n\n ITEM: 11");
	ReportResult("String1", first->def_count == 2);
	ReportResult("String2", strcmp(first->definitions[0].name, "THING") == 0);
	ReportResult("String3", strcmp(first->definitions[1].regex, "11") == 0);

#ifdef VERBOSE
	printf("[!] Starting Vocabulary Make Tests\n");
#endif
}

int main(void) {
	//warn_level = LWT_DEBUG;
	VocabularyMakeTests();
	return 0;
}
