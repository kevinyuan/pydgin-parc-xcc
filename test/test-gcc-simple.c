//========================================================================
// Simple C Test                                                        
//========================================================================

__attribute__ ((noinline))
int foo( int a, int b )
{
  return a + b;
}

int test()
{
  int res = foo(1,1);
  if ( res != 2 )
    return 1;

  return 0;
}

