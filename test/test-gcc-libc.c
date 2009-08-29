//========================================================================
// Simple test of libc
//========================================================================
// This test sorts an array of strings and then assembles a new string
// for displaying the sorted array. It acts as a primitive test to make
// sure at least some of libc is working. If a test fails we return a
// number which indicates which test failed. Theoretically none of these
// library calls will end up calling the libgloss routines except for
// sbrk, so this test should work even with a stubbed out version of
// libgloss.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//------------------------------------------------------------------------
// Integer comparison
//------------------------------------------------------------------------

int int_cmp( const void* a, const void* b )
{
  const int* ia = (const int*) a;
  const int* ib = (const int*) b;
  return *ia - *ib;
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test data and known good answers

  int n = 10;
  int vec_int_in[]  =  { 29, 61, 18, 21, 80, 10, 67, 19, 37, 53 };
  int vec_int_ref[] =  { 10, 18, 19, 21, 29, 37, 53, 61, 67, 80 };
  char* vec_str_ref = "{ 10, 18, 19, 21, 29, 37, 53, 61, 67, 80 }";

  // Sort the input array

  qsort( vec_int_in, n, sizeof(int), int_cmp ); 

  // Verify the sorted order

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_int_in[i] != vec_int_ref[i] )
      return error_code;
  }

  // Create display string for sorted array

  char vec_str[100];
  strcpy( vec_str, "{ " );
  for ( int i = 0; i < n; i++ ) {

    char str[20];
    if ( i != n-1 )
      sprintf( str, "%d, ", vec_int_in[i] );
    else
      sprintf( str, "%d", vec_int_in[i] );

    strcat( vec_str, str );
  }
  strcat( vec_str, " }" );

  // Verify display string

  error_code++;
  if ( strcmp( vec_str, vec_str_ref ) != 0 )
    return error_code;

  // If all tests passed return zero

  return 0;
}

