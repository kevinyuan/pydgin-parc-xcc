//========================================================================
// time.c
//========================================================================

#include <sys/time.h>

#include <machine/timex.h>
#include <machine/syscfg.h>

//------------------------------------------------------------------------
// gettimeofday
//------------------------------------------------------------------------
// For now we just return 0 so that essentially it always looks like the
// time is 00:00 hours, January 1, 1970. This could break some
// applications (since time effectively stands still), but it might also
// at least be enough to get some applications working. Note that this
// function already seems to be declared here:
//
//  xcc/src/newlib/libc/include/sys/time.h
//
// but it doesn't seem to be defined anywhere. So if you try and
// cross-compile a program that uses time() you will get a link error --
// so at least this bogus definition allows the application to link.

int gettimeofday( struct timeval* p, void* z )
{
  p->tv_sec  = 0;
  p->tv_usec = 0;
  return 0;
}

