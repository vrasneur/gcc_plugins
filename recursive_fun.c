#include <stdbool.h>
#include <stdio.h>

#include <gcc-plugin.h>
#include <tree.h>
#include <diagnostic.h>

#include "gcc_utils.h"

int plugin_is_GPL_compatible;

tree walk_statements(tree *decl, int *subtrees, void *dummy)
{
    (void)subtrees;
    (void)dummy;
    enum tree_code code = TREE_CODE(*decl);

    if(code == CALL_EXPR)
    {
        tree fn_decl = get_function_decl_from_call(*decl);
            
        if(fn_decl == current_function_decl)
        {
            WARNING_DECL(*decl, "recursive call to %qD found", fn_decl);
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
