//========================================================================
// Vector Arguments
//========================================================================
// Test passing and returning vectors to functions which are forced not
// to be inlined.

#include <machine/syscfg.h>

//------------------------------------------------------------------------
// vector types
//------------------------------------------------------------------------

typedef unsigned int   vword_t   __attribute__ ((vector_size(128)));
typedef unsigned short vhword_t  __attribute__ ((vector_size(64)));
typedef unsigned char  vbyte_t   __attribute__ ((vector_size(32)));
typedef float          vfloat_t  __attribute__ ((vector_size(128)));

typedef int            vint_t    __attribute__ ((vector_size(128)));
typedef short          vshort_t  __attribute__ ((vector_size(64)));
typedef unsigned short vushort_t __attribute__ ((vector_size(64)));

//------------------------------------------------------------------------
// Miscellaneous helper functions
//------------------------------------------------------------------------

__attribute__ ((always_inline))
inline int vt_setvl( int n )
{
  int vlen;
  asm volatile ( "setvl %0, %1" : "=r"(vlen) : "r"(n) );
  return vlen;
}

__attribute__ ((always_inline))
inline void vt_sync_l_cv()
{
  asm volatile ( "sync.l.cv" ::: "memory" );
}

//------------------------------------------------------------------------
// Vector load helper functions
//------------------------------------------------------------------------

__attribute__ ((always_inline))
inline void vt_vload_int( vint_t* v_out_ptr, int* in_ptr )
{
  *v_out_ptr = (vint_t) __builtin_mips_maven_vload_vsi(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_short( vshort_t* v_out_ptr, short* in_ptr )
{
  *v_out_ptr = (vshort_t) __builtin_mips_maven_vload_vhi(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_ushort( vushort_t* v_out_ptr, unsigned short* in_ptr )
{
  *v_out_ptr = (vushort_t) __builtin_mips_maven_vload_vhi(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_float( vfloat_t* v_out_ptr, float* in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_vsf(in_ptr);
}

//------------------------------------------------------------------------
// Vector store helper functions
//------------------------------------------------------------------------

__attribute__ ((always_inline))
inline void vt_vstore_int( vint_t v_in, int* out_ptr )
{
  __builtin_mips_maven_vstore_vsi((vword_t) v_in,out_ptr);
}

__attribute__ ((always_inline))
inline void vt_vstore_short( vshort_t v_in, short* out_ptr )
{
  __builtin_mips_maven_vstore_vhi((vhword_t) v_in,out_ptr);
}

__attribute__ ((always_inline))
inline void vt_vstore_ushort( vushort_t v_in, unsigned short* out_ptr )
{
  __builtin_mips_maven_vstore_vhi((vhword_t) v_in,out_ptr);
}

__attribute__ ((always_inline))
inline void vt_vstore_float( vfloat_t v_in, float* out_ptr )
{
  __builtin_mips_maven_vstore_vsf(v_in,out_ptr);
}

//------------------------------------------------------------------------
// Functions which take a variable number of integer vector arguments
//------------------------------------------------------------------------

__attribute__ ((noinline))
vint_t vint_init( int a0 )
{
  int temp[MAVEN_SYSCFG_VLEN_MIN];
  for ( int i = 0; i < MAVEN_SYSCFG_VLEN_MIN; i++ )
    temp[i] = a0;

  vint_t ret;
  vt_sync_l_cv();
  vt_vload_int( &ret, temp );
  return ret;
}

__attribute__ ((noinline))
int vint_arg1( vint_t a0 )
{
  int ret[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_int( a0, ret );
  vt_sync_l_cv();
  return ( ret[0] == 1 );
}

__attribute__ ((noinline))
int vint_arg2( vint_t a0, vint_t a1 )
{
  int ret[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_int( a0 + a1, ret );
  vt_sync_l_cv();
  return ( ret[0] == 3 );
}

__attribute__ ((noinline))
int vint_arg9( vint_t a0, vint_t a1, vint_t a2,
               vint_t a3, vint_t a4, vint_t a5, 
               vint_t a6, vint_t a7, vint_t a8 )
{
  int ret[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_int( a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8, ret );
  vt_sync_l_cv();
  return ( ret[0] == 45 );
}

//------------------------------------------------------------------------
// Functions which take a variable number of ushort vector arguments
//------------------------------------------------------------------------

__attribute__ ((noinline))
vushort_t vushort_init( int a0 )
{
  unsigned short temp[MAVEN_SYSCFG_VLEN_MIN];
  for ( int i = 0; i < MAVEN_SYSCFG_VLEN_MIN; i++ )
    temp[i] = a0;

  vushort_t ret;
  vt_sync_l_cv();
  vt_vload_ushort( &ret, temp );
  return ret;
}

__attribute__ ((noinline))
int vushort_arg1( vushort_t a0 )
{
  unsigned short ret[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_ushort( a0, ret );
  vt_sync_l_cv();
  return ( ret[0] == 1 );
}

__attribute__ ((noinline))
int vushort_arg2( vushort_t a0, vushort_t a1 )
{
  unsigned short ret[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_ushort( a0 + a1, ret );
  vt_sync_l_cv();
  return ( ret[0] == 3 );
}

__attribute__ ((noinline))
int vushort_arg9( vushort_t a0, vushort_t a1, vushort_t a2,
                  vushort_t a3, vushort_t a4, vushort_t a5, 
                  vushort_t a6, vushort_t a7, vushort_t a8 )
{
  unsigned short ret[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_ushort( a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8, ret );
  vt_sync_l_cv();
  return ( ret[0] == 45 );
}

//------------------------------------------------------------------------
// Functions which take a variable number of short vector arguments
//------------------------------------------------------------------------

__attribute__ ((noinline))
vshort_t vshort_init( int a0 )
{
  short temp[MAVEN_SYSCFG_VLEN_MIN];
  for ( int i = 0; i < MAVEN_SYSCFG_VLEN_MIN; i++ )
    temp[i] = a0;

  vshort_t ret;
  vt_sync_l_cv();
  vt_vload_short( &ret, temp );
  return ret;
}

__attribute__ ((noinline))
int vshort_arg1( vshort_t a0 )
{
  short ret[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_short( a0, ret );
  vt_sync_l_cv();
  return ( ret[0] == 1 );
}

__attribute__ ((noinline))
int vshort_arg2( vshort_t a0, vshort_t a1 )
{
  short ret[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_short( a0 + a1, ret );
  vt_sync_l_cv();
  return ( ret[0] == 3 );
}

__attribute__ ((noinline))
int vshort_arg9( vshort_t a0, vshort_t a1, vshort_t a2,
                 vshort_t a3, vshort_t a4, vshort_t a5, 
                 vshort_t a6, vshort_t a7, vshort_t a8 )
{
  short ret[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_short( a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8, ret );
  vt_sync_l_cv();
  return ( ret[0] == 45 );
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // We will use the minimum vlen for these tests to avoid stripmining

  int vlen = MAVEN_SYSCFG_VLEN_MIN;
  vt_setvl(vlen);

  // Test return argument

  error_code++;
  vint_t vec_int = vint_init(42);
  int vec_int_array[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_int( vec_int, vec_int_array );
  vt_sync_l_cv();
  for ( int i = 0; i < MAVEN_SYSCFG_VLEN_MIN; i++ ) {
    if ( vec_int_array[i] != 42 )
      return error_code;
  }

  error_code++;
  vushort_t vec_ushort = vushort_init(42);
  unsigned short vec_ushort_array[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_ushort( vec_ushort, vec_ushort_array );
  vt_sync_l_cv();
  for ( int i = 0; i < MAVEN_SYSCFG_VLEN_MIN; i++ ) {
    if ( vec_ushort_array[i] != 42 )
      return error_code;
  }

  error_code++;
  vshort_t vec_short = vshort_init(42);
  unsigned short vec_short_array[MAVEN_SYSCFG_VLEN_MIN];
  vt_vstore_short( vec_short, vec_short_array );
  vt_sync_l_cv();
  for ( int i = 0; i < MAVEN_SYSCFG_VLEN_MIN; i++ ) {
    if ( vec_short_array[i] != 42 )
      return error_code;
  }

  // Test with one argument

  error_code++;
  if ( !vint_arg1( vint_init(1) ) )
    return error_code;

  error_code++;
  if ( !vushort_arg1( vushort_init(1) ) )
    return error_code;

  error_code++;
  if ( !vshort_arg1( vshort_init(1) ) )
    return error_code;

  // Test with two arguments

  error_code++;
  if ( !vint_arg2( vint_init(1), vint_init(2) ) )
    return error_code;

  error_code++;
  if ( !vushort_arg2( vushort_init(1), vushort_init(2) ) )
    return error_code;

  error_code++;
  if ( !vshort_arg2( vshort_init(1), vshort_init(2) ) )
    return error_code;

  // Test with nine arguments

  error_code++;
  if ( !vint_arg9( vint_init(1), vint_init(2), vint_init(3),
                   vint_init(4), vint_init(5), vint_init(6),
                   vint_init(7), vint_init(8), vint_init(9) ) )
    return error_code;

  error_code++;
  if ( !vushort_arg9( vushort_init(1), vushort_init(2), vushort_init(3),
                      vushort_init(4), vushort_init(5), vushort_init(6),
                      vushort_init(7), vushort_init(8), vushort_init(9) ) )
    return error_code;

  error_code++;
  if ( !vshort_arg9( vshort_init(1), vshort_init(2), vshort_init(3),
                     vshort_init(4), vshort_init(5), vshort_init(6),
                     vshort_init(7), vshort_init(8), vshort_init(9) ) )
    return error_code;

  // If all tests passed return zero

  return 0;
}
