#!/bin/sh
#=========================================================================
# upgrade.sh [options] <upgrade-dir>
#=========================================================================
#
#  -h  Display this message
#  -v  Verbose mode
#  -V  List current versions and exit
#
# Upgrade the gcc cross-compiler source code from the original source
# repositories. This script will fetch, extract, and trim this source
# code so that it is suitable for use in a mips cross-compiler. The
# given upgrade directory should specify the name of the directory which
# will hold the combined tree.
#
# Author : Christopher Batten
# Date   : July 21, 2009

set -e

#-------------------------------------------------------------------------
# Command line parsing
#-------------------------------------------------------------------------

function usage
{
  echo ""
  sed -n '3p' $0 | sed -e 's/#//'
  sed -n '5,/^$/p' $0 | sed -e 's/#//'
  exit 1
}

versions=no
verbose=no
while getopts "hvV" OPTION; do
  case $OPTION in
    v)   verbose=yes  ;;
    V)   versions=yes ;;
    h|?) usage ;;
  esac
done

if !( test -n "$1" ); then
  echo ""
  echo " ERROR: Must specify upgrade directory on command line!"
  usage
fi
upgrade_dir="$1"

#-------------------------------------------------------------------------
# Package version numbers
#-------------------------------------------------------------------------
# To upgrade to a new version, a user should really only need to change
# the appropriate version number below.

swsrc_ver=2009-07-20
gmp_ver=4.3.1
mpfr_ver=2.4.1
gcc_ver=4.4.0

if ( test "$versions" = "yes" ); then
  echo "binutils (cvs) : ${swsrc_ver}"
  echo "newlib (cvs)   : ${swsrc_ver}"
  echo "gmp            : ${gmp_ver}"
  echo "mpfr           : ${mpfr_ver}"
  echo "gcc            : ${gcc_ver}"
  exit
fi

#-------------------------------------------------------------------------
# Delete previous upgrade directory
#-------------------------------------------------------------------------

echo "Deleting and recreating upgrade directory"
rm -rf ${upgrade_dir}
mkdir -p ${upgrade_dir}
cd ${upgrade_dir}

#-------------------------------------------------------------------------
# Running variables
#-------------------------------------------------------------------------

# List of fetched subdirectories
sub_dirs=

#-------------------------------------------------------------------------
# Fetch binutils, newlib, sim
#-------------------------------------------------------------------------
# We fetch the base source code for binutils, newlib, and the simulators
# from the sourceware CVS repository. We could use the actual release
# tarballs for these various packages but it is much more likely a
# combined tree build will work if we start directly from CVS
# (http://gcc.gnu.org/simtest-howto.html). Note that we cannot export
# all three modules at once since they have overlapping subdirectories
# (and we will get "CVS: Move away" errors) so instead we export each
# individually and overwrite them into the top-level directory.

swsrc_cvsroot=:pserver:anoncvs@sourceware.org:/cvs/src
 
echo "Fetch binutils from sourceware cvs (${swsrc_ver})"
cvs -Q -z9 -d ${swsrc_cvsroot} export -D ${swsrc_ver} binutils
mv src binutils-${swsrc_ver}
sub_dirs="${sub_dirs} binutils-${swsrc_ver}"
 
echo "Fetch newlib from sourceware cvs (${swsrc_ver})"
cvs -Q -z9 -d ${swsrc_cvsroot} export -D ${swsrc_ver} newlib
mv src newlib-${swsrc_ver}
sub_dirs="${sub_dirs} newlib-${swsrc_ver}"
 
echo "Fetch sim from sourceware cvs (${swsrc_ver})"
cvs -Q -z9 -d ${swsrc_cvsroot} export -D ${swsrc_ver} sim
mv src sim-${swsrc_ver}
sub_dirs="${sub_dirs} sim-${swsrc_ver}"

#-------------------------------------------------------------------------
# Fetch and extract gmp/mpfr
#-------------------------------------------------------------------------
# We also need to fetch the GMP (GNU Multiple Precision Arithmetic
# Library) and MPFR (GNU Multiple Precision Floating-Point Library)
# since these are no longer distributed with gcc.

echo "Fetch and extract gmp-${gmp_ver}"
gmp_base_url=http://ftp.gnu.org/gnu/gmp
wget -O- ${gmp_base_url}/gmp-${gmp_ver}.tar.gz | tar xzf -
mv gmp-${gmp_ver} gmp
mkdir -p gmp-${gmp_ver}
mv gmp gmp-${gmp_ver}
sub_dirs="${sub_dirs} gmp-${gmp_ver}"

echo "Fetch and extract mpfr-${mpfr_ver}"
mpfr_base_url=http://www.mpfr.org/mpfr-current
wget -O- ${mpfr_base_url}/mpfr-${mpfr_ver}.tar.gz | tar xzf -
mv mpfr-${mpfr_ver} mpfr
mkdir -p mpfr-${mpfr_ver}
mv mpfr mpfr-${mpfr_ver}
sub_dirs="${sub_dirs} mpfr-${mpfr_ver}"

#-------------------------------------------------------------------------
# Fetch and extract gcc
#-------------------------------------------------------------------------
# We fetch the gcc-core, gcc-g++, gcc-testsuite tarballs instead of just
# fetching the full gcc tarball since we really don't need all the other
# language frontends.

gcc_base_url=http://ftp.gnu.org/gnu/gcc/gcc-${gcc_ver}
 
echo "Fetch and extract gcc-core-${gcc_ver}"
wget -O- ${gcc_base_url}/gcc-core-${gcc_ver}.tar.gz | tar xzf -
  
echo "Fetch and extract gcc-testsuite-${gcc_ver}"
wget -O- ${gcc_base_url}/gcc-testsuite-${gcc_ver}.tar.gz | tar xzf -
  
echo "Fetch and extract gcc-g++-${gcc_ver}"
wget -O- ${gcc_base_url}/gcc-g++-${gcc_ver}.tar.gz | tar xzf -
  
sub_dirs="${sub_dirs} gcc-${gcc_ver}"
 
echo "Trimming unnecessary testsuites in gcc-${gcc_ver}"
rm -rf gcc-${gcc_ver}/gcc/testsuite/ada
rm -rf gcc-${gcc_ver}/gcc/testsuite/gfortran.dg
rm -rf gcc-${gcc_ver}/gcc/testsuite/gfortran.fortran-torture
rm -rf gcc-${gcc_ver}/gcc/testsuite/gnat.dg
rm -rf gcc-${gcc_ver}/gcc/testsuite/obj-c++.dg
rm -rf gcc-${gcc_ver}/gcc/testsuite/objc
rm -rf gcc-${gcc_ver}/gcc/testsuite/objc.dg

#-------------------------------------------------------------------------
# Create combined tree
#-------------------------------------------------------------------------
# We now copy all the subdirectories into the upgrade directory on top
# of each other. When we are done we delete all the subdirectories so
# that the combined source tree is ready to move where needed.

for sub_dir in ${sub_dirs}; do

  echo "Merging subdirectory ${sub_dir}"
  tar -C ${sub_dir} -cf - . | tar -xpf -

  echo "Deleting subdirectory ${sub_dir}"
  rm -rf ${sub_dir}

done
