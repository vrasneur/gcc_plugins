#ifndef GCC_UTILS_H
#define GCC_UTILS_H

#include <stdbool.h>

#include <gcc-plugin.h>
#include <tree.h>
#include <toplev.h>

#define WARNING_DECL(decl, fmt, ...)                                    \
    do                                                                  \
    {                                                                   \
        location_t loc = EXPR_HAS_LOCATION(decl) ? EXPR_LOCATION(decl) : input_location; \
        warning_at(loc, 0, fmt, ## __VA_ARGS__);                        \
    } while(0);

bool has_fndecl_user_attr(tree fndecl, char const * const value);

tree get_function_decl_from_call(tree decl);

bool is_function_call(tree decl, char const * const name);

tree get_function_assign_rhs(tree decl, char const * const name);

tree get_function_assign_lhs(tree decl, char const * const name);

tree get_function_assign_call(tree decl, char const * const name);

#endif // GCC_UTILS_H
