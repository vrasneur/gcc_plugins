#include <stdbool.h>
#include <stdio.h>

#include <gcc-plugin.h>
#include <tree.h>
#include <tree-iterator.h>
#include <diagnostic.h>
#include <tm.h>
#include <cp/cp-tree.h>

#include "gpp_utils.h"
#include "gcc_utils.h"

int plugin_is_GPL_compatible;

tree walk_init(tree *decl, int *subtrees, void *var)
{
    (void)subtrees;
    tree var_decl = (tree)var;
    enum tree_code code = TREE_CODE(*decl);
    tree res = NULL_TREE;

    if(code == INIT_EXPR)
    {
        tree init_var = TREE_OPERAND(*decl, 0);
        
        if(init_var == var_decl)
        {
            res = init_var;
        }
    }

    return res;
}

tree visit_fun(tree *decl, int *subtrees, void *dummy)
{
    (void)subtrees;
    (void)dummy;
    enum tree_code code = TREE_CODE(*decl);
    tree var = NULL_TREE;
    
    if(code == BIND_EXPR)
    {
        for(var = BIND_EXPR_VARS(*decl); var; var = TREE_CHAIN(var))
        {
            if(TREE_CODE(var) == VAR_DECL)
            {
                if(auto_var_in_fn_p(var, current_function_decl) &&
                   !DECL_ARTIFICIAL(var))
                {
                    if(!DECL_INITIAL(var))
                    {
                        tree init_var = walk_tree_without_duplicates(decl, walk_init, var);

                        if(init_var == NULL_TREE)
                        {
                            // don't check classes initialization (too complicated)
                            if(!(TREE_CODE(TREE_TYPE(var)) == RECORD_TYPE &&
                                 CLASSTYPE_DECLARED_CLASS(TREE_TYPE(var))))
                            {
                                WARNING_DECL(*decl, "uninititialized auto var %qD", var);
                            }
                        }
                    }
                }
            }
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
      walk_tree_without_duplicates(&body_chain, visit_fun, NULL);
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
