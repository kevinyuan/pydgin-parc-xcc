//========================================================================
// GCD Test
//========================================================================
// The ubiguitous greatest common divsor test. We implement Euclid's
// algorithm iteratively with substraction, iteratively with division,
// and recursively with division and then compare the results to a known
// good answer. If a test fails we return a number which indicates which
// test failed.

//------------------------------------------------------------------------
// gcd_itr_sub
//------------------------------------------------------------------------

int gcd_itr_sub( int a, int b )
{
  if ( a == 0 )
    return b;

  while ( b != 0 ) {
    if ( a > b )
      a = a - b;
    else
      b = b - a;
  }

  return a;
}

//------------------------------------------------------------------------
// gcd_itr_div
//------------------------------------------------------------------------

int gcd_itr_div( int a, int b )
{
  int temp;
  while ( a != 0 ) {
    temp = a;
    a = b % a;
    b = temp;
  }
  return b;
}

//------------------------------------------------------------------------
// gcd_recursive
//------------------------------------------------------------------------

int gcd_rec_div( int a, int b )
{
  if ( a == 0 )
    return b;

  return gcd_rec_div( b % a, a );
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test data and known good answers

  int n = 7;
  int vec_a[]   = { 0, 1, 13, 15, 24, 16, 24 };
  int vec_b[]   = { 0, 1, 13, 24, 15, 24, 16 };
  int vec_ref[] = { 0, 1, 13,  3,  3,  8,  8 };

  // Generate results using interative subtraction and verify

  for ( int i = 0; i < n; i++ ) {
    int res = gcd_itr_sub( vec_a[i], vec_b[i] );

    error_code++;
    if ( res != vec_ref[i] )
      return error_code;
  }

  // Generate results using interative division and verify

  for ( int i = 0; i < n; i++ ) {
    int res = gcd_itr_div( vec_a[i], vec_b[i] );

    error_code++;
    if ( res != vec_ref[i] )
      return error_code;
  }

  // Generate results using recursive division and verify

  for ( int i = 0; i < n; i++ ) {
    int res = gcd_rec_div( vec_a[i], vec_b[i] );

    error_code++;
    if ( res != vec_ref[i] )
      return error_code;
  }

  // If all tests passed return zero

  return 0;
}

