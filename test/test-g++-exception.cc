//========================================================================
// File I/O Test
//========================================================================
// Test basic file I/O to see if the syscalls are working.

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>

//------------------------------------------------------------------------
// throw_exception
//------------------------------------------------------------------------

__attribute__ ((noinline)) 
void throw_runtime_error()
{
  throw std::runtime_error("runtime_error");
}

__attribute__ ((noinline)) 
void throw_logic_error()
{
  throw std::logic_error("logic_error");
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{

  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Basic throw/catch

  bool caught_exception_0 = false;
  try {
    throw_runtime_error();
  } 
  catch ( std::runtime_error& exception ) {
    caught_exception_0 = true;
  }

  error_code++;
  if ( !caught_exception_0 )
    return error_code;

  // Selective catch

  bool caught_exception_1 = false;
  bool caught_exception_2 = false;
  try {
    throw_runtime_error();
  } 
  catch ( std::logic_error& exception ) {
    caught_exception_1 = true;
  }
  catch ( std::runtime_error& exception ) {
    caught_exception_2 = true;
  }

  error_code++;
  if ( caught_exception_1 || !caught_exception_2 )
    return error_code;

  // Nested throw/catch/rethrow/catch

  bool caught_exception_3 = false;
  bool caught_exception_4 = false;
  try {
    try {
      throw_runtime_error();
    }
    catch ( std::runtime_error& exception ) {
      caught_exception_3 = true;
      throw_logic_error();
    }
  } 
  catch ( std::logic_error& exception ) {
    caught_exception_4 = true;
  }

  error_code++;
  if ( !caught_exception_3 || !caught_exception_4 )
    return error_code;
  
  // If all tests passed return zero

  return 0;
}

