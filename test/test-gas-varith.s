#=========================================================================
# Basic gas test for tvec integer arithmetic ops
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
vec_b:    .word  4, 2, 3, 1
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
        slt.f.vv   $flag2, $vt1, $vt2
        not.f      $flag3, $flag1
        mov.f      $flag4, $flag3
        or.f       $flag4, $flag3, $flag1
        and.f      $flag4, $flag3, $flag1
        mtvps.f    $vt0,   $flag4 
        mfvps.f    $flag1, $vt0
        popc.f     $t0,    $flag1
        findfone.f $t0,    $flag1
        
        mov.vv    $vt0, $vt1
        mov.vv    $vt0, $vt1, $flag1
        addu.vv   $vt0, $vt1, $vt2
        addu.vv   $vt0, $vt1, $vt2, $flag1
        addu.vs   $vt0, $vt1, $a0
        addu.vs   $vt0, $vt1, $a0, $flag1
        
        subu.vv   $vt0, $vt1, $vt2
        subu.vv   $vt0, $vt1, $vt2, $flag1
        subu.vs   $vt0, $vt1, $a0
        subu.vs   $vt0, $vt1, $a0, $flag1
        subu.sv   $vt0, $a0, $vt1
        subu.sv   $vt0, $a0, $vt1, $flag1
            
        div.vv   $vt0, $vt1, $vt2
        div.vv   $vt0, $vt1, $vt2, $flag1
        div.vs   $vt0, $vt1, $a0
        div.vs   $vt0, $vt1, $a0, $flag1
        div.sv   $vt0, $a0, $vt1
        div.sv   $vt0, $a0, $vt1, $flag1
            
        rem.vv   $vt0, $vt1, $vt2
        rem.vv   $vt0, $vt1, $vt2, $flag1
        rem.vs   $vt0, $vt1, $a0
        rem.vs   $vt0, $vt1, $a0, $flag1
        rem.sv   $vt0, $a0, $vt1
        rem.sv   $vt0, $a0, $vt1, $flag1
             
        divu.vv   $vt0, $vt1, $vt2
        divu.vv   $vt0, $vt1, $vt2, $flag1
        divu.vs   $vt0, $vt1, $a0
        divu.vs   $vt0, $vt1, $a0, $flag1
        divu.sv   $vt0, $a0, $vt1
        divu.sv   $vt0, $a0, $vt1, $flag1
             
        remu.vv   $vt0, $vt1, $vt2
        remu.vv   $vt0, $vt1, $vt2, $flag1
        remu.vs   $vt0, $vt1, $a0
        remu.vs   $vt0, $vt1, $a0, $flag1
        remu.sv   $vt0, $a0, $vt1
        remu.sv   $vt0, $a0, $vt1, $flag1

        sll.vv   $vt0, $vt1, $vt2
        sll.vv   $vt0, $vt1, $vt2, $flag1
        sll.vs   $vt0, $vt1, $a0
        sll.vs   $vt0, $vt1, $a0, $flag1
        sll.sv   $vt0, $a0, $vt1
        sll.sv   $vt0, $a0, $vt1, $flag1
                               
        srl.vv   $vt0, $vt1, $vt2
        srl.vv   $vt0, $vt1, $vt2, $flag1
        srl.vs   $vt0, $vt1, $a0
        srl.vs   $vt0, $vt1, $a0, $flag1
        srl.sv   $vt0, $a0, $vt1
        srl.sv   $vt0, $a0, $vt1, $flag1
                                
        sra.vv   $vt0, $vt1, $vt2
        sra.vv   $vt0, $vt1, $vt2, $flag1
        sra.vs   $vt0, $vt1, $a0
        sra.vs   $vt0, $vt1, $a0, $flag1
        sra.sv   $vt0, $a0, $vt1
        sra.sv   $vt0, $a0, $vt1, $flag1
                                 
        and.vv   $vt0, $vt1, $vt2
        and.vv   $vt0, $vt1, $vt2, $flag1
        and.vs   $vt0, $vt1, $a0
        and.vs   $vt0, $vt1, $a0, $flag1
                                    
        or.vv   $vt0, $vt1, $vt2
        or.vv   $vt0, $vt1, $vt2, $flag1
        or.vs   $vt0, $vt1, $a0
        or.vs   $vt0, $vt1, $a0, $flag1
                              
        xor.vv   $vt0, $vt1, $vt2
        xor.vv   $vt0, $vt1, $vt2, $flag1
        xor.vs   $vt0, $vt1, $a0
        xor.vs   $vt0, $vt1, $a0, $flag1
                              
        nor.vv   $vt0, $vt1, $vt2
        nor.vv   $vt0, $vt1, $vt2, $flag1
        nor.vs   $vt0, $vt1, $a0
        nor.vs   $vt0, $vt1, $a0, $flag1
        
        j         pass

pass:
        li        $v0, 0          
        jr        $ra
fail:
        li        $v0, 2          
        jr        $ra 

        .end      test
