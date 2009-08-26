#=========================================================================
# Simple Tests for gcc
#=========================================================================

test_gcc_srcs = \
  test-gcc-simple.c \
  test-gcc-gcd.c \
  test-gcc-libc.c \

#-------------------------------------------------------------------------
# Compile tests
#-------------------------------------------------------------------------

test_gcc_O0_objs = $(patsubst %.c, %-O0.o, $(test_gcc_srcs))
test_gcc_O3_objs = $(patsubst %.c, %-O3.o, $(test_gcc_srcs))
test_gcc_objs    = $(test_gcc_O0_objs) $(test_gcc_O3_objs)

test_gcc_O0_compile_outs = $(patsubst %.c, %-O0-compile.out, $(test_gcc_srcs))
test_gcc_O3_compile_outs = $(patsubst %.c, %-O3-compile.out, $(test_gcc_srcs))
test_gcc_compile_outs = \
  $(test_gcc_O0_compile_outs) $(test_gcc_O3_compile_outs)

$(test_gcc_O0_objs) : %-O0.o : %.c $(CROSS_GCC)
	-{ $(CROSS_GCC) -std=gnu99 -c -o $@ $<; \
    echo "*** gcc compile exit = $$?"; \
  } | tee $*-O0-compile.out

$(test_gcc_O3_objs) : %-O3.o : %.c $(CROSS_GCC)
	-{ $(CROSS_GCC) -O3 -std=gnu99 -c -o $@ $<; \
    echo "*** gcc compile exit = $$?"; \
  } | tee $*-O3-compile.out

check-gcc-compile : $(test_gcc_objs)
	$(scripts_dir)/check-summary.rb $(test_gcc_compile_outs)

test_gcc_outs += $(test_gcc_compile_outs)
test_gcc_junk += $(test_gcc_objs) $(test_gcc_compile_outs)

#-------------------------------------------------------------------------
# Link tests
#-------------------------------------------------------------------------

test_gcc_exes      = $(patsubst %.o, %, $(test_gcc_objs))
test_gcc_link_outs = $(patsubst %.o, %-link.out, $(test_gcc_objs))

$(test_gcc_exes) : % : %.o test-gcc-main.c $(CROSS_GCC)
	-{ $(CROSS_GCC) -o $@ $(test_dir)/gcc/test-gcc-main.c $<; \
    echo "*** gcc link exit = $$?"; \
  } | tee $*-link.out

check-gcc-link : $(test_gcc_exes)
	$(scripts_dir)/check-summary.rb \
    $(test_gcc_compile_outs) \
    $(test_gcc_link_outs)

test_gcc_outs += $(test_gcc_link_outs)
test_gcc_junk += $(test_gcc_exes) $(test_gcc_link_outs) 

#-------------------------------------------------------------------------
# Run tests
#-------------------------------------------------------------------------

test_gcc_run_outs = $(patsubst %, %-run.out, $(test_gcc_exes))

$(test_gcc_run_outs) : %-run.out : % $(cross_run_dep)
	-{ $(CROSS_RUN) $<; \
    echo "*** run exit = $$?"; \
  } | tee $@

check-gcc-run : $(test_gcc_run_outs)
	$(scripts_dir)/check-summary.rb \
    $(test_gcc_compile_outs) \
    $(test_gcc_link_outs) \
    $(test_gcc_run_outs)

test_gcc_outs += $(test_gcc_run_outs)
test_gcc_junk += $(test_gcc_run_outs)

#-------------------------------------------------------------------------
# Module specific targets
#-------------------------------------------------------------------------
# Ideally our prereqs would be .out files but we need to use .o/exe
# files since we generate two files for the compile/link tests (eg. a .o
# and a .out) so we just really tell make about the .o/exe files.

check-gcc : $(test_gcc_objs) $(test_gcc_exes) $(test_gcc_run_outs)
	$(scripts_dir)/check-summary.rb $(test_gcc_outs)

clean-check-gcc :
	rm -rf $(test_gcc_junk)

.PHONY : check-gcc clean-check-gcc

#-------------------------------------------------------------------------
# Running variables
#-------------------------------------------------------------------------

test_objs     += $(test_gcc_objs)
test_exes     += $(test_gcc_exes)
test_run_outs += $(test_gcc_run_outs)
test_outs     += $(test_gcc_outs)
test_junk     += $(test_gcc_junk)
