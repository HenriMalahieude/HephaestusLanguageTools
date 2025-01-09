#define TEST_NAME "First Sets"

#include "syn_helper.h"

bool FirstTest() {
	test_count = 0;
	int valid = 0;

	print_test("Start!");
	
	//Step 0: Test trivial/edge-cases
	/*SyntacBook *book1 = SyntacBookAllocate();
	SyntacBookRuleAdd(book1, "A", "B");
	SyntacBookRuleAdd(book1, "A", "C");
	SyntacBookRuleAdd(book1, "C", "");
	if (book1->rule_count != 3) {
		print_test("SyntacBookRuleAdd failed to add 3 rules as requested?");
		return false;
	}

	firsts_of_book(book1);
	valid += TEST(SetCount(book1->rules[0].first_set), 1); //r1 A
	valid += TEST_STRING(book1->rules[0].first_set[0], "B");
	valid += TEST(book1->rules[0].first_set[0] != book1->rules[0].elements[0], 1);

	valid += TEST(SetCount(book1->rules[1].first_set), 1); //r2 A
	valid += TEST(book1->rules[1].first_set[0][0] == EPSILON[0], 1);
	valid += TEST(book1->rules[1].first_set[0][1] == '\0', 1);

	valid += TEST(SetCount(book1->rules[2].first_set), 1); //r3 C
	valid += TEST(book1->rules[2].first_set[0][0] == EPSILON[0], 1);
	valid += TEST(book1->rules[2].first_set[0][1] == '\0', 1);

	SyntacBookFree(book1); book1 = NULL;
	if (valid != test_count) {
		print_test("Failed trivial/edge cases");
		return false;
	} // */

	//Step 1: Test a simple book (from internet)
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
		print_test("SyntacBookRuleAdd failed to add 8 rules as requested?");
		return false;
	}

	firsts_of_book(book0);
	valid += TEST(SetCount(book0->rules[0].first_set), 2); //r1 E
	valid += TEST(SetContains(book0->rules[0].first_set, "("), 1);
	valid += TEST(SetContains(book0->rules[0].first_set, "id"), 1);
	//SetPrint(book0->rules[0].first_set);

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

	//Step 2: Test File Grammar
	/*SyntacBook *book = SyntacBookFromFile("../grammar.stc");
	valid += TEST(book->rule_count, 9);

	firsts_of_book(book);
	
	valid += TEST(SetCount(book->rules[0].first_set), 1); //r1 func
	valid += TEST_STRING(book->rules[0].first_set[0], "TYPE");

	valid += TEST(SetCount(book->rules[1].first_set), 1); //r2 args
	valid += TEST_STRING(book->rules[1].first_set[0], "TYPE");

	valid += TEST(SetCount(book->rules[2].first_set), 1); //r3 args
	//should be epsilon... don't really know how to test it properly
	
	valid += TEST(SetCount(book->rules[4].first_set), 2);
	valid += TEST(SetContains(book->rules[4].first_set, "WORD"), 1);
	//other one is epsilon

	valid += TEST(SetCount(book->rules[8].first_set), 1);

	SyntacBookFree(book); book = NULL;
	if (valid != test_count) {
		print_test("Failed file case");
		return false;
	} // */

	//Step 3: Complicated File with many first tokens and some Epsilon tokens
	/*SyntacBook *book3 = SyntacBookFromFile("../grammar2.stc");
	valid += TEST(book3->rule_count, 10);

	firsts_of_book(book3);
	for (int i = 0; i < book3->rule_count; i++) {
		printf("Rule %s: ", book3->rules[i].name);
		SetPrint(book3->rules[i].elements);
		printf("  First Set: ");
		SetPrint(book3->rules[i].first_set);
	}

	char **rule0 = SetCreate(4, "d", "g", "h", EPSILON);
	valid += TEST(SetCount(book3->rules[0].first_set), 4);
	valid += TEST(SetContains(book3->rules[0].first_set, "d"), 1);
	valid += TEST(SetContains(book3->rules[0].first_set, "g"), 1);
	valid += TEST(SetContains(book3->rules[0].first_set, "h"), 1);
	valid += TEST(SetEquality(book3->rules[0].first_set, rule0), 1);

	char **rule1 = SetCreate(2, "h", "b");
	valid += TEST(SetCount(book3->rules[1].first_set), 2);
	valid += TEST(SetContains(book3->rules[1].first_set, "h"), 1);
	valid += TEST(SetContains(book3->rules[1].first_set, "b"), 1);
	valid += TEST(SetEquality(book3->rules[1].first_set, rule1), 1);
	
	char **rule2 = SetCreate(2, "g", "a");
	valid += TEST(SetCount(book3->rules[2].first_set), 2);
	valid += TEST(SetContains(book3->rules[2].first_set, "g"), 1);
	valid += TEST(SetContains(book3->rules[2].first_set, "a"), 1);
	valid += TEST(SetEquality(book3->rules[2].first_set, rule2), 1);

	char **rule3 = SetCreate(1, "d");
	valid += TEST(SetEquality(book3->rules[3].first_set, rule3), 1);

	char **rule4 = SetCreate(3, "g", "h", EPSILON);
	valid += TEST(SetEquality(book3->rules[4].first_set, rule4), 1);

	valid += TEST(SetCount(book3->rules[5].first_set), 1);

	valid += TEST(SetCount(book3->rules[6].first_set), 1);
	
	valid += TEST(SetCount(book3->rules[7].first_set), 1);

	valid += TEST(SetCount(book3->rules[8].first_set), 1);

	char **rule9 = SetCreate(6, "d", "g", "h", "b", "a", EPSILON);
	valid += TEST(SetEquality(book3->rules[9].first_set, rule9), 1); // */

	print_test("Finished!");

	return valid == test_count;
}

#ifndef ALL_TESTS
int test_count = 0;
int main(void) {
	warn_level = HLT_DEBUG;
	return !FirstTest();
}
#endif
