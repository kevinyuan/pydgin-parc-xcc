#=========================================================================
# Simple Tests for gcc
#=========================================================================

test_gcc_srcs = \
  test-gcc-simple.c \
  test-gcc-gcd.c \
  test-gcc-ext-ins.c \
  test-gcc-static-init.c \
  test-gcc-setjmp.c \
  test-gcc-libc.c \
  test-gcc-malloc.c \
  test-gcc-fileio.c \
  test-gcc-regalloc.c \
  test-gcc-regnames.c \
  test-gcc-args-int.c \
  test-gcc-args-longlong.c \
  test-gcc-args-single.c \
  test-gcc-args-double.c \
  test-gcc-arth-single.c \
  test-gcc-arth-double.c \
  test-gcc-vpfunc.c \

#-------------------------------------------------------------------------
# Compile tests
#-------------------------------------------------------------------------

test_gcc_O0_objs = $(patsubst %.c, %.o, $(test_gcc_srcs))
test_gcc_O3_objs = $(patsubst %.c, %-O3.o, $(test_gcc_srcs))
test_gcc_objs    = $(test_gcc_O0_objs) $(test_gcc_O3_objs)

test_gcc_O0_compile_outs = $(patsubst %.c, %-compile.out, $(test_gcc_srcs))
test_gcc_O3_compile_outs = $(patsubst %.c, %-O3-compile.out, $(test_gcc_srcs))
test_gcc_compile_outs = \
  $(test_gcc_O0_compile_outs) $(test_gcc_O3_compile_outs)

$(test_gcc_O0_objs) : %.o : %.c $(CROSS_GCC) $(CROSS_OBJDUMP)
	-{ $(CROSS_GCC) -std=gnu99 -c -o $@ $<; \
    echo "*** gcc compile exit = $$?"; \
  } 2>&1 | tee $*-compile.out

$(test_gcc_O3_objs) : %-O3.o : %.c $(CROSS_GCC) $(CROSS_OBJDUMP)
	-{ $(CROSS_GCC) -O3 -std=gnu99 -c -o $@ $<; \
    echo "*** gcc compile exit = $$?"; \
  } 2>&1 | tee $*-O3-compile.out

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
	-{ $(CROSS_GCC) -o $@ $(test_dir)/test-gcc-main.c $<; \
    echo "*** gcc link exit = $$?"; \
  } 2>&1 | tee $*-link.out

check-gcc-link : $(test_gcc_exes)
	$(scripts_dir)/check-summary.rb \
    $(test_gcc_compile_outs) \
    $(test_gcc_link_outs)

test_gcc_outs += $(test_gcc_link_outs)
test_gcc_junk += $(test_gcc_exes) $(test_gcc_link_outs) 

#-------------------------------------------------------------------------
# Run tests
#-------------------------------------------------------------------------

test_gcc_run_outs += $(patsubst %, %-run.out, $(test_gcc_exes))

$(test_gcc_run_outs) : %-run.out : % $(cross_run_dep)
	-{ $(CROSS_RUN) $<; \
    echo "*** run exit = $$?"; \
  } 2>&1 | tee $@

check-gcc-run : $(test_gcc_run_outs)
	$(scripts_dir)/check-summary.rb \
    $(test_gcc_compile_outs) \
    $(test_gcc_link_outs) \
    $(test_gcc_run_outs)

test_gcc_outs += $(test_gcc_run_outs)
test_gcc_junk += $(test_gcc_run_outs)

#-------------------------------------------------------------------------
# test-gcc-main-args
#-------------------------------------------------------------------------
# Since we need to run test-gcc-main-args with special command line
# arguments to actually test argc and argv we special case building and
# running this test.

test_gcc_args_src = test-gcc-main-args.c
test_gcc_args_exe = $(patsubst %.c, %, $(test_gcc_args_src))

test_gcc_args_compile_out = \
   $(patsubst %.c, %-compile.out, $(test_gcc_args_src))

test_gcc_args_run_out = \
  $(patsubst %.c, %-run.out, $(test_gcc_args_src))

$(test_gcc_args_exe) : % : %.c $(CROSS_GCC)
	-{ $(CROSS_GCC) -std=gnu99 -o $@ $<; \
    echo "*** gcc compile exit = $$?"; \
  } 2>&1 | tee $*-compile.out

$(test_gcc_args_run_out) : %-run.out : % $(cross_run_dep)
	-{ $(CROSS_RUN) $< apple pear mango; \
    echo "*** run exit = $$?"; \
  } 2>&1 | tee $@

test_gcc_run_outs += $(test_gcc_args_run_out)
test_gcc_outs += $(test_gcc_args_compile_out) $(test_gcc_args_run_out)
test_gcc_junk += $(test_gcc_args_exe)

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
