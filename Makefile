GCC := ../../gcc-install/bin/gcc
GPP := ../../gcc-install/bin/g++
GCCPLUGINS_DIR := $(shell $(GCC) -print-file-name=plugin)
CFLAGS += -I$(GCCPLUGINS_DIR)/include -fPIC -O0 -g -Wall -Wextra
LIBS := gcc_utils gpp_utils
PLUGINS := debug_tree find_goto_cnt find_goto recursive_fun single_return else_if setjmp_check getchar_check uninit_vars
PLUGINS_OBJECTS := $(addsuffix .o,$(PLUGINS))
LIBS_OBJECTS := $(addsuffix .o,$(LIBS))
OBJECTS := $(LIBS_OBJECTS) $(PLUGINS_OBJECTS)
PLUGINS_SHARED := $(addsuffix .so,$(PLUGINS))

test-plugin = -fplugin=$(PWD)/$(1) test.c -o /dev/null $(2)

define run-plugin
$(GCC) $(call test-plugin,$(1),$(2))
$(GPP) $(call test-plugin,$(1),$(2))
endef

all: $(OBJECTS) $(PLUGINS_SHARED)

$(OBJECTS): %.o: %.c
	$(GCC) $(CFLAGS) -c $^

$(PLUGINS_SHARED): %.so: %.o
	$(GCC) -shared $^ $(LIBS_OBJECTS) -o $@

clean:
	rm -f *.so *.o test_prog

distclean: clean
	rm -f *~ *.dump

dump-ast:
	$(GCC) $(call test-plugin,debug_tree.so,) 2> test_c_gcc.dump
	$(GPP) $(call test-plugin,debug_tree.so,) 2> test_c_gpp.dump

test: test-goto test-rec test-ret test-else test-setjmp test-uninit test-poison

test-goto-cnt:
	$(call run-plugin,find_goto_cnt.so,)

test-goto:
	$(call run-plugin,find_goto.so,)

test-rec:
	$(call run-plugin,recursive_fun.so,)

test-ret:
	$(call run-plugin,single_return.so,)

test-else:
	$(call run-plugin,else_if.so,)

test-setjmp:
	$(call run-plugin,setjmp_check.so,-include setjmp_check.h)

test-getchar:
	$(call run-plugin,getchar_check.so,-include getchar_check.h)

test-uninit:	
	$(call run-plugin,uninit_vars.so,)
	$(GPP) -fplugin=$(PWD)/uninit_vars.so test.cpp -o /dev/null

test-poison:
	$(GCC) -include poison.h test.c -o /dev/null || :

test-progs:
	$(GCC) -v test.c -o test_prog

.PHONY: test clean poison
