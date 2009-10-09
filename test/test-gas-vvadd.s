#=========================================================================
# Basic gas test for addu.vv & mov.vv command
#=========================================================================
# This test does not exhaustively attempt to try and test if the 
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.

                 
#-------------------------------------------------------------------------
# Test data 
#-------------------------------------------------------------------------

        .data
        .align  4
  
vec_a:    .word 1, 2, 3, 4, 5, 6, 7, 8
vec_b:    .word 1, 2, 3, 4, 5, 6, 7, 8
vec_out:  .word 0, 0, 0, 0, 0, 0, 0, 0
mov_ref:  .word 1, 2, 3, 4, 5, 6, 7, 8
add_ref:  .word 2, 4, 6, 8,10,12,14,16

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------
 
        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:
        li        $a0, 8
        la        $a1, vec_a
        la        $a2, vec_b
        la        $a3, vec_out
        la        $a4, mov_ref
        la        $a5, add_ref

        # load everything to registers
        setvl     $a0, $a0          # set the VPs
        lw.v      $vt1, $a1         # lw.v from vec_a
        lw.v      $vt2, $a2         # lw.v from vec_b
        lw.v      $vt4, $a4         # lw.v from mov_ref
        lw.v      $vt5, $a5         # lw.v from add_ref
        
add_test:
        addu.vv   $vt0, $vt1, $vt2  # vec_out = vec_a + vec_b
        sw.v      $vt0, $a3         # store answer to memory

mov_test:
        mov.vv    $vt0, $vt1        # vec_out = vec_a
        sw.v      $vt0, $a3         # store answer to memory

# do not actually verify the results
#verify_add:

#verify_mov:
        
        j         pass

pass:
        li        $v0, 0          
        jr        $ra
fail:
        jr        $ra 

        .end      test
