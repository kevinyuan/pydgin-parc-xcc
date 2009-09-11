#=========================================================================
# Basic gas test for synchronization instructions
#=========================================================================
# This test does not really attempt to try and test if the sync
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.

        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:
        sync
        sync.g
        sync.l
        sync.l.v
        sync.g.v
        sync.l.cv
        sync.g.cv
        
        li      $v0, 0
        jr      $ra

        .end    test

