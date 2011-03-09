#include "gcc_utils.h"

#include <stdarg.h>

bool has_fndecl_user_attr(tree fndecl, char const * const value)
{
    tree attrlist = DECL_ATTRIBUTES(fndecl);

    if(attrlist != NULL_TREE && value != NULL)
    {
        tree attr = NULL;

        for (attr = lookup_attribute("user", attrlist);
             attr != NULL_TREE;
             attr = lookup_attribute("user", TREE_CHAIN(attr)))
        {
            const char *attrarg = TREE_STRING_POINTER(TREE_VALUE(TREE_VALUE(attr)));

            if(attrarg != NULL && strcmp(attrarg, value) == 0)
            {
                return true;
            }
        }
    }

    return false;
}

#define SKIP_CONVERT_EXPRS(decl)                \
    while(decl != NULL &&                       \
          CONVERT_EXPR_P(decl))                 \
    {                                           \
        decl = TREE_OPERAND(decl, 0);           \
    };

tree get_function_decl_from_call(tree decl)
{
    tree ret = NULL_TREE;
    tree call_fn = CALL_EXPR_FN(decl);

    if(TREE_CODE(call_fn) == ADDR_EXPR)
    {
        tree fndecl = TREE_OPERAND(call_fn, 0);

        if(TREE_CODE(fndecl) == FUNCTION_DECL)
        {
            ret = fndecl;
        }
    }

    return ret;
}

bool is_function_call(tree decl, char const * const name)
{
    bool ret = false;

    SKIP_CONVERT_EXPRS(decl);

    if(TREE_CODE(decl) == CALL_EXPR)
    {
        tree fndecl = get_function_decl_from_call(decl);

        if(fndecl != NULL_TREE)
        {
            if(name != NULL)
            {
                ret = has_fndecl_user_attr(fndecl, name);
            }
            else
            {
                ret = true;
            }
        }
    }

    return ret;
}

tree get_function_assign_rhs(tree decl, char const * const name)
{
    tree ret = NULL_TREE;
    tree rhs = NULL_TREE;

    if(decl != NULL)
    {
        enum tree_code code = TREE_CODE(decl);

        switch(code)
        {
        case MODIFY_EXPR:
        case INIT_EXPR:
            rhs = TREE_OPERAND(decl, 1);
            break;
        case VAR_DECL:
            rhs = DECL_INITIAL(decl);
            break;
        default:
            break;
        }

        if(rhs != NULL_TREE &&
           is_function_call(rhs, name))
        {
            ret = rhs;
        }
    }

    return ret;
}

tree get_function_assign_lhs(tree decl, char const * const name)
{
    tree ret = NULL_TREE;

    if(decl != NULL_TREE &&
       get_function_assign_rhs(decl, name) != NULL_TREE)
    {
        enum tree_code code = TREE_CODE(decl);

        switch(code)
        {
        case MODIFY_EXPR:
        case INIT_EXPR:
            ret = TREE_OPERAND(decl, 0);
            break;
        case VAR_DECL:
            ret = decl;
            break;
        default:
            break;
        }
    }

    return ret;
}

tree get_function_assign_call(tree decl, char const * const name)
{
    tree ret = get_function_assign_rhs(decl, name);

    SKIP_CONVERT_EXPRS(ret);

    return ret;
}
