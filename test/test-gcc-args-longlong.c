//========================================================================
// Long Long Arguments
//========================================================================
// Since we are using EABI which has 8 argument registers, we need to make
// sure that it works.  This test focuses on long long arguments.

__attribute__ ((noinline))
int longlong_arg0()
{
  return 1;
}

__attribute__ ((noinline))
int longlong_arg1(long long a0)
{
  return a0 == 1;
}

__attribute__ ((noinline))
int longlong_arg2(long long a0, long long a1)
{
  return (a0 + a1) == (1 + 2);
}

__attribute__ ((noinline))
int longlong_arg3(long long a0, long long a1, long long a2)
{
  return (a0 + a1 + a2) == (1 + 2 + 3);
}

__attribute__ ((noinline))
int longlong_arg4(long long a0, long long a1, long long a2, long long a3)
{
  return (a0 + a1 + a2 + a3) == (1 + 2 + 3 + 4);
}

__attribute__ ((noinline))
int longlong_arg5(long long a0, long long a1, long long a2, long long a3, long long a4)
{
  return (a0 + a1 + a2 + a3 + a4) == (1 + 2 + 3 + 4 + 5);
}

__attribute__ ((noinline))
int longlong_arg6(long long a0, long long a1, long long a2, long long a3, long long a4, long long a5)
{
  return (a0 + a1 + a2 + a3 + a4 + a5) == (1 + 2 + 3 + 4 + 5 + 6);
}

__attribute__ ((noinline))
int longlong_arg7(long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6) == (1 + 2 + 3 + 4 + 5 + 6 + 7);
}

__attribute__ ((noinline))
int longlong_arg8(long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6, long long a7)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8);
}

__attribute__ ((noinline))
int longlong_arg9(long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6, long long a7, long long a8)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9);
}

__attribute__ ((noinline))
int longlong_arg10(long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6, long long a7, long long a8, long long a9)
{
  return (a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) == (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10);
}

__attribute__ ((noinline))
long long longlong_return(long long a0)
{
  return a0;
}

int test()
{
  int error_code = 0;

  error_code++;
  if (!longlong_arg0())
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg1(1))
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg2(1, 2))
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg3(1, 2, 3))
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg4(1, 2, 3, 4))
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg5(1, 2, 3, 4, 5))
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg6(1, 2, 3, 4, 5, 6))
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg7(1, 2, 3, 4, 5, 6, 7))
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg8(1, 2, 3, 4, 5, 6, 7, 8))
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg9(1, 2, 3, 4, 5, 6, 7, 8, 9))
  {
    return error_code;
  }

  error_code++;
  if (!longlong_arg10(1, 2, 3, 4, 5, 6, 7, 8, 9, 10))
  {
    return error_code;
  }

  error_code++;
  if (longlong_return(1) != 1LL)
  {
    return error_code;
  }
  
  return 0;
}
