#=========================================================================
# Simple Assembly Test
#=========================================================================

        .text
        .align  2
        .globl  test
        .type   test,@function
        .ent    test

test:

        li      $t0, 1
        li      $t1, 1
        add     $t2, $t0, $t1
        li      $t3, 2
	bne     $t3, $t2, failed
	li      $v0, 0
	j       done

failed:

        li      $v0, 1
	
done:

        jr       $ra
        .end    test

