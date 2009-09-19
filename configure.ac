#=========================================================================
# Toplevel configure.ac for building the maven cross-compiler
#=========================================================================
# This script will configure a gcc maven cross-compiler. It sets several
# default configure arguments and then recursively calls the configure
# script in the gcc combined source tree.

#-------------------------------------------------------------------------
# Project metadata
#-------------------------------------------------------------------------

m4_define( proj_name,         [Maven GCC Cross-Compiler])
m4_define( proj_maintainer,   [Christopher Batten])
m4_define( proj_abbreviation, [maven-sys-xcc])

#-------------------------------------------------------------------------
# Project version information
#-------------------------------------------------------------------------
# Version information is meant to be managed through a version control
# system's tags and revision numbers. In a working copy the version will
# not be defined here (you should just use the version control system's
# mechanisms). When we make a distribution then we can set the version
# here as formed by the scripts/vcs-version.sh script so that the
# distribution knows what version it came from. If you are not using
# version control then it is fine to set this directly.

m4_define( proj_version, [?])

#-------------------------------------------------------------------------
# Setup
#-------------------------------------------------------------------------

AC_INIT(proj_name,proj_version,proj_maintainer,proj_abbreviation)
AC_CONFIG_SRCDIR([src/configure.ac])
AC_CONFIG_AUX_DIR([scripts])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

# Stow-based installation
AX_PROG_STOW_INSTALL

#-------------------------------------------------------------------------
# Target
#-------------------------------------------------------------------------
# We set the default target to maven. We have modified
# scripts/config.sub and src/config.sub so that this alias is translated
# into mipsmavenel-ucb-elf. This target will set several things up
# automatically.
#
#  - mips*-*-*   : use many mips defaults
#  - mips*el-*-* : configure for little endian
#  - *-*-elf     : use bare elf based "os"
#
# The we only need to worry about overriding this which are explicitly
# different from mips*el-*-elf. Many places we just make mipsmaven*-*-*
# act like mipsisa32r2*-*-*.

default_target_alias=maven
AS_IF([test -z "${target_alias}"],
[
  target_alias=${default_target_alias}
])

AC_CANONICAL_TARGET

# Export target_alias variable to makefile and sub configures

AX_DEFAULT_CONFIGURE_ARG([--target=${target_alias}])
AC_SUBST([target_alias])

#-------------------------------------------------------------------------
# Test Wrappers
#-------------------------------------------------------------------------
# In our makefile we create simple shell scripts which act as wrappers
# around the various programs. This makes it easier to test the compiler
# before doing an installation. Each shell script will include a pointer
# to the actual binary (probably deep in the build tree) and the
# required flags. Some of these flags (like the libgloss settings and
# the location of the simulator) are target specific. Note that the
# libgloss ldflags are also exported to scripts/longcheck-setup.exp so
# that dejagnu knows about maven libgloss. We might need to something
# similar if we want a dedicated newlib maven machine directory.

# absolute build and source directories

abs_build_dir=`pwd`
abs_src_dir=`cd ${srcdir} && pwd`

# cross gas

cross_gas_bin="${abs_build_dir}/src/gas/as-new"
cross_gas_flags=

AC_SUBST([cross_gas_bin])
AC_SUBST([cross_gas_flags])

# cross objdump

cross_objdump_bin="${abs_build_dir}/src/binutils/objdump"
cross_objdump_flags=

AC_SUBST([cross_objdump_bin])
AC_SUBST([cross_objdump_flags])

# newlib (target specific)

case ${target} in
  mipsmaven*-*-*) 
    cross_newlib_cppflags=" \
      -I ${abs_src_dir}/src/newlib/libc/machine/maven"
  ;;
esac

AC_SUBST([cross_newlib_cppflags])

# libgloss (target specific)

case ${target} in

  mipsmaven*-*-*) 
    cross_libgloss_ldflags=" \
      -B ${abs_build_dir}/src/${target_alias}/libgloss/maven/ \
      -L ${abs_build_dir}/src/${target_alias}/libgloss/maven \
      -L ${abs_src_dir}/src/libgloss/maven"
  ;;

  mips*-*-*) 
    cross_libgloss_ldflags=" \
      -B ${abs_build_dir}/src/${target_alias}/libgloss/mips/ \
      -L ${abs_build_dir}/src/${target_alias}/libgloss/mips \
      -L ${abs_src_dir}/src/libgloss/mips"
  ;;

esac

AC_SUBST([cross_libgloss_ldflags])

# cross gcc

cross_gcc_bin="${abs_build_dir}/src/gcc/xgcc"
cross_gcc_flags=" \
  -B ${abs_build_dir}/src/gcc/ \
  ${cross_libgloss_ldflags} \
  ${cross_newlib_cppflags} \
  -I ${abs_src_dir}/src/newlib/libc/include \
  -L ${abs_build_dir}/src/${target_alias}/newlib"

AC_SUBST([cross_gcc_bin])
AC_SUBST([cross_gcc_flags])

# cross g++

cross_gxx_bin="${abs_build_dir}/src/gcc/g++"
cross_gxx_flags=" \
  ${cross_gcc_flags} \
  -I ${abs_src_dir}/src/libstdc++-v3/libsupc++ \
  -I ${abs_build_dir}/src/${target_alias}/libstdc++-v3/include \
  -I ${abs_build_dir}/src/${target_alias}/libstdc++-v3/include/${target_alias} \
  -L ${abs_build_dir}/src/${target_alias}/libstdc++-v3/src/.libs"

AC_SUBST([cross_gxx_bin])
AC_SUBST([cross_gxx_flags])

# cross run (target specific)

case ${target} in

  mipsmaven*-*-*) 
    cross_run_internal="no"
    cross_run_target_board="longcheck-setup"
    AC_CHECK_TARGET_TOOL([cross_run],[isa-run],[no])
    AS_IF([ test ${cross_run} = "no" ],
    [
      AC_MSG_ERROR([Cannot find maven ISA simulator])
    ])
  ;;

  mips*-*-*) 
    cross_run_internal="yes"
    cross_run="./${target_alias}-run"
    cross_run_bin="${abs_build_dir}/src/sim/mips/run"
    cross_run_flags=
    cross_run_target_board="mips-sim"
  ;;

esac

AC_SUBST([cross_run_internal])
AC_SUBST([cross_run])
AC_SUBST([cross_run_bin])
AC_SUBST([cross_run_flags])
AC_SUBST([cross_run_target_board])

#-------------------------------------------------------------------------
# Other Default Configure Arguments
#-------------------------------------------------------------------------
# We setup the configure arguments for the combined tree configure
# script so that by default we build a mips cross-compiler. Each default
# should include a justification for why we need to use it.

# We should have only fetched the gcc and g++ language front-ends so by
# default the build should only be for c and c++, but we make sure with
# this configure argument.

AX_DEFAULT_CONFIGURE_ARG([--enable-languages=c,c++])

# We want our cross-compiler to use newlib instead of glibc. Newlib is a
# simpler and easier to port implementation of libc. One reason newlib
# is easier to port is because it has a relatively narrow syscall
# interface - which we will be implementing with a proxy kernel for now. 

AX_DEFAULT_CONFIGURE_ARG([--with-newlib])

# If we enable multilib the compiler will build additional versions of
# newlib. By default it seems to build versions for 32b, 64b,
# soft-float, big-endian and various combinations of these. So we
# disable multilib. We should configure the build so that only the
# version of newlib we actually want to use is built.

AX_DEFAULT_CONFIGURE_ARG([--disable-multilib])

# There is no need to enable the statck smashing protection runtime
# library since it is just one more thing to build. 

AX_DEFAULT_CONFIGURE_ARG([--disable-libssp])

# By default, the OpenMP library does not seem to be built for this mips
# target, but we make sure with this argument. I think it does work with
# a mips-linux target though so it might be possible to port it to this
# bare mips system.

AX_DEFAULT_CONFIGURE_ARG([--disable-libgomp])

# I think by default, cross-compilers do not include native language
# support, but we make sure with this argument. Native language support
# allows gcc ouptut in languages other than english.

AX_DEFAULT_CONFIGURE_ARG([--disable-nls])

#-------------------------------------------------------------------------
# Call combined tree configure
#-------------------------------------------------------------------------

AC_CONFIG_SUBDIRS([src])

#-------------------------------------------------------------------------
# Output
#-------------------------------------------------------------------------

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
