//========================================================================
// Issue with register allocation
//========================================================================
// The maven port had an issue where it was using subroutine registers
// when really it should be using temporary registers. This required
// unnecessary save and restores to the stack. Turns out we were not
// setting the processor costs correctly in maven-mips.c. We now set the
// costs to DEFAULT_COSTS and the generated code is the same as what we
// were seeing on the upstream branch. We may want to adjust these costs
// in the future to more accurately represent the maven
// microarchitecture.
//
// Assembly from gcc-4.4.1 on upstream branch with -O3 (commit:bdb290b)
//
//  00000000 <foo>:
//    0:   70a41802        mul     v1,a1,a0
//    4:   00a41021        addu    v0,a1,a0
//    8:   00622021        addu    a0,v1,v0
//    c:   00862021        addu    a0,a0,a2
//   10:   03e00008        jr      ra
//   14:   00871021        addu    v0,a0,a3
//
// Assembly from master branch with -O3 (commit:7682ea5)
//
//  00000000 <foo>:
//    0:   70a41002        mul     v0,a1,a0
//    4:   27bdfff8        addiu   sp,sp,-8
//    8:   afb00004        sw      s0,4(sp)
//    c:   00a48021        addu    s0,a1,a0
//   10:   00502021        addu    a0,v0,s0
//   14:   00862021        addu    a0,a0,a2
//   18:   00871021        addu    v0,a0,a3
//   1c:   8fb00004        lw      s0,4(sp)
//   20:   03e00008        jr      ra
//   24:   27bd0008        addiu   sp,sp,8
//
// Assembly from master branch with -O3 after fixing mips-maven.c
//
//  00000000 <foo>:
//    0:   70a41802        mul     v1,a1,a0
//    4:   00a41021        addu    v0,a1,a0
//    8:   00622021        addu    a0,v1,v0
//    c:   00862021        addu    a0,a0,a2
//   10:   03e00008        jr      ra
//   14:   00871021        addu    v0,a0,a3
//

__attribute__ ((noinline))
int foo( int a, int b, int c, int d )
{
  int e = a * b;
  return a + b + c + d + e;
}

int test()
{
  int res = foo(1,2,3,4);
  if ( res != 12 )
    return 1;

  return 0;
}

