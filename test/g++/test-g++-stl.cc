//========================================================================
// Simple test of the STL
//========================================================================
// This test sorts an array of strings and then assembles a new string
// for displaying the sorted array. It acts as a primitive test to make
// sure at least some of the STL is working. If a test fails we return a
// number which indicates which test failed.

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

//------------------------------------------------------------------------
// mk_vec
//------------------------------------------------------------------------

template < typename T >
std::vector<T> mk_vec( const T& val0, const T& val1, const T& val2, 
                       const T& val3, const T& val4, const T& val5, 
                       const T& val6, const T& val7, const T& val8, 
                       const T& val9 )
{
  std::vector<T> vec(10);
  vec.at(0) = val0; vec.at(1) = val1; vec.at(2) = val2;
  vec.at(3) = val3; vec.at(4) = val4; vec.at(5) = val5;
  vec.at(6) = val6; vec.at(7) = val7; vec.at(8) = val8;
  vec.at(9) = val9;
  return vec;
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test data and known good answers

  typedef std::vector<int> vint;
  vint vec_int_in  =   mk_vec( 29, 61, 18, 21, 80, 10, 67, 19, 37, 53 );
  vint vec_int_ref =   mk_vec( 10, 18, 19, 21, 29, 37, 53, 61, 67, 80 );
  std::string vec_str_ref = "{ 10, 18, 19, 21, 29, 37, 53, 61, 67, 80 }";

  // Sort the input array

  std::sort( vec_int_in.begin(), vec_int_in.end() );

  // Verify the sorted order

  for ( int i = 0; i < static_cast<int>(vec_int_in.size()); i++ ) {
    error_code++;
    if ( vec_int_in[i] != vec_int_ref[i] )
      return error_code;
  }

  // Create display string for sorted array

  std::ostringstream ost;
  ost << "{ ";
  for ( int i = 0; i < static_cast<int>(vec_int_in.size()); i++ ) {
    ost << vec_int_in[i];
    if ( i != static_cast<int>(vec_int_in.size())-1 )
      ost << ", ";
  }
  ost << " }";

  // Verify display string

  error_code++;
  if ( ost.str() != vec_str_ref )
    return error_code;

  // If all tests passed return zero

  return 0;
}

