#=========================================================================
# configure.ac for maven libgloss and crt0
#=========================================================================

#-------------------------------------------------------------------------
# Project metadata
#-------------------------------------------------------------------------

m4_define( proj_name,         [Maven libgloss])
m4_define( proj_version,      [?])
m4_define( proj_maintainer,   [Christopher Batten])
m4_define( proj_abbreviation, [maven-libgloss])

#-------------------------------------------------------------------------
# Setup
#-------------------------------------------------------------------------

AC_INIT([proj_name],[proj_version],[proj_maintainer],[proj_abbreviation])
AC_CONFIG_SRCDIR([crt1.S])
AC_CONFIG_AUX_DIR([${srcdir}/../..])

#-------------------------------------------------------------------------
# Checks for programs
#-------------------------------------------------------------------------

LIB_AC_PROG_CC
AC_CHECK_TOOL([AR],[ar])
AC_CHECK_TOOL([RANLIB],[ranlib])
AC_PROG_INSTALL

#-------------------------------------------------------------------------
# Output
#-------------------------------------------------------------------------

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
