//========================================================================
// Double Floating-Point Arithmetic
//========================================================================
// Simple test which does double floating-point arithmetic

#define TRUE 1
#define FALSE 0

#define EPSILON 0.0001
int double_equal(double a, double b)
{
  double s = a - b;

  return (-EPSILON < s) && (s < EPSILON);
}

__attribute__ ((noinline))
int double_add()
{
  int n = 2;
  double vec_a[]   = {0.1, 10.0};
  double vec_b[]   = {0.2, -0.7};
  double vec_ref[] = {0.3, 9.3};

  for (int i=0; i<n; i++)
  {
    double sum = vec_a[i] + vec_b[i];

    if (!double_equal(sum, vec_ref[i]))
      return FALSE;
  }

  return TRUE;
}

__attribute__ ((noinline))
int double_sub()
{
  int n = 2;
  double vec_a[]   = {0.1, 10.0};
  double vec_b[]   = {0.2, -0.7};
  double vec_ref[] = {-0.1, 10.7};

  for (int i=0; i<n; i++)
  {
    double sub = vec_a[i] - vec_b[i];

    if (!double_equal(sub, vec_ref[i]))
      return FALSE;
  }

  return TRUE;
}

__attribute__ ((noinline))
int double_mul()
{
  int n = 2;
  double vec_a[]   = {0.1, 10.0};
  double vec_b[]   = {0.02, -0.7};
  double vec_ref[] = {0.002, -7.0};

  for (int i=0; i<n; i++)
  {
    double sub = vec_a[i] * vec_b[i];

    if (!double_equal(sub, vec_ref[i]))
      return FALSE;
  }

  return TRUE;
}

int test()
{
  int error_code = 0;

  error_code++;
  if (!double_add())
  {
    return error_code;
  }

  error_code++;
  if (!double_sub())
  {
    return error_code;
  }

  error_code++;
  if (!double_mul())
  {
    return error_code;
  }

  return 0;
}
