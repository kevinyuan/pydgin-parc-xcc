//========================================================================
// File I/O Test
//========================================================================
// Test basic file I/O to see if the syscalls are working.

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>

//------------------------------------------------------------------------
// vec_init
//------------------------------------------------------------------------

std::vector<int> vec_init( int val0, int val1, int val2, int val3, 
                           int val4, int val5 )
{
  std::vector<int> vec(6);
  vec.at(0) = val0; vec.at(1) = val1; vec.at(2) = val2;
  vec.at(3) = val3; vec.at(4) = val4; vec.at(5) = val5;
  return vec;
}

//------------------------------------------------------------------------
// vec_write
//------------------------------------------------------------------------

__attribute__ ((noinline)) 
void vec_write( std::ofstream* file, const std::vector<int>& vec )
{
  *file << "{ ";
  for ( int i = 0; i < static_cast<int>(vec.size()); i++ ) {
    *file << vec.at(i);
    if ( i != static_cast<int>(vec.size())-1 )
      *file << ", ";
  }
  *file << " }" << std::endl;
}

//------------------------------------------------------------------------
// vec_read
//------------------------------------------------------------------------

__attribute__ ((noinline)) 
void vec_read( std::ifstream* file, std::vector<int>* vec )
{
  std::string delim;
  *file >> delim;
  if ( delim != "{" )
    throw std::runtime_error("Invalid opening delimiter");

  int element;
  while ( delim != "}" ) {
    *file >> element >> delim;
    vec->push_back(element);
  }
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test data and known good answers

  std::string test_file_name = "test-gcc-fileio.data";

  typedef std::vector<int> vint;
  vint vec_a = vec_init(  0,  1,  2,  3,  5,   8 );
  vint vec_b = vec_init( 13, 21, 34, 55, 89, 144 );

  // Open new file, write vector of data, close file

  std::ofstream file0;
  file0.open( test_file_name.c_str() );

  error_code++;
  if ( !file0.is_open() )
    return error_code;

  vec_write( &file0, vec_a );

  file0.close();

  // Open file, read/verify vector of data, close file

  std::ifstream file1;
  file1.open( test_file_name.c_str() );

  error_code++;
  if ( !file1.is_open() )
    return error_code;

  vint vec_read_0;
  vec_read( &file1, &vec_read_0 );

  for ( int i = 0; i < static_cast<int>(vec_read_0.size()); i++ ) {
    error_code++;
    if ( vec_read_0.at(i) != vec_a.at(i) )
      return error_code;
  }

  file1.close();

  // Open file, append vector of data, close file

  std::ofstream file2;
  file2.open( test_file_name.c_str(), std::ios::app );

  error_code++;
  if ( !file2.is_open() )
    return error_code;

  vec_write( &file2, vec_b );

  file2.close();

  // Open file, read/verify vector of data, close file

  std::ifstream file3;
  file3.open( test_file_name.c_str() );

  error_code++;
  if ( !file3.is_open() )
    return error_code;

  vint vec_read_1;
  vec_read( &file3, &vec_read_1 );

  for ( int i = 0; i < static_cast<int>(vec_read_1.size()); i++ ) {
    error_code++;
    if ( vec_read_1.at(i) != vec_a.at(i) )
      return error_code;
  }

  vint vec_read_2;
  vec_read( &file3, &vec_read_2 );

  for ( int i = 0; i < static_cast<int>(vec_read_2.size()); i++ ) {
    error_code++;
    if ( vec_read_2.at(i) != vec_b.at(i) )
      return error_code;
  }

  file3.close();

  // Delete the file, try to open it and see if there is an error

  error_code++;
  if ( remove( test_file_name.c_str() ) != 0 )
    return error_code;

  std::ifstream file4;
  file4.open( test_file_name.c_str() );

  error_code++;
  if ( file4.is_open() )
    return error_code;
  
  // If all tests passed return zero

  return 0;
}

