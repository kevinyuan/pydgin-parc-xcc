//========================================================================
// File I/O Test
//========================================================================
// Test basic file I/O to see if the syscalls are working.

#include <stdio.h>

//------------------------------------------------------------------------
// vec_write
//------------------------------------------------------------------------

__attribute__ ((noinline)) 
int vec_write( FILE* file, int* vec, int n )
{
  // Write opening delimiter
  if ( fprintf( file, "{ " ) == -1 )
    return 0;

  for ( int i = 0; i < n; i++ ) {

    // Write element
    if ( fprintf( file, "%d", vec[i] ) == -1 )
      return 0;

    // Write per-element delimieter
    if ( i != n-1 ) {
      if ( fprintf( file, ", " ) == -1 )
        return 0;
    }

  }

  // Write closing delimiter
  if ( fprintf( file, " }\n" ) == -1 )
    return 0;

  return 1;
}

//------------------------------------------------------------------------
// vec_read
//------------------------------------------------------------------------

__attribute__ ((noinline)) 
int vec_read( FILE* file, int* vec, int n )
{
  // Read opening delimiter
  if ( fscanf( file, "{ " ) == -1 )
    return 0;

  for ( int i = 0; i < n; i++ ) {

    // Read element
    if ( fscanf( file, "%d", &vec[i] ) == -1 )
      return 0;

    // Read per-element delimieter
    if ( i != n-1 ) {
      if ( fscanf( file, ", " ) == -1 )
        return 0;
    }

  }

  // Read closing delimiter
  if ( fscanf( file, " }\n" ) == -1 )
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

  // Test data and known good answers

  char* test_file_name_0 = "test-gcc-fileio-0.data";
  char* test_file_name_1 = "test-gcc-fileio-1.data";

  int n = 6;
  int vec_a[] = {  0,  1,  2,  3,  5,   8 };
  int vec_b[] = { 13, 21, 34, 55, 89, 144 };

  // Open new file, write vector of data, close file

  FILE* file0 = fopen( test_file_name_0, "w" );

  error_code++;
  if ( file0 == 0 )
    return error_code;

  error_code++;
  if ( !vec_write( file0, vec_a, n ) )
    return error_code;

  error_code++;
  if ( fclose(file0) )
    return error_code;

  // Open file, read/verify vector of data, close file

  int vec_read_0[n];
  FILE* file1 = fopen( test_file_name_0, "r" );

  error_code++;
  if ( file1 == 0 )
    return error_code;

  error_code++;
  if ( !vec_read( file1, vec_read_0, n ) )
    return error_code;

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_read_0[i] != vec_a[i] )
      return error_code;
  }

  error_code++;
  if ( fclose(file1) )
    return error_code;

  // Open file, append vector of data, close file

  FILE* file2 = fopen( test_file_name_0, "a" );

  error_code++;
  if ( file2 == 0 )
    return error_code;

  error_code++;
  if ( !vec_write( file2, vec_b, n ) )
    return error_code;

  error_code++;
  if ( fclose(file2) )
    return error_code;

  // Rename file, make sure cannot open old file name

  error_code;
  if ( rename( test_file_name_0, test_file_name_1 ) == -1 )
    return error_code;

  FILE* file3 = fopen( test_file_name_0, "r" );

  error_code++;
  if ( file3 != 0 )
    return error_code;

  // Open file, read/verify vector of data, close file

  int vec_read_1[n];
  FILE* file4 = fopen( test_file_name_1, "r" );

  error_code++;
  if ( file4 == 0 )
    return error_code;

  error_code++;
  if ( !vec_read( file4, vec_read_1, n ) )
    return error_code;

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_read_1[i] != vec_a[i] )
      return error_code;
  }

  error_code++;
  if ( !vec_read( file4, vec_read_1, n ) )
    return error_code;

  for ( int i = 0; i < n; i++ ) {
    error_code++;
    if ( vec_read_1[i] != vec_b[i] )
      return error_code;
  }

  error_code++;
  if ( fclose(file4) )
    return error_code;

  // Delete the file, try to open it and see if there is an error

  error_code++;
  if ( remove( test_file_name_1 ) != 0 )
    return error_code;

  FILE* file5 = fopen( test_file_name_1, "r" );

  error_code++;
  if ( file5 != 0 )
    return error_code;
  
  // If all tests passed return zero

  return 0;
}

