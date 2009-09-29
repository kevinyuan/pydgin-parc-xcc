#=========================================================================
# Basic gas test for vector load instructions
#=========================================================================
# This test does not exhaustively attempt to try and test if the vload
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.

                 
#-------------------------------------------------------------------------
# Test data 
#-------------------------------------------------------------------------

        .data
        .align  4
        
wvalue: .word  0xaabbccdd
hvalue: .hword 0xccdd
bvalue: .byte  0xdd

ans_w:  .word  0xaabbccdd
ans_h:  .word  0x0000ccdd
ans_hu: .word  0xFFFFccdd
ans_b:  .word  0x000000dd
ans_bu: .word  0xFFFFFFdd

vp_ret: .word  0xe

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------
 
        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:
        la        $v0, wvalue     
        lw.v      $vt2, $t0        # not really testing functionality
        lh.v      $vt2, $t0        # not really testing functionality
        lhu.v     $vt2, $t0        # not really testing functionality
        lb.v      $vt2, $t0        # not really testing functionality
        lbu.v     $vt2, $t0        # not really testing functionality
        
        li        $t1, 1

# commented out the below instructions since they are
# unsupported by the ISA simulator 
#        lwai.v    $vt2, $t0, $t1   # not really testing functionality
#        lhai.v    $vt2, $t0, $t1   # not really testing functionality
#        lhuai.v   $vt2, $t0, $t1   # not really testing functionality
#        lbai.v    $vt2, $t0, $t1   # not really testing functionality
#        lbuai.v   $vt2, $t0, $t1   # not really testing functionality
        
        lwst.v    $vt2, $t0, $t1   # not really testing functionality
        lhst.v    $vt2, $t0, $t1   # not really testing functionality
        lhust.v   $vt2, $t0, $t1   # not really testing functionality
        lbst.v    $vt2, $t0, $t1   # not really testing functionality
        lbust.v   $vt2, $t0, $t1   # not really testing functionality
                    
#        lwseg.v   $vt2, $t0, 0x4     # not really testing functionality
#        lhseg.v   $vt2, $t0, 0x4     # not really testing functionality
#        lhuseg.v  $vt2, $t0, 0x4     # not really testing functionality
#        lbseg.v   $vt2, $t0, 0x4     # not really testing functionality
#        lbuseg.v  $vt2, $t0, 0x4     # not really testing functionality

        # begin functional test of lw.v
        li        $t0, 1
        setvl     $t0, $t0        # vlen = t0
        la        $t0, wvalue     # t0 = value_ptr
        lw.v      $vt2, $t0        # t2 = *value_ptr
        vf        vp_w_verify     
        
        sync.l.cv
        lw        $t4, vp_ret
        li        $v0, 0x3
        bne       $t4, $zero, fail
        nop
        
        j         pass
        nop

fail:
        jr        $ra
        nop
        
pass:
        li        $v0, 0x0
        jr        $ra
        nop
        
vp_w_verify:
        lw        $t4, ans_w      # load answer to t4
        subu      $t4, $t4, $t2   # compare data with the answer
        sw        $t4, vp_ret     # save return value for CP to read
        stop
        
        .end      test
