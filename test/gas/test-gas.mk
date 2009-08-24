#=========================================================================
# Simple Tests for GNU Assembler
#=========================================================================

test_gas_srcs = \
  test-gas-simple.s \
  test-gas-gcd.s \

#-------------------------------------------------------------------------
# Rules for testing just compilation
#-------------------------------------------------------------------------

test_gas_outs = $(patsubst %.s, %.out, $(test_gas_srcs))
test_gas_objs = $(patsubst %.s, %.o, $(test_gas_srcs))

$(test_gas_objs) : %.o : %.s $(CROSS_GAS)
	-{ $(CROSS_GAS) -o $@ $<; \
    echo "*** gas exit = $$?"; \
  } | tee $*.out

test_gas_junk += $(test_gas_outs) $(test_gas_objs)

#-------------------------------------------------------------------------
# Rules for actually executing the tests
#-------------------------------------------------------------------------

test_gas_exes     = $(patsubst %.s, %, $(test_gas_srcs))
test_gas_run_outs = $(patsubst %, %-run.out, $(test_gas_exes))

$(test_gas_exes) : % : %.o test-gas-main.c $(CROSS_GCC)
	-$(CROSS_GCC) -o $@ $(test_dir)/gas/test-gas-main.c $<

$(test_gas_run_outs) : %-run.out : % $(cross_run_dep)
	-{ $(CROSS_RUN) $<; \
    echo "*** run exit = $$?"; \
  } | tee $@

test_gas_junk += $(test_gas_exes) $(test_gas_run_outs)

#-------------------------------------------------------------------------
# Module specific targets
#-------------------------------------------------------------------------
# Ideally our prereqs would be .out files but we need to use .o files
# since we generate two files for the compilation tests, a .o and a .out
# so we just really tell make about the .o files.

check-gas : $(test_gas_objs)
	$(scripts_dir)/check-summary.rb $(test_gas_outs)

check-gas-run : $(test_gas_run_outs)
	$(scripts_dir)/check-summary.rb $(test_gas_outs) $(test_gas_run_outs)

clean-check-gas :
	rm -rf $(test_gas_junk)

.PHONY : check-gas clean-check-gas

#-------------------------------------------------------------------------
# Running variables
#-------------------------------------------------------------------------

test_objs     += $(test_gas_objs)
test_run_outs += $(test_gas_run_outs)
test_outs     += $(test_gas_outs) $(test_gas_run_outs)
test_junk     += $(test_gas_junk)
