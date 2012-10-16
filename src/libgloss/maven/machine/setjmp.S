#=========================================================================
# setjmp/longjmp
#=========================================================================
# This is a simple version of setjmp and longjmp for maven based on the
# MIPS32 code in xcc/src/newlib/libc/machine/mips/setjmp.S These
# functions allow control flow outside the normal function call and
# return mechanims. You first call setjmp which intializes a jmp_buf
# (jump buffer). Then a later call to longjmp with the same jmp_buf will
# essentially jump back to the original setjmp call. The return value
# from setjmp tells the user whether or not we are returning from just
# setting up the jmp_buf or if we are returning from a later longjmp
# call. Here is an example (http://en.wikipedia.org/wiki/setjmp.h)
#
#  jmp_buf g_jmp_buf;
#
#  void foo1() {
#    printf("foo1 : enter\n");
#    longjmp(g_jmp_buf,1);
#    printf("foo1 : exit");
#  }
#
#  void foo0() {
#    printf("foo0 : enter\n");
#    foo1();
#    printf("foo0 : exit\n");
#  }
#
#  int main( int argc, char* argv[] ) {
#:   if ( !setjmp(g_jmp_buf) ) {
#      foo0();
#    } 
#:   else {
#      printf("caught longjmp\n"); 
#    }
#  }
#
# Which will print this:
#
#  foo0 : enter
#  foo1 : enter
#  caught longjmp
#
# Here we see that our main function first sets up a jump buffer before
# calling the foo0 function. foo0 calls foo1 which then jumps directly
# back into the main fuction (without finishing the foo1 or the foo1
# functions). 
#
# Note that the jump buffer is required to be an array type. This means
# we don't pass in a pointer to the buffer but instead just pass in the
# buffer directly - since it is an array type this is the same as
# passing in a pointer. Usually the array has one element for each
# callee register, and basically we just save the registers into this
# array. The jump buffer type is defined in machine/setjmp.h

#-------------------------------------------------------------------------
# int setjmp( jmp_buf buf )
#-------------------------------------------------------------------------
# This is a C function written in assembly. It stores all of the callee
# saved reigsters into the given jmp_buf buffer structure. It returns
# zero. The longjump function will basically used the return address
# saved here to make it look like it is returning from setjmp.

        .text
        .align  4
        .globl  setjmp
        .type   setjmp,@function
        .ent    setjmp

setjmp:

        # Save general purpose registers
        
        sw      $s0,   0($a0)
        sw      $s1,   4($a0)
        sw      $s2,   8($a0)
        sw      $s3,  12($a0)
        sw      $s4,  16($a0)
        sw      $s5,  20($a0)
        sw      $s6,  24($a0)
        sw      $s7,  28($a0)
        sw      $sp,  32($a0)
        sw      $s8,  36($a0)
        sw      $ra,  40($a0)

        # YUNSUP: no FPRs anymore
        # Save floating point registers

        # swc1    $f20, 44($a0)
        # swc1    $f21, 48($a0)
        # swc1    $f22, 52($a0)
        # swc1    $f23, 56($a0)
        # swc1    $f24, 60($a0)
        # swc1    $f25, 64($a0)
        # swc1    $f26, 68($a0)
        # swc1    $f27, 72($a0)
        # swc1    $f28, 76($a0)
        # swc1    $f29, 80($a0)
        # swc1    $f30, 84($a0)
        # swc1    $f31, 88($a0)

        # Always return zero
        
        move    $2, $0
        j       $31

        .end    setjmp

#-------------------------------------------------------------------------
# void longjmp( jmp_buf buf, int ret_val )
#-------------------------------------------------------------------------
# This is a C function written in assembly. It loads all of the callee
# saved registers from the given jmp_buf buffer array into the actual
# registers. This includes loading the return address so that when we
# return from this function we essentially return from the earlier
# setjmp function call. The given ret_val is the return value - it will
# look like this return value is being returned from setjmp. The one
# exceptino is if the ret_val given to longjmp is zero - then it will
# actually make setjmp look like it returns one.

        .text
        .align  4
        .globl  longjmp
        .type   longjmp,@function
        .ent    longjmp

longjmp:

        # Load general purpose registers

        lw      $s0,   0($a0)
        lw      $s1,   4($a0)
        lw      $s2,   8($a0)
        lw      $s3,  12($a0)
        lw      $s4,  16($a0)
        lw      $s5,  20($a0)
        lw      $s6,  24($a0)
        lw      $s7,  28($a0)
        lw      $sp,  32($a0)
        lw      $s8,  36($a0)
        lw      $ra,  40($a0)

        # YUNSUP: no FPRs anymore
        # Load floating point registers

        # lwc1    $f20, 44($a0)
        # lwc1    $f21, 48($a0)
        # lwc1    $f22, 52($a0)
        # lwc1    $f23, 56($a0)
        # lwc1    $f24, 60($a0)
        # lwc1    $f25, 64($a0)
        # lwc1    $f26, 68($a0)
        # lwc1    $f27, 72($a0)
        # lwc1    $f28, 76($a0)
        # lwc1    $f29, 80($a0)
        # lwc1    $f30, 84($a0)        

        # Check ret_val and if zero, return 1, otherwise return ret_val

        bne     $5, $0, 1f
        li      $5, 1
1:      move    $2, $5
        j       $31

        .end    longjmp
