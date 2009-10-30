//========================================================================
// vpfunc Function Attribute
//========================================================================
// We use a vpfunc function attribute to annotate the helper function
// created by the VT_VFETCH preprocessor macro. This attribute tells gcc
// that it doesn't need to abide by the standard ABI in terms of saving
// callee saved registers. The attribute also tells gcc to insert a
// stop instruction right before the function returns with jr.
//
// We can't actually test this functionality - ideally you would grep the
// compiled binary.  We don't have any support for that so you will need
// to take a look at the dissassembly yourself.

//------------------------------------------------------------------------
// Standard function
//------------------------------------------------------------------------
// Compiles to something like what is below. Notice that gcc has to save
// the s0 register to the stack since it is callee saved.
//
//   0: addiu   sp, sp, -8
//   4: sw      s0, 4(sp)
//   8: move    s0, a0
//   c: sll     v0, a0, 0x1
//  10: lw      s0, 4(sp)
//  14: addiu   sp, sp, 8
//  18: jr      ra
//

int standard_func( int a )
{
  register int t asm("s0") = a;
  asm("" :: "r"(t));
  return a + t;
}

//------------------------------------------------------------------------
// vpfunc_simple
//------------------------------------------------------------------------
// Compiles to something like what is below. Notice that gcc no loner
// needs to save the s0 register to the stack since we have annotated
// this function as a vpfunc. Also notice that gcc has inserted a stop
// instruction right before the final jr.
//
//  1c:   move    s0, a0
//  20:   sll     v0, a0, 0x1
//  24:   stop
//  28:   jr      ra
//

__attribute__ ((vpfunc))
int vpfunc_simple( int a )
{
  register int t asm("s0") = a;
  asm("" :: "r"(t));
  return a + t;
}

//------------------------------------------------------------------------
// vpfunc_with_stack
//------------------------------------------------------------------------
// In this function we actually try and get gcc to use the stack to make
// sure that gcc inserts the stop instruction _after_ the final stack
// pointer adjustment. Compiles to something like what is below. Notice
// that the stop instruction is placed after the final stack pointer
// adjustment at address 0x50 - gcc has decided to schedule the stack
// pointer adjustment earlier.
//
//  30: addiu   sp, sp, -16
//  38: lw      v1, 4(sp)
//  3c: lw      v0, 0(sp)
//  40: addu    v0, v1, v0
//  44: lw      v1, 8(sp)
//  48: addu    v0, v0, v1
//  4c: lw      v1, 12(sp)
//  50: addiu   sp, sp, 16
//  54: addu    v0, v0, v1
//  58: addu    v0, v0, a0
//  60: stop
//  64: jr      ra
//

__attribute__ ((noinline))
void noinline_func( int* dest, int* src )
{
  dest = src;
}

__attribute__ ((vpfunc))
int vpfunc_with_stack( int a )
{
  // This should be allocated on the stack
  int src[4];
  src[0] = 0;
  src[1] = 1;
  src[2] = 2;
  src[3] = 3;

  // This array should also be allocated on the stack
  int dest[4];

  // Pointers to the two arrays are passed into the function
  noinline_func( dest, src );

  return a + dest[0] + dest[1] + dest[2] + dest[3];
}

//------------------------------------------------------------------------
// vpfunc_multiple_exit
//------------------------------------------------------------------------

__attribute__ ((vpfunc))
int vfunc_multiple_exit( int a )
{
  if ( a > 0 )
    return 0;

  // This should be allocated on the stack
  int src[4];
  src[0] = 0;
  src[1] = 1;
  src[2] = 2;
  src[3] = 3;

  // This array should also be allocated on the stack
  int dest[4];

  // Pointers to the two arrays are passed into the function
  noinline_func( dest, src );

  return a + dest[0] + dest[1] + dest[2] + dest[3];
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  return ( foo(1) != 2 );
}
