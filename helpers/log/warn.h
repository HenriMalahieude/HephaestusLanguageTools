#ifndef __HEPH_INTERNAL_WARN__
#define __HEPH_INTERNAL_WARN__

enum hlt_warn_type {
	HLT_SILENT = 0,
	HLT_MJRWRN,
	HLT_STDWRN,
	HLT_VERBSE,
	HLT_DEBUG //should only be needed by developers
};

extern enum hlt_warn_type warn_level; //warnings lower prio or greater value (x > (int)warn_level) ignored
void HLTError(char *msg, int line, int col);
void HLTWarn(char *msg, int line, int col, enum hlt_warn_type type);

#endif
