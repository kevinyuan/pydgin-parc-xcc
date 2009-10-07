//========================================================================
// Structure with Single Floating-Point Arguments
//========================================================================
// Test passing and returning structures with a single floating point
// value (should be register allocated) or with multiple floating point
// values (should be stack allocated and a pointer passed in a
// register).

//------------------------------------------------------------------------
// fp_equal
//------------------------------------------------------------------------
// Test that two floating point numbers are close enough to consider
// equal.

int fp_equal( float num0, float num1 )
{
  return ( ((num0 - num1) < 0.001f) && ((num1 - num0) < 0.001f) );
}

//------------------------------------------------------------------------
// sm_struct_t
//------------------------------------------------------------------------
// This struct is small enough to be passed in registers.

struct sm_struct_ {
  float data;
};
typedef struct sm_struct_ sm_struct_t;

sm_struct_t sm_struct_init( float value )
{
  sm_struct_t ret;
  ret.data = value;
  return ret;
}

sm_struct_t sm_struct_add( sm_struct_t s0, sm_struct_t s1 )
{
  sm_struct_t ret;
  ret.data = s0.data + s1.data;
  return ret;
}

int sm_struct_eq( sm_struct_t s0, float value )
{
  return ( fp_equal( s0.data, value ) );
}

//------------------------------------------------------------------------
// Functions which take a variable number of arguments
//------------------------------------------------------------------------

__attribute__ ((noinline))
int sm_struct_arg1( sm_struct_t a0 )
{
  return sm_struct_eq( a0, 1.0f );
}

__attribute__ ((noinline))
int sm_struct_arg2( sm_struct_t a0, sm_struct_t a1 )
{
  return sm_struct_eq( sm_struct_add(a0,a1), 3.0f );
}

__attribute__ ((noinline))
int sm_struct_arg9( sm_struct_t a0, sm_struct_t a1, sm_struct_t a2,
                    sm_struct_t a3, sm_struct_t a4, sm_struct_t a5, 
                    sm_struct_t a6, sm_struct_t a7, sm_struct_t a8 )
{
  return sm_struct_eq( 
           sm_struct_add( a0,
             sm_struct_add( a1,
               sm_struct_add( a2,
                 sm_struct_add( a3,
                   sm_struct_add( a4,
                     sm_struct_add( a5,
                       sm_struct_add( a6,
                         sm_struct_add( a7, a8 )))))))), 45.0f );
}

__attribute__ ((noinline))
sm_struct_t sm_struct_return( sm_struct_t a0 )
{
  return a0;
}

//------------------------------------------------------------------------
// lg_struct_t
//------------------------------------------------------------------------
// This struct is larger so that it has to be passed on the stack.

struct lg_struct_ {
  float data[4];
};
typedef struct lg_struct_ lg_struct_t;

lg_struct_t lg_struct_init( float value )
{
  lg_struct_t ret;
  ret.data[0] = value;
  ret.data[1] = value;
  ret.data[2] = value;
  ret.data[3] = value;
  return ret;
}

lg_struct_t lg_struct_add( lg_struct_t s0, lg_struct_t s1 )
{
  lg_struct_t ret;
  ret.data[0] = s0.data[0] + s1.data[0];
  ret.data[1] = s0.data[1] + s1.data[1];
  ret.data[2] = s0.data[2] + s1.data[2];
  ret.data[3] = s0.data[3] + s1.data[3];
  return ret;
}

int lg_struct_eq( lg_struct_t s0, float value )
{
  return (    fp_equal( s0.data[0], value )
           && fp_equal( s0.data[1], value )
           && fp_equal( s0.data[2], value )
           && fp_equal( s0.data[3], value ) );
}

//------------------------------------------------------------------------
// Functions which take a variable number of arguments
//------------------------------------------------------------------------

__attribute__ ((noinline))
int lg_struct_arg1( lg_struct_t a0 )
{
  return lg_struct_eq( a0, 1.0f );
}

__attribute__ ((noinline))
int lg_struct_arg2( lg_struct_t a0, lg_struct_t a1 )
{
  return lg_struct_eq( lg_struct_add(a0,a1), 3.0f );
}

__attribute__ ((noinline))
int lg_struct_arg9( lg_struct_t a0, lg_struct_t a1, lg_struct_t a2,
                    lg_struct_t a3, lg_struct_t a4, lg_struct_t a5, 
                    lg_struct_t a6, lg_struct_t a7, lg_struct_t a8 )
{
  return lg_struct_eq( 
           lg_struct_add( a0,
             lg_struct_add( a1,
               lg_struct_add( a2,
                 lg_struct_add( a3,
                   lg_struct_add( a4,
                     lg_struct_add( a5,
                       lg_struct_add( a6,
                         lg_struct_add( a7, a8 )))))))), 45.0f );
}

__attribute__ ((noinline))
lg_struct_t lg_struct_return( lg_struct_t a0 )
{
  return a0;
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test with one argument

  error_code++;
  if ( !sm_struct_arg1( sm_struct_init(1.0f) ) )
    return error_code;

  error_code++;
  if ( !lg_struct_arg1( lg_struct_init(1.0f) ) )
    return error_code;

  // Test with two arguments

  error_code++;
  if ( !sm_struct_arg2( sm_struct_init(1.0f), sm_struct_init(2.0f) ) )
    return error_code;

  error_code++;
  if ( !lg_struct_arg2( lg_struct_init(1.0f), lg_struct_init(2.0f) ) )
    return error_code;

  // Test with nine arguments

  error_code++;
  if ( !sm_struct_arg9( 
          sm_struct_init(1.0f), sm_struct_init(2.0f), sm_struct_init(3.0f),
          sm_struct_init(4.0f), sm_struct_init(5.0f), sm_struct_init(6.0f),
          sm_struct_init(7.0f), sm_struct_init(8.0f), sm_struct_init(9.0f) ) )
    return error_code;

  error_code++;
  if ( !lg_struct_arg9( 
          lg_struct_init(1.0f), lg_struct_init(2.0f), lg_struct_init(3.0f),
          lg_struct_init(4.0f), lg_struct_init(5.0f), lg_struct_init(6.0f),
          lg_struct_init(7.0f), lg_struct_init(8.0f), lg_struct_init(9.0f) ) )
    return error_code;

  // Test return value

  error_code++;
  if ( !sm_struct_eq( sm_struct_return( sm_struct_init(1.0f) ), 1.0f ) )
    return error_code;

  error_code++;
  if ( !lg_struct_eq( lg_struct_return( lg_struct_init(1.0f) ), 1.0f ) )
    return error_code;

  // If all tests passed return zero

  return 0;
}
