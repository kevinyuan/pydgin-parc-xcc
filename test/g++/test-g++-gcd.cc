//========================================================================
// GCD Test
//========================================================================
// The ubiguitous greatest common divsor test. We implement Euclid's
// algorithm iteratively with substraction, iteratively with division,
// and recursively with division and then compare the results to a known
// good answer. Since we already have a basic GCD test written in C, we
// try to use more C++ language features such as virtual functions and
// templates in this test. If a test fails we return a number which
// indicates which test failed.

//------------------------------------------------------------------------
// IGcdImpl
//------------------------------------------------------------------------

class IGcdImpl
{
 public:
  virtual ~IGcdImpl() { }
  virtual int operator()( int a, int b ) const = 0;
};

//------------------------------------------------------------------------
// GcdItrSub
//------------------------------------------------------------------------

class GcdItrSub : public IGcdImpl
{
 public:
  virtual ~GcdItrSub() { }
  virtual int operator()( int a, int b ) const
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
};

//------------------------------------------------------------------------
// GcdItrDiv
//------------------------------------------------------------------------

class GcdItrDiv : public IGcdImpl
{
 public:
  virtual ~GcdItrDiv() { }
  virtual int operator()( int a, int b ) const
  {
    int temp;
    while ( a != 0 ) {
      temp = a;
      a = b % a;
      b = temp;
    }
    return b;
  }
};

//------------------------------------------------------------------------
// GcdRecDiv
//------------------------------------------------------------------------

class GcdRecDiv : public IGcdImpl
{
 public:
  virtual ~GcdRecDiv() { }
  virtual int operator()( int a, int b ) const
  {
    if ( a == 0 )
      return b;

    return operator()( b % a, a );
  }
};

//------------------------------------------------------------------------
// foreach_gcd (with templates)
//------------------------------------------------------------------------

template < typename Algo >
void foreach_gcd( int vec_a[], int vec_b[], int vec_res[], int n )
{
  Algo algo;
  for ( int i = 0; i < n; i++ )
    vec_res[i] = algo( vec_a[i], vec_b[i] );
}

//------------------------------------------------------------------------
// foreach_gcd (with virtual functions)
//------------------------------------------------------------------------

void foreach_gcd( const IGcdImpl& algo, 
                  int vec_a[], int vec_b[], int vec_res[], int n )
{
  for ( int i = 0; i < n; i++ )
    vec_res[i] = algo( vec_a[i], vec_b[i] );
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

  // Generate results with templates & interative subtraction and verify

  int vec_itr_sub_tmpl[n];
  foreach_gcd<GcdItrSub>( vec_a, vec_b, vec_itr_sub_tmpl, n );

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_itr_sub_tmpl[i] != vec_ref[i] )
      return error_code;
  }

  // Generate results with templates & interative division and verify

  int vec_itr_div_tmpl[n];
  foreach_gcd<GcdItrDiv>( vec_a, vec_b, vec_itr_div_tmpl, n );

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_itr_div_tmpl[i] != vec_ref[i] )
      return error_code;
  }

  // Generate results with templates & recursive division and verify

  int vec_rec_div_tmpl[n];
  foreach_gcd<GcdRecDiv>( vec_a, vec_b, vec_rec_div_tmpl, n );

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_rec_div_tmpl[i] != vec_ref[i] )
      return error_code;
  }

  // Generate results with vfunc & interative subtraction and verify

  int vec_itr_sub_vfunc[n];
  foreach_gcd( GcdItrSub(), vec_a, vec_b, vec_itr_sub_vfunc, n );

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_itr_sub_vfunc[i] != vec_ref[i] )
      return error_code;
  }

  // Generate results with vfunc & interative division and verify

  int vec_itr_div_vfunc[n];
  foreach_gcd( GcdItrDiv(), vec_a, vec_b, vec_itr_div_vfunc, n );

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_itr_div_vfunc[i] != vec_ref[i] )
      return error_code;
  }

  // Generate results with vfunc & recursive division and verify

  int vec_rec_div_vfunc[n];
  foreach_gcd( GcdRecDiv(), vec_a, vec_b, vec_rec_div_vfunc, n );

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_rec_div_vfunc[i] != vec_ref[i] )
      return error_code;
  }

  // If all tests passed return zero

  return 0;
}

