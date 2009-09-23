//========================================================================
// resource.c
//========================================================================

#include <machine/resource.h>
#include <sys/timex.h>
#include <errno.h>

// This is temporary until we get reentrancy working. 
#undef errno
extern int errno;

//------------------------------------------------------------------------
// getrusage
//------------------------------------------------------------------------
// Since maven does not currently support processes we set both of the
// children's times to zero. Eventually we might want to separately
// account for user vs system time, but for now we just return the total
// time since starting the program.

int getrusage( int who, struct rusage* ru )
{
  // Return utilization of current process
  if ( who == RUSAGE_SELF ) {

    // Create conversion factors
    cycles_t cycles_per_sec  = get_cycles_per_sec();
    cycles_t cycles_per_usec = cycles_per_sec / (cycles_t) 1000000;

    // Get the number of cycles since program started
    cycles_t cycles = get_cycles();

    // Fill return structure appropriately
    ru->ru_utime.tv_sec  = cycles / cycles_per_sec;
    ru->ru_utime.tv_usec = cycles / cycles_per_usec;
    ru->ru_stime.tv_sec  = 0;
    ru->ru_stime.tv_usec = 0;
  }

  // Return utilization of children (currently always zero)
  else if ( who == RUSAGE_CHILDREN ) {
    ru->ru_utime.tv_sec  = 0;
    ru->ru_utime.tv_usec = 0;
    ru->ru_stime.tv_sec  = 0;
    ru->ru_stime.tv_usec = 0;
  }

  // Invalid who argument
  else {
    errno = EINVAL;
    return -1;
  }

  return 0;
}


