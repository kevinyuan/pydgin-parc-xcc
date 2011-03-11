#=========================================================================
# Basic gas test for set.f.vv and masked addu.vv
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
vec_sum:  .word 17,17,17,17 #not zero to avoid thinking we passed
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
        addiu     $v0, $a0, 0       # 
        bne       $t0, $a0, fail    # verify we got the setvl we wanted

        # load everything to registers
        lw.v      $vt1, $a1         #lw.v from vec_a
        lw.v      $vt2, $a2         #lw.v from vec_b
        lw.v      $vt4, $a5         #lw.v from sum_ref
        lw.v      $vt5, $a6         #lw.v from msum_ref
 
        
comp_test:        
        # test compilation of flag instructions
        seq.f.vv   $flag1, $vt0, $vt0
        slt.f.vv   $flag1, $vt0, $vt0
        sltu.f.vv  $flag1, $vt0, $vt0
        not.f      $flag3, $flag1
        mov.f      $flag4, $flag3
        or.f       $flag4, $flag3, $flag1
        and.f      $flag4, $flag3, $flag1
        mtvps.f    $vt0,   $flag4 
        mfvps.f    $flag1, $vt0
        popc.f     $t0,    $flag1
        findfone.f $t0,    $flag1
        
        mov.vv    $vt0, $vt1, $flag1
        addu.vv   $vt0, $vt1, $vzero
        addu.vv   $vt0, $vt1, $vt1
          
add_test:
        addu.vv   $vt0, $vt1, $vt2, $flag0  # vec_out = vec_a + vec_b
        la        $t0, vec_sum
        sw.v      $vt0, $t0                 #store answer to memory


verify_sum:
        addiu     $t0, $a0, 0       # set the counter to vlen
        la        $t1, vec_sum
        la        $t2, sum_ref

vs_loop:   
        addiu     $v0, $t0, 0
        lw        $t3, ($t1)        # load vec_out[i]  
        lw        $t4, ($t2)        # load add_ref[i]
        bne       $t3, $t4, fail    # if (vec_out[i] != add_ref[i]) fail
        
        addiu     $t1, 4            # update pointers
        addiu     $t2, 4
        addiu     $t0, -1
        bne       $zero, $t0, vs_loop

mask_test:
        seq.f.vv  $flag2, $vt1, $vt2        # if (vec_a == vec_b) ...
        addu.vv   $vt0, $vt1, $vt2, $flag2  # vec_msum = vec_a + vec_b
        la        $t0, vec_msum
        sw.v      $vt0, $t0                 #store answer to memory
         
verify_mask:
        j pass
        addiu     $t0, $a0, 0       # set the counter to vlen
        la        $t1, vec_msum
        la        $t2, msum_ref

vm_loop:   
        addiu     $v0, $t0, 0
        lw        $t3, ($t1)        # load vec_msum[i]  
        lw        $t4, ($t2)        # load msum_ref[i]
        addiu     $v0, $t3, 0
#        addiu     $v0, $t4, 0
#        addiu     $v0, $t0, 0
        bne       $t3, $t4, fail    # if (vec_msum[i] != msum_ref[i]) fail
        
        addiu     $t1, 4            # update pointers
        addiu     $t2, 4
        addiu     $t0, -1
        bne       $zero, $t0, vm_loop
         
        j         pass

pass:
        li        $v0, 0          
        jr        $ra
fail:
        jr        $ra 

        .end      test
