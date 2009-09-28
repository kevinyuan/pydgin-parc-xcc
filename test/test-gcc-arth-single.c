//========================================================================
// Single Floating-Point Arithmetic
//========================================================================
// Simple test which does single floating-point arithmetic

#define TRUE 1
#define FALSE 0

#define EPSILON 0.0001f
int float_equal(float a, float b)
{
  float s = a - b;

  return (-EPSILON < s) && (s < EPSILON);
}

__attribute__ ((noinline))
int single_add()
{
  int n = 2;
  float vec_a[]   = {0.1, 10.0};
  float vec_b[]   = {0.2, -0.7};
  float vec_ref[] = {0.3, 9.3};

  for (int i=0; i<n; i++)
  {
    float sum = vec_a[i] + vec_b[i];

    if (!float_equal(sum, vec_ref[i]))
      return FALSE;
  }

  return TRUE;
}

__attribute__ ((noinline))
int single_sub()
{
  int n = 2;
  float vec_a[]   = {0.1, 10.0};
  float vec_b[]   = {0.2, -0.7};
  float vec_ref[] = {-0.1, 10.7};

  for (int i=0; i<n; i++)
  {
    float sub = vec_a[i] - vec_b[i];

    if (!float_equal(sub, vec_ref[i]))
      return FALSE;
  }

  return TRUE;
}

__attribute__ ((noinline))
int single_mul()
{
  int n = 2;
  float vec_a[]   = {0.1, 10.0};
  float vec_b[]   = {0.02, -0.7};
  float vec_ref[] = {0.002, -7.0};

  for (int i=0; i<n; i++)
  {
    float sub = vec_a[i] * vec_b[i];

    if (!float_equal(sub, vec_ref[i]))
      return FALSE;
  }

  return TRUE;
}

int test()
{
  int error_code = 0;

  error_code++;
  if (!single_add())
  {
    return error_code;
  }

  error_code++;
  if (!single_sub())
  {
    return error_code;
  }

  error_code++;
  if (!single_mul())
  {
    return error_code;
  }

  return 0;
}
