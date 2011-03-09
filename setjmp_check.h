#include <setjmp.h>

extern int setjmp(jmp_buf env) __attribute__((user("setjmp")));
extern int sigsetjmp(jmp_buf env, int savesigs) __attribute__((user("setjmp")));
