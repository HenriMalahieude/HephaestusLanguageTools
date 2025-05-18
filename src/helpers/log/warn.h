#ifndef __HEPH_INTERNAL_WARN__
#define __HEPH_INTERNAL_WARN__
#include <stdio.h>
#include <stdarg.h>

#define HLT_ERR_SYMB "[X] "
#define HLT_WRN_SYMB "[W] "
#define HLT_OUTFILE fprintf(stderr, "%s", __FILE__)
#define HLT_OUTFUNC fprintf(stderr, "%s", __func__)

//Internal: Log All
#define HLT_LOGLOC \
	fprintf(stderr, "["); \
	HLT_OUTFILE; \
	fprintf(stderr, ":"); \
	HLT_OUTFUNC; \
	printf("]")

//Warn: All
#define HLT_AWRN(type, format, ...) {\
	if ((int)type <= (warn_level)) { \
		fprintf(stderr, HLT_WRN_SYMB); \
		HLT_LOGLOC; \
		fprintf(stderr, "> "); } \
	HLTWarn(-1, -1, type, format __VA_OPT__(,) __VA_ARGS__); }

//Warn: Function
#define HLT_WRNLC(line, col, type, format, ...) {\
	if ((int)type <= (int)warn_level){ \
		fprintf(stderr, HLT_WRN_SYMB); \
		HLT_OUTFUNC; \
		fprintf(stderr, "> ");} \
	HLTWarn(line, col, type, format __VA_OPT__(,) __VA_ARGS__);}

#define HLT_WRN(type, format, ...) HLT_WRNLC(-1, -1, type, format __VA_OPT__(,) __VA_ARGS__)

//Error: All
#define HLT_AERR(format, ...) {\
	fprintf(stderr, HLT_ERR_SYMB); \
	HLT_LOGLOC; \
	fprintf(stderr, "> "); \
	HLTError(-1, -1, format __VA_OPT__(,) __VA_ARGS__);}

//Error: Function
#define HLT_ERRLC(line, col, format, ...) {\
	fprintf(stderr, HLT_ERR_SYMB); \
	HLT_OUTFUNC; \
	fprintf(stderr, "> "); \
	HLTError(line, col, format __VA_OPT__(,) __VA_ARGS__);}

#define HLT_ERR(format, ...) HLT_ERRLC(-1, -1, format __VA_OPT__(,) __VA_ARGS__)

enum hlt_warn_type {
	HLT_SILENT = 0,
	HLT_MJRWRN,
	HLT_STDWRN,
	HLT_VERBSE,
	HLT_DEBUG //should only be needed by developers
};

extern enum hlt_warn_type warn_level; //warnings lower prio or greater value (x > (int)warn_level) ignored
void HLTError(int line, int col, const char *format, ...);
void HLTWarn(int line, int col, enum hlt_warn_type type, const char *format, ...);

#endif
