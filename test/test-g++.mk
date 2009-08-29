#=========================================================================
# Simple Tests for g++
#=========================================================================

test_gxx_srcs = \
  test-g++-simple.cc \
  test-g++-gcd.cc \
  test-g++-static-init.cc \
  test-g++-exception.cc \
  test-g++-stl.cc \
  test-g++-fileio.cc \

#-------------------------------------------------------------------------
# Compile tests
#-------------------------------------------------------------------------

test_gxx_O0_objs = $(patsubst %.cc, %.o, $(test_gxx_srcs))
test_gxx_O3_objs = $(patsubst %.cc, %-O3.o, $(test_gxx_srcs))
test_gxx_objs    = $(test_gxx_O0_objs) $(test_gxx_O3_objs)

test_gxx_O0_compile_outs = $(patsubst %.cc, %-compile.out, $(test_gxx_srcs))
test_gxx_O3_compile_outs = $(patsubst %.cc, %-O3-compile.out, $(test_gxx_srcs))
test_gxx_compile_outs = \
  $(test_gxx_O0_compile_outs) $(test_gxx_O3_compile_outs)

$(test_gxx_O0_objs) : %.o : %.cc $(CROSS_GXX)
	-{ $(CROSS_GXX) -c -o $@ $<; \
    echo "*** g++ compile exit = $$?"; \
  } | tee $*-compile.out

$(test_gxx_O3_objs) : %-O3.o : %.cc $(CROSS_GXX)
	-{ $(CROSS_GXX) -O3 -c -o $@ $<; \
    echo "*** g++ compile exit = $$?"; \
  } | tee $*-O3-compile.out

check-g++-compile : $(test_gxx_objs)
	$(scripts_dir)/check-summary.rb $(test_gxx_compile_outs)

test_gxx_outs += $(test_gxx_compile_outs)
test_gxx_junk += $(test_gxx_objs) $(test_gxx_compile_outs)

#-------------------------------------------------------------------------
# Link tests
#-------------------------------------------------------------------------

test_gxx_exes      = $(patsubst %.o, %, $(test_gxx_objs))
test_gxx_link_outs = $(patsubst %.o, %-link.out, $(test_gxx_objs))

$(test_gxx_exes) : % : %.o test-g++-main.cc $(CROSS_GXX)
	-{ $(CROSS_GXX) -o $@ $(test_dir)/test-g++-main.cc $<; \
    echo "*** g++ link exit = $$?"; \
  } | tee $*-link.out

check-g++-link : $(test_gxx_exes)
	$(scripts_dir)/check-summary.rb \
    $(test_gxx_compile_outs) \
    $(test_gxx_link_outs)

test_gxx_outs += $(test_gxx_link_outs)
test_gxx_junk += $(test_gxx_exes) $(test_gxx_link_outs) 

#-------------------------------------------------------------------------
# Run tests
#-------------------------------------------------------------------------

test_gxx_run_outs = $(patsubst %, %-run.out, $(test_gxx_exes))

$(test_gxx_run_outs) : %-run.out : % $(cross_run_dep)
	-{ $(CROSS_RUN) $<; \
    echo "*** run exit = $$?"; \
  } | tee $@

check-g++-run : $(test_gxx_run_outs)
	$(scripts_dir)/check-summary.rb \
    $(test_gxx_compile_outs) \
    $(test_gxx_link_outs) \
    $(test_gxx_run_outs)

test_gxx_outs += $(test_gxx_run_outs)
test_gxx_junk += $(test_gxx_run_outs)

#-------------------------------------------------------------------------
# Module specific targets
#-------------------------------------------------------------------------
# Ideally our prereqs would be .out files but we need to use .o/exe
# files since we generate two files for the compile/link tests (eg. a .o
# and a .out) so we just really tell make about the .o/exe files.

check-g++ : $(test_gxx_objs) $(test_gxx_exes) $(test_gxx_run_outs)
	$(scripts_dir)/check-summary.rb $(test_gxx_outs)

clean-check-g++ :
	rm -rf $(test_gxx_junk)

.PHONY : check-g++ clean-check-g++

#-------------------------------------------------------------------------
# Running variables
#-------------------------------------------------------------------------

test_objs     += $(test_gxx_objs)
test_exes     += $(test_gxx_exes)
test_run_outs += $(test_gxx_run_outs)
test_outs     += $(test_gxx_outs)
test_junk     += $(test_gxx_junk)
