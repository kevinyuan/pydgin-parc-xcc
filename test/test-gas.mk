#=========================================================================
# Simple Tests for GNU Assembler
#=========================================================================

test_gas_srcs = \
  test-gas-simple.s \
  test-gas-gcd.s \
  test-gas-sync.s \
  test-gas-amo.s \
  test-gas-single.s \
  test-gas-vmemops.s \
  test-gas-vvadd.s \
  test-gas-flags.s \
  test-gas-vconds.s \
  test-gas-varith.s \
  test-gas-variths.s \
  test-gas-divrem.s \
  test-gas-vutidx.s \
  test-gas-vamo.s \

# These no longer pass because vf no longer works
#  test-gas-vcmds.s \
#  test-gas-vload.s \
#  test-gas-regnames.s \
#  test-gas-vstore.s \
#  test-gas-cpvpcom.s \
#  test-gas-vpidx.s \

#-------------------------------------------------------------------------
# Compile tests
#-------------------------------------------------------------------------

test_gas_objs         = $(patsubst %.s, %.o, $(test_gas_srcs))
test_gas_compile_outs = $(patsubst %.s, %-compile.out, $(test_gas_srcs))

$(test_gas_objs) : %.o : %.s $(CROSS_GAS) $(CROSS_OBJDUMP)
	-{ $(CROSS_GAS) -o $@ $<; \
    echo "*** gas compile exit = $$?"; \
  } 2>&1 | tee $*-compile.out

check-gas-compile : $(test_gas_objs)
	$(scripts_dir)/check-summary.rb $(test_gas_compile_outs)

test_gas_outs += $(test_gas_compile_outs)
test_gas_junk += $(test_gas_objs) $(test_gas_compile_outs) 

#-------------------------------------------------------------------------
# Link tests
#-------------------------------------------------------------------------

test_gas_exes      = $(patsubst %.s, %, $(test_gas_srcs))
test_gas_link_outs = $(patsubst %.s, %-link.out, $(test_gas_srcs))

$(test_gas_exes) : % : %.o test-gas-main.c $(CROSS_GCC)
	-{ $(CROSS_GCC) -o $@ $(test_dir)/test-gas-main.c $<; \
    echo "*** gcc link exit = $$?"; \
  } 2>&1 | tee $*-link.out

check-gas-link : $(test_gas_exes)
	$(scripts_dir)/check-summary.rb \
    $(test_gas_compile_outs) \
    $(test_gas_link_outs)

test_gas_outs += $(test_gas_link_outs)
test_gas_junk += $(test_gas_exes) $(test_gas_link_outs) 

#-------------------------------------------------------------------------
# Run tests
#-------------------------------------------------------------------------

test_gas_run_outs = $(patsubst %, %-run.out, $(test_gas_exes))

$(test_gas_run_outs) : %-run.out : % $(cross_run_dep)
	-{ $(CROSS_RUN) ./$<; \
    echo "*** run exit = $$?"; \
  } 2>&1 | tee $@

check-gas-run : $(test_gas_run_outs)
	$(scripts_dir)/check-summary.rb \
    $(test_gas_compile_outs) \
    $(test_gas_link_outs) \
    $(test_gas_run_outs)

test_gas_outs += $(test_gas_run_outs)
test_gas_junk += $(test_gas_run_outs)

#-------------------------------------------------------------------------
# Module specific targets
#-------------------------------------------------------------------------
# Ideally our prereqs would be .out files but we need to use .o/exe
# files since we generate two files for the compile/link tests (eg. a .o
# and a .out) so we just really tell make about the .o/exe files.

check-gas : $(test_gas_objs) $(test_gas_exes) $(test_gas_run_outs)
	$(scripts_dir)/check-summary.rb $(test_gas_outs)

clean-check-gas :
	rm -rf $(test_gas_junk)

.PHONY : check-gas clean-check-gas

#-------------------------------------------------------------------------
# Running variables
#-------------------------------------------------------------------------

test_objs     += $(test_gas_objs)
test_exes     += $(test_gas_exes)
test_run_outs += $(test_gas_run_outs)
test_outs     += $(test_gas_outs)
test_junk     += $(test_gas_junk)
