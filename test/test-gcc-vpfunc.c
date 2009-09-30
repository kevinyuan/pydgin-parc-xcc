//========================================================================
// vpfunc Function Attribute
//========================================================================
// The function that is called in the VPs, it shouldn't save any registers
// that its using to the stack.  We implement a "vpfunc" attribute to 
// specify that the following function doesn't need to save anything
//
// You can't actually test the functionality - ideally you would grep the
// compiled binary.  We don't have any support for that - you need to
// take a look your self

__attribute__ ((vpfunc))
int vp_foo(int a)
{
  register int t asm("s0") = a;

  asm("" :: "r"(t));

  return a + t;
}

int foo(int a)
{
  register int t asm("s0") = a;

  asm("" :: "r"(t));

  return a + t;
}

int test()
{
  return foo(1) != 2;
}
