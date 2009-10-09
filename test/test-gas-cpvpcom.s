#=========================================================================
# Basic gas test for control proc/virtual proc communications
#=========================================================================
# This test does not exhaustively attempt to try and test if 
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.

                 
#-------------------------------------------------------------------------
# Test data 
#-------------------------------------------------------------------------

        .data
        .align  4
        
wvalue: .word 0xfffffffa
w_out:  .word 0
w_ref:  .word 0xfffffffb

#-------------------------------------------------------------------------
# test
#-------------------------------------------------------------------------
 
        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:
        la        $a0, wvalue
        la        $a1, w_out
        la        $a2, w_ref
        li        $a3, 2
        setvl     $a3, $a3         # get two VPs

mtvp_test:
        lw        $t0, wvalue      # lw from wvalue
        mtvp      $0, $vt0, $t0    # give VP[0],R[vt] = $t0
        li        $t0, 0           # clear $t0
        vf        vp_add_one

        mfvp      $0, $vt0, $t0    # get value back
        lw        $t1, w_ref
        li        $v0, 0xa
        bne       $t0, $t1, fail                 

        lw        $t1, wvalue
        mtvps     $vt1, $t1        # load wvalue to VP[0], VP[1]
        vf        vp_mv_add_private
        
        li        $t1, 1           # get value from VP[1]
        mfvp      $t1, $vt0, $t0   # get value back from VP[1]
        lw        $t1, w_ref
        li        $v0, 0xb
        bne       $t0, $t1, fail                 

pass:
        li        $v0, 0x0
fail:   sync.l.cv
        jr        $ra

vp_add_one:
        addiu     $t0, $t0, 1
        stop

# mtvps performed to shared register
# have VP move it to a private register for mfvp to fetch
# assumes t1 can be considered private
vp_mv_add_private:
        addiu     $t0, $t1, 1
        stop

        .end      test
