#include <stdlib.h>
#include <stdio.h>

#include <gcc-plugin.h>
#include <tree.h>

#include "gpp_utils.h"

tree global_namespace = NULL_TREE;

bool is_gpp(void)
{
   return global_namespace != NULL_TREE;
}

// not defined in cc1, will be override by the internal cc1plus function
void lang_check_failed (const char* file, int line, const char* function)
{
    (void)file;
    (void)line;
    (void)function;

    abort();
}
