#=========================================================================
# Simple Assembly Test for Vector Commands
#=========================================================================
# This is a simple test to make sure that we can assemble basic vector
# commands correctly. Eventually we might want a less adhoc way to test
# assembling all of the vector commands.

#-------------------------------------------------------------------------
# Test data and known good answers
#-------------------------------------------------------------------------

        .rdata
        .align  4
        
n:        .word 7
vec_a:    .word 0, 1,  3, 15, 21,  0, 13 
vec_b:    .word 0, 1,  7, 24, 23, 24,  7
vec_ref:  .word 0, 2, 10, 39, 44, 24, 20
vec_out:  .word 0, 0,  0,  0,  0,  0,  0

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------

        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:
        # Do vector-vector add with stripmine loop
        
        lw      $t0, n              # t0 = count
        la      $t1, vec_a          # t1 = vec_a_ptr
        la      $t2, vec_b          # t2 = vec_b_ptr
        la      $t3, vec_out        # t3 = vec_out_ptr
        
        setvl   $s0, $t0            # s0 = vlen
        sll     $s1, $s0, 2         # s1 = stride = vlen * 4
        
stripmine_loop:

        setvl   $s0, $t0
        
        lw.v    $t1, $a0            # vload from vec_a
        lw.v    $t2, $a1            # vload from vec_b
        vf      add_vp              # vfetch add_vp
        sw.v    $t3, $a2            # vstore to vec_out

        subu    $t0, $s0            # count = count - vlen
        addu    $t1, $s1            # vec_a_ptr   = vec_a_ptr + stride
        addu    $t2, $s1            # vec_b_ptr   = vec_b_ptr + stride
        addu    $t3, $s1            # vec_out_ptr = vec_out_ptr + stride
                                     
        bnez    $t0, stripmine_loop
        sync.l
        
        # Verify results on control processor

        lw      $t0, n              # t0 = count
        la      $t1, vec_out        # t1 = vec_out_ptr
        la      $t2, vec_ref        # t2 = vec_ref_ptr

verify_loop:

        lw      $t3, ($t1)          # t3 = *vec_out_ptr
        lw      $t4, ($t2)          # t4 = *vec_ref_ptr
        bne     $t3, $t4, fail      # if ( t3 != t4 ) goto fail
        
        subu    $t0, 1              # count = count - 1
        addu    $t1, 4              # vec_out_ptr = vec_out_ptr + 4
        addu    $t2, 4              # vec_ref_ptr = vec_ref_ptr + 4

        bnez    $t0, verify_loop
        j       pass

        # Pass/fail exit code
fail:                                
        lw      $t4, n              # return index of failure
        subu    $v0, $t4, $t0        
        addu    $v0, 1          
        jr      $ra                  
                                     
pass:
        li      $v0, 0              # return zero for success
        jr      $ra

        # Add VP code
add_vp:
        addu    $a2, $a0, $a1
        stop
        
        .end    test

