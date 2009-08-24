#=========================================================================
# Simple Tests for g++
#=========================================================================

test_gxx_srcs = \
  test-g++-simple.cc \
  test-g++-gcd.cc \
  test-g++-stl.cc \

#-------------------------------------------------------------------------
# Rules for testing just compilation
#-------------------------------------------------------------------------

test_gxx_outs = $(patsubst %.cc, %.out, $(test_gxx_srcs))
test_gxx_objs = $(patsubst %.cc, %.o, $(test_gxx_srcs))

$(test_gxx_objs) : %.o : %.cc $(CROSS_GXX)
	-{ $(CROSS_GXX) -c -o $@ $<; \
    echo "*** g++ exit = $$?"; \
  } | tee $*.out

test_gxx_junk += $(test_gxx_outs) $(test_gxx_objs)

#-------------------------------------------------------------------------
# Rules for actually executing the tests
#-------------------------------------------------------------------------

test_gxx_exes     = $(patsubst %.cc, %, $(test_gxx_srcs))
test_gxx_run_outs = $(patsubst %, %-run.out, $(test_gxx_exes))

$(test_gxx_exes) : % : %.o test-g++-main.cc $(CROSS_GXX)
	-$(CROSS_GXX) -o $@ $(test_dir)/g++/test-g++-main.cc $<

$(test_gxx_run_outs) : %-run.out : % $(cross_run_dep)
	-{ $(CROSS_RUN) $<; \
    echo "*** run exit = $$?"; \
  } | tee $@

test_gxx_junk += $(test_gxx_exes) $(test_gxx_run_outs)

#-------------------------------------------------------------------------
# Module specific targets
#-------------------------------------------------------------------------
# Ideally our prereqs would be .out files but we need to use .o files
# since we generate two files for the compilation tests, a .o and a .out
# so we just really tell make about the .o files.

check-g++ : $(test_gxx_objs)
	$(scripts_dir)/check-summary.rb $(test_gxx_outs)

check-g++-run : $(test_gxx_run_outs)
	$(scripts_dir)/check-summary.rb $(test_gxx_outs) $(test_gxx_run_outs)

clean-check-g++ :
	rm -rf $(test_gxx_junk)

.PHONY : check-g++ clean-check-g++

#-------------------------------------------------------------------------
# Running variables
#-------------------------------------------------------------------------

test_objs     += $(test_gxx_objs)
test_run_outs += $(test_gxx_run_outs)
test_outs     += $(test_gxx_outs) $(test_gxx_run_outs)
test_junk     += $(test_gxx_junk)
