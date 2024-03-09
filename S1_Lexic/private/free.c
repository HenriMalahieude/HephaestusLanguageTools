#include "regex.h"

void Regex_Free(struct regex reg) {
	switch (reg.type) {
		case RT_DIRECT:
			break;
		case RT_OR:
			Regex_Free(((struct regex *)reg->attached_data)[0]);
			Regex_Free(((struct regex *)reg->attached_data)[1]);
			break;
		case RT_BRACKETS:
			int len = ((unsigned long long *)reg.attached_data)[0];
			struct regex *arr = ((unsigned long long *)reg.attached_data)[1];
			for (int i = 0; i < len; i++) {
				Regex_Free(arr[i]);
			}
			break;
		case RT_SEQUENCE:
			break;
		case RT_QUALIFIER:
			Regex_Free(((unsigned long long *)reg->attach_data)[1]);
			break;
		case RT_ESCAPED:
			break;
		case RT_GROUP:
			int total = ((unsigned long long *)reg->attached_data)[0];
			struct regex *group = ((unsigned long long *)reg->attached_data)[1];
			for (int i = 0; i < total; i++) {
				Regex_Free(group[i]);
			}
			free(group);
			break;
		default:
			Regex_Warning("Attempting to free an undefined regex!");
			break;
	}

	free(reg->attached_data);
}
