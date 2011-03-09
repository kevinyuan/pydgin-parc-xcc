#=========================================================================
# Basic gas test for utidx.v instruction
#=========================================================================
# This test does not really attempt to try and test if the functionality
# works but rather tries to make sure that the assembler and the ISA
# simulator decode is working correctly.

#-------------------------------------------------------------------------
# Test data
#-------------------------------------------------------------------------

    .data
    .align  4

vec_out:   .word 5, 5, 5, 5

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------

    .text
    .align     4
    .globl     test
    .type      test,@function
    .ent       test

test:
    la         $a0, vec_out       # output array

    li         $t0, 4
    vcfgivl    $t1, $t0, 32       # configure for four uTs

    utidx.v    $vt0               # put utidx in vt0
    sw.v       $vt0, $a0          # store utidx to vec_out
    sync.l.cv

    li         $v0, 1             # test number 1
    lw         $t0, 0x0($a0)      # check element 0
    bne        $t0, 0, fail

    li         $v0, 2             # test number 2
    lw         $t0, 0x4($a0)      # check element 1
    bne        $t0, 1, fail

    li         $v0, 3             # test number 3
    lw         $t0, 0x8($a0)      # check element 2
    bne        $t0, 2, fail

    li         $v0, 4             # test number 4
    lw         $t0, 0xc($a0)      # check element 3
    bne        $t0, 3, fail

pass:
    li         $v0, 0x0
fail:
    sync.l.cv
    jr         $ra

    .end      test

