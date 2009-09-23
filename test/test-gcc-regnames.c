//========================================================================
// Basic test for symbolic register names in inline assembly
//========================================================================
// This test checks to make sure symbolic register names work in GCC
// inline assembly. The EABI has eight argument registers, so for now we
// are mostly just checking that this works. Eventually we could also
// check to make sure symbolic vector registers are handled correctly
// here.

__attribute__ ((noinline))
int foo( int a, int b, int c, int d )
{
  register int temp0 __asm__ ("a4") = a;
  register int temp1 __asm__ ("a5") = b;
  register int temp2 __asm__ ("a6") = c;
  register int temp3 __asm__ ("a7") = d;
  return temp0 + temp1 + temp2 + temp3;
}

int test()
{
  int res = foo(1,2,3,4);
  if ( res != 10 )
    return 1;

  return 0;
}

