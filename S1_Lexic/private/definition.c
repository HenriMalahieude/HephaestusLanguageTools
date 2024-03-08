#include <string.h>
#include "regex.h"

struct regex** Regex_New_Definition(char *definition, int *count) {
	struct regex **sequence = NULL;
	*count = 0;

	size_t consume_pos = 0, end_pos = strlen(definition)-1;
	struct regex *ss = Regex_New_Escaped("\\s");

	//find where to start, eliminate front white space
	char formatted[2] = {'\0', '\0'};
	for (size_t i = 0; i < strlen(definition); i++){
		formatted[0] = definition[i];
		if (ss->match_function(formatted, ss->attached_data)){
			consume_pos = i+1;
		} else {
			break;
		}
	}

	if (consume_pos >= end_pos) Regex_Error("Provided regex definition cannot be empty/whitespace. If explicit white space, use '\\s'.");
	
	//find where to end, eliminate back white space
	for (size_t i = end_pos; i >= consume_pos; i++) {
		formatted[0] = definition[i];
		if (ss->match_function(formatted, ss->attached_data)) {
			end_pos = i-1;
		} else {
			break;
		}
	}

	char *trimmed_internals = calloc((end_pos-consume_pos)+1, sizeof(char)); 
	strncopy(trimmed_internals, definition + consume_pos, (end_pos-consume_pos));

	printf("\tRegex Debug: trimmed definition is '%s'\n", trimmed_internals);
	
	struct regex *group = Regex_New_Group(trimmed_internals);
	if (group->type != RT_GROUP) {
		sequence = malloc(sizeof(struct regex *));
		sequence[0] = group;
		*count = 1;
	} else {
		*count = *(int*)((long long int *)group->attached_data)[0];
		sequence = (struct regex **)((long long int *)group->attached_data)[1];
		
		//Precautions
		group->type = RT_UNDEFINED;
		group->match_function = NULL;

		free(group->attached_data);
		group->attached_data = NULL; //more precautions

		free(group);
		group = NULL:
	}

	return sequence;
}
