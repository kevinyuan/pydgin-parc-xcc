//========================================================================
// Vector Flag Registers
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
typedef unsigned char  vflags_t  __attribute__ ((vector_size(32)));
typedef float          vfloat_t  __attribute__ ((vector_size(128)));

typedef int            vint_t    __attribute__ ((vector_size(128)));
typedef short          vshort_t  __attribute__ ((vector_size(64)));
typedef unsigned short vushort_t __attribute__ ((vector_size(64)));
typedef unsigned char  vuchar_t  __attribute__ ((vector_size(32)));

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
inline void vt_vload_uchar( vuchar_t* v_out_ptr, unsigned char* in_ptr )
{
  *v_out_ptr = (vuchar_t) __builtin_mips_maven_vload_vqi(in_ptr);
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
// Helper Macros
//------------------------------------------------------------------------

#define ELM_TYPE_int    int
#define ELM_TYPE_uint   unsigned int
#define ELM_TYPE_short  short
#define ELM_TYPE_ushort unsigned short
#define ELM_TYPE_char   signed char
#define ELM_TYPE_uchar  unsigned char
#define ELM_TYPE_float  float

#define ELM_TYPE( short_name_ ) \
  ELM_TYPE_ ## short_name_

#define VEC_TYPE_int    vint_t
#define VEC_TYPE_uint   vuint_t
#define VEC_TYPE_short  vshort_t
#define VEC_TYPE_ushort vushort_t
#define VEC_TYPE_char   vchar_t
#define VEC_TYPE_uchar  vuchar_t
#define VEC_TYPE_float  vfloat_t

#define VEC_TYPE( short_name_ ) \
  VEC_TYPE_ ## short_name_

#define STRIP_PAREN_H0( args_... ) args_

#define STRIP_PAREN( input_ ) \
  STRIP_PAREN_H0 input_

#define MK_TEST_FUNC_VV( name_, elm_, insn_ )                           \
 __attribute__ ((noinline))                                             \
 void name_ ## _ ## elm_ ( ELM_TYPE(elm_)* dest,                        \
                           ELM_TYPE(elm_)* src0,                        \
                           ELM_TYPE(elm_)* src1,                        \
                           unsigned char* flags )                       \
 {                                                                      \
   VEC_TYPE(elm_) vdest, vsrc0, vsrc1;                                  \
   vt_sync_l_cv();                                                      \
   vt_vload_ ## elm_ ( &vsrc0, src0 );                                  \
   vt_vload_ ## elm_ ( &vsrc1, src1 );                                  \
                                                                        \
   vflags_t vflags;                                                     \
   vt_vload_uchar( &vflags, flags );                                    \
                                                                        \
   __asm__                                                              \
    ( "mov.vv %0, %1"                                                   \
      : "=Z"(vdest) : "Z"(vsrc0) );                                     \
                                                                        \
   __asm__ volatile                                                     \
     ( insn_ " %0, %1, %2, %3"                                          \
      : "=Z"(vdest) : "Z"(vsrc0), "Z"(vsrc1), "q"(vflags) );            \
                                                                        \
   vt_vstore_ ## elm_ ( vdest, dest );                                  \
   vt_sync_l_cv();                                                      \
 }                                                                      \

#define RUN_TEST( name_, elm_, src0_, src1_, flags_, dest_ref_ )        \
{                                                                       \
  ELM_TYPE(elm_) src0[]     = STRIP_PAREN(src0_);                       \
  ELM_TYPE(elm_) src1[]     = STRIP_PAREN(src1_);                       \
  ELM_TYPE(elm_) dest_ref[] = STRIP_PAREN(dest_ref_);                   \
  unsigned char  flags[]    = STRIP_PAREN(flags_);                      \
                                                                        \
  ELM_TYPE(elm_) dest[vlen];                                            \
  name_ ## _ ## elm_( dest, src0, src1, flags );                        \
                                                                        \
  error_code++;                                                         \
  for ( int i = 0; i < vlen; i++ ) {                                    \
    if ( dest[i] != dest_ref[i] )                                       \
      return error_code;                                                \
  }                                                                     \
}                                                                       \

//------------------------------------------------------------------------
// Test functions
//------------------------------------------------------------------------

MK_TEST_FUNC_VV( test_add_vv, int,    "addu.vv"  )
MK_TEST_FUNC_VV( test_add_vv, short,  "addu.vv"  )
MK_TEST_FUNC_VV( test_add_vv, ushort, "addu.vv"  )
MK_TEST_FUNC_VV( test_add_vv, float,  "add.s.vv" )

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

  if ( vlen != 4 )
    return 255;

  // Test add_vv_int

  RUN_TEST( test_add_vv, int,
    ({  0, 2,  8, 34 }),
    ({ -1, 3, 13, 55 }),
    ({  1, 0,  1,  1 }),
    ({ -1, 2, 21, 89 }) );

  RUN_TEST( test_add_vv, int,
    ({  0, 2,  8, 34 }),
    ({ -1, 3, 13, 55 }),
    ({  0, 1,  0,  0 }),
    ({  0, 5,  8, 34 }) );

  RUN_TEST( test_add_vv, int,
    ({  0, 2,  8, 34 }),
    ({ -1, 3, 13, 55 }),
    ({  0, 0,  0,  0 }),
    ({  0, 2,  8, 34 }) );

  // Test add_vv_short

  RUN_TEST( test_add_vv, short,
    ({  0, 2,  8, 34 }),
    ({ -1, 3, 13, 55 }),
    ({  1, 0,  1,  1 }),
    ({ -1, 2, 21, 89 }) );

  RUN_TEST( test_add_vv, short,
    ({  0, 2,  8, 34 }),
    ({ -1, 3, 13, 55 }),
    ({  0, 1,  0,  0 }),
    ({  0, 5,  8, 34 }) );

  RUN_TEST( test_add_vv, short,
    ({  0, 2,  8, 34 }),
    ({ -1, 3, 13, 55 }),
    ({  0, 0,  0,  0 }),
    ({  0, 2,  8, 34 }) );

  // Test add_vv_ushort

  RUN_TEST( test_add_vv, ushort,
    ({  0, 2,  8, 34 }),
    ({  1, 3, 13, 55 }),
    ({  1, 0,  1,  1 }),
    ({  1, 2, 21, 89 }) );

  RUN_TEST( test_add_vv, ushort,
    ({  0, 2,  8, 34 }),
    ({  1, 3, 13, 55 }),
    ({  0, 1,  0,  0 }),
    ({  0, 5,  8, 34 }) );

  RUN_TEST( test_add_vv, ushort,
    ({  0, 2,  8, 34 }),
    ({  1, 3, 13, 55 }),
    ({  0, 0,  0,  0 }),
    ({  0, 2,  8, 34 }) );

  // Test add_vv_float

  RUN_TEST( test_add_vv, float,
    ({ 0.0, 1.5,  7.5, 13.2 }),
    ({ 1.0, 3.0,  3.3,  6.3 }),
    ({   1,   0,    1,    1 }),
    ({ 1.0, 1.5, 10.8, 19.5 }) );

  RUN_TEST( test_add_vv, float,
    ({ 0.0, 1.5,  7.5, 13.2 }),
    ({ 1.0, 3.0,  3.3,  6.3 }),
    ({   0,   1,    0,    0 }),
    ({ 0.0, 4.5,  7.5, 13.2 }) );

  RUN_TEST( test_add_vv, float,
    ({ 0.0, 1.5,  7.5, 13.2 }),
    ({ 1.0, 3.0,  3.3,  6.3 }),
    ({   0,   0,    0,    0 }),
    ({ 0.0, 1.5,  7.5, 13.2 }) );

  // If all tests passed return zero

  return 0;
}

