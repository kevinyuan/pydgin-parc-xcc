#=========================================================================
# Single Floating-Point Arithmetic
# To convert decimal numbers to hexadecimal IEEE 754 bits visit:
# (or vice versa)
# http://babbage.cs.qc.edu/IEEE-754/
#=========================================================================

        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:

        li      $t0, 0x3DCCCCCD # 0.1
        li      $t1, 0x3E4CCCCD # 0.2
        li      $t3, 0x3E99999A # 0.3=0.1+0.2
        add.s   $t2, $t0, $t1
        li      $v0, 1
        bne     $t2, $t3, fail

pass:
        li        $v0, 0x0
fail:   jr        $ra

        .end    test

