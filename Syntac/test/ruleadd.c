#define TEST_NAME "RuleAdd"
#include "syn_helper.h"

bool RuleAddTest() {
	int valid = 0;
	
#ifdef VERBOSE
	printf("[!] Syntac Rule Add Test Start \n");
#endif

	SyntacBook *book = SyntacBookAllocate();

	//Base Cases
	enum hlt_warn_type tmp = warn_level;
	if (warn_level != HLT_DEBUG) warn_level = HLT_SILENT; //silence other than debug mode
	SyntacBookRuleAdd(book, "", ""); //these will warn, hence the silence
	SyntacBookRuleAdd(book, "", "item");
	SyntacBookRuleAdd(book, "item", "");
	if (!TEST(book->rule_count, 0)) return false; //if any of these added 1, this is fatal
	warn_level = tmp; //reset

	valid += (int)TEST_NULL(book->rules);
	
	SyntacBookRuleAdd(book, "left", "right");
	valid += (int)TEST(book->rule_count, 1);
	valid += (int)TEST_STRING(book->rules[0].name, "left");
	valid += (int)TEST_STRING(book->rules[0].elements[0], "right");
	valid += (int)TEST_NULL(book->rules[0].elements[1]);

	SyntacBookRuleAdd(book, "thing", "left:right");
	valid += (int)TEST(book->rule_count, 2);
	valid += (int)TEST_STRING(book->rules[1].name, "thing");
	valid += (int)TEST_NULL(book->rules[1].elements[2]);
	valid += (int)TEST_STRING(book->rules[1].elements[0], "left");
	valid += (int)TEST_STRING(book->rules[1].elements[1], "right");
	
	SyntacBookRuleAdd(book, "thing2", "thing:left:right:thing:item:ahaha");
	valid += (int)TEST(book->rule_count, 3);
	valid += (int)TEST_STRING(book->rules[2].name, "thing2");
	valid += (int)TEST_NULL(book->rules[2].elements[6]); //there should be 6 elms
	valid += (int)TEST_STRING(book->rules[2].elements[3], "thing");
	valid += (int)TEST_STRING(book->rules[2].elements[5], "ahaha");
	
	SyntacBookFree(book);

#ifdef VERBOSE
	printf("[!] Syntac Rule Add Test Finished\n");
#endif
	return valid == (test_count-1);
}

#ifndef ALL_TESTS
int test_count = 0;
int main(void) {
	//warn_level = HLT_DEBUG;
	return !RuleAddTest();
}
#endif
