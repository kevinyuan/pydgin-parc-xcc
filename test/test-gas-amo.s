#=========================================================================
# Basic gas test atomic memory instructions
#=========================================================================
# This test does not really attempt to try and test if the amo
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.
        
#-------------------------------------------------------------------------
# Test data and known good answers
#-------------------------------------------------------------------------

        .rdata
        .align  4
        
value:  .word 0

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

        # test amo.add

        la      $t0, value
        li      $t1, 1
        amo.add $t1, $t0, $t1
        addu    $v0, 1
        bnez    $t1, fail

        lw      $t2, 0($t0)
        addu    $v0, 1
        li      $t3, 1
        bne     $t2, $t2, fail

        # test amo.and
        
        li      $t1, 0
        amo.and $t1, $t0, $t1
        addu    $v0, 1
        li      $t3, 1
        bne     $t1, $t3, fail
        
        lw      $t2, 0($t0)
        addu    $v0, 1
        bnez    $t2, fail
        
        # test amo.or
        
        li      $t1, 2
        amo.or  $t1, $t0, $t1
        addu    $v0, 1
        bnez    $t1, fail
        
        lw      $t2, 0($t0)
        addu    $v0, 1
        li      $t3, 2
        bne     $t2, $t3, fail
        
pass:
        li        $v0, 0x0
fail:   jr        $ra

        .end    test

