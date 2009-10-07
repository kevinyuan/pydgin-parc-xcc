//========================================================================
// Test Traditional Vector Ops Builtins
//========================================================================

#include <machine/syscfg.h>
#if ( MAVEN_SYSCFG_VLEN_MIN != 4 )
#error "Test currently only works if MAVEN_SYSCFG_VLEN_MIN == 4"
#endif

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
// vector types
//------------------------------------------------------------------------

typedef unsigned int   word_t;
typedef unsigned short hword_t;
typedef unsigned char  byte_t;

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
// add_vv_int
//------------------------------------------------------------------------

__attribute__ ((noinline))
void add_vv_int( int* a_ptr, int* b_ptr, int* c_ptr )
{
  vint_t v_a, v_b, v_c;
  vt_vload_int( &v_a, a_ptr );
  vt_vload_int( &v_b, b_ptr );
  
  v_c = v_a + v_b;
  
  vt_vstore_int( v_c, c_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// add_vv_short
//------------------------------------------------------------------------

__attribute__ ((noinline))
void add_vv_short( short* a_ptr, short* b_ptr, short* c_ptr )
{
  vshort_t v_a, v_b, v_c;
  vt_vload_short( &v_a, a_ptr );
  vt_vload_short( &v_b, b_ptr );
  
  v_c = v_a + v_b;
  
  vt_vstore_short( v_c, c_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// add_vv_ushort
//------------------------------------------------------------------------

__attribute__ ((noinline))
void add_vv_ushort( unsigned short* a_ptr, unsigned short* b_ptr, 
                    unsigned short* c_ptr )
{
  vushort_t v_a, v_b, v_c;
  vt_vload_ushort( &v_a, a_ptr );
  vt_vload_ushort( &v_b, b_ptr );
  
  v_c = v_a + v_b;
  
  vt_vstore_ushort( v_c, c_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// add_vv_float
//------------------------------------------------------------------------
// add.s.vv not implemented yet

/*
__attribute__ ((noinline))
void add_vv_float( float* a_ptr, float* b_ptr, float* c_ptr )
{
  vfloat_t v_a, v_b, v_c;
  vt_vload_float( &v_a, a_ptr );
  vt_vload_float( &v_b, b_ptr );
  
  v_c = v_a + v_b;
  
  vt_vstore_float( v_c, c_ptr );
  vt_sync_l_cv();
}
*/

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

  // Test add_vv_int

  int vec_int_a[]   = { 0, 2,  8, 34 };
  int vec_int_b[]   = { 1, 3, 13, 55 };
  int vec_int_ref[] = { 1, 5, 21, 89 };

  int vec_int_c[vlen];
  add_vv_int( vec_int_a, vec_int_b, vec_int_c );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_int_c[i] != vec_int_ref[i] )
      return error_code;
  }

  // Test add_vv_short

  short vec_short_a[]   = { 0, 2,  8, 34 };
  short vec_short_b[]   = { 1, 3, 13, 55 };
  short vec_short_ref[] = { 1, 5, 21, 89 };

  short vec_short_c[vlen];
  add_vv_short( vec_short_a, vec_short_b, vec_short_c );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_short_c[i] != vec_short_ref[i] )
      return error_code;
  }

  // Test add_vv_ushort

  unsigned short vec_ushort_a[]   = { 0, 2,  8, 34 };
  unsigned short vec_ushort_b[]   = { 1, 3, 13, 55 };
  unsigned short vec_ushort_ref[] = { 1, 5, 21, 89 };

  unsigned short vec_ushort_c[vlen];
  add_vv_ushort( vec_ushort_a, vec_ushort_b, vec_ushort_c );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_ushort_c[i] != vec_ushort_ref[i] )
      return error_code;
  }

  // Test add_vv_float

  /*
  float vec_float_a[]   = { 0.0, 1.5,  7.5, 13.2 };
  float vec_float_b[]   = { 1.0, 3.0,  3.3,  6.3 };
  float vec_float_ref[] = { 1.0, 4.5, 10.8, 19.5 };

  float vec_float_c[vlen];
  add_vv_float( vec_float_a, vec_float_b, vec_float_c );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( fp_equal( vec_float_c[i], vec_float_ref[i] ) )
      return error_code;
  }
  */

  // If all tests passed return zero

  return 0;
}
