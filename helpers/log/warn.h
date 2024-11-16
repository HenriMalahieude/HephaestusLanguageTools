#ifndef __HEPH_INTERNAL_WARN__
#define __HEPH_INTERNAL_WARN__

#define HLT_ERR_SYMB "[!] "
#define HLT_WRN_SYMB "[?] "
#define HLT_OUTFILE printf("%s", __FILE__)
#define HLT_OUTFUNC printf("%s", __func__)

//Internal: Log All
#define HLT_LOGLOC \
	HLT_OUTFILE; \
	printf(" ("); \
	HLT_OUTFUNC; \
	printf(") > ")

//Warn: All
#define HLT_AWRN(msg, type) {\
	printf(HLT_WRN_SYMB); \
	HLT_LOGLOC; \
	HLTWarn(msg, -1, -1, type);}

//Warn: Function
#define HLT_WRNLC(msg, line, col, type) {\
	printf(HLT_WRN_SYMB); \
	HLT_OUTFUNC; \
	printf(" > "); \
	HLTWarn(msg, line, col, type);}

#define HLT_WRN(msg, type) HLT_WRNLC(msg, -1, -1, type)

//Error: All
#define HLT_AERR(msg) {\
	printf(HLT_ERR_SYMB); \
	HLT_LOGLOC; \
	HLTError(msg, -1, -1);}

//Error: Function
#define HLT_ERRLC(msg, line, col) {\
	printf(HLT_ERR_SYMB); \
	HLT_OUTFUNC; \
	printf(" > "); \
	HLTError(msg, line, col);}

#define HLT_ERR(msg) HLT_ERRLC(msg, -1, -1)

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
