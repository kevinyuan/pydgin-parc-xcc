#=========================================================================
# Basic gas test for the new scalar, integer div and rem
#=========================================================================
# This test does not really attempt to try and test 
# functionality but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.
        
#-------------------------------------------------------------------------
# Test data and known good answers
#-------------------------------------------------------------------------

        .rdata
        .align  4
        
avalue:  .word 5
bvalue:  .word 2
ref_div: .word 2
ref_rem: .word 1

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------

        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:

        li      $v0, 0

        # test div
        lw      $a0, avalue
        lw      $a1, bvalue

        div     $t0, $a0, $a1
        
        lw      $t1, ref_div
        li      $v0, 2
        bne     $t0, $t1, fail
        
        # rem div
        lw      $a0, avalue
        lw      $a1, bvalue

        rem     $t0, $a0, $a1
        
        lw      $t1, ref_rem
        li      $v0, 3
        bne     $t0, $t1, fail
        
pass:
        li        $v0, 0x0
fail:   jr        $ra

        .end    test

