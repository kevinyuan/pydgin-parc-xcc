//========================================================================
// setjmp
//========================================================================
// setjmp/longjmp are written in assembly for maven so we do a quick
// test here to make sure they work.

#include <setjmp.h>

jmp_buf g_jmp_buf;

void foo1() 
{
  longjmp(g_jmp_buf,1);
}

void foo0() 
{
  foo1();
}

int test() 
{
  if ( !setjmp(g_jmp_buf) ) {
    foo0();
    return 1;
  } 
  return 0;
}
