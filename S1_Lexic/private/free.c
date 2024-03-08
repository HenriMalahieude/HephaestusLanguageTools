#include "regex.h"

void Regex_Free(struct regex *reg) {
	switch (reg->type) {
		case RT_DIRECT:
			break;
		case RT_OR:
			Regex_Free(((struct regex **)reg->attached_data)[0]);
			Regex_Free(((struct regex **)reg->attached_data)[1]);
			break;
		case RT_BRACKETS:
			for(struct regex *c = reg->attached_data; *c != NULL; c++) {
				Regex_Free(c);
			}
			break;
		case RT_SEQUENCE:
			break;
		case RT_QUALIFIER:
			free((char*)((long long int *)reg->attached_data)[0]); //qualifier char
			Regex_Free((struct regex *)((long long int *)reg->attach_data)[1]);
			break;
		case RT_ESCAPED:
			break;
		case RT_GROUP:
			int *total = (int *)((long long int *)reg->attached_data)[0];
			struct regex **group = (struct regex **)((long long int *)reg->attached_data)[1];
			for (int i = 0; i < *total; i++) {
				Regex_Free(group[i]);
			}
			free(total);
			free(group);
			break;
		default:
			Regex_Warning("Attempting to free an undefined regex!");
			break;
	}

	free(reg->attached_data);
}
