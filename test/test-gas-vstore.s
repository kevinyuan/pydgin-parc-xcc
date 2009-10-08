#=========================================================================
# Basic gas test for vector store instructions
#=========================================================================
# This test does not exhaustively attempt to try and test if the vstore
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.

                 
#-------------------------------------------------------------------------
# Test data 
#-------------------------------------------------------------------------

        .rdata
        .align  4
        
wvalue: .word  0xaabbccdd
hvalue: .hword 0xccdd
bvalue: .byte  0xdd

sw_addr: .word  0, 0, 0, 0, 0, 0, 0, 0

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------
 
        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:
        li        $t0, 1
        setvl     $t0, $t0          # set vector length to 1
        vf        vp_w_load         # tell VP to load word into t0
        sync.l.cv
        
        la        $t1, sw_addr
        sw.v      $vt0, $t1          # store vector t0 register at sw_addr (t1)
        lw        $t0, sw_addr      # read back store value
        
        lw        $t1, wvalue       # load the answer
        subu      $t1, $t1, $t0     # grade the answr
        li        $v0, 0x3          # load fail value (just in case)
        bne       $t1, $zero, fail  # failure if not the same
        

        la        $t1, sw_addr      
        sh.v      $vt0, $t1          # not a very tough test....
        sb.v      $vt0, $t1          # not a very tough test....
        
        li        $t2, 16            # t4 = stride amount
        swst.v    $vt0, $t1, $t2     # not a very tough test....
        shst.v    $vt0, $t1, $t2     # not a very tough test....
        sbst.v    $vt0, $t1, $t2     # not a very tough test....

        swseg.v   $vt0, $t1, 0x2     # not a very tough test....
        shseg.v   $vt0, $t1, 0x2     # not a very tough test....
        sbseg.v   $vt0, $t1, 0x2     # not a very tough test....
        
        swsegst.v    $vt0, $t1, 0x2, $t2     # not a very tough test....
        shsegst.v    $vt0, $t1, 0x2, $t2     # not a very tough test....
        sbsegst.v    $vt0, $t1, 0x2, $t2     # not a very tough test....
        
        
        j         pass
        nop

fail:
        jr        $ra
        nop
        
pass:
        li        $v0, 0x0
        jr        $ra
        nop
        
vp_w_load:
        lw        $t0, wvalue     # load value into t0
        stop
        
        .end      test
