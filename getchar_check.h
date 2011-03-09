#include <stdio.h>

extern int fgetc(FILE *stream) __attribute__((user("getchar")));
extern int getc(FILE *stream) __attribute__((user("getchar")));
extern int getchar(void) __attribute__((user("getchar")));
