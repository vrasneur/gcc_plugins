#include <stdbool.h>
#include <stdio.h>

#include <gcc-plugin.h>
#include <tree.h>
#include <diagnostic.h>
#include <tm.h>
#include <cp/cp-tree.h>

#include "gpp_utils.h"
#include "gcc_utils.h"

int plugin_is_GPL_compatible;

bool is_c_ternary_expr(tree cond)
{
    return !(TREE_TYPE(cond) == NULL || TREE_TYPE(cond) == void_type_node);
}

tree get_else_clause(tree cond)
{
    tree res = NULL_TREE;

    if(is_gpp())
    {
        res = ELSE_CLAUSE(cond);
    }
    else
    {
        res = COND_EXPR_ELSE(cond);
    }

    return res;
}

bool is_if(tree cond)
{
    return (is_gpp() ?
            TREE_CODE(cond) == IF_STMT :
            (TREE_CODE(cond) == COND_EXPR && !is_c_ternary_expr(cond)));
}

tree walk_statements(tree *decl, int *subtrees, void *dummy)
{
    (void)subtrees;
    (void)dummy;

    if(is_if(*decl))
    {
        tree else_clause = get_else_clause(*decl);

        if(else_clause != NULL_TREE &&
           is_if(else_clause) &&
           get_else_clause(else_clause) == NULL_TREE)
        {
            WARNING_DECL(*decl, "no else found after else if");
        }
    }

    return NULL_TREE;
}

void pre_genericize(tree fndecl, void *dummy)
{
    (void)dummy;
   // get the function body
   tree body_chain = DECL_SAVED_TREE(fndecl);

   if(errorcount == 0 && sorrycount == 0)
   {
      walk_tree_without_duplicates(&body_chain, walk_statements, NULL);
   }
}

int plugin_init (struct plugin_name_args *plugin_info, struct plugin_gcc_version *version)
{
    (void)version;

   register_callback(plugin_info->base_name,
		     PLUGIN_PRE_GENERICIZE,
		     (plugin_callback_func) pre_genericize,
		     NULL);

   return 0;
}
