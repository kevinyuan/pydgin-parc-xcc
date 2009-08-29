//========================================================================
// Static Initializer Test
//========================================================================
// Static initializers rely on correctly setting up the linker script so
// we do a quick test here and make sure it works.

//------------------------------------------------------------------------
// Initialize some data
//------------------------------------------------------------------------

int   s_num   = 42;
char* s_str   = "foobar";
int   s_vec[] = { 0, 1, 2, 3, 5, 8 };

//------------------------------------------------------------------------
// Compare functions
//------------------------------------------------------------------------
// We want to make sure the compiler doesn't optimize everything away.

__attribute__ ((noinline))
int cmp_num( int a, int b )
{
  return ( a == b );
}

__attribute__ ((noinline))
int cmp_str( char* str_a, char* str_b )
{
  while ( (*str_a != '\0') && (*str_b != '\0') )
    if ( *(str_a++) != *(str_b++) )
      return 0;

  return 1;
}

__attribute__ ((noinline))
int cmp_vec( int vec_a[], int vec_b[], int n )
{
  for ( int i = 0; i < n; i++ )
    if ( vec_a[i] != vec_b[i] )
      return 0;

  return 1;
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test static integer initialization

  error_code++;
  if ( !cmp_num( s_num, 42 ) )
    return error_code;

  // Test static character string initialization

  error_code++;
  if ( !cmp_str( s_str, "foobar" ) )
    return error_code;

  // Test static array inialization

  int n = 6;
  int vec[] = { 0, 1, 2, 3, 5, 8 };

  error_code++;
  if ( !cmp_vec( s_vec, vec, n ) )
    return error_code;

  // If all tests passed return zero

  return 0;
}

