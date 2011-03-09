#include <gcc-plugin.h>
#include <tree.h>

#include <stdio.h>

int plugin_is_GPL_compatible;

void pre_genericize(tree fndecl, void *dummy)
{
    (void)dummy;
   // get the function body
   tree body_chain = DECL_SAVED_TREE(fndecl);

   fputs("* function:\n", stderr);
   debug_tree(fndecl);
   fputs("* function body:\n", stderr);
   debug_tree(body_chain);
   fputs("\n", stderr);
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
