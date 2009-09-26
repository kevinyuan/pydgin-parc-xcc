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

#-------------------------------------------------------------------------
# data
#-------------------------------------------------------------------------

        .data
        .align  4
        
data_in:  .word 0
data_out: .word 0

#-------------------------------------------------------------------------
# Helper code
#-------------------------------------------------------------------------

        .text
        .align  4

test_vreg_prologue:

        addiu   $v0, 1
        sw      $v0, data_in
        sync.l.cv
        jr      $ra
        
test_vreg_epilogue:

        sync.l.cv
        lw      $v1, data_out
        bne     $v0, $v1, fail
        jr      $ra
        
#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------

        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test
        
test:

        # Save ra register

        sw      $ra, -4($sp)
        
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

        # Test vector register names, setvl to one

        li      $t0, 1
        setvl   $t0, $t0
        la      $t0, data_in
        la      $t1, data_out
        
        # Test vector at and return registers (vat,vv0-vv1)

        jal     test_vreg_prologue
        lw.v    $vat, $t0
        sw.v    $1, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vv0, $t0
        sw.v    $2, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vv1, $t0
        sw.v    $3, $t1
        jal     test_vreg_epilogue
        
        # Test vector argument registers (va0-va7)

        jal     test_vreg_prologue
        lw.v    $va0, $t0
        sw.v    $4, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $va1, $t0
        sw.v    $5, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $va2, $t0
        sw.v    $6, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $va3, $t0
        sw.v    $7, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $va4, $t0
        sw.v    $8, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $va5, $t0
        sw.v    $9, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $va6, $t0
        sw.v    $10, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $va7, $t0
        sw.v    $11, $t1
        jal     test_vreg_epilogue

        # Test vector temporary registers (vt0-vt7)

        jal     test_vreg_prologue
        lw.v    $vt0, $t0
        sw.v    $8, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vt1, $t0
        sw.v    $9, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vt2, $t0
        sw.v    $10, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vt3, $t0
        sw.v    $11, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vt4, $t0
        sw.v    $12, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vt5, $t0
        sw.v    $13, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vt6, $t0
        sw.v    $14, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vt7, $t0
        sw.v    $15, $t1
        jal     test_vreg_epilogue

        # Test vector saved registers (vs0-vs7)

        jal     test_vreg_prologue
        lw.v    $vs0, $t0
        sw.v    $16, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vs1, $t0
        sw.v    $17, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vs2, $t0
        sw.v    $18, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vs3, $t0
        sw.v    $19, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vs4, $t0
        sw.v    $20, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vs5, $t0
        sw.v    $21, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vs6, $t0
        sw.v    $22, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vs7, $t0
        sw.v    $23, $t1
        jal     test_vreg_epilogue
        
        # Test vector extra temporary registers (vt8-vt9)

        jal     test_vreg_prologue
        lw.v    $vt8, $t0
        sw.v    $24, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vt9, $t0
        sw.v    $25, $t1
        jal     test_vreg_epilogue
        
        # Test vector misc registers

        jal     test_vreg_prologue
        lw.v    $vgp, $t0
        sw.v    $28, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vsp, $t0
        sw.v    $29, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vfp, $t0
        sw.v    $30, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vs8, $t0
        sw.v    $30, $t1
        jal     test_vreg_epilogue
        
        jal     test_vreg_prologue
        lw.v    $vra, $t0
        sw.v    $31, $t1
        jal     test_vreg_epilogue
        
        # Test vector zero register (vzero)

        addiu   $v0, 1
        sw.v    $vzero, $t1
        sync.l.cv
        lw      $v1, ($t1)
        bnez    $v1, fail
        
        # Pass/fail (return zero for success, else return $v0)

        li      $v0, 0
fail:   lw      $ra, -4($sp)
        jr      $ra

        .end    test

