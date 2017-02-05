#ifndef INFO_HEADER
#define INFO_HEADER

#ifdef INFO
	#define info(fmt, ...) do{fprintf(stderr, "INFO: " fmt, ##__VA_ARGS__);}while(0)
#else
	#define info(fmt, ...)
#endif

#endif