#=========================================================================
# configure.ac for maven newlib libc machine directory
#=========================================================================

#-------------------------------------------------------------------------
# Project metadata
#-------------------------------------------------------------------------

m4_define( proj_name,         [Maven Newlib Machine Library])
m4_define( proj_version,      [?])
m4_define( proj_maintainer,   [Christopher Batten])
m4_define( proj_abbreviation, [maven-newlib-machine])

#-------------------------------------------------------------------------
# Setup
#-------------------------------------------------------------------------

AC_INIT([proj_name],[proj_version],[proj_maintainer],[proj_abbreviation])
AC_CONFIG_SRCDIR([setjmp.S])
AC_CONFIG_AUX_DIR([${srcdir}/../../../..])

#-------------------------------------------------------------------------
# Checks for programs
#-------------------------------------------------------------------------
# It looks like newlib uses -fno-builtin for all of its compiles. Since
# we are not actually using the newlib configure stuff, I set it
# manually here.

LIB_AC_PROG_CC
AC_CHECK_TOOL([AR],[ar])
AC_CHECK_TOOL([RANLIB],[ranlib])
AC_SUBST([CFLAGS],["-O3 -fno-builtin"])
AC_PROG_INSTALL

#-------------------------------------------------------------------------
# Output
#-------------------------------------------------------------------------

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
