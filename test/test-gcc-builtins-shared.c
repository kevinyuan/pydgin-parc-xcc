//========================================================================
// Test Vector Load Shared Builtins
//========================================================================

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
// Vector load shared helper functions
//------------------------------------------------------------------------

__attribute__ ((always_inline))
inline void vt_vload_shared_word( vword_t* v_out_ptr, word_t* in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_shared_vsi(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_shared_hword( vhword_t* v_out_ptr, hword_t* in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_shared_vhi(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_shared_byte( vbyte_t* v_out_ptr, byte_t* in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_shared_vqi(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_shared_float( vfloat_t* v_out_ptr, float* in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_shared_vsf(in_ptr);
}

__attribute__ ((always_inline))
inline void vt_vload_shared_ptr( vptr_t* v_out_ptr, void** in_ptr )
{
  *v_out_ptr = __builtin_mips_maven_vload_shared_vsi(in_ptr);
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
// setall_word
//------------------------------------------------------------------------

__attribute__ ((noinline))
void setall_word( word_t* in_ptr, word_t* out_ptr )
{
  vword_t v_a;
  vt_vload_shared_word( &v_a, in_ptr  );
  vt_vstore_word( v_a, out_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// setall_hword
//------------------------------------------------------------------------

__attribute__ ((noinline))
void setall_hword( hword_t* in_ptr, hword_t* out_ptr )
{
  vhword_t v_a;
  vt_vload_shared_hword( &v_a, in_ptr  );
  vt_vstore_hword( v_a, out_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// setall_byte
//------------------------------------------------------------------------

__attribute__ ((noinline))
void setall_byte( byte_t* in_ptr, byte_t* out_ptr )
{
  vbyte_t v_a;
  vt_vload_shared_byte( &v_a, in_ptr  );
  vt_vstore_byte( v_a, out_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// setall_float
//------------------------------------------------------------------------

__attribute__ ((noinline))
void setall_float( float* in_ptr, float* out_ptr )
{
  vfloat_t v_a;
  vt_vload_shared_float( &v_a, in_ptr  );
  vt_vstore_float( v_a, out_ptr );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// setall_ptr
//------------------------------------------------------------------------

__attribute__ ((noinline))
void setall_ptr( void** in_ptr, void** out_ptr )
{
  vptr_t v_a;
  vt_vload_shared_ptr( &v_a, in_ptr  );
  vt_vstore_ptr( v_a, out_ptr );
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
  vec_word_in[0] = 10;
  for ( int i = 1; i < vlen; i++ )
    vec_word_in[i] = vlen - i;

  word_t vec_word_out[vlen];
  setall_word( vec_word_in, vec_word_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_word_out[i] != 10 )
      return error_code;
  }

  // Test halfword builtins

  hword_t vec_hword_in[vlen];
  vec_hword_in[0] = 11;
  for ( int i = 1; i < vlen; i++ )
    vec_hword_in[i] = vlen - i;

  hword_t vec_hword_out[vlen];
  setall_hword( vec_hword_in, vec_hword_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_hword_out[i] != 11 )
      return error_code;
  }

  // Test byte builtins

  byte_t vec_byte_in[vlen];
  vec_byte_in[0] = 12;
  for ( int i = 1; i < vlen; i++ )
    vec_byte_in[i] = vlen - i;

  byte_t vec_byte_out[vlen];
  setall_byte( vec_byte_in, vec_byte_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_byte_out[i] != 12 )
      return error_code;
  }

  // Test float builtins

  float vec_float_in[vlen];
  vec_float_in[0] = 13.0f;
  for ( int i = 1; i < vlen; i++ )
    vec_float_in[i] = (vlen - i) * 1.0f;

  float vec_float_out[vlen];
  setall_float( vec_float_in, vec_float_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_float_out[i] != 13.0f )
      return error_code;
  }

  // Test integer builtin with pointers

  word_t* vec_ptr_in[vlen];
  for ( int i = 0; i < vlen; i++ )
    vec_ptr_in[i] = &vec_word_in[i];

  word_t* vec_ptr_out[vlen];
  setall_ptr( (void*) vec_ptr_in, (void*) vec_ptr_out );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_ptr_out[i] != &vec_word_in[0] )
      return error_code;
  }

  // If all tests passed return zero

  return 0;
}

