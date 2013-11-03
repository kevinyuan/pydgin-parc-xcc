//========================================================================
// timex.c
//========================================================================

#include <machine/timex.h>
#include <machine/syscfg.h>

//------------------------------------------------------------------------
// get_cycles
//------------------------------------------------------------------------
// We need to carefully read the count since it is a 64b value stored in
// two separate 32b control registers. If we just read one and then the
// other there might be an overflow from the lo to the hi register in
// between our reads. We can read both registers twice though in a
// specific order though to detect when overflow occurs. To see how this
// works imagine we have two 2b registers (instead of two 32b
// registers). The count would look like following where the first
// column is the hi portion and the second column is the lo portion.
//
//  hi lo  count
//   0  0
//   0  1
//   0  2
//   0  3  mfc0 t0, cp0_countlo (t0 = 3)
//   1  0  mfc0 t1, cp0_counthi (t1 = 1) {t1,t0} = 13
//   1  1  mfc0 t2, cp0_countlo (t2 = 1)
//   1  2  mfc0 t3, cp0_countlo (t3 = 1) {t3,t2} = 11
//   1  3
//
// I have also illustrated reading the count registers twice in a row.
// You can see that with 32b registers we can reasonably assume that an
// overflow can only happen in between on or the other sets of mfc0
// instructions but not both. If we read the lo bits first and then
// compare the final 64b values we know an overflow occurred during the
// first set of mfc0 instructions if the total count does not increase.
// In this case we can just use the second count.

cycles_t get_cycles()
{
  int count_0_hi, count_0_lo;
  int count_1_hi, count_1_lo;

  long long count_0;
  long long count_1;

  // Read the hi and lo portion of the cycle count twice (in a specific
  // order) so that we can detect overflow.

  __asm__ ( "mfc0 %0, $%4; mfc0 %1, $%5; mfc0 %2, $%4; mfc0 %3, $%5"
          : "=r"(count_0_lo), "=r"(count_0_hi),
            "=r"(count_1_lo), "=r"(count_1_hi)
          : "i"(MAVEN_SYSCFG_REGDEF_COP0_COUNT_LO),
            "i"(MAVEN_SYSCFG_REGDEF_COP0_COUNT_HI) );

  count_0 = (((long long) count_0_hi) << 32) | ((long long) count_0_lo);
  count_1 = (((long long) count_1_hi) << 32) | ((long long) count_1_lo);

  // If count_0 > count_1 then an overflow occurred during the first set
  // of mfc0 instructions and we should use count_1. Othwerise an
  // overflow did not occur during the first set of mfc0 instructions so
  // count_0 is correct. Note that we might end up using count_0 when an
  // overflow _did_ occur during the second set of mcf0 instructions but
  // that doesn't matter.

  if ( count_0 > count_1 )
    return count_1;

  return count_0;
}

