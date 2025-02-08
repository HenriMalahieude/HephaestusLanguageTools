#define TEST_NAME "Follow Sets"

#include "syn_helper.h"

bool FollowTest() {
	test_count = 0;
	int valid = 0;

	print_test("Start!");

	//Step 1: Trivial
	SyntacBook *book1 = SyntacBookAllocate();
	SyntacBookRuleAdd(book1, "A", "B");
	SyntacBookRuleAdd(book1, "A", "CB");
	SyntacBookRuleAdd(book1, "C", "");
	if (book1->rule_count != 3) {
		print_test("SyntacBookRuleAdd failed to add 3 rules as requested?");
		return false;
	}

	firsts_of_book(book1);
	follow_of_book(book1);

	char **set_start = SetCreate(1, ENDMRKR);
	valid += TEST(SetEquality(set_start, book1->rule[0].follow_set), 1);
	valid += TEST(SetEquality(set_start, book1->rule[1].follow_set), 1);
	
	char **set_c_t1 = SetCreate(1, "B");
	valid += TEST(SetEquality(set_start, book1->rule[2].follow_set), 1);
	SetFree(set_c_t1); set_c_t1 = NULL;

	if (valid != test_count) {
		print_test("Failed Trivial Case");
		return false;
	}

	//Step 2: Test a simple book (from internet)
	SyntacBook *book2 = SyntacBookAllocate();

	SyntacBookRuleAdd(book2, "E", "T:E'");
	SyntacBookRuleAdd(book2, "E'", "+:T:E'");
	SyntacBookRuleAdd(book2, "E'", "");
	SyntacBookRuleAdd(book2, "T", "F:T'");
	SyntacBookRuleAdd(book2, "T'", "*:F:T'");
	SyntacBookRuleAdd(book2, "T'", "");
	SyntacBookRuleAdd(book2, "F", "(:E:)");
	SyntacBookRuleAdd(book2, "F", "id");
	if (book2->rule_count != 8) {
		print_test("SyntacBookRuleAdd failed to add 8 rules as requested?");
		return false;
	}

	firsts_of_book(book2);
	follow_of_book(book2);

	char **set_e_t2 = SetCreate(1, ")");
	valid += TEST(SetEquality(set_e_t2, book2->rules[0].follow_set), 1);
	SetFree(set_e_t2); set_e_t2 = NULL;

	printf("TODO: Start symbol setting!\n");

	char **set_ep_t2 = SetCreate(2, ")", ENDMRKR);
	valid += TEST(SetEquality(set_ep_t2, book2->rules[1].follow_set), 1);
	valid += TEST(SetEquality(set_ep_t2, book2->rules[2].follow_set), 1);
	SetFree(set_ep_t2); set_ep_t2 = NULL;

	char **set_t_t2 = SetCreate(3, "+", ")", ENDMRKR);
	valid += TEST(SetEquality(set_t_t2, book2->rules[3].follow_set), 1);
	
	valid += TEST(SetEquality(set_t_t2, book2->rules[4].follow_set), 1);
	valid += TEST(SetEquality(set_t_t2, book2->rules[5].follow_set), 1);
	SetFree(set_t_t2); set_t_t2 = NULL;

	char **set_f_t2 = SetCreate(4, "*", "+", ")", ENDMRKR);
	valid += TEST(SetEquality(set_f_t2, book2->rules[6].follow_set), 1);
	valid += TEST(SetEquality(set_f_t3, book2->rules[7].follow_set), 1);
	SetFree(set_f_t2); set_f_t2 = NULL;

	if (valid != test_count) {
		print_test("Failed Simple Case");
		return false;
	}

	SetFree(set_start); set_start = NULL;
	return valid != test_count;
}

#ifndef ALL_TESTS
int test_count = 0;
int main(void) {
	return !FollowTest();
}
#endif
