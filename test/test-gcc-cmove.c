//========================================================================
// Test conditional moves
//========================================================================
// gcc will use conditional moves instead of branches for small
// conditionals so we try and see if we can get gcc to generate these
// instructions in this test.

//------------------------------------------------------------------------
// movz
//------------------------------------------------------------------------
// Without conditional moves this compiles to something like this:
//
//  00000000 <test_movz>:
//   0: move    v0, a1
//   4: beqz    a0, c <test_movz+0xc>
//   8: move    v0, a2
//   c: jr      ra
//
// With conditional moves this compiles to something like this:
//
//  00000000 <test_movz>:
//   0: move    v0, a2
//   4: movz    v0, a1, a0
//   8: jr      ra
//

__attribute__ ((noinline))
int test_movz( int sel, int a, int b )
{
  if ( sel == 0 )
    return a;
  else
    return b;
}

//------------------------------------------------------------------------
// movz_fp
//------------------------------------------------------------------------

__attribute__ ((noinline))
float test_movz_fp( float sel, float a, float b )
{
  if ( sel != 0.0f )
    return a;
  else
    return b;
}

//------------------------------------------------------------------------
// movz
//------------------------------------------------------------------------

__attribute__ ((noinline))
int test_movn( int sel, int a, int b )
{
  if ( sel != 0 )
    return a;
  else
    return b;
}

//------------------------------------------------------------------------
// movn_fp
//------------------------------------------------------------------------

__attribute__ ((noinline))
float test_movn_fp( float cmp0, float cmp1, float a, float b )
{
  if ( cmp0 < cmp1 )
    return a;
  else
    return b;
}

//------------------------------------------------------------------------
// test_overwrite_cmp
//------------------------------------------------------------------------
// Our triangle transform kernel has two small conditionals which the
// compiler was not turning into conditional moves. The original code
// looked like v0 below while a slightly different implementation is in
// v1. The big different is that in v0, src0 is both an input to the
// comparison _and_ it is overwitten by the conditional code while in v1
// src0 is only an input. Semantically these are exactly the same, but
// the if conversion in the compiler will convert v0 but it will convert
// v1. Here is what the generate code looks like:
//
//  00000000 <test_overwrite_cmp_v0>:
//    0: c.lt.s  v0, a2,a1
//    4: beqz    v0, c <test_overwrite_cmp_v0+0xc>
//    8: move    a1, a2
//    c: sw      a1, 0(a0)
//   10: jr      ra
//
//  00000014 <test_overwrite_cmp_v1>:
//   14: c.lt.s  v0, a2, a1
//   18: movn    a1, a2, v0
//   1c: sw      a1, 0(a0)
//   20: jr      ra
//

__attribute__ ((noinline))
void test_overwrite_cmp_v0( float* dest, float src0, float src1 )
{
  if ( src0 > src1 )
    src0 = src1;
  *dest = src0;
}

__attribute__ ((noinline))
void test_overwrite_cmp_v1( float* dest, float src0, float src1 )
{
  float temp = src0;
  if ( src0 > src1 )
    temp = src1;
  *dest = temp;
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test conditional move if zero

  error_code++;
  if ( test_movz( 0, 3, 5 ) != 3 )
    return error_code;

  error_code++;
  if ( test_movz( 1, 3, 5 ) != 5 )
    return error_code;

  // Test conditional move if zero (fp)

  error_code++;
  if ( test_movn_fp( 0.0f, 1.0f, 3.0f, 5.0f ) != 3.0f )
    return error_code;

  error_code++;
  if ( test_movn_fp( 1.0f, 0.0f, 3.0f, 5.0f ) != 5.0f )
    return error_code;

  // Test conditional move if non-zero

  error_code++;
  if ( test_movn( 0, 3, 5 ) != 5 )
    return error_code;

  error_code++;
  if ( test_movn( 1, 3, 5 ) != 3 )
    return error_code;

  // Test conditional move if non-zero (fp)

  error_code++;
  if ( test_movz_fp( 0.0f, 3.0f, 5.0f ) != 5.0f )
    return error_code;

  error_code++;
  if ( test_movz_fp( 1.0f, 3.0f, 5.0f ) != 3.0f )
    return error_code;

  // Test ovewrite comparison input

  error_code++;
  float dest_v0;
  test_overwrite_cmp_v0( &dest_v0, 0.0, 1.0 );
  if ( dest_v0 != 0.0 )
    return error_code;

  error_code++;
  float dest_v1;
  test_overwrite_cmp_v1( &dest_v1, 0.0, 1.0 );
  if ( dest_v1 != 0.0 )
    return error_code;

  // If all tests passed return zero

  return 0;
}


