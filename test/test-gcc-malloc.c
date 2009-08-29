//========================================================================
// Malloc Test                                                        
//========================================================================
// Try using malloc to dynamically allocate some data. This will help
// exercise the sbrk system call and make sure that it is at least
// basically working.

#include <stdlib.h>

//------------------------------------------------------------------------
// vcopy
//------------------------------------------------------------------------
// Create a dynamically allocated copy of a given input vector.

__attribute__ ((noinline))
int* vcopy( int* vec, int n )
{
  int* vec_copy = (int*) malloc(n*sizeof(int));
  for ( int i = 0; i < n; i++ )
    vec_copy[i] = vec[i];

  return vec_copy;
}

//------------------------------------------------------------------------
// vvadd
//------------------------------------------------------------------------
// Create a dynamically allocated vector which contiains the
// element-wise sum of two input vectors.

__attribute__ ((noinline))
int* vvadd( int* vec_a, int* vec_b, int n )
{
  int* vec_out = (int*) malloc(n*sizeof(int));
  for ( int i = 0; i < n; i++ )
    vec_out[i] = vec_a[i] + vec_b[i];

  return vec_out;
}

//------------------------------------------------------------------------
// vvsub
//------------------------------------------------------------------------
// Create a dynamically allocated vector which contiains the
// element-wise subtraction of two input vectors.

__attribute__ ((noinline))
int* vvsub( int* vec_a, int* vec_b, int n )
{
  int* vec_out = (int*) malloc(n*sizeof(int));
  for ( int i = 0; i < n; i++ )
    vec_out[i] = vec_a[i] - vec_b[i];

  return vec_out;
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
  int vec_a[] = { 0, 1, 1, 2, 3,  5,  8 };
  int vec_b[] = { 0, 1, 2, 3, 5,  8, 13 };
  int vec_c[] = { 0, 2, 3, 5, 8, 13, 21 };
  
  // Create two dynamically allocated vectors
  
  int* vec_copy_0 = vcopy( vec_a, n );
  int* vec_copy_1 = vcopy( vec_b, n );

  // vvadd returns another dynamically allocated vector

  int* vec_out = vvadd( vec_copy_0, vec_copy_1, n );

  // Verify results

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_out[i] != vec_c[i] )
      return error_code;
  }

  // Free dynamically allocated vectors

  free( vec_copy_0 );
  free( vec_copy_1 );
  free( vec_out    );

  // Create more dynamically allocated vectors
  
  int* vec_copy_2 = vcopy( vec_c, n );
  vec_copy_0 = vcopy( vec_b, n );

  // vvsub returns another dynamically allocated vector

  vec_out = vvsub( vec_copy_2, vec_copy_0, n );

  // Verify results

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_out[i] != vec_a[i] )
      return error_code;
  }

  // Free dynamically allocated vectors

  free( vec_copy_2 );
  free( vec_copy_0 );
  free( vec_out    );

  return 0;
}

