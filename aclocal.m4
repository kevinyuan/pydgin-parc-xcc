#=========================================================================
# Local Autoconf Macros
#=========================================================================

#-------------------------------------------------------------------------
# AX_STOW_INSTALL
#-------------------------------------------------------------------------
# This macro will add an --enable-stow command line option to the
# configure script. When enabled, this macro will first check to see if
# the stow program is available and if so it will set the $stow shell
# variable to the binary name and the $enable_stow shell variable to
# "yes". These variables can be used in a makefile to conditionally use
# stow for installation. This macro will also check to see if the
# $STOW_PREFIX environment variable is set, and if so it will change the
# prefix to this environment variable (assuming --prefix was not set on
# the command line). This allows a user to specify once were all
# packages should be installed.
#
# Author : Christopher Batten
# Date   : August 19, 2009

AC_DEFUN([AX_STOW_INSTALL],
[
  AC_PROG_INSTALL
   
  AC_ARG_VAR([STOW_PREFIX],[Prefix for stow-based installs])
  AC_ARG_ENABLE(stow,
    AS_HELP_STRING(--enable-stow,[Enable stow-based install]),
      [enable_stow="yes"],[enable_stow="no"])
   
  AS_IF([ test ${enable_stow} = "yes" ],
  [
    AC_CHECK_PROGS([stow],[stow],[no])  
    AS_IF([ test ${stow} = "no" ],
    [
      AC_MSG_ERROR([Cannot use --enable-stow since stow is not available])
    ])
   
    AS_IF([ test "${prefix}" = "NONE" && test -n "${STOW_PREFIX}" ],
    [
      prefix="${STOW_PREFIX}"
      AC_MSG_NOTICE([Using \$STOW_PREFIX from environment])
      AC_MSG_NOTICE([prefix=${prefix}])
    ])
   
  ])

  AC_SUBST([enable_stow])
])

#-------------------------------------------------------------------------
# AX_DEFAULT_CONFIGURE_ARG
#-------------------------------------------------------------------------
# Simple little macro which adds a configure commane line option to an
# internal autoconf shell variable. Not sure how safe this is, but it
# seems to work fine.
#
# Author : Christopher Batten
# Date   : August 20, 2009

AC_DEFUN([AX_DEFAULT_CONFIGURE_ARG],
[
  AC_MSG_NOTICE([adding default configure arg: $1])
  ac_configure_args="$1 ${ac_configure_args}"
])
