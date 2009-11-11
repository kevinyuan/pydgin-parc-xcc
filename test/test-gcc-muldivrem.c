//========================================================================
// Test updated integer multiply, divide, and remainder
//========================================================================

//------------------------------------------------------------------------
// Unsigned Multiplication
//------------------------------------------------------------------------

__attribute__ ((noinline))
unsigned int mul_uint( unsigned int src0, unsigned int src1 )
{
  return src0 * src1;
}

__attribute__ ((noinline))
unsigned short mul_ushort( unsigned short src0, unsigned short src1 )
{
  return src0 * src1;
}

__attribute__ ((noinline))
unsigned char mul_uchar( unsigned char src0, unsigned char src1 )
{
  return src0 * src1;
}

__attribute__ ((noinline))
unsigned long long mul_ulonglong( unsigned long long src0,
                                  unsigned long long src1 )
{
  return src0 * src1;
}

//------------------------------------------------------------------------
// Signed Multiplication
//------------------------------------------------------------------------

__attribute__ ((noinline))
int mul_int( int src0, int src1 )
{
  return src0 * src1;
}

__attribute__ ((noinline))
short mul_short( short src0, short src1 )
{
  return src0 * src1;
}

__attribute__ ((noinline))
signed char mul_char( signed char src0, signed char src1 )
{
  return src0 * src1;
}

__attribute__ ((noinline))
long long mul_longlong( long long src0, long long src1 )
{
  return src0 * src1;
}

//------------------------------------------------------------------------
// Unsigned Division
//------------------------------------------------------------------------

__attribute__ ((noinline))
unsigned int div_uint( unsigned int src0, unsigned int src1 )
{
  return src0 / src1;
}

__attribute__ ((noinline))
unsigned short div_ushort( unsigned short src0, unsigned short src1 )
{
  return src0 / src1;
}

__attribute__ ((noinline))
unsigned char div_uchar( unsigned char src0, unsigned char src1 )
{
  return src0 / src1;
}

__attribute__ ((noinline))
unsigned long long div_ulonglong( unsigned long long src0,
                                  unsigned long long src1 )
{
  return src0 / src1;
}

//------------------------------------------------------------------------
// Signed Division
//------------------------------------------------------------------------

__attribute__ ((noinline))
int div_int( int src0, int src1 )
{
  return src0 / src1;
}

__attribute__ ((noinline))
short div_short( short src0, short src1 )
{
  return src0 / src1;
}

__attribute__ ((noinline))
char div_char( char src0, char src1 )
{
  return src0 / src1;
}

__attribute__ ((noinline))
long long div_longlong( long long src0, long long src1 )
{
  return src0 / src1;
}

//------------------------------------------------------------------------
// Unsigned Remainder
//------------------------------------------------------------------------

__attribute__ ((noinline))
unsigned int rem_uint( unsigned int src0, unsigned int src1 )
{
  return src0 % src1;
}

__attribute__ ((noinline))
unsigned short rem_ushort( unsigned short src0, unsigned short src1 )
{
  return src0 % src1;
}

__attribute__ ((noinline))
unsigned char rem_uchar( unsigned char src0, unsigned char src1 )
{
  return src0 % src1;
}

__attribute__ ((noinline))
unsigned long long rem_ulonglong( unsigned long long src0,
                                  unsigned long long src1 )
{
  return src0 % src1;
}

//------------------------------------------------------------------------
// Signed Remainder
//------------------------------------------------------------------------

__attribute__ ((noinline))
int rem_int( int src0, int src1 )
{
  return src0 % src1;
}

__attribute__ ((noinline))
short rem_short( short src0, short src1 )
{
  return src0 % src1;
}

__attribute__ ((noinline))
char rem_char( char src0, char src1 )
{
  return src0 % src1;
}

__attribute__ ((noinline))
long long rem_longlong( long long src0, long long src1 )
{
  return src0 % src1;
}

//------------------------------------------------------------------------
// Helper Macro
//------------------------------------------------------------------------

#define DO_TEST( func_, src0_, src1_, dest_ )                           \
  error_code++;                                                         \
  if ( func_ ( src0_, src1_ ) != dest_ )                                \
    return error_code;                                                  \

//------------------------------------------------------------------------
// Test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Unsigned multiplication tests

  DO_TEST( mul_uint, 0, 0, 0 );
  DO_TEST( mul_uint, 0, 1, 0 );
  DO_TEST( mul_uint, 1, 0, 0 );
  DO_TEST( mul_uint, 1, 1, 1 );
  DO_TEST( mul_uint, 2, 2, 4 );

  DO_TEST( mul_ushort, 0, 0, 0 );
  DO_TEST( mul_ushort, 0, 1, 0 );
  DO_TEST( mul_ushort, 1, 0, 0 );
  DO_TEST( mul_ushort, 1, 1, 1 );
  DO_TEST( mul_ushort, 2, 2, 4 );

  DO_TEST( mul_uchar, 0, 0, 0 );
  DO_TEST( mul_uchar, 0, 1, 0 );
  DO_TEST( mul_uchar, 1, 0, 0 );
  DO_TEST( mul_uchar, 1, 1, 1 );
  DO_TEST( mul_uchar, 2, 2, 4 );

  DO_TEST( mul_ulonglong, 0, 0, 0 );
  DO_TEST( mul_ulonglong, 0, 1, 0 );
  DO_TEST( mul_ulonglong, 1, 0, 0 );
  DO_TEST( mul_ulonglong, 1, 1, 1 );
  DO_TEST( mul_ulonglong, 2, 2, 4 );

  // Signed multiplication tests

  DO_TEST( mul_int,  2,  2,  4 );
  DO_TEST( mul_int,  2, -2, -4 );
  DO_TEST( mul_int, -2,  2, -4 );
  DO_TEST( mul_int, -2, -2,  4 );

  DO_TEST( mul_short,  2,  2,  4 );
  DO_TEST( mul_short,  2, -2, -4 );
  DO_TEST( mul_short, -2,  2, -4 );
  DO_TEST( mul_short, -2, -2,  4 );

  DO_TEST( mul_char,  2,  2,  4 );
  DO_TEST( mul_char,  2, -2, -4 );
  DO_TEST( mul_char, -2,  2, -4 );
  DO_TEST( mul_char, -2, -2,  4 );

  DO_TEST( mul_longlong,  2,  2,  4 );
  DO_TEST( mul_longlong,  2, -2, -4 );
  DO_TEST( mul_longlong, -2,  2, -4 );
  DO_TEST( mul_longlong, -2, -2,  4 );

  // Unsigned division tests

  DO_TEST( div_uint, 1,  1, 1 );
  DO_TEST( div_uint, 4,  2, 2 );
  DO_TEST( div_uint, 12, 3, 4 );
  DO_TEST( div_uint, 12, 5, 2 );

  DO_TEST( div_ushort, 1,  1, 1 );
  DO_TEST( div_ushort, 4,  2, 2 );
  DO_TEST( div_ushort, 12, 3, 4 );
  DO_TEST( div_ushort, 12, 5, 2 );

  DO_TEST( div_uchar, 1,  1, 1 );
  DO_TEST( div_uchar, 4,  2, 2 );
  DO_TEST( div_uchar, 12, 3, 4 );
  DO_TEST( div_uchar, 12, 5, 2 );

  DO_TEST( div_ulonglong, 1,  1, 1 );
  DO_TEST( div_ulonglong, 4,  2, 2 );
  DO_TEST( div_ulonglong, 12, 3, 4 );
  DO_TEST( div_ulonglong, 12, 5, 2 );

  // Signed division tests

  DO_TEST( div_int,  1,   1,  1 );
  DO_TEST( div_int,  1,   1,  1 );
  DO_TEST( div_int,  4,   2,  2 );
  DO_TEST( div_int,  12,  5,  2 );

  DO_TEST( div_short,  1,  1,  1 );
  DO_TEST( div_short,  4,  2,  2 );
  DO_TEST( div_short,  12,  5,  2 );

  DO_TEST( div_char,  1,  1,  1 );
  DO_TEST( div_char,  4,  2,  2 );
  DO_TEST( div_char,  12,  5,  2 );

  DO_TEST( div_longlong,  1,  1,  1 );
  DO_TEST( div_longlong,  4,  2,  2 );
  DO_TEST( div_longlong,  12,  5,  2 );

  // Unsigned remainder tests

  DO_TEST( rem_uint, 1,  1, 0 );
  DO_TEST( rem_uint, 12, 7, 5 );
  DO_TEST( rem_uint, 10, 3, 1 );

  DO_TEST( rem_ushort, 1,  1, 0 );
  DO_TEST( rem_ushort, 12, 7, 5 );
  DO_TEST( rem_ushort, 10, 3, 1 );

  DO_TEST( rem_uchar, 1,  1, 0 );
  DO_TEST( rem_uchar, 12, 7, 5 );
  DO_TEST( rem_uchar, 10, 3, 1 );

  DO_TEST( rem_ulonglong, 1,  1, 0 );
  DO_TEST( rem_ulonglong, 12, 7, 5 );
  DO_TEST( rem_ulonglong, 10, 3, 1 );

  // Signed remainder tests

  DO_TEST( rem_int,   1,  1,  0 );
  DO_TEST( rem_int,  12,  7,  5 );
  DO_TEST( rem_int,  10,  3,  1 );

  DO_TEST( rem_short, 1,  1, 0 );
  DO_TEST( rem_short, 12, 7, 5 );
  DO_TEST( rem_short, 10, 3, 1 );

  DO_TEST( rem_char, 1,  1, 0 );
  DO_TEST( rem_char, 12, 7, 5 );
  DO_TEST( rem_char, 10, 3, 1 );

  DO_TEST( rem_longlong, 1,  1, 0 );
  DO_TEST( rem_longlong, 12, 7, 5 );
  DO_TEST( rem_longlong, 10, 3, 1 );

  // Passed all tests, return zero

  return 0;
}


