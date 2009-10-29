#=========================================================================
# Basic gas test for tvec floating point arithmetic ops
#=========================================================================
# This test does not exhaustively attempt to try and test if the 
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.

                 
#-------------------------------------------------------------------------
# Test data 
#-------------------------------------------------------------------------

        .data
        .align  4
  
vec_a:    .word  1, 2, 3, 4
vec_b:    .word  0, 2, 3, 1
vec_sum:  .word 17,17,17,17 
vec_msum: .word 17,17,17,17
sum_ref:  .word  1, 4, 6, 5
msum_ref: .word 17, 4, 6,17

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------
 
        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:
        li        $a0, 4            # a0 = vlen (4 to avoid stripmine)
        la        $a1, vec_a
        la        $a2, vec_b

        # set vlen and make sure we got it
        addiu     $t0, $a0, 0       # 
        setvl     $a0, $a0          # set the VPs
        bne       $t0, $a0, fail    # verify we got the setvl we wanted

        # load everything to registers
        lw.v      $vt1, $a1         #lw.v from vec_a
        lw.v      $vt2, $a2         #lw.v from vec_b
        lw.v      $vt4, $a5         #lw.v from sum_ref
        lw.v      $vt5, $a6         #lw.v from msum_ref
        
comp_test:        
        # test compilation of flag instructions
        seq.f.vv   $flag1, $vt1, $vt2
        
        add.s.vv   $vt0, $vt1, $vt2
        add.s.vv   $vt0, $vt1, $vt2, $flag1
        add.s.vs   $vt0, $vt1, $a0
        add.s.vs   $vt0, $vt1, $a0,  $flag1
        add.s.sv   $vt0, $a0,  $vt1
        add.s.sv   $vt0, $a0,  $vt1, $flag1

        sub.s.vv   $vt0, $vt1, $vt2
        sub.s.vv   $vt0, $vt1, $vt2, $flag1
        sub.s.vs   $vt0, $vt1, $a0
        sub.s.vs   $vt0, $vt1, $a0,  $flag1
        sub.s.sv   $vt0, $a0,  $vt1
        sub.s.sv   $vt0, $a0,  $vt1, $flag1

        mul.s.vv   $vt0, $vt1, $vt2
        mul.s.vv   $vt0, $vt1, $vt2, $flag1
        mul.s.vs   $vt0, $vt1, $a0
        mul.s.vs   $vt0, $vt1, $a0,  $flag1
        mul.s.sv   $vt0, $a0,  $vt1
        mul.s.sv   $vt0, $a0,  $vt1, $flag1

        div.s.vv   $vt0, $vt1, $vt2
        div.s.vv   $vt0, $vt1, $vt2, $flag1
        div.s.vs   $vt0, $vt1, $a0
        div.s.vs   $vt0, $vt1, $a0,  $flag1
        div.s.sv   $vt0, $a0,  $vt1
        div.s.sv   $vt0, $a0,  $vt1, $flag1

        abs.s.v     $vt0, $vt1
        abs.s.v     $vt0, $vt1, $flag1
        neg.s.v     $vt0, $vt1
        neg.s.v     $vt0, $vt1, $flag1
        round.w.s.v $vt0, $vt1
        round.w.s.v $vt0, $vt1, $flag1
        trunc.w.s.v $vt0, $vt1
        trunc.w.s.v $vt0, $vt1, $flag1
        ceil.w.s.v  $vt0, $vt1
        ceil.w.s.v  $vt0, $vt1, $flag1
        floor.w.s.v $vt0, $vt1
        floor.w.s.v $vt0, $vt1, $flag1
        recip.s.v   $vt0, $vt1
        recip.s.v   $vt0, $vt1, $flag1
        rsqrt.s.v   $vt0, $vt1
        rsqrt.s.v   $vt0, $vt1, $flag1
        sqrt.s.v    $vt0, $vt1
        sqrt.s.v    $vt0, $vt1, $flag1
        cvt.s.w.v   $vt0, $vt1
        cvt.s.w.v   $vt0, $vt1, $flag1
        cvt.w.s.v   $vt0, $vt1
        cvt.w.s.v   $vt0, $vt1, $flag1

        j         pass

pass:
        li        $v0, 0          
        jr        $ra
fail:
        li        $v0, 2          
        jr        $ra 

        .end      test
