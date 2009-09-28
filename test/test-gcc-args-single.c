//========================================================================
// Single Floating-Point Arguments
//========================================================================
// Since we are using EABI which has 8 argument registers, we need to make
// sure that it works.  This test focuses on single floating point
// arguments.

__attribute__ ((noinline))
int float_arg0()
{
  return 1;
}

__attribute__ ((noinline))
int float_arg1(float a0)
{
  return a0 == 1;
}

__attribute__ ((noinline))
int float_arg2(float a0, float a1)
{
  return (a0 + a1) == (1 + 2);
}

__attribute__ ((noinline))
int float_arg3(float a0, float a1, float a2)
{
  return (a0 + a1 + a2) == (1 + 2 + 3);
}

__attribute__ ((noinline))
int float_arg4(float a0, float a1, float a2, float a3)
{
  return (a0 + a1 + a2 + a3) == (1 + 2 + 3 + 4);
}

__attribute__ ((noinline))
int float_arg5(float a0, float a1, float a2, float a3, float a4)
{
  return (a0 + a1 + a2 + a3 + a4) == (1 + 2 + 3 + 4 + 5);
}

__attribute__ ((noinline))
int float_arg6(float a0, float a1, float a2, float a3, float a4, float a5)
{
  return (a0 + a1 + a2 + a3 + a4 + a5) == (1 + 2 + 3 + 4 + 5 + 6);
}

__attribute__ ((noinline))
int float_arg7(float a0, float a1, float a2, float a3, float a4, float a5, float a6)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6) == (1 + 2 + 3 + 4 + 5 + 6 + 7);
}

__attribute__ ((noinline))
int float_arg8(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8);
}

__attribute__ ((noinline))
int float_arg9(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9);
}

__attribute__ ((noinline))
int float_arg10(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10);
}

__attribute__ ((noinline))
float float_return(float a0)
{
  return a0;
}

int test()
{
  int error_code = 0;

  error_code++;
  if (!float_arg0())
  {
    return error_code;
  }

  error_code++;
  if (!float_arg1(1))
  {
    return error_code;
  }

  error_code++;
  if (!float_arg2(1, 2))
  {
    return error_code;
  }

  error_code++;
  if (!float_arg3(1, 2, 3))
  {
    return error_code;
  }

  error_code++;
  if (!float_arg4(1, 2, 3, 4))
  {
    return error_code;
  }

  error_code++;
  if (!float_arg5(1, 2, 3, 4, 5))
  {
    return error_code;
  }

  error_code++;
  if (!float_arg6(1, 2, 3, 4, 5, 6))
  {
    return error_code;
  }

  error_code++;
  if (!float_arg7(1, 2, 3, 4, 5, 6, 7))
  {
    return error_code;
  }

  error_code++;
  if (!float_arg8(1, 2, 3, 4, 5, 6, 7, 8))
  {
    return error_code;
  }

  error_code++;
  if (!float_arg9(1, 2, 3, 4, 5, 6, 7, 8, 9))
  {
    return error_code;
  }

  error_code++;
  if (!float_arg10(1, 2, 3, 4, 5, 6, 7, 8, 9, 10))
  {
    return error_code;
  }

  error_code++;
  if (float_return(1.0f) != 1.0f)
  {
    return error_code;
  }
  
  return 0;
}
