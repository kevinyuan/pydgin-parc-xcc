#=========================================================================
# Basic gas test vector atomic memory instructions
#=========================================================================
# This test does not really attempt to try and test if the amo
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.

#-------------------------------------------------------------------------
# Test data
#-------------------------------------------------------------------------

    .data
    .align  4

vec_add_data: .word 1, 2, 3, 4
vec_add_test: .word 9, 9, 9, 9

vec_and_data: .word 0, 1, 0, 1
vec_and_test: .word 9, 9, 9, 9

vec_or_data:  .word 0, 1, 0, 1
vec_or_test:  .word 9, 9, 9, 9

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------

    .text
    .align     4
    .globl     test
    .type      test,@function
    .ent       test

test:
    li         $t0, 4
    vcfgivl    $t1, $t0, 32

    #---------------------------------------------------------------------
    # amo.add.vv
    #---------------------------------------------------------------------

    utidx.v    $vt0                  # index in vt0
    li         $t0, 2                # multiply index by 4
    sll.vs     $vt0, $vt0, $t0       #
    la         $a0, vec_add_data     # add index * 4 to base addr
    addu.vs    $vt0, $vt0, $a0       #

    li         $t0, 1
    mtvps      $vt1, $t0
    amo.add.vv $vt2, $vt0, $vt1

    # Test the destination vector register

    la         $a1, vec_add_test
    sw.v       $vt2, $a1
    sync.l.cv

    li         $v0, 1
    lw         $t0, 0x0($a1)
    bne        $t0, 1, fail

    li         $v0, 2
    lw         $t0, 0x4($a1)
    bne        $t0, 2, fail

    li         $v0, 3
    lw         $t0, 0x8($a1)
    bne        $t0, 3, fail

    li         $v0, 4
    lw         $t0, 0xc($a1)
    bne        $t0, 4, fail

    # Test the memory locations

    li         $v0, 5
    lw         $t0, 0x0($a0)
    bne        $t0, 2, fail

    li         $v0, 6
    lw         $t0, 0x4($a0)
    bne        $t0, 3, fail

    li         $v0, 7
    lw         $t0, 0x8($a0)
    bne        $t0, 4, fail

    li         $v0, 8
    lw         $t0, 0xc($a0)
    bne        $t0, 5, fail

    #---------------------------------------------------------------------
    # amo.and.vv
    #---------------------------------------------------------------------

    utidx.v    $vt0                  # index in vt0
    li         $t0, 2                # multiply index by 4
    sll.vs     $vt0, $vt0, $t0       #
    la         $a0, vec_and_data     # add index * 4 to base addr
    addu.vs    $vt0, $vt0, $a0       #

    li         $t0, 1
    mtvps      $vt1, $t0
    amo.and.vv $vt2, $vt0, $vt1

    # Test the destination vector register

    la         $a1, vec_add_test
    sw.v       $vt2, $a1
    sync.l.cv

    li         $v0, 9
    lw         $t0, 0x0($a1)
    bne        $t0, 0, fail

    li         $v0, 10
    lw         $t0, 0x4($a1)
    bne        $t0, 1, fail

    li         $v0, 11
    lw         $t0, 0x8($a1)
    bne        $t0, 0, fail

    li         $v0, 12
    lw         $t0, 0xc($a1)
    bne        $t0, 1, fail

    # Test the memory locations

    li         $v0, 13
    lw         $t0, 0x0($a0)
    bne        $t0, 0, fail

    li         $v0, 14
    lw         $t0, 0x4($a0)
    bne        $t0, 1, fail

    li         $v0, 15
    lw         $t0, 0x8($a0)
    bne        $t0, 0, fail

    li         $v0, 16
    lw         $t0, 0xc($a0)
    bne        $t0, 1, fail

    #---------------------------------------------------------------------
    # amo.or.vv
    #---------------------------------------------------------------------

    utidx.v    $vt0                  # index in vt0
    li         $t0, 2                # multiply index by 4
    sll.vs     $vt0, $vt0, $t0       #
    la         $a0, vec_and_data     # add index * 4 to base addr
    addu.vs    $vt0, $vt0, $a0       #

    li         $t0, 1
    mtvps      $vt1, $t0
    amo.or.vv  $vt2, $vt0, $vt1

    # Test the destination vector register

    la         $a1, vec_add_test
    sw.v       $vt2, $a1
    sync.l.cv

    li         $v0, 17
    lw         $t0, 0x0($a1)
    bne        $t0, 0, fail

    li         $v0, 18
    lw         $t0, 0x4($a1)
    bne        $t0, 1, fail

    li         $v0, 19
    lw         $t0, 0x8($a1)
    bne        $t0, 0, fail

    li         $v0, 20
    lw         $t0, 0xc($a1)
    bne        $t0, 1, fail

    # Test the memory locations

    li         $v0, 21
    lw         $t0, 0x0($a0)
    bne        $t0, 1, fail

    li         $v0, 22
    lw         $t0, 0x4($a0)
    bne        $t0, 1, fail

    li         $v0, 23
    lw         $t0, 0x8($a0)
    bne        $t0, 1, fail

    li         $v0, 24
    lw         $t0, 0xc($a0)
    bne        $t0, 1, fail

pass:
    li         $v0, 0x0
fail:
    sync.l.cv
    jr         $ra

    .end      test

