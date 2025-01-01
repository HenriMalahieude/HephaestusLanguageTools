#define TEST_NAME "First Sets"

#include "syn_helper.h"

bool FirstTest() {
	test_count = 0;
	int valid = 0;

	print_test("Start!");

	//Step 0: Test trivial/edge-cases
	SyntacBook *book1 = SyntacBookAllocate();
	SyntacBookRuleAdd(book1, "A", "B");
	SyntacBookRuleAdd(book1, "A", "C");
	SyntacBookRuleAdd(book1, "C", NULL);
	if (book1->rule_count != 3) {
		print_test("SyntacBookRuleAdd failed to add 3 rules as requested?");
		return false;
	}

	firsts_of_book(book1);
	valid += TEST(SetCount(book1->rules[0].first_set), 1); //r1 A
	valid += TEST_STRING(book1->rules[0].first_set[0], "B");
	valid += TEST(book1->rules[0].first_set[0] != book1->rules[0].elements[0], 1);

	valid += TEST(SetCount(book1->rules[1].first_set), 1);
	valid += TEST(book1->rules[1].first_set[0][0] == EPSILON[0], 1);
	valid += TEST(book1->rules[1].first_set[0][1] == '\0', 1);

	valid += TEST(SetCount(book1->rules[2].first_set), 1);
	valid += TEST(book1->rules[2].first_set[0][0] == EPSILON[0], 1);
	valid += TEST(book1->rules[2].first_set[0][1] == '\0', 1);

	SyntacBookFree(book1); book1 = NULL;
	if (valid != test_count) {
		print_test("Failed trivial/edge cases");
		return false;
	}

	//Step 1: Test a simple book
	SyntacBook *book0 = SyntacBookAllocate();

	SyntacBookRuleAdd(book0, "E", "T:E'");
	SyntacBookRuleAdd(book0, "E'", "+:T:E'");
	SyntacBookRuleAdd(book0, "E'", "?");
	SyntacBookRuleAdd(book0, "T", "F:T'");
	SyntacBookRuleAdd(book0, "T'", "*:F:T'");
	SyntacBookRuleAdd(book0, "T'", "?");
	SyntacBookRuleAdd(book0, "F", "(:E:)");
	SyntacBookRuleAdd(book0, "F", "id");
	if (book0->rule_count != 8) {
		HLT_AWRN("SyntacBookRuleAdd failed to add 8 rules as requested?", HLT_MJRWRN);
		return false;
	}

	firsts_of_book(book0);
	valid += TEST(SetCount(book0->rules[0].first_set), 2); //r1 E
	valid += TEST(SetContains(book0->rules[0].first_set, "("), 1);
	valid += TEST(SetContains(book0->rules[0].first_set, "id"), 1);

	valid += TEST(SetCount(book0->rules[1].first_set), 1); //r2 E'
	valid += TEST_STRING(book0->rules[1].first_set[0], "+");

	valid += TEST(SetCount(book0->rules[2].first_set), 1); //r3 E'
	valid += TEST_STRING(book0->rules[2].first_set[0], "?");

	valid += TEST(SetCount(book0->rules[3].first_set), 2); //r4 T
	valid += TEST(SetContains(book0->rules[3].first_set, "("), 1);
	valid += TEST(SetContains(book0->rules[3].first_set, "id"), 1);
	valid += TEST_NULL(book0->rules[3].first_set[2]);

	valid += TEST(SetCount(book0->rules[4].first_set), 1); //r5 T'
	valid += TEST_STRING(book0->rules[4].first_set[0], "*");

	valid += TEST(SetCount(book0->rules[5].first_set), 1); //r6 T'
	valid += TEST_STRING(book0->rules[5].first_set[0], "?");

	valid += TEST(SetCount(book0->rules[6].first_set), 1); //r7 F
	valid += TEST_STRING(book0->rules[6].first_set[0], "(");
	
	valid += TEST(SetCount(book0->rules[7].first_set), 1); //r8 F
	valid += TEST_STRING(book0->rules[7].first_set[0], "id");

	SyntacBookFree(book0); book0 = NULL;
	if (valid != test_count) {
		print_test("Failed simple case");
		return false;
	}
	

	print_test("TODO");
	/*SyntacBook *book = SyntacBookFromFile("../grammar.stc");
	first_of_book(book); //Great news if it doesn't error out
	
	//Ensure no changed to the underlying rule structure
	//TODO
	
	//Check correctly generated first set of all of file
	//TODO
	
	// */

	print_test("Finished!");

	return valid == test_count;
}

#ifndef ALL_TESTS
int test_count = 0;
int main(void) {
	return !FirstTest();
}
#endif
