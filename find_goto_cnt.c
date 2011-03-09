#include <stdbool.h>
#include <stdio.h>

#include <gcc-plugin.h>
#include <tree.h>
#include <tree-iterator.h>
#include <predict.h>
#include <diagnostic.h>

#include "gcc_utils.h"

int plugin_is_GPL_compatible;

void keyword_location(tree decl, char const *keyword)
{
    WARNING_DECL(decl, "%s found", keyword ?: "<unknown keyword>");
}

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

    switch(code)
    {
    case CONTINUE_STMT:
        keyword_location(*decl, "continue");
        break;
    case GOTO_EXPR:
        if(!is_goto_articifial(*decl))
        {
            keyword_location(*decl, "goto");
        }
        break;
    case PREDICT_EXPR:
        if(PREDICT_EXPR_PREDICTOR(*decl) == PRED_CONTINUE)
        {
            keyword_location(*decl, "continue");
        }
        break;
    default:
        break;
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
