//========================================================================
// Basic test for symbolic register names in inline assembly
//========================================================================
// This test checks to make sure symbolic register names work in GCC
// inline assembly. The EABI has eight argument registers, so for now we
// are mostly just checking that this works. 

//------------------------------------------------------------------------
// test_arg_regs
//------------------------------------------------------------------------

__attribute__ ((noinline))
int test_arg_regs( int a0, int a1, int a2, int a3, 
                   int a4, int a5, int a6, int a7 )
{
  register int arg0 __asm__ ("a0") = a0;
  register int arg1 __asm__ ("a1") = a1;
  register int arg2 __asm__ ("a2") = a2;
  register int arg3 __asm__ ("a3") = a3;
  register int arg4 __asm__ ("a4") = a4;
  register int arg5 __asm__ ("a5") = a5;
  register int arg6 __asm__ ("a6") = a6;
  register int arg7 __asm__ ("a7") = a7;
  return arg0 + arg1 + arg2 + arg3 + arg4 + arg5 + arg6 + arg7;
}

//------------------------------------------------------------------------
// test_tmp_regs
//------------------------------------------------------------------------

__attribute__ ((noinline))
int test_tmp_regs( int a0, int a1, int a2, int a3, int a4,
                   int a5, int a6, int a7, int a8, int a9 )
{
  register int tmp0 __asm__ ("t0") = a0;
  register int tmp1 __asm__ ("t1") = a1;
  register int tmp2 __asm__ ("t2") = a2;
  register int tmp3 __asm__ ("t3") = a3;
  register int tmp4 __asm__ ("t4") = a4;
  register int tmp5 __asm__ ("t5") = a5;
  register int tmp6 __asm__ ("t6") = a6;
  register int tmp7 __asm__ ("t7") = a7;
  register int tmp8 __asm__ ("t8") = a8;
  register int tmp9 __asm__ ("t9") = a9;
  return tmp0 + tmp1 + tmp2 + tmp3 + tmp4
       + tmp5 + tmp6 + tmp7 + tmp8 + tmp9;
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  int res_0 = test_arg_regs(0,1,2,3,4,5,6,7);
  if ( res_0 != 28 )
    return 1;

  int res_1 = test_tmp_regs(0,1,2,3,4,5,6,7,8,9);
  if ( res_1 != 45 )
    return 2;

  return 0;
}

