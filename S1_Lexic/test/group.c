#include <stdlib.h>

#define TEST_NAME "Group"
#include "regex_helper.h"

void GroupTest(){
#ifdef VERBOSE
	printf("[!] Regex Group Test Start\n");
#endif
	if (false) { //debug purposes
		struct regex manual_construct;
		manual_construct.type = RT_GROUP;

		struct regex *sequence = malloc(2*sizeof(struct regex));
		sequence[0] = Regex_New_Escaped("\\.");
		sequence[1] = Regex_New_Qualifier(Regex_New_Brackets("0-9"), '+');

		manual_construct.attached_data = malloc(2*sizeof(unsigned long long));
		((unsigned long long *)manual_construct.attached_data)[0] = 2;
		((unsigned long long *)manual_construct.attached_data)[1] = (unsigned long long)sequence;

#ifdef VERBOSE
		printf("[.] Initialized group through manual means\n");
#endif

		TEST_REGEX(manual_construct, ".0", true);
		TEST_REGEX(manual_construct, ".00000012381239487", true);
		TEST_REGEX(manual_construct, "09234", false);
		TEST_REGEX(manual_construct, ".12309a098", false);
		TEST_REGEX(manual_construct, "", false);
		TEST_REGEX(manual_construct, ".01a", false);
	}

	if (false) {
		struct regex built0 = Regex_New_Group("char|int|double");
		if (built0.type != RT_OR) printf("[X] Did not simplify group into or!"), exit(1);
		TEST_REGEX(built0, "char", true);
		TEST_REGEX(built0, "int", true);
		TEST_REGEX(built0, "double", true);

		TEST_REGEX(built0, "ch", false);
		TEST_REGEX(built0, "ar", false);	
		TEST_REGEX(built0, "dou", false);
		TEST_REGEX(built0, "ble", false);
		TEST_REGEX(built0, "in", false);
	}

	if (true) {
		struct regex built = Regex_New_Group("-?[0-9]+(\\.[0-9]+)?");
		if (((unsigned long long *)built.attached_data)[0] != 3) printf("[X] Wrong amount of members within group!!\n"), exit(1);

#ifdef VERBOSE
		printf("[.] Initialized group through automatic means\n");
#endif

		//TEST_REGEX(built, "9", true);
		//TEST_REGEX(built, "-0", true);
		TEST_REGEX(built, "09.123", true);
		//TEST_REGEX(built, "-10.005", true);
		//TEST_REGEX(built, "-1ab", false);
		//TEST_REGEX(built, "asdf", false);
		//TEST_REGEX(built, "--1-1", false);
	}

#ifdef VERBOSE
	printf("[!] Regex Group Test Finished\n");
#endif
}

#ifndef ALL_TESTS
int regex_line_no = 0;
int regex_colu_no = 0;
int test_count = 1;
int main(void) {
	GroupTest();
	return 0;
}
#endif
