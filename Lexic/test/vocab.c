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
	printf("[!] Starting Vocabulary Make Tests");
#endif

	struct lexic_vocabulary *first = Lexic_Vocabulary_Allocate(); ReportResult("Allocation", first != NULL);
	
	bool succ = Lexic_Vocabulary_Add_Definition(first, "digit", "[0-9]"); 
	ReportResult("Add Def0", succ);
	ReportResult("Add Def1", first->def_count == 1);
	ReportResult("Add Def2", strcmp(first->definitions[0].name, "digit") == 0);
	ReportResult("Add Def3", strcmp(first->definitions[0].regex, "[0-9]") == 0);

	succ = Lexic_Vocabulary_Add_Definition(first, "abc", "abc");
	ReportResult("Add Def4", succ);
	ReportResult("Add Def5", first->def_count == 2);

	Lexic_Vocabulary_Free(first);
	
	//Note that this will be built in _build folder/directory
	first = Lexic_Vocabulary_Make_From_File("../example.vocab");
	ReportResult("File1", first->def_count == 5);
	ReportResult("File2", strcmp(first->definitions[0].name, "TYPE") == 0);
	ReportResult("File3", strcmp(first->definitions[3].name, "NUMBER") == 0);
	ReportResult("File4", strcmp(first->definitions[4].name, "SEMICOLON") == 0);
	ReportResult("File5", strcmp(first->definitions[4].regex, ";") == 0);
	ReportResult("File6", strcmp(first->definitions[2].regex, "=") == 0);
	ReportResult("File7", strcmp(first->definitions[3].regex, "-?[0-9]+(\\.[0-9]+)?") == 0);

	Lexic_Vocabulary_Free(first);
	first = Lexic_Vocabulary_Make_From_String("THING: 00\n ITEM: 11");
	ReportResult("String1", first->def_count == 2);
	ReportResult("String2", strcmp(first->definitions[0].name, "THING") == 0);
	ReportResult("String3", strcmp(first->definitions[1].regex, "11") == 0);

#ifdef VERBOSE
	printf("[!] Starting Vocabulary Make Tests");
#endif
}

int main(void) {
	VocabularyMakeTests();
	return 0;
}
