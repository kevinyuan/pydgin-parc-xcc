//========================================================================
// Integer Arguments
//========================================================================
// Since we are using EABI which has 8 argument registers, we need to make
// sure that it works.  This test focuses on integer arguments.

__attribute__ ((noinline))
int int_arg0()
{
  return 1;
}

__attribute__ ((noinline))
int int_arg1(int a0)
{
  return a0 == 1;
}

__attribute__ ((noinline))
int int_arg2(int a0, int a1)
{
  return (a0 + a1) == (1 + 2);
}

__attribute__ ((noinline))
int int_arg3(int a0, int a1, int a2)
{
  return (a0 + a1 + a2) == (1 + 2 + 3);
}

__attribute__ ((noinline))
int int_arg4(int a0, int a1, int a2, int a3)
{
  return (a0 + a1 + a2 + a3) == (1 + 2 + 3 + 4);
}

__attribute__ ((noinline))
int int_arg5(int a0, int a1, int a2, int a3, int a4)
{
  return (a0 + a1 + a2 + a3 + a4) == (1 + 2 + 3 + 4 + 5);
}

__attribute__ ((noinline))
int int_arg6(int a0, int a1, int a2, int a3, int a4, int a5)
{
  return (a0 + a1 + a2 + a3 + a4 + a5) == (1 + 2 + 3 + 4 + 5 + 6);
}

__attribute__ ((noinline))
int int_arg7(int a0, int a1, int a2, int a3, int a4, int a5, int a6)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6) == (1 + 2 + 3 + 4 + 5 + 6 + 7);
}

__attribute__ ((noinline))
int int_arg8(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8);
}

__attribute__ ((noinline))
int int_arg9(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9);
}

__attribute__ ((noinline))
int int_arg10(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10);
}

int test()
{
  int error_code = 0;

  error_code++;
  if (!int_arg0())
  {
    return error_code;
  }

  error_code++;
  if (!int_arg1(1))
  {
    return error_code;
  }

  error_code++;
  if (!int_arg2(1, 2))
  {
    return error_code;
  }

  error_code++;
  if (!int_arg3(1, 2, 3))
  {
    return error_code;
  }

  error_code++;
  if (!int_arg4(1, 2, 3, 4))
  {
    return error_code;
  }

  error_code++;
  if (!int_arg5(1, 2, 3, 4, 5))
  {
    return error_code;
  }

  error_code++;
  if (!int_arg6(1, 2, 3, 4, 5, 6))
  {
    return error_code;
  }

  error_code++;
  if (!int_arg7(1, 2, 3, 4, 5, 6, 7))
  {
    return error_code;
  }

  error_code++;
  if (!int_arg8(1, 2, 3, 4, 5, 6, 7, 8))
  {
    return error_code;
  }

  error_code++;
  if (!int_arg9(1, 2, 3, 4, 5, 6, 7, 8, 9))
  {
    return error_code;
  }

  error_code++;
  if (!int_arg10(1, 2, 3, 4, 5, 6, 7, 8, 9, 10))
  {
    return error_code;
  }
  
  return 0;
}
