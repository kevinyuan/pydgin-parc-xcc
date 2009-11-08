#=========================================================================
# Basic test for vpidx instruction
#=========================================================================
# This test simply has each vp write its index to memory using the vpidx
# instruction and a unit-stride vector store. This test assumes that the
# minimum vector length is equal to or greater than four.

#-------------------------------------------------------------------------
# data
#-------------------------------------------------------------------------

        .data
        .align  4

vpidx_out: .word 4, 4, 4, 4

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------

        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:

        # Set vector length to four

        li      $v0, 4
        setvl   $v0, $v0

        # Initialize $v0 which will hold error code

        li      $v0, 0

        # Check that vpidx on cp returns -1

        addiu   $v0, 1
        vpidx   $v1
        li      $a0, -1
        bne     $v1, $a0, fail

        # Keep the address of vpidx_out in a0

        la      $a0, vpidx_out

        # Vector fetch code so that each vp uses vpidx

        vf      vpidx_test
        sw.v    $vv1, $a0
        sync.l.cv

        # Verify that the correct values were written

        addiu   $v0, 1
        lw      $v1, 0($a0)
        bne     $v1, 0, fail

        addiu   $v0, 1
        lw      $v1, 4($a0)
        bne     $v1, 1, fail

        addiu   $v0, 1
        lw      $v1, 8($a0)
        bne     $v1, 2, fail

        addiu   $v0, 1
        lw      $v1, 12($a0)
        bne     $v1, 3, fail

        # Pass/fail (return zero for success, else return $v0)
pass:
        li      $v0, 0
fail:   jr      $ra

        # Vector-fetch block for each vp to use vpidx

vpidx_test:

        vpidx   $v1
        stop

        .end    test

