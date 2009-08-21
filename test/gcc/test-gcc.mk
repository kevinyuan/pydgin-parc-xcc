#=========================================================================
# Simple Tests for gcc
#=========================================================================

test_gcc_srcs = \
	test-gcc-simple.c \
	test-gcc-gcd.c \
	test-gcc-libc.c \

#-------------------------------------------------------------------------
# Rules for testing just compilation
#-------------------------------------------------------------------------

test_gcc_outs = $(patsubst %.c, %.out, $(test_gcc_srcs))
test_gcc_objs = $(patsubst %.c, %.o, $(test_gcc_srcs))

$(test_gcc_objs) : %.o : %.c $(CROSS_GCC)
	{ $(CROSS_GCC) -std=gnu99 -c -o $@ $<; \
    echo "*** gcc exit = $$?"; \
  } | tee $*.out

test_gcc_junk += $(test_gcc_outs) $(test_gcc_objs)

#-------------------------------------------------------------------------
# Rules for actually executing the tests
#-------------------------------------------------------------------------

test_gcc_exes     = $(patsubst %.c, %, $(test_gcc_srcs))
test_gcc_run_outs = $(patsubst %, %-run.out, $(test_gcc_exes))

$(test_gcc_exes) : % : %.o test-gcc-main.c $(CROSS_GCC)
	$(CROSS_GCC) -o $@ $(test_dir)/gcc/test-gcc-main.c $<

$(test_gcc_run_outs) : %-run.out : % $(CROSS_RUN)
	{ $(CROSS_RUN) $<; \
    echo "*** run exit = $$?"; \
  } | tee $@

test_gcc_junk += $(test_gcc_exes) $(test_gcc_run_outs)

#-------------------------------------------------------------------------
# Module specific targets
#-------------------------------------------------------------------------
# Ideally our prereqs would be .out files but we need to use .o files
# since we generate two files for the compilation tests, a .o and a .out
# so we just really tell make about the .o files.

check-gcc : $(test_gcc_objs)
	$(scripts_dir)/check-summary.rb $(test_gcc_outs)

check-gcc-run : $(test_gcc_run_outs)
	$(scripts_dir)/check-summary.rb $(test_gcc_outs) $(test_gcc_run_outs)

clean-check-gcc :
	rm -rf $(test_gcc_junk)

.PHONY : check-gcc clean-check-gcc

#-------------------------------------------------------------------------
# Running variables
#-------------------------------------------------------------------------

test_objs     += $(test_gcc_objs)
test_run_outs += $(test_gcc_run_outs)
test_outs     += $(test_gcc_outs) $(test_gcc_run_outs)
test_junk     += $(test_gcc_junk)
