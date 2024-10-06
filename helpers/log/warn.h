#ifndef __HEPH_INTERNAL_WARN__
#define __HEPH_INTERNAL_WARN__

#define HLT_ERR_SYMB "[!]"
#define HLT_WRN_SYMB "[?]"
#define HLT_FILE_PREPEND printf("%s", __FILE__)
#define HLT_FUNC_PREPEND printf("%s", __func__)

//All Report
#define HLT_RALL \
	printf(HLT_ERR_SYMB); \
	HLT_FILE_PREPEND; \
	printf("("); \
	HLT_FUNC_PREPEND; \
	printf("): ")

//Function Report
#define HLT_RFNC \
	printf(HLT_ERR_SYMB); \
	HLT_FUNC_PREPEND; \
	printf(": ")

//File Report
#define HLT_RFIL \
	printf(HLT_ERR_SYMB); \
	HLT_FILE_PREPEND; \
	printf(": ")

//Warn: All
#define HLT_AWRN(msg, type) \
	HLT_RALL; \
	HLTarn(msg, -1, -1, type)

//Warn: Function
#define HLT_UWRN(msg, type) \
	HLT_RFNC; \
	HLTarn(msg, -1, -1, type)

//Error: All
#define HLT_AERR(msg) \
	HLT_RALL; \
	HLTError(msg, -1, -1)

//Error: Function
#define HLT_UERR(msg) \
	HLT_RFNC; \
	HLTError(msg, -1, -1)

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
