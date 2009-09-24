#=========================================================================
# Basic test for symbolic register names
#=========================================================================
# This test checks to make sure the symbolic register names work. The
# EABI has eight argument registers, so for now we are mostly just
# checking that this works. Probably good to run these through the
# disassembler as well and just make sure they are printed correctly.
# Eventually we could also check to make sure symbolic vector registers
# are handled correctly here.
#
# Assumes error code is in $v0. Loads current error code into the
# register under test and then compares it to itself using the symbolic
# and numeric register names. Jumps to fail if there is a difference.
# This won't work for callee saved registers but for now we just want to
# test caller saved registers (eg. the extended argument registers).

        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:

        # Initialize $v0 which will hold error code

        li      $v0, 0
        
        # Test standard argument registers (a0-a3)

        addiu   $v0, 1
        move    $a0, $v0
        bne     $a0, $4, fail

        addiu   $v0, 1
        move    $a1, $v0
        bne     $a1, $5, fail

        addiu   $v0, 1
        move    $a2, $v0
        bne     $a2, $6, fail

        addiu   $v0, 1
        move    $a3, $v0
        bne     $a3, $7, fail

        # Test extended argument registers (a4-a7)

        addiu   $v0, 1
        move    $a4, $v0
        bne     $a4, $8, fail

        addiu   $v0, 1
        move    $a5, $v0
        bne     $a5, $9, fail

        addiu   $v0, 1
        move    $a6, $v0
        bne     $a6, $10, fail

        addiu   $v0, 1
        move    $a7, $v0
        bne     $a7, $11, fail
        
        # Test temporary registers (t0-t3, aliases for a4-a7)

        addiu   $v0, 1
        move    $t0, $v0
        bne     $t0, $8, fail

        addiu   $v0, 1
        move    $t1, $v0
        bne     $t1, $9, fail

        addiu   $v0, 1
        move    $t2, $v0
        bne     $t2, $10, fail

        addiu   $v0, 1
        move    $t3, $v0
        bne     $t3, $11, fail

        # Test temporary registers (t4-t7)

        addiu   $v0, 1
        move    $t4, $v0
        bne     $t4, $12, fail

        addiu   $v0, 1
        move    $t5, $v0
        bne     $t5, $13, fail

        addiu   $v0, 1
        move    $t6, $v0
        bne     $t6, $14, fail

        addiu   $v0, 1
        move    $t7, $v0
        bne     $t7, $15, fail

        # Test extra temporary registers (t8-t9)
        
        addiu   $v0, 1
        move    $t8, $v0
        bne     $t8, $24, fail

        addiu   $v0, 1
        move    $t9, $v0
        bne     $t9, $25, fail
        
        # Pass/fail (return zero for success, else return $v0)

        li      $v0, 0
fail:   jr      $ra

        .end    test
