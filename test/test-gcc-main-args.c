//========================================================================
// Test main's argc and argv 
//========================================================================
// This little test verifies that the arguments are being correctly
// passed into main. We assume that the source is compiled into an
// executable named "test-gcc-main-args" and that it is called like
// this:
//
//  test-gcc-main-args apple pear mango
//

#include <string.h>

int main( int argc, char* argv[] )
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Verify that argc is correct

  error_code++;
  if ( argc != 4 )
    return error_code;

  // Find just the executable's name without any path

  char* exe_name = strrchr( argv[0], '/' );
  if ( exe_name == 0 )
    exe_name = argv[0];
  else
    exe_name++;

  // Verify first argument which is the program name

  error_code++;
  if ( strcmp( exe_name, "test-gcc-main-args" ) != 0 )
    return error_code;

  // Verify remaining arguments 

  char* argv_ref[] = { "apple", "pear", "mango" };
  
  for ( int i = 1; i < argc; i++ ) {
    error_code++;
    if ( strcmp( argv[i], argv_ref[i-1] ) != 0 )
      return error_code;
  }
  
  // Verify that argv ends with a null pointer

  error_code++;
  if ( argv[argc] != 0 )
    return error_code;

  return 0;
}
