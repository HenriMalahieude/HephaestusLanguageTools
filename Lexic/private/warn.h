#ifndef __HEPH_INTERNAL_WARN__
#define __HEPH_INTERNAL_WARN__

enum lex_warn_type {
	LWT_SILENT = 0,
	LWT_MJRWRN,
	LWT_STDWRN,
	LWT_VERBSE,
	LWT_DEBUG //should only be needed by lexic developers
};

extern enum lex_warn_type warn_level; //warnings lower prio (x > (int)warn_level) ignored
void LexicError(char *msg);
void LexicWarn(char *msg, enum lex_warn_type type);

#endif
