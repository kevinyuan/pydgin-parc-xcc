#=========================================================================
# crt1.S : Basic C runtime startup file
#=========================================================================
# crt0 and crt1 are the basic C runtime startup files. The code in these
# files is the very first thing which is run when a user application
# starts. I think crt0 is an older style startup file and that crt1 is
# more common. crt1 should define the _start symbol as this is what the
# kernel will jump to when starting the program. crt1 should then setup
# and registers, setup argc/argv, and setup the environment. It should
# execute the initialization functions by calling _init and register the
# _fini function (with the termination functions) with the runtime so
# that it is called on exit. Finally it should call main, and when main
# returns it should call exit.
#
# For now we ignore command line arguments and use an empty environment,
# but eventually we should set these up correctly. We would probably
# need to move the environ definition in gloss.c to here
        
#include <machine/regdef.h>

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
        .globl _start
        .ent   _start
_start:

        la      gp, _gp         # Initialize global pointer

        jal     _init           # Run global initialization functions
        nop

        la      a0, _fini       # Register global termination functions
        jal     atexit          # to be called upon exit
        nop

        li      a0, 0           # a0 = argc
        li      a1, 0           # a1 = argv

        jal     main            # Start the program

        move    a0, v0          # Pass exit code to exit function
        jal     exit

        .end    _start
