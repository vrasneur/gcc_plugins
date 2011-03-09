#include <stdbool.h>
#include <stdio.h>

#include <gcc-plugin.h>
#include <tree.h>
#include <tree-iterator.h>
#include <predict.h>
#include <diagnostic.h>
#include <tm.h>
#include <cp/cp-tree.h>

#include "gpp_utils.h"
#include "gcc_utils.h"

int plugin_is_GPL_compatible;

tree visit_fun(tree *decl, int *subtrees, void *res)
{
    (void)subtrees;
    tree *returns = (tree *)res;
    enum tree_code code = TREE_CODE(*decl);

    if(code == RETURN_EXPR && *decl != returns[0] && *decl != returns[1])
    {
        warning_at(EXPR_LOCATION(*decl), 0, "non final return found");
    }

    return NULL_TREE;
}

void pre_genericize(tree fndecl, void *dummy)
{
    (void)dummy;
    tree body_chain = DECL_SAVED_TREE(fndecl);
    tree returns[2] = { NULL_TREE, NULL_TREE };

    if(errorcount == 0 && sorrycount == 0)
    {
       if(TREE_CODE(body_chain) == BIND_EXPR)
       {
	  body_chain = BIND_EXPR_BODY(body_chain);
       }

       if(TREE_CODE(body_chain) == STATEMENT_LIST)
       {
	  tree_stmt_iterator iter = tsi_start(body_chain);

	  for (; !tsi_end_p(iter); tsi_next(&iter))
	  {
	     tree stmt = *tsi_stmt_ptr(iter);

	     if(TREE_CODE(stmt) == RETURN_EXPR)
	     {
                if(is_gpp() && DECL_MAIN_P(fndecl) && tsi_one_before_end_p(iter))
                {
                    returns[1] = stmt;
                }
                else
                {
		   returns[0] = stmt;
                }
	     }
	  }
       }
    
       walk_tree_without_duplicates(&body_chain, visit_fun, returns);
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
