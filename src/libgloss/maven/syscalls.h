/*======================================================================*/
/* syscalls.h : Maven system call numbers                               */
/*======================================================================*/
/* This is installed from xcc/src/libgloss/maven. Since these numbers
   have to exactly match the ones used in the proxy kernel, we install
   this header file into ${sysroot}/include/machine. This way the proxy
   kernel can just include this file and use the preprocessor macros
   directly. See xcc/src/libgloss/maven/syscalls.c for more information
   about the how we implement libgloss with these syscalls. */

#ifndef _MAVEN_SYSCALL_H
#define _MAVEN_SYSCALL_H

#define _MAVEN_SYSCALL_OPEN   4005
#define _MAVEN_SYSCALL_LSEEK  4019  
#define _MAVEN_SYSCALL_READ   4003
#define _MAVEN_SYSCALL_WRITE  4004
#define _MAVEN_SYSCALL_FSTAT  4108
#define _MAVEN_SYSCALL_STAT   4106
#define _MAVEN_SYSCALL_CLOSE  4006
#define _MAVEN_SYSCALL_LINK   4011
#define _MAVEN_SYSCALL_UNLINK 4010
#define _MAVEN_SYSCALL_EXIT   4001

#endif
