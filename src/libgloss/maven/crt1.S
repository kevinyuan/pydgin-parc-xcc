#=========================================================================
# crt1.S : Basic C runtime startup file
#=========================================================================
# crt0 and crt1 are the basic C runtime startup files. The code in these
# files is the very first thing which is run when a user application
# starts. I think crt0 is an older style startup file and that crt1 is
# more common. crt1 should define the _start symbol as this is what the
# kernel will jump to when starting the program (as defined by the ENTRY
# command in the maven linker script). crt1 should then setup registers,
# setup argc/argv, setup the environment, and zero out unitilized data
# regions. It should execute the initialization functions by calling
# _init and register the _fini function (with the termination functions)
# with the runtime so that it is called on exit. Finally it should call
# main, and when main returns it should call exit.
#
# For now we do not proxy the host environment to the running
# application, but eventually we might want to set this up. When we do
# we will need to move the environ definition in syscalls.c to here.
#
# We assume that the application server or proxy kernel has put
# the command line arguments on the stack and initialized the stack
# pointer appropriately as illustrated below.
#
#  address = MEMORY_SIZE - ARGS_SIZE
#  -------------------------------------------
#  argc         : argument count                <--- stack pointer
#  argv[0]      : argument pointer 0
#  argv[1]      : argument pointer 1
#  ...
#  argv[argc-1] : argument pointer argc-1
#  NULL         : end of argument pointers
#  NULL         : extra null pointer
#  -------------------------------------------
#  arg(0)       : argument 0
#  arg(1)       : argument 1
#  ...
#  arg(argc-1)  : argument argc-1
#  -------------------------------------------
#  undefined memory from here to MEMORY_SIZE-1
#  -------------------------------------------
#  address = MEMORY_SIZE - 1
#
# Notice that we reserve a fixed size for all the argument (and
# eventually environment) storage. This is to make sure that the stack
# alignment is not a function of the arguments or environment. Based on
# this layout we need to set a0 and a1 correctly as they are the
# arguments to main(). Basically argc is stored in a0 and equals the
# value pointed to by sp, and argv is stored in a1 equals the sp+4.
#
# The crt1 will setup the stack pointers for each of the hard threads.
# It looks in a control register to see how many hard threads are
# present, and then allocates space for each stack starting at the
# initial stack pointer (MEMORY_SIZE - ARGS_SIZE) and growing towards
# smaller memory addresses. The final memory layout will look something
# like this:
#
#  address = 0
#  -------------------------------------------
#  .text and .rodata sections
#  -------------------------------------------
#  .data sections
#  ------------------------------------------- <--- _end
#  heap                                          | heap grows down
#  ...                                           v
#  -------------------------------------------
#  stack space for thread 1                      ^
#  ...                                           | stack grows up
#  ------------------------------------------- <--- thread 1 sp
#  stack space for thread 0                      ^
#  ...                                           | stack grows up
#  ------------------------------------------- <--- thread 0 sp
#  argument count and argument pointers
#  ...
#  -------------------------------------------
#  argument strings
#  ...
#  -------------------------------------------
#  undefined memory from here to MEMORY_SIZE-1
#  -------------------------------------------
#  address = MEMORY_SIZE - 1
#

#include <machine/syscfg.h>

# We define some shorter macro names for use in this file

#define c0_coreid MAVEN_SYSCFG_REGDEF_COP0_CORE_ID
#define tstack_sz MAVEN_SYSCFG_THREAD_STACK_SIZE

#-------------------------------------------------------------------------
# Global symbols defined elsewhere
#-------------------------------------------------------------------------

        .global _init           # Initialization functions
        .global _fini           # Termination functions
        .global atexit          # Register exit functions
        .global main            # User program
        .global exit            # Exit function

#-------------------------------------------------------------------------
# _start
#-------------------------------------------------------------------------

        .text
        .global _start
        .ent    _start
_start:

        la      $gp, _gp        # Initialize global pointer

        # Normally when using the pkernel, the pkernel-bootstrap.S code
        # will ensure that only core 0 ever makes it into this
        # user-level _start function. gem5 is setup such that all cores
        # begin execution in the _start function. So we check here to see
        # whether or not this code is being executed by a core other than
        # core 0. If so we just spin forever, and we will let the SEND_AM
        # system call take care of making this core do something
        # interesting.

        mfc0    $t0, $c0_coreid
_notcore0:
        bnez    $t0, _notcore0

#        la      $v0, _fbss      # Zero out uninitialized data sections
#        la      $v1, _end       #  (the .sbss and .bss sections). See
#1:      sw      $0,  0($v0)     #  linker script for how _fbss and
#        addiu   $v0, $v0, 4     #  _end are defined.
#        bltu    $v0, $v1, 1b    #

        jal     _init           # Run global initialization functions

        la      $a0, _fini      # Register global termination functions
        jal     atexit          #  to be called upon exit

        lw      $a0, 0($sp)     # Start the program, setup args
        addiu   $a1, $sp, 4     #  a0 = argc
        jal     main            #  a1 = argv

        move    $a0, $v0        # Call exit function with return value
        jal     exit            #  from main as exit code

        .end    _start
