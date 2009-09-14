#=========================================================================
# Basic gas test for vector load instructions
#=========================================================================
# This test does not really attempt to try and test if the sync
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.

                 
#-------------------------------------------------------------------------
# Test data 
#-------------------------------------------------------------------------

        .rdata
        .align  4
        
value:  .word 0x77777777

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------
 
        .text
        .align  4
        .set    noreorder
        .globl  test
        .type   test,@function
        .ent    test

test:
        la        $t0, value      # t0 = value_ptr

        lw.v      $t2, $t0        # t2 = *value_ptr
        lh.v      $t2, $t0
        lhu.v     $t2, $t0
        lb.v      $t2, $t0
        lbu.v     $t2, $t0
        
        li        $v0, 0
        j         done

done:
        jr        $ra
        .end      test
