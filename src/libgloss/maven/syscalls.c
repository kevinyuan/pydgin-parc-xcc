//========================================================================
// syscalls.c : Newlib operating system interface                       
//========================================================================
// This is the maven implementation of the narrow newlib operating
// system interface. It is based on the minimum stubs in the newlib
// documentation, the error stubs in libnosys, and the previous scale
// implementation.
//
// Here is a list of the functions which make up the operating system
// interface. The file management instructions execute syscall assembly
// instructions so that a proxy kernel (or the simulator) can marshal up
// the request to the host machine. The process management functions are
// mainly just stubs since for now maven only supports a single process.
//
//  - File management functions
//     + open   : (v) open file
//     + lseek  : (v) set position in file
//     + read   : (v) read from file
//     + write  : (v) write to file
//     + fstat  : (z) status of an open file
//     + stat   : (z) status of a file by name
//     + close  : (z) close a file
//     + link   : (z) rename a file
//     + unlink : (z) remote file's directory entry
//
//  - Process management functions
//     + execve : (z) transfer control to new proc
//     + fork   : (z) create a new process 
//     + getpid : (v) get process id 
//     + kill   : (z) send signal to child process
//     + wait   : (z) wait for a child process
//     
//  - Misc functions
//     + isatty : (v) query whether output stream is a terminal
//     + times  : (z) timing information for current process
//     + sbrk   : (v) increase program data space
//     + _exit  : (-) exit program without cleaning up files
//
// The current implementation of these functions assumes the following
// mips syscall instruction interface:
//
//  - a0 : syscall argument 0
//  - a1 : syscall argument 1
//  - a2 : syscall argument 2
//  - a3 : error flag (0 = success, 1 = failure)
//  - v0 : syscall id, then return value from system call
// 
// There are two types of system calls. Those which return a value when
// everything is okay (marked with (v) in above list) and those which
// return a zero when everything is okay (marked with (z) in above
// list). On an error (ie. when the error flag is 1) the return value is
// always an errno which should correspond to the numbers in
// newlib/libc/include/sys/errno.h The implementations currently assume
// that syscall arguments stay in a0-a2 which they should unless of
// course one of the functions are inlined. So keep that in mind if you
// wanted to call one function from another function.
//
// See the newlib documentation for more information 
// http://sourceware.org/newlib/libc.html#Syscalls

#include <machine/syscfg.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <errno.h>

//------------------------------------------------------------------------
// errno                                                                
//------------------------------------------------------------------------
// The newlib documentation notes the following: 
//
// Graceful failure is permitted by returning an error code. A minor
// complication arises here: the C library must be compatible with
// development environments that supply fully functional versions of
// these subroutines. Such environments usually return error codes in a
// global errno. However, the Red Hat newlib C library provides a macro
// definition for errno in the header file errno.h, as part of its
// support for reentrant routines (see Reentrancy).
//
// The bridge between these two interpretations of errno is
// straightforward: the C library routines with OS interface calls
// capture the errno values returned globally, and record them in the
// appropriate field of the reentrancy structure (so that you can query
// them using the errno macro from errno.h).
//
// This mechanism becomes visible when you write stub routines for OS
// interfaces. You must include errno.h, then disable the macro. */

#undef errno
extern int errno;

//------------------------------------------------------------------------
// environment                                                          
//------------------------------------------------------------------------
// A pointer to a list of environment variables and their values. For a
// minimal environment, this empty list is adequate. */

char* __env[1] = { 0 };
char** environ = __env;

//------------------------------------------------------------------------
// open                                                                 
//------------------------------------------------------------------------
// Open a file.

int open( const char* name, int flags, int mode )
{
  register int result     asm ("v0");
  register int error_flag asm ("a3");
  __asm__ ( "li $v0, %2; syscall" : "=r" (result), "=r" (error_flag)
                                  : "i"  (MAVEN_SYSCFG_SYSCALL_OPEN) );

  if ( error_flag == 1 ) {
    errno = result;
    return -1;
  }
  
  return result;
}

//------------------------------------------------------------------------
// lseek                                                                
//------------------------------------------------------------------------
// Set position in a file.

int lseek( int file, int ptr, int dir )
{
  register int result     asm ("v0");
  register int error_flag asm ("a3");
  __asm__ ( "li $v0, %2; syscall" : "=r" (result), "=r" (error_flag)
                                  : "i"  (MAVEN_SYSCFG_SYSCALL_LSEEK) );

  if ( error_flag == 1 ) {
    errno = result;
    return -1;
  }
  
  return result;
}

//----------------------------------------------------------------------
// read                                                                 
//----------------------------------------------------------------------
// Read from a file.

int read( int file, char* ptr, int len )
{
  register int result     asm ("v0");
  register int error_flag asm ("a3");
  __asm__ ( "li $v0, %2; syscall" : "=r" (result), "=r" (error_flag)
                                  : "i"  (MAVEN_SYSCFG_SYSCALL_READ) );

  if ( error_flag == 1 ) {
    errno = result;
    return -1;
  }
  
  return result;
}

//------------------------------------------------------------------------
// write                                                                
//------------------------------------------------------------------------
// Write to a file.

int write( int file, char* ptr, int len )
{
  register int result     asm ("v0");
  register int error_flag asm ("a3");
  __asm__ ( "li $v0, %2; syscall" : "=r" (result), "=r" (error_flag)
                                  : "i"  (MAVEN_SYSCFG_SYSCALL_WRITE) );

  if ( error_flag == 1 ) {
    errno = result;
    return -1;
  }
  
  return result;
}

//------------------------------------------------------------------------
// fstat                                                                
//------------------------------------------------------------------------
// Status of an open file. The sys/stat.h header file required is
// distributed in the include subdirectory for this C library.

int fstat( int file, struct stat* st )
{
  register int result     asm ("v0");
  register int error_flag asm ("a3");
  __asm__ ( "li $v0, %2; syscall" : "=r" (result), "=r" (error_flag)
                                  : "i"  (MAVEN_SYSCFG_SYSCALL_FSTAT) );

  if ( error_flag == 1 ) {
    errno = result;
    return -1;
  }
  
  return 0;
}

//------------------------------------------------------------------------
// stat                                                                 
//------------------------------------------------------------------------
// Status of a file (by name).

int stat( const char* file, struct stat* st )
{
  register int result     asm ("v0");
  register int error_flag asm ("a3");
  __asm__ ( "li $v0, %2; syscall" : "=r" (result), "=r" (error_flag)
                                  : "i"  (MAVEN_SYSCFG_SYSCALL_STAT) );

  if ( error_flag == 1 ) {
    errno = result;
    return -1;
  }
  
  return 0;
}

//------------------------------------------------------------------------
// close                                                                
//------------------------------------------------------------------------
// Close a file.

int close( int file ) 
{
  register int result     asm ("v0");
  register int error_flag asm ("a3");
  __asm__ ( "li $v0, %2; syscall" : "=r" (result), "=r" (error_flag)
                                  : "i"  (MAVEN_SYSCFG_SYSCALL_CLOSE) );

  if ( error_flag == 1 ) {
    errno = result;
    return -1;
  }
  
  return 0;
}

//------------------------------------------------------------------------
// link                                                                 
//------------------------------------------------------------------------
// Establish a new name for an existing file.

int link( char* old_name, char* new_name )
{
  register int result     asm ("v0");
  register int error_flag asm ("a3");
  __asm__ ( "li $v0, %2; syscall" : "=r" (result), "=r" (error_flag)
                                  : "i"  (MAVEN_SYSCFG_SYSCALL_LINK) );

  if ( error_flag == 1 ) {
    errno = result;
    return -1;
  }
  
  return 0;
}

//------------------------------------------------------------------------
// unlink                                                               
//------------------------------------------------------------------------
// Remove a file's directory entry.

int unlink( char* name )
{
  register int result     asm ("v0");
  register int error_flag asm ("a3");
  __asm__ ( "li $v0, %2; syscall" : "=r" (result), "=r" (error_flag)
                                  : "i"  (MAVEN_SYSCFG_SYSCALL_UNLINK) );

  if ( error_flag == 1 ) {
    errno = result;
    return -1;
  }
  
  return 0;
}

//------------------------------------------------------------------------
// execve                                                               
//------------------------------------------------------------------------
// Transfer control to a new process. Minimal implementation for a
// system without processes from newlib documentation.

int execve( char* name, char** argv, char** env )
{
  errno = ENOMEM;
  return -1;
}

//------------------------------------------------------------------------
// fork                                                                 
//------------------------------------------------------------------------
// Create a new process. Minimal implementation for a system without
// processes from newlib documentation.

int fork() 
{
  errno = EAGAIN;
  return -1;
}

//------------------------------------------------------------------------
// getpid                                                               
//------------------------------------------------------------------------
// Get process id. This is sometimes used to generate strings unlikely
// to conflict with other processes. Minimal implementation for a
// system without processes just returns 1.

int getpid() 
{
  return 1;
}

//------------------------------------------------------------------------
// kill                                                                 
//------------------------------------------------------------------------
// Send a signal. Minimal implementation for a system without processes
// just causes an error.

int kill( int pid, int sig )
{
  errno = EINVAL;
  return -1;
}

//------------------------------------------------------------------------
// wait                                                                 
//------------------------------------------------------------------------
// Wait for a child process. Minimal implementation for a system without
// processes just causes an error.

int wait( int* status )
{
  errno = ECHILD;
  return -1;
}

//------------------------------------------------------------------------
// isatty                                                               
//------------------------------------------------------------------------
// Query whether output stream is a terminal. For consistency with the
// other minimal implementations, which only support output to stdout,
// this minimal implementation is suggested by the newlib docs.

int isatty( int file )
{
  return 1;
}

//------------------------------------------------------------------------
// times                                                                
//------------------------------------------------------------------------
// Timing information for current process. From
// newlib/libc/include/sys/times.h the tms struct fields are as follows:
//
//  - clock_t tms_utime  : user time
//  - clock_t tms_stime  : system time
//  - clock_t tms_cutime : children's user time
//  - clock_t tms_cstime : children's system time
//
// Since maven does not currently support processes we set both of the
// children's times to zero. Eventually we might want to separately
// account for user vs system time, but for now we just return the total
// clock ticks since starting the program.
//
// We want microsecond resolution, so we will use the count control
// register from COP0. We use the macros in
// newlib/libc/machine/maven/machine/time.h to convert cycles into clock
// ticks.

clock_t times( struct tms* buf )
{
  int count;
  __asm__ ( "mfc0 %0, $%1" : "=r"(count) 
                           : "i"(MAVEN_SYSCFG_REGDEF_COP0_COUNT) );

  buf->tms_utime  = count / MAVEN_SYSCFG_CYCLES_PER_CLOCK;
  buf->tms_stime  = 0;
  buf->tms_cutime = 0;
  buf->tms_cstime = 0;
  return 0;
}

//----------------------------------------------------------------------
// sbrk                                                                 
//----------------------------------------------------------------------
// Increase program data space. As malloc and related functions depend
// on this, it is useful to have a working implementation. The following
// is suggested by the newlib docs and suffices for a standalone
// system.

caddr_t sbrk( int incr )
{
  extern char _end; // Defined by linker
  static char* heap_end;
  char* prev_heap_end;

  if ( heap_end == 0 )
    heap_end = &_end;

  prev_heap_end = heap_end;
  heap_end += incr;
  return (caddr_t) prev_heap_end;
}

//------------------------------------------------------------------------
// _exit                                                                
//------------------------------------------------------------------------
// Exit a program without cleaning up files.

void _exit( int exit_status )
{
  __asm__ ( "li $v0, %0; syscall" : : "i"(MAVEN_SYSCFG_SYSCALL_EXIT) );

  // Convince gcc this function never returns to avoid warnings (this is
  // from _exit in libnosys)
  for (;;)
    ;
}
