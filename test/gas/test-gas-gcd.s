#=========================================================================
# GCD Test
#=========================================================================
# The ubiguitous greatest common divsor test. We implement Euclid's
# algorithm iteratively using subtraction and then compare the results
# to a known good answer. If a test fails we return a number which
# indicates which test failed.

        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

#-------------------------------------------------------------------------
# Test data and known good answers
#-------------------------------------------------------------------------

n:        .word 7
vec_a:    .word 0, 1, 13, 15, 24, 16, 24 
vec_b:    .word 0, 1, 13, 24, 15, 24, 16
vec_ref:  .word 0, 1, 13,  3,  3,  8,  8

#-------------------------------------------------------------------------
# gcd_itr_sub( a, b )
#-------------------------------------------------------------------------

gcd_itr_sub:

        bne     $a0, $0, 1f         # if ( a == 0 )
        move    $v0, $a1            #   return b
        j       gis_done            #
1:

gis_while_loop:

        bne     $a1, $0, 1f         # if ( b == 0 )
        move    $v0, $a0            #   return a
        j       gis_done            #
1:      ble     $a0, $a1, 1f        # if ( a > b )
        subu    $a0, $a0, $a1       #   a = a - b
1:      bgt     $a0, $a1, 1f        # if ( a <= b )
        subu    $a1, $a1, $a0       #   b = b - a
1:      j       gis_while_loop
  
gis_done:

        jr      $ra

#-------------------------------------------------------------------------
# Test
#-------------------------------------------------------------------------

test:

        lw    $t0, n                # t0 = count
        la    $t1, vec_a            # t1 = vec_a_ptr
        la    $t2, vec_b            # t2 = vec_b_ptr
        la    $t3, vec_ref          # t3 = vec_ref_ptr

loop:

        beq   $t0, $0, pass

        lw    $a0, ($t1)            # a0 = *vec_a_ptr
        lw    $a1, ($t2)            # a1 = *vec_b_ptr
                                     
        sw    $ra, 0($sp)           # 
        jal   gcd_itr_sub           # v0 = gcd_itr_sub( a0, a1 )
        lw    $ra, 0($sp)           # 
                                     
        lw    $t4, ($t3)            # t4 = *vec_ref_ptr
        bne   $v0, $t4, fail        # if ( v0 != t4 ) goto fail
                                     
        subu  $t0, $t0, 1           # count = count - 1
        addu  $t1, $t1, 4           # vec_a_ptr   = vec_a_ptr + 4
        addu  $t2, $t2, 4           # vec_b_ptr   = vec_b_ptr + 4
        addu  $t3, $t3, 4           # vec_ref_ptr = vec_ref_ptr + 4
                                     
        j       loop                 
                                     
fail:                                
                                     
        lw    $t4, n                # return index of failure
        subu  $v0, $t4, $t0          
        addu  $v0, $v0, 1            
        jr    $ra                    
                                     
pass:                                
                                     
        li    $v0, 0                # return zero for success
        jr    $ra

        .end    test

