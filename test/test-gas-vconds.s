#=========================================================================
# Basic gas test for c.<cond>.s.f.{vv,vs,sv}
#=========================================================================
# This test does not exhaustively attempt to try and test if the 
# functionality works but rather tries to make sure that the assembler
# and the ISA simulator decode is working correctly.

        .text
        .align  4
        .globl  test
        .type   test,@function
        .ent    test

test:   
        # set vlen and make sure we got it
        li        $a0, 4            # a0 = vlen = 4
        setvl     $t0, $a0          # set the VPs
        bne       $t0, $a0, fail    # verify we got the setvl we wanted
 
        c.f.s.f.vv $flag1, $vt0, $vt1
        c.un.s.f.vv $flag1, $vt0, $vt1
        c.eq.s.f.vv $flag1, $vt0, $vt1
        c.ueq.s.f.vv $flag1, $vt0, $vt1
        c.olt.s.f.vv $flag1, $vt0, $vt1
        c.ult.s.f.vv $flag1, $vt0, $vt1
        c.ole.s.f.vv $flag1, $vt0, $vt1
        c.ule.s.f.vv $flag1, $vt0, $vt1
        c.sf.s.f.vv $flag1, $vt0, $vt1
        c.ngle.s.f.vv $flag1, $vt0, $vt1
        c.seq.s.f.vv $flag1, $vt0, $vt1
        c.ngl.s.f.vv $flag1, $vt0, $vt1
        c.lt.s.f.vv $flag1, $vt0, $vt1
        c.nge.s.f.vv $flag1, $vt0, $vt1
        c.le.s.f.vv $flag1, $vt0, $vt1
        c.ngt.s.f.vv $flag1, $vt0, $vt1

        c.f.s.f.vs $flag1, $vt0, $t1
        c.un.s.f.vs $flag1, $vt0, $t1
        c.eq.s.f.vs $flag1, $vt0, $t1
        c.ueq.s.f.vs $flag1, $vt0, $t1
        c.olt.s.f.vs $flag1, $vt0, $t1
        c.ult.s.f.vs $flag1, $vt0, $t1
        c.ole.s.f.vs $flag1, $vt0, $t1
        c.ule.s.f.vs $flag1, $vt0, $t1
        c.sf.s.f.vs $flag1, $vt0, $t1
        c.ngle.s.f.vs $flag1, $vt0, $t1
        c.seq.s.f.vs $flag1, $vt0, $t1
        c.ngl.s.f.vs $flag1, $vt0, $t1
        c.lt.s.f.vs $flag1, $vt0, $t1
        c.nge.s.f.vs $flag1, $vt0, $t1
        c.le.s.f.vs $flag1, $vt0, $t1
        c.ngt.s.f.vs $flag1, $vt0, $t1

        c.f.s.f.sv $flag1, $t0, $vt1
        c.un.s.f.sv $flag1, $t0, $vt1
        c.eq.s.f.sv $flag1, $t0, $vt1
        c.ueq.s.f.sv $flag1, $t0, $vt1
        c.olt.s.f.sv $flag1, $t0, $vt1
        c.ult.s.f.sv $flag1, $t0, $vt1
        c.ole.s.f.sv $flag1, $t0, $vt1
        c.ule.s.f.sv $flag1, $t0, $vt1
        c.sf.s.f.sv $flag1, $t0, $vt1
        c.ngle.s.f.sv $flag1, $t0, $vt1
        c.seq.s.f.sv $flag1, $t0, $vt1
        c.ngl.s.f.sv $flag1, $t0, $vt1
        c.lt.s.f.sv $flag1, $t0, $vt1
        c.nge.s.f.sv $flag1, $t0, $vt1
        c.le.s.f.sv $flag1, $t0, $vt1
        c.ngt.s.f.sv $flag1, $t0, $vt1

        li        $v0, 0          
        jr        $ra 

fail:
        li        $v0, 1
        jr        $ra

        .end      test
