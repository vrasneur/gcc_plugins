#include <stdbool.h>
#include <stdio.h>

#include <gcc-plugin.h>
#include <plugin.h>
#include <tree.h>
#include <diagnostic.h>

#include "gcc_utils.h"
#include "gpp_utils.h"

// not declared in the plugin API
extern int comptypes(tree t1, tree t2);

int plugin_is_GPL_compatible;

tree handle_user_attribute(tree *node, tree name, tree args,
                           int flags, bool *no_add_attrs)
{
    (void)node;
    (void)name;
    (void)args;
    (void)flags;
    (void)no_add_attrs;

    return NULL_TREE;
}
 
struct attribute_spec user_attr =
{ "user", 1, 1, false,  false, false, handle_user_attribute };
 
void register_attributes(void *event_data, void *data)
{
    (void)event_data;
    (void)data;

    register_attribute(&user_attr);
}

tree walk_statements(tree *decl, int *subtrees, void *dummy)
{
    (void)subtrees;
    (void)dummy;
    tree var = get_function_assign_lhs(*decl, "getchar");

    if(var != NULL_TREE)
    {
        tree var_type = TREE_TYPE(var);

        if(comptypes(var_type, unsigned_char_type_node) == 1 ||
           comptypes(var_type, signed_char_type_node) == 1 ||
           comptypes(var_type, char_type_node) == 1)
        {
            tree call = get_function_assign_call(*decl, "getchar");

            if(call != NULL_TREE)
            {
                WARNING_DECL(call, "(getchar) assignation to %qD, type is %qT instead of int",
                             var, TYPE_MAIN_VARIANT(var_type));
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

   register_callback(plugin_info->base_name,
                     PLUGIN_ATTRIBUTES,
                     (plugin_callback_func)register_attributes,
                     NULL);

   return 0;
}
