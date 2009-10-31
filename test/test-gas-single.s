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

        # add.s -sNaN, sNaN, -qNaN, qNaN test
        li      $t0, 0xff982e13 # -sNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0xffd82e13 # -qNaN
        add.s   $t2, $t0, $t1
        li      $v0, 2
        bne     $t2, $t3, fail
        add.s   $t2, $t1, $t0
        li      $v0, 3
        bne     $t2, $t3, fail

        li      $t0, 0x7f982e13 # sNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0x7fd82e13 # qNaN
        add.s   $t2, $t0, $t1
        li      $v0, 4
        bne     $t2, $t3, fail
        add.s   $t2, $t1, $t0
        li      $v0, 5
        bne     $t2, $t3, fail

        li      $t0, 0xffd82e13 # -qNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0xffd82e13 # -qNaN
        add.s   $t2, $t0, $t1
        li      $v0, 6
        bne     $t2, $t3, fail
        add.s   $t2, $t1, $t0
        li      $v0, 7
        bne     $t2, $t3, fail

        li      $t0, 0x7fd82e13 # qNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0x7fd82e13 # qNaN
        add.s   $t2, $t0, $t1
        li      $v0, 8
        bne     $t2, $t3, fail
        add.s   $t2, $t1, $t0
        li      $v0, 9
        bne     $t2, $t3, fail

        # mul.s -sNaN, sNaN, -qNaN, qNaN test
        li      $t0, 0xff982e13 # -sNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0xffd82e13 # -qNaN
        mul.s   $t2, $t0, $t1
        li      $v0, 10
        bne     $t2, $t3, fail
        mul.s   $t2, $t1, $t0
        li      $v0, 11
        bne     $t2, $t3, fail

        li      $t0, 0x7f982e13 # sNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0x7fd82e13 # qNaN
        mul.s   $t2, $t0, $t1
        li      $v0, 12
        bne     $t2, $t3, fail
        mul.s   $t2, $t1, $t0
        li      $v0, 13
        bne     $t2, $t3, fail

        li      $t0, 0xffd82e13 # -qNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0xffd82e13 # -qNaN
        mul.s   $t2, $t0, $t1
        li      $v0, 14
        bne     $t2, $t3, fail
        mul.s   $t2, $t1, $t0
        li      $v0, 15
        bne     $t2, $t3, fail

        li      $t0, 0x7fd82e13 # qNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0x7fd82e13 # qNaN
        mul.s   $t2, $t0, $t1
        li      $v0, 16
        bne     $t2, $t3, fail
        mul.s   $t2, $t1, $t0
        li      $v0, 17
        bne     $t2, $t3, fail

        # div.s -sNaN, sNaN, -qNaN, qNaN test
        li      $t0, 0xff982e13 # -sNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0xffd82e13 # -qNaN
        div.s   $t2, $t0, $t1
        li      $v0, 18
        bne     $t2, $t3, fail
        div.s   $t2, $t1, $t0
        li      $v0, 19
        bne     $t2, $t3, fail

        li      $t0, 0x7f982e13 # sNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0x7fd82e13 # qNaN
        div.s   $t2, $t0, $t1
        li      $v0, 20
        bne     $t2, $t3, fail
        div.s   $t2, $t1, $t0
        li      $v0, 21
        bne     $t2, $t3, fail

        li      $t0, 0xffd82e13 # -qNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0xffd82e13 # -qNaN
        div.s   $t2, $t0, $t1
        li      $v0, 22
        bne     $t2, $t3, fail
        div.s   $t2, $t1, $t0
        li      $v0, 23
        bne     $t2, $t3, fail

        li      $t0, 0x7fd82e13 # qNaN
        li      $t1, 0xcb3fa1a9 # -12558761
        li      $t3, 0x7fd82e13 # qNaN
        div.s   $t2, $t0, $t1
        li      $v0, 24
        bne     $t2, $t3, fail
        div.s   $t2, $t1, $t0
        li      $v0, 25
        bne     $t2, $t3, fail

        # sqrt.s -sNaN, sNaN, -qNaN, qNaN test
        li      $t0, 0xff982e13 # -sNaN
        li      $t3, 0xffd82e13 # -qNaN
        sqrt.s  $t2, $t0
        li      $v0, 26
        bne     $t2, $t3, fail

        li      $t0, 0x7f982e13 # sNaN
        li      $t3, 0x7fd82e13 # qNaN
        sqrt.s  $t2, $t0
        li      $v0, 27
        bne     $t2, $t3, fail

        li      $t0, 0xffd82e13 # -qNaN
        li      $t3, 0xffd82e13 # -qNaN
        sqrt.s  $t2, $t0
        li      $v0, 28
        bne     $t2, $t3, fail

        li      $t0, 0x7fd82e13 # qNaN
        li      $t3, 0x7fd82e13 # qNaN
        sqrt.s  $t2, $t0
        li      $v0, 29
        bne     $t2, $t3, fail

        # sqrt.s negative number test
        li      $t0, 0x933ebf83 # -2.4075794086102232e-27
        li      $t3, 0xffc00000 # qNaN
        sqrt.s  $t2, $t0
        li      $v0, 30
        bne     $t2, $t3, fail

pass:
        li        $v0, 0x0
fail:   jr        $ra

        .end    test

