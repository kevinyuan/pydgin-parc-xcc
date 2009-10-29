//========================================================================
// Test scheduling of floating point multiplies and additions
//========================================================================
// The default mips pipeline description has fully pipelined floating
// point units. The maven floating point units are also fully pipelined,
// but when executing a vector-fetched block the vector length
// essentially makes the occupancy look much longer. So here I am trying
// out changing the pipeline description. With the default pipeline
// description I get this:
//
//  00000000 <test_fp_muladd>:
//    0: mul.s   a0,a0,a1
//    4: mul.s   a2,a2,a3
//    8: mul.s   a4,a4,a5
//    c: mul.s   a6,a6,a7
//   10: add.s   a0,a0,a2
//   14: add.s   a4,a0,a4
//   18: add.s   v0,a4,a6
//   1c: jr      ra
//
// But with a customized vp pipeline description I get this for the same
// function but with an additional attribute specifying to tune for the
// vp.
//
//  00000020 <test_fp_muladd_vp>:
//   20: mul.s   a0,a0,a1
//   24: mul.s   a2,a2,a3
//   28: add.s   a0,a0,a2
//   2c: mul.s   a4,a4,a5
//   30: add.s   a0,a0,a4
//   34: mul.s   v0,a6,a7
//   38: add.s   v0,a0,v0
//   3c: jr      ra

//------------------------------------------------------------------------
// test_fp_muladd (tuned for cp)
//------------------------------------------------------------------------

__attribute__ ((noinline))
float test_fp_muladd( float src0a, float src0b, 
                      float src1a, float src1b, 
                      float src2a, float src2b, 
                      float src3a, float src3b )
{
  float temp0 = src0a * src0b;
  float temp1 = src1a * src1b;
  float temp2 = src2a * src2b;
  float temp3 = src3a * src3b;
  return ( temp0 + temp1 + temp2 + temp3 );
}

//------------------------------------------------------------------------
// test_fp_muladd (tuned for vp)
//------------------------------------------------------------------------

__attribute__ ((noinline,target("tune=maven_vp")))
float test_fp_muladd_vp( float src0a, float src0b, 
                         float src1a, float src1b, 
                         float src2a, float src2b, 
                         float src3a, float src3b )
{
  float temp0 = src0a * src0b;
  float temp1 = src1a * src1b;
  float temp2 = src2a * src2b;
  float temp3 = src3a * src3b;
  return ( temp0 + temp1 + temp2 + temp3 );
}

//------------------------------------------------------------------------
// test_fp_muladd_inline (tuned for cp)
//------------------------------------------------------------------------

static inline float 
test_fp_muladd_inner( float src0a, float src0b, 
                      float src1a, float src1b, 
                      float src2a, float src2b, 
                      float src3a, float src3b )
{
  float temp0 = src0a * src0b;
  float temp1 = src1a * src1b;
  float temp2 = src2a * src2b;
  float temp3 = src3a * src3b;
  return ( temp0 + temp1 + temp2 + temp3 );
}

__attribute__ ((noinline))
float 
test_fp_muladd_inline( float src0a, float src0b, 
                       float src1a, float src1b, 
                       float src2a, float src2b, 
                       float src3a, float src3b )
{
  return test_fp_muladd_inner( src0a, src0b, src1a, src1b,
                               src2a, src2b, src3a, src3b );
}

//------------------------------------------------------------------------
// test_fp_muladd_inline (tuned for vp)
//------------------------------------------------------------------------
// Makes sure that we can inline functions even if they do not have the 
// target("tune=maven_vp") function attribute.

static inline float 
test_fp_muladd_inner_vp( float src0a, float src0b, 
                         float src1a, float src1b, 
                         float src2a, float src2b, 
                         float src3a, float src3b )
{
  float temp0 = src0a * src0b;
  float temp1 = src1a * src1b;
  float temp2 = src2a * src2b;
  float temp3 = src3a * src3b;
  return ( temp0 + temp1 + temp2 + temp3 );
}

__attribute__ ((noinline,target("tune=maven_vp")))
float 
test_fp_muladd_inline_vp( float src0a, float src0b, 
                          float src1a, float src1b, 
                          float src2a, float src2b, 
                          float src3a, float src3b )
{
  return test_fp_muladd_inner_vp( src0a, src0b, src1a, src1b,
                                  src2a, src2b, src3a, src3b );
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test data and known good answers

  float src0a = 0.1;
  float src0b = 0.1;
  float src1a = 0.1;
  float src1b = 0.1;
  float src2a = 0.1;
  float src2b = 0.1;
  float src3a = 0.1;
  float src3b = 0.1;

  float result_ref
    = ( src0a * src0b ) + ( src1a * src1b )
    + ( src2a * src2b ) + ( src3a * src3b );

  // Try cp tuned code

  float result_0
    = test_fp_muladd( src0a, src0b, src1a, src1b, 
                      src2a, src2b, src3a, src3b );
                                   
  error_code++;
  if ( result_0 != result_ref )
    return error_code;

  // Try vp tuned code

  float result_1
    = test_fp_muladd_vp( src0a, src0b, src1a, src1b, 
                         src2a, src2b, src3a, src3b );
                                   
  error_code++;
  if ( result_1 != result_ref )
    return error_code;

  // Try cp tuned code (with inline function)

  float result_2
    = test_fp_muladd_inline( src0a, src0b, src1a, src1b, 
                             src2a, src2b, src3a, src3b );
                                   
  error_code++;
  if ( result_2 != result_ref )
    return error_code;

  // Try vp tuned code (with inlined function)

  float result_3
    = test_fp_muladd_inline_vp( src0a, src0b, src1a, src1b, 
                                src2a, src2b, src3a, src3b );
                                   
  error_code++;
  if ( result_3 != result_ref )
    return error_code;

  // If all tests passed return zero

  return 0;  
}
