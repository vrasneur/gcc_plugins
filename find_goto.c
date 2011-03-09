#include <stdbool.h>
#include <stdio.h>

#include <gcc-plugin.h>
#include <tree.h>
#include <tree-iterator.h>
#include <diagnostic.h>

#include "gcc_utils.h"

int plugin_is_GPL_compatible;

bool is_goto_articifial(tree decl)
{
    tree dest = GOTO_DESTINATION(decl);

    return (TREE_CODE(dest) == LABEL_DECL &&
            DECL_ARTIFICIAL(dest));
}

tree visit_fun(tree *decl, int *subtrees, void *dummy)
{
    (void)subtrees;
    (void)dummy;
    enum tree_code code = TREE_CODE(*decl);

    if(code == GOTO_EXPR && !is_goto_articifial(*decl))
    {
        warning_at(EXPR_LOCATION(*decl), 0, "goto found");
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
