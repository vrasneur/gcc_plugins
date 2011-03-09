#ifndef GPP_UTILS_H
#define GPP_UTILS_H

#include <stdbool.h>

bool is_gpp(void);

void lang_check_failed (const char* file, int line, const char* function);

#endif // GPP_UTILS_H
