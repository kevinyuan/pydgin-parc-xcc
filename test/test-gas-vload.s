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
#        lwai.v    $vt2, $t0, $t1
#        lhai.v    $vt2, $t0, $t1
#        lhuai.v   $vt2, $t0, $t1
#        lbai.v    $vt2, $t0, $t1
#        lbuai.v   $vt2, $t0, $t1
        
        lwst.v    $vt2, $t0, $t1
        lhst.v    $vt2, $t0, $t1
        lhust.v   $vt2, $t0, $t1
        lbst.v    $vt2, $t0, $t1
        lbust.v   $vt2, $t0, $t1
                    
        lwseg.v   $vt2, $t0, 0x4
        lhseg.v   $vt2, $t0, 0x4
        lhuseg.v  $vt2, $t0, 0x4
        lbseg.v   $vt2, $t0, 0x4
        lbuseg.v  $vt2, $t0, 0x4
                     
        lwsegst.v   $vt2, $t0, 0x4, $t1
        lhsegst.v   $vt2, $t0, 0x4, $t1
        lhusegst.v  $vt2, $t0, 0x4, $t1
        lbsegst.v   $vt2, $t0, 0x4, $t1
        lbusegst.v  $vt2, $t0, 0x4, $t1
 
        # begin functional test of lw.v

        li        $t0, 1
        setvl     $t0, $t0        # vlen = t0
        la        $t0, wvalue     # t0 = value_ptr
        lw.v      $vt2, $t0       # t2 = *value_ptr
        vf        vp_w_verify             
        sync.l.cv
        lw        $t4, vp_ret
        li        $v0, 0x3
        bne       $t4, $zero, fail
        nop

pass:
        li        $v0, 0x0
fail:   sync.l.cv
        jr        $ra
        
vp_w_verify:
        lw        $t4, ans_w      # load answer to t4
        subu      $t4, $t4, $t2   # compare data with the answer
        sw        $t4, vp_ret     # save return value for CP to read
        stop
        
        .end      test
