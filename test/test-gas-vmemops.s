#=========================================================================
# Basic gas test for vector memory ops
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
vec_out:  .word 0, 0, 0, 0, 0, 0, 0, 0
lw_ref:   .word 1, 2, 3, 4, 5, 6, 7, 8
lwsh_ref: .word 1, 2, 3, 4, 5, 6, 7, 8

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------
 
        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:
        la        $a0, vec_a
        li        $a2, 2
        la        $a3, vec_out
        la        $a1, lw_ref

w_test:
        setvl     $a2, $a2      # get two VPs
        lw.v      $vt0, $a0     #lw.v from vec_a
        sw.v      $vt0, $a3     #sw.v to vec_out
        sync.l.cv

        li        $v0, 2          # test fail number
        lw        $t0, 0($a3)     # t0 = vec_out[0]
        lw        $t1, 0($a1)     # t1 = w_ref[0]
        sw        $0, 0($a3)      # clear vec_out[0]
        bne       $t0, $t1, fail
        lw        $t0, 4($a3)     # t0 = vec_out[1]
        lw        $t1, 4($a1)     # t1 = w_ref[1]
        sw        $0, 4($a3)      # clear vec_out[1]
        bne       $t0, $t1, fail                 

wst_test:
        setvl     $a2, $a2        # get one VP
        li        $t4, 8          # t4 = stride (8 bytes)
        lwst.v    $vt0, $a0, $t4  #lwst.v from vec_a ([0] [2])
        swst.v    $vt0, $a3, $t4  #swst.v to vec_out ([0] [2])
        sync.l.cv
        
        li        $v0, 3          # test fail number
        lw        $t0, 0($a3)     # t0 = vec_out[0]
        lw        $t1, 0($a1)     # t1 = w_ref[0]
        sw        $0, 0($a3)      # clear vec_out[0]
        bne       $t0, $t1, fail
        li        $v0, 4          # test fail number
        lw        $t0, 8($a3)     # t0 = vec_out[2]
        lw        $t1, 8($a1)     # t1 = w_ref[2]
        sw        $0, 8($a3)      # clear vec_out[2]
        bne       $t0, $t1, fail
         
wsegst_test:
        setvl     $a2, $a2            # get one VP
        li        $t4, 16             # t4 = stride (16 bytes)
        lwsegst.v $vt0, $a0, 2, $t4   #lwsegst.v from vec_a ([0] [1] [4] [5])
        swsegst.v $vt0, $a3, 2, $t4   #swsegst.v to vec_out ([0] [2] [4] [5])
        sync.l.cv
        
        li        $v0, 5          # test fail number
        lw        $t0, 0($a3)     # t0 = vec_out[0]
        lw        $t1, 0($a1)     # t1 = w_ref[0]
        sw        $0, 0($a3)      # clear vec_out[0]
        bne       $t0, $t1, fail
        li        $v0, 6          # test fail number
        lw        $t0, 4($a3)     # t0 = vec_out[1]
        lw        $t1, 4($a1)     # t1 = w_ref[1]
        sw        $0, 4($a3)      # clear vec_out[1]
        bne       $t0, $t1, fail
        li        $v0, 7           # test fail number
        lw        $t0, 16($a3)     # t0 = vec_out[4]
        lw        $t1, 16($a1)     # t1 = w_ref[4]
        sw        $0, 16($a3)      # clear vec_out[4]
        bne       $t0, $t1, fail
        li        $v0, 8           # test fail number
        lw        $t0, 20($a3)     # t0 = vec_out[5]
        lw        $t1, 20($a1)     # t1 = w_ref[5]
        sw        $0, 20($a3)      # clear vec_out[5]
        bne       $t0, $t1, fail
  
wsh_test:
        lwsh.v    $vt0, $a0       #lw.v from vec_a
        sync.l.cv

        li        $v0, 9          # test fail number
        j         pass

         
pass:
        li        $v0, 0          
        jr        $ra
fail:
        jr        $ra 

        .end      test
