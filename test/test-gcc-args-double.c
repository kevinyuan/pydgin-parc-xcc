//========================================================================
// Double Floating-Point Arguments
//========================================================================
// Since we are using EABI which has 8 argument registers, we need to make
// sure that it works.  This test focuses on double floating point
// arguments.

__attribute__ ((noinline))
int double_arg0()
{
  return 1;
}

__attribute__ ((noinline))
int double_arg1(double a0)
{
  return a0 == 1;
}

__attribute__ ((noinline))
int double_arg2(double a0, double a1)
{
  return (a0 + a1) == (1 + 2);
}

__attribute__ ((noinline))
int double_arg3(double a0, double a1, double a2)
{
  return (a0 + a1 + a2) == (1 + 2 + 3);
}

__attribute__ ((noinline))
int double_arg4(double a0, double a1, double a2, double a3)
{
  return (a0 + a1 + a2 + a3) == (1 + 2 + 3 + 4);
}

__attribute__ ((noinline))
int double_arg5(double a0, double a1, double a2, double a3, double a4)
{
  return (a0 + a1 + a2 + a3 + a4) == (1 + 2 + 3 + 4 + 5);
}

__attribute__ ((noinline))
int double_arg6(double a0, double a1, double a2, double a3, double a4, double a5)
{
  return (a0 + a1 + a2 + a3 + a4 + a5) == (1 + 2 + 3 + 4 + 5 + 6);
}

__attribute__ ((noinline))
int double_arg7(double a0, double a1, double a2, double a3, double a4, double a5, double a6)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6) == (1 + 2 + 3 + 4 + 5 + 6 + 7);
}

__attribute__ ((noinline))
int double_arg8(double a0, double a1, double a2, double a3, double a4, double a5, double a6, double a7)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8);
}

__attribute__ ((noinline))
int double_arg9(double a0, double a1, double a2, double a3, double a4, double a5, double a6, double a7, double a8)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9);
}

__attribute__ ((noinline))
int double_arg10(double a0, double a1, double a2, double a3, double a4, double a5, double a6, double a7, double a8, double a9)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10);
}

__attribute__ ((noinline))
double double_return(double a0)
{
  return a0;
}

int test()
{
  int error_code = 0;

  error_code++;
  if (!double_arg0())
  {
    return error_code;
  }

  error_code++;
  if (!double_arg1(1))
  {
    return error_code;
  }

  error_code++;
  if (!double_arg2(1, 2))
  {
    return error_code;
  }

  error_code++;
  if (!double_arg3(1, 2, 3))
  {
    return error_code;
  }

  error_code++;
  if (!double_arg4(1, 2, 3, 4))
  {
    return error_code;
  }

  error_code++;
  if (!double_arg5(1, 2, 3, 4, 5))
  {
    return error_code;
  }

  error_code++;
  if (!double_arg6(1, 2, 3, 4, 5, 6))
  {
    return error_code;
  }

  error_code++;
  if (!double_arg7(1, 2, 3, 4, 5, 6, 7))
  {
    return error_code;
  }

  error_code++;
  if (!double_arg8(1, 2, 3, 4, 5, 6, 7, 8))
  {
    return error_code;
  }

  error_code++;
  if (!double_arg9(1, 2, 3, 4, 5, 6, 7, 8, 9))
  {
    return error_code;
  }

  error_code++;
  if (!double_arg10(1, 2, 3, 4, 5, 6, 7, 8, 9, 10))
  {
    return error_code;
  }

  error_code++;
  if (double_return(1.0) != 1.0)
  {
    return error_code;
  }
  
  return 0;
}
