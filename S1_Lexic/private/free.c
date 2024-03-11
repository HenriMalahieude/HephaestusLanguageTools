#include <stdlib.h>
#include "regex.h"

void Regex_Free(struct regex reg) {
	switch (reg.type) {
		case RT_DIRECT:
			break;
		case RT_OR:
			Regex_Free(((struct regex *)reg.attached_data)[0]);
			Regex_Free(((struct regex *)reg.attached_data)[1]);
			break;
		case RT_GROUP: //fall through to next
		case RT_BRACKETS:
			int len = ((unsigned long long *)reg.attached_data)[0];
			struct regex *arr = (struct regex *)((unsigned long long *)reg.attached_data)[1];
			for (int i = 0; i < len; i++) {
				Regex_Free(arr[i]);
			}
			free(arr);
			break;
		case RT_SEQUENCE:
			break;
		case RT_QUALIFIER:
			Regex_Free(*(struct regex *)(((unsigned long long *)reg.attached_data)[1])); //workin with c, amirite bois?
			free((struct regex *)(((unsigned long long *)reg.attached_data)[1]));
			break;
		case RT_ESCAPED:
			break;
		default:
			Regex_Warning("Attempting to free an undefined regex!");
			break;
	}

	free(reg.attached_data);
}
