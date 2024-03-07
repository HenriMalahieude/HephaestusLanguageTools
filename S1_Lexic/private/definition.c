#include <string.h>
#include "regex.h"

//TODO: Groups like (\.[0-9]+)?
//TODO: Refactor Or Regexes to be more global

void add_to_regex_sequence(struct regex ***sequence_ptr, int *count, struct regex *reg) {
	*count += 1;
	*sequence_ptr = realloc(*sequence_ptr, *count * sizeof(struct regex **));
	if (*sequence_ptr == NULL) Regex_Error("Got realloc error in definition creation");

	(*sequence_ptr)[*count-1] = reg;
}

void create_or_regex_and_add_to_sequence(struct regex ***sequence_ptr, int *count, struct regex *reg) {
	struct regex *last = (*sequence_ptr)[*count-1];
	struct regex *new_or = Regex_New_Or(last, reg); //NOTE: y'know, we could change the ordering to hopefully avoid a bunch of func calls? meh...?
	
	(*sequence_ptr)[*count-1] = reg;
}

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

	char *debug_copy = calloc((end_pos-consume_pos)+1, sizeof(char)); strncopy(debug_copy, definition + consume_pos, (end_pos-consume_pos));
	printf("\tRegex Debug: trimmed definition is '%s'\n", debug_copy);

	enum regex_type mode = RT_DIRECT;//default interpretation
	int or_level = 0;
	char substr[100]; //If things really get long, this can be changed
	for (size_t i = consume_pos; i <= end_pos; i++) {//NOTE: end_pos is an index, not length.
		//detect empty space
		formatted[0] = definition[i];
		if (ss->match_function(formatted, ss->attached_data)) Regex_Warning("Provided regex definition has empty/whitespace within definition. Consider using '\\s' instead.");

		char cur = definition[i];
		if (mode == RT_DIRECT) {
			if (cur == ']') Regex_Error("Mismatched right bracket. Did you mean '\\]'?");

			if (cur == '\\') {
				mode = RT_ESCAPED;
			} else if (cur == '?' || cur == '*' || cur == '+') {
				mode = RT_QUALIFIER;
			} else if (cur == '[') {
				mode = RT_BRACKETS;
			} else if (cur == '|') {
				or_level++;
			} else if (i == end_pos){
				mode = RT_UNDEFINED; //reached end of the line
			}

			if (consume_pos >= (i-1)) { //we consumed everything before it
				if (mode == RT_QUALIFIER && *count == 0) Regex_Error("Qualifier must have preceding regex");
				if (mode == RT_OR && *count == 0) Regex_Error("Or statements must have preceding regex");
			} else if (mode != RT_DIRECT) {
				int use = (mode == RT_UNDEFINED) ? i : i-1;
				strncopy(substr, definition+consume_pos, use-consume_pos);
				substr[use-consume_pos+1] = '\0';

				struct regex *direct_reg = Regex_New_Direct(substr);
				if (or_level <= 0) {
					add_to_regex_sequence(&sequence, count, direct_reg);
				} else {
					create_or_regex_and_add_to_sequence(&sequence, count, direct_reg);
					or_level--;
				}

				consume_pos = i+1;
			}
		} else if (mode == RT_ESCAPED) {
			substr[0] = '\\';
			substr[1] = definition[i];
			substr[2] = '\0';
			if (sequence[*count-1]->type != RT_OR) {
				add_to_regex_sequence(&sequence, count, Regex_New_Escaped(substr));
			} else {
				create_or_regex_and_add_to_sequence(&sequence, count, Regex_New_Escaped(substr));
				or_level--;
			}

			mode = RT_DIRECT;
			consume_pos = i+1;
		} else if (mode == RT_QUALIFIER) {
			struct regex *last = sequence[*count-1];
			if (last->type == RT_QUALIFIER) Regex_Error("Cannot have a qualifier after a qualifer.");

			char qualifier = definition[i-1];
			if (or_level <= 0) {
				struct regex *qualified = Regex_New_Qualifier(last, qualifier);
				sequence[*count-1] = qualified;
			} else {
				if (last->type != RT_OR) Regex_Error("How did we have an or level > 0 but no or?");

				struct regex *last_or_item = ((struct regex **)last->attached_data)[1];
				if (last_or_item->type == RT_QUALIFIER) Regex_Error("Cannot have a qualifier after a qualifier.");

				struct regex *qualified = Regex_New_Qualifier(last_or_item, qualifier);
				((struct regex **)last->attached_data)[1] = qualified;

				or_level--;
			}
			
			mode = RT_DIRECT;
			consume_pos = i;
			i--;//we haven't yet looked at this character, so go back!
		} else if (mode == RT_BRACKETS) {
			if (definition[i] == ']' && definition[i-1] != '\\') {
				strncopy(substr, definition+consume_pos, (i-1)-consume_pos);
				substr[(i-1)-consume_pos+1] = '\0';
				if (or_level <= 0) {
					add_to_regex_sequence(&sequence, count, Regex_New_Brackets(substr));
				} else {
					create_or_regex_and_add_to_sequence(&sequence, count, Regex_New_Brackets(substr));
					or_level--;
				}

				consume_pos = i+1;
			} else if (i == end_pos) {
				Regex_Error("Mismatched right bracket. Reached end of definition without left bracket ']'");
			}
		}
	}

	return sequence;
}
