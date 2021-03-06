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
        
wvalue:  .word  0xaabbccdd
hvalue:  .hword 0xccdd
bvalue:  .byte  0xdd
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
        
        la        $t1, sw_addr
        sw.v      $vt0, $t1         # store vt0 register at sw_addr (t1)
        sync.l.cv
        lw        $t0, sw_addr      # read back store value
        
        lw        $t2, wvalue       # load the answer
        subu      $t2, $t2, $t0     # grade the answr
        li        $v0, 0x3          # load fail value (just in case)
        bne       $t2, $zero, fail  # failure if not the same

        la        $t1, sw_addr      
        sh.v      $vt0, $t1         # not a very tough test....
        sb.v      $vt0, $t1         # not a very tough test....
        
        li        $t2, 16           # t4 = stride amount
        swst.v    $vt0, $t1, $t2    # not a very tough test....
        shst.v    $vt0, $t1, $t2    # not a very tough test....
        sbst.v    $vt0, $t1, $t2    # not a very tough test....

        swseg.v   $vt0, $t1, 0x2    # not a very tough test....
        shseg.v   $vt0, $t1, 0x2    # not a very tough test....
        sbseg.v   $vt0, $t1, 0x2    # not a very tough test....
        
        swsegst.v $vt0, $t1, 0x2, $t2  # not a very tough test....
        shsegst.v $vt0, $t1, 0x2, $t2  # not a very tough test....
        sbsegst.v $vt0, $t1, 0x2, $t2  # not a very tough test....
        
        swx.v     $vt1, $t1, $vzero      # not a very tough test....
        shx.v     $vt1, $t1, $vzero      # not a very tough test....
        sbx.v     $vt1, $t1, $vzero      # not a very tough test....
#        swx.v     $vt1, $t1, $vt0      # not a very tough test....
#        shx.v     $vt1, $t1, $vt0      # not a very tough test....
#        sbx.v     $vt1, $t1, $vt0      # not a very tough test....
pass:
        li        $v0, 0x0
fail:   sync.l.cv
        jr        $ra
        
vp_w_load:
        lw        $t0, wvalue     # load value into t0
        stop
        
        .end      test
