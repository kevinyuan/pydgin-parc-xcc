//========================================================================
// Test Vector Unit Stride Memory Ops Builtins
//========================================================================
// We use memcpy as an example to help test the vector memory builtins
// for various types. For ints also write an unrolled version. This
// helps test vector instruction scheduling and vector register
// allocation. The final call to the unrolled version of memcpy_int
// takes care of the extra iterations.

#include <machine/syscfg.h>

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
typedef unsigned int   vptr_t    __attribute__ ((vector_size(128)));

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
inline void vt_vload_word( vword_t* v_out_ptr, word_t* in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_vsi(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_hword( vhword_t* v_out_ptr, hword_t* in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_vhi(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_byte( vbyte_t* v_out_ptr, byte_t* in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_vqi(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_float( vfloat_t* v_out_ptr, float* in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_vsf(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_ptr( vptr_t* v_out_ptr, void** in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_vsi(in_ptr);
}

//------------------------------------------------------------------------
// Vector store helper functions
//------------------------------------------------------------------------

__attribute__ ((always_inline))
inline void vt_vstore_word( vword_t v_in, word_t* out_ptr )
{
  __builtin_mips_maven_vstore_vsi(v_in,out_ptr);
}

__attribute__ ((always_inline))
inline void vt_vstore_hword( vhword_t v_in, hword_t* out_ptr )
{
  __builtin_mips_maven_vstore_vhi(v_in,out_ptr);
}

__attribute__ ((always_inline))
inline void vt_vstore_byte( vbyte_t v_in, byte_t* out_ptr )
{
  __builtin_mips_maven_vstore_vqi(v_in,out_ptr);
}

__attribute__ ((always_inline))
inline void vt_vstore_float( vfloat_t v_in, float* out_ptr )
{
  __builtin_mips_maven_vstore_vsf(v_in,out_ptr);
}

__attribute__ ((always_inline))
inline void vt_vstore_ptr( vptr_t v_in, void** out_ptr )
{
  __builtin_mips_maven_vstore_vsi(v_in,out_ptr);
}

//------------------------------------------------------------------------
// func_noinline
//------------------------------------------------------------------------

__attribute__ ((noinline))
int func_noinline( int i )
{
  return i;
}

//------------------------------------------------------------------------
// memcpy_word
//------------------------------------------------------------------------

__attribute__ ((noinline))
void memcpy_word( word_t* in_ptr, word_t* out_ptr )
{
  vword_t v_a;
  vt_vload_word( &v_a, in_ptr  );
  vt_vstore_word( v_a, out_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// memcpy_hword
//------------------------------------------------------------------------

__attribute__ ((noinline))
void memcpy_hword( hword_t* in_ptr, hword_t* out_ptr )
{
  vhword_t v_a;
  vt_vload_hword( &v_a, in_ptr  );
  vt_vstore_hword( v_a, out_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// memcpy_byte
//------------------------------------------------------------------------

__attribute__ ((noinline))
void memcpy_byte( byte_t* in_ptr, byte_t* out_ptr )
{
  vbyte_t v_a;
  vt_vload_byte( &v_a, in_ptr  );
  vt_vstore_byte( v_a, out_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// memcpy_float
//------------------------------------------------------------------------

__attribute__ ((noinline))
void memcpy_float( float* in_ptr, float* out_ptr )
{
  vfloat_t v_a;
  vt_vload_float( &v_a, in_ptr  );
  vt_vstore_float( v_a, out_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// memcpy_ptr
//------------------------------------------------------------------------

__attribute__ ((noinline))
void memcpy_ptr( void** in_ptr, void** out_ptr )
{
  vptr_t v_a;
  vt_vload_ptr( &v_a, in_ptr  );
  vt_vstore_ptr( v_a, out_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// memcpy_unrolled_word
//------------------------------------------------------------------------

__attribute__ ((noinline))
void memcpy_unrolled_word( word_t* in_ptr, word_t* out_ptr, int n )
{
  int i = 0;
  int vlen = vt_setvl(n);
  while ( i + 4*vlen < n ) {
    vlen = vt_setvl((n-i)/4);

    vword_t v_a, v_b, v_c, v_d;

    vt_vload_word( &v_a, &in_ptr[i]        );
    vt_vload_word( &v_b, &in_ptr[i+vlen]   );
    vt_vload_word( &v_c, &in_ptr[i+2*vlen] );
    vt_vload_word( &v_d, &in_ptr[i+3*vlen] );

    vlen = func_noinline(vlen);
    
    vt_vstore_word( v_a, &out_ptr[i]        );
    vt_vstore_word( v_b, &out_ptr[i+vlen]   );
    vt_vstore_word( v_c, &out_ptr[i+2*vlen] );
    vt_vstore_word( v_d, &out_ptr[i+3*vlen] );

    i += 4*vlen;
  }

  // Epilogue

  vlen = vt_setvl(n-i);
  for ( int j = i; j < n; j += vlen ) {
    vlen = vt_setvl(n-j);

    vword_t v_a;
    vt_vload_word( &v_a, &in_ptr[j] );
    vt_vstore_word( v_a, &out_ptr[j] );
  }

  vt_sync_l_cv();
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

  // Test word builtins

  word_t vec_word_in[vlen];
  for ( int i = 0; i < vlen; i++ )
    vec_word_in[i] = vlen - i;

  word_t vec_word_out[vlen];
  memcpy_word( vec_word_in, vec_word_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_word_in[i] != vec_word_out[i] )
      return error_code;
  }

  // Test halfword builtins

  hword_t vec_hword_in[vlen];
  for ( int i = 0; i < vlen; i++ )
    vec_hword_in[i] = vlen - i;

  hword_t vec_hword_out[vlen];
  memcpy_hword( vec_hword_in, vec_hword_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_hword_in[i] != vec_hword_out[i] )
      return error_code;
  }

  // Test byte builtins

  byte_t vec_byte_in[vlen];
  for ( int i = 0; i < vlen; i++ )
    vec_byte_in[i] = vlen - i;

  byte_t vec_byte_out[vlen];
  memcpy_byte( vec_byte_in, vec_byte_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_byte_in[i] != vec_byte_out[i] )
      return error_code;
  }

  // Test float builtins

  float vec_float_in[vlen];
  for ( int i = 0; i < vlen; i++ )
    vec_float_in[i] = (vlen - i) * 1.0f;

  float vec_float_out[vlen];
  memcpy_float( vec_float_in, vec_float_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_float_in[i] != vec_float_out[i] )
      return error_code;
  }

  // Test integer builtin with pointers

  word_t* vec_ptr_in[vlen];
  for ( int i = 0; i < vlen; i++ )
    vec_ptr_in[i] = &vec_word_in[i];

  word_t* vec_ptr_out[vlen];
  memcpy_ptr( (void*) vec_ptr_in, (void*) vec_ptr_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_ptr_in[i] != vec_ptr_out[i] )
      return error_code;
  }

  // Test unrolled version

  int vec_word_ur_sz = 32*8 + 13;
  int vec_word_ur_in[vec_word_ur_sz];
  for ( int i = 0; i < vec_word_ur_sz; i++ )
    vec_word_ur_in[i] = vec_word_ur_sz - i;

  int vec_word_ur_out[vec_word_ur_sz];
  memcpy_unrolled_word( vec_word_ur_in, vec_word_ur_out, vec_word_ur_sz );

  error_code++;
  for ( int i = 0; i < vec_word_ur_sz; i++ ) {
    if ( vec_word_ur_in[i] != vec_word_ur_out[i] )
      return error_code;
  }

  // If all tests passed return zero

  return 0;
}

