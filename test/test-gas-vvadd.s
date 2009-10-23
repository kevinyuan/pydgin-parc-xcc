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
  
vec_a:    .word  1, 2, 3, 4, 5, 6, 7, 8
vec_b:    .word  1, 2, 3, 4, 5, 6, 7, 8
vec_out:  .word 17,17,17,17,17,17,17,17 #not zero to avoid thinking we passed
mov_ref:  .word  1, 2, 3, 4, 5, 6, 7, 8
add_ref:  .word  2, 4, 6, 8,10,12,14,16

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
        la        $a3, vec_out
        la        $a4, mov_ref
        la        $a5, add_ref

        # set vlen and make sure we got it
        addiu     $t0, $a0, 0       # 
        setvl     $a0, $a0          # set the VPs
        addiu     $v0, $a0, 0       # 
        bne       $t0, $a0, fail    # verify we got the setvl we wanted

        # load everything to registers
        lw.v      $vt1, $a1         #lw.v from vec_a
        lw.v      $vt2, $a2         #lw.v from vec_b
        lw.v      $vt4, $a4         #lw.v from mov_ref
        lw.v      $vt5, $a5         #lw.v from add_ref
        
add_test:
        addu.vv   $vt0, $vt1, $vt2  # vec_out = vec_a + vec_b
        sw.v      $vt0, $a3         # store answer to memory

verify_add:
        addiu     $t0, $a0, 0       # set the counter to vlen
        la        $t1, vec_out
        la        $t2, add_ref

va_loop:   
        addiu     $v0, $t0, 0
        lw        $t3, ($t1)        # load vec_out[i]  
        lw        $t4, ($t2)        # load add_ref[i]
        bne       $t3, $t4, fail    # if (vec_out[i] != add_ref[i]) fail
        
        addiu     $t1, 4            # update pointers
        addiu     $t2, 4
        addiu     $t0, -1
        bne       $zero, $t0, va_loop

mov_test:
        mov.vv    $vt0, $vt1        # vec_out = vec_a
        sw.v      $vt0, $a3         #store answer to memory
         
verify_mov:
        addiu     $t0, $a0, 0       # set the counter to vlen
        la        $t1, vec_out
        la        $t2, mov_ref

vm_loop:   
        addiu     $v0, $t0, 0
        lw        $t3, ($t1)        # load vec_out[i]  
        lw        $t4, ($t2)        # load add_ref[i]
        bne       $t3, $t4, fail    # if (vec_out[i] != add_ref[i]) fail
        
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
