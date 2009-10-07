//========================================================================
// Test Vector Unit Strided Memory Ops Builtins
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
// Unit stride vector load helper functions
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

//------------------------------------------------------------------------
// Unit stride vector store helper functions
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

//------------------------------------------------------------------------
// Strided vector load helper functions
//------------------------------------------------------------------------

__attribute__ ((always_inline))
inline void vt_vload_strided_word( vword_t* v_out_ptr, word_t* in_ptr, 
                                   int byte_stride )
{
  *v_out_ptr = __builtin_mips_maven_vload_strided_vsi(in_ptr,byte_stride);
}

__attribute__ ((always_inline))
inline void vt_vload_strided_hword( vhword_t* v_out_ptr, hword_t* in_ptr, 
                                    int byte_stride )
{
  *v_out_ptr = __builtin_mips_maven_vload_strided_vhi(in_ptr,byte_stride);
}

//------------------------------------------------------------------------
// Strided vector store helper functions
//------------------------------------------------------------------------

__attribute__ ((always_inline))
inline void vt_vstore_strided_word( vword_t v_in, word_t* out_ptr, 
                                    int byte_stride )
{
  __builtin_mips_maven_vstore_strided_vsi(v_in,out_ptr,byte_stride);
}

__attribute__ ((always_inline))
inline void vt_vstore_strided_hword( vhword_t v_in, hword_t* out_ptr, 
                                     int byte_stride )
{
  __builtin_mips_maven_vstore_strided_vhi(v_in,out_ptr,byte_stride);
}

//------------------------------------------------------------------------
// memcpy_word
//------------------------------------------------------------------------

__attribute__ ((noinline))
void memcpy_strided_input_word( word_t* in_ptr, word_t* out_ptr, 
                                int stride )
{
  vword_t v_a;
  vt_vload_strided_word( &v_a, in_ptr, stride*sizeof(word_t) );
  vt_vstore_word( v_a, out_ptr );
  vt_sync_l_cv();
}

__attribute__ ((noinline))
void memcpy_strided_output_word( word_t* in_ptr, word_t* out_ptr, 
                                 int stride )
{
  vword_t v_a;
  vt_vload_word( &v_a, in_ptr );
  vt_vstore_strided_word( v_a, out_ptr, stride*sizeof(word_t) );
  vt_sync_l_cv();
}

//------------------------------------------------------------------------
// memcpy_hword
//------------------------------------------------------------------------

__attribute__ ((noinline))
void memcpy_strided_input_hword( hword_t* in_ptr, hword_t* out_ptr, 
                                 int stride )
{
  vhword_t v_a;
  vt_vload_strided_hword( &v_a, in_ptr, stride*sizeof(hword_t) );
  vt_vstore_hword( v_a, out_ptr );
  vt_sync_l_cv();
}

__attribute__ ((noinline))
void memcpy_strided_output_hword( hword_t* in_ptr, hword_t* out_ptr, 
                                  int stride )
{
  vhword_t v_a;
  vt_vload_hword( &v_a, in_ptr );
  vt_vstore_strided_hword( v_a, out_ptr, stride*sizeof(hword_t) );
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

  word_t vec_word_in[vlen*5];
  for ( int i = 0; i < vlen*5; i++ )
    vec_word_in[i] = vlen*5 - i;

  word_t vec_word_out_st2i[vlen];
  memcpy_strided_input_word( vec_word_in, vec_word_out_st2i, 2 );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_word_out_st2i[i] != vec_word_in[i*2] )
      return error_code;
  }

  word_t vec_word_out_st3i[vlen];
  memcpy_strided_input_word( vec_word_in, vec_word_out_st3i, 3 );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_word_out_st3i[i] != vec_word_in[i*3] )
      return error_code;
  }

  word_t vec_word_out_st2o[2*vlen];
  memcpy_strided_output_word( vec_word_in, vec_word_out_st2o, 2 );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_word_out_st2o[i*2] != vec_word_in[i] )
      return error_code;
  }

  word_t vec_word_out_st3o[3*vlen];
  memcpy_strided_output_word( vec_word_in, vec_word_out_st3o, 3 );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_word_out_st3o[i*3] != vec_word_in[i] )
      return error_code;
  }

  // Test halfhword builtins

  hword_t vec_hword_in[vlen*5];
  for ( int i = 0; i < vlen*5; i++ )
    vec_hword_in[i] = vlen*5 - i;

  hword_t vec_hword_out_st2i[vlen];
  memcpy_strided_input_hword( vec_hword_in, vec_hword_out_st2i, 2 );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_hword_out_st2i[i] != vec_hword_in[i*2] )
      return error_code;
  }

  hword_t vec_hword_out_st3i[vlen];
  memcpy_strided_input_hword( vec_hword_in, vec_hword_out_st3i, 3 );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_hword_out_st3i[i] != vec_hword_in[i*3] )
      return error_code;
  }

  hword_t vec_hword_out_st2o[2*vlen];
  memcpy_strided_output_hword( vec_hword_in, vec_hword_out_st2o, 2 );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_hword_out_st2o[i*2] != vec_hword_in[i] )
      return error_code;
  }

  hword_t vec_hword_out_st3o[3*vlen];
  memcpy_strided_output_hword( vec_hword_in, vec_hword_out_st3o, 3 );

  error_code++;
  for ( int i = 0; i < vlen; i++ ) {
    if ( vec_hword_out_st3o[i*3] != vec_hword_in[i] )
      return error_code;
  }

  // If all tests passed return zero

  return 0;
}
