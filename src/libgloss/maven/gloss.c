/*======================================================================*/
/* gloss.c : GNU Low-Level Operating System Support                     */
/*======================================================================*/
/* This is the maven implementation of the narrow newlib operating
   system interface (gloss). It is based on the minimum stubs in the
   newlib documentation, the error stubs in libnosys, and the previous
   scale implementation. 

   Here is a list of the functions which make up the operating system
   interface. The file management instructions execute syscall assembly
   instructions so that a proxy kernel (or the simulator) can marshal up
   the request to the host machine. The process management functions are
   mainly just stubs since for now maven only supports a single process.

    - File management functions
       + open   : open file
       + lseek  : set position in file
       + read   : read from file
       + write  : write to file
       + fstat  : status of an open file
       + stat   : status of a file by name
       + close  : close a file
       + link   : rename a file
       + unlink : remote file's directory entry

    - Process management functions
       + execve : transfer control to new process
       + fork   : create a new process
       + getpid : get process id
       + kill   : send signal to child process
       + wait   : wait for a child process
       
    - Misc functions
       + isatty : query whether output stream is a terminal
       + times  : timing information for current process
       + sbrk   : increase program data space
       + _exit  : exit program without cleaning up files
   
   See the newlib documentatin for more information 
   http://sourceware.org/newlib/libc.html#Syscalls */

#include <sys/stat.h>
#include <sys/times.h>
#include <errno.h>

/*----------------------------------------------------------------------*/
/* errno                                                                */
/*----------------------------------------------------------------------*/
/* The newlib documentation notes the following: 

   Graceful failure is permitted by returning an error code. A minor
   complication arises here: the C library must be compatible with
   development environments that supply fully functional versions of
   these subroutines. Such environments usually return error codes in a
   global errno. However, the Red Hat newlib C library provides a macro
   definition for errno in the header file errno.h, as part of its
   support for reentrant routines (see Reentrancy).

   The bridge between these two interpretations of errno is
   straightforward: the C library routines with OS interface calls
   capture the errno values returned globally, and record them in the
   appropriate field of the reentrancy structure (so that you can query
   them using the errno macro from errno.h).

   This mechanism becomes visible when you write stub routines for OS
   interfaces. You must include errno.h, then disable the macro. */

#undef errno
extern int errno;

/*----------------------------------------------------------------------*/
/* environment                                                          */
/*----------------------------------------------------------------------*/
/* A pointer to a list of environment variables and their values. For a
   minimal environment, this empty list is adequate. */

char* __env[1] = { 0 };
char** environ = __env;

/*----------------------------------------------------------------------*/
/* open                                                                 */
/*----------------------------------------------------------------------*/
/* Open a file. */

int open( const char* name, int flags, int mode )
{
  return -1;
}

/*----------------------------------------------------------------------*/
/* lseek                                                                */
/*----------------------------------------------------------------------*/
/* Set position in a file. */

int lseek( int file, int ptr, int dir )
{
  return 0;
}

/*----------------------------------------------------------------------*/
/* read                                                                 */
/*----------------------------------------------------------------------*/
/* Read from a file. */

int read( int file, char* ptr, int len )
{
  return 0;
}

/*----------------------------------------------------------------------*/
/* write                                                                */
/*----------------------------------------------------------------------*/
/* Write to a file. libc subroutines will use this system routine for
   output to all files, including stdout - so if you need to generate
   any output, for example to a serial port for debugging, you should
   make your minimal write capable of doing this. The following minimal
   implementation is an incomplete example; it relies on a outbyte
   subroutine (not shown; typically, you must write this in assembler
   from examples provided by your hardware manufacturer) to actually
   perform the output. 

    int todo;
    for ( todo = 0; todo < len; todo++ )
      outbyte (*ptr++);
     
    return len; */

int write( int file, char* ptr, int len )
{
  return -1;
}

/*----------------------------------------------------------------------*/
/* fstat                                                                */
/*----------------------------------------------------------------------*/
/* Status of an open file. For consistency with other minimal
   implementations in these examples, all files are regarded as
   character special devices. The sys/stat.h header file required is
   distributed in the include subdirectory for this C library. */

int fstat( int file, struct stat* st )
{
  st->st_mode = S_IFCHR;
  return 0;
}

/*----------------------------------------------------------------------*/
/* stat                                                                 */
/*----------------------------------------------------------------------*/
/* Status of a file (by name). */

int stat( const char* file, struct stat* st )
{
  st->st_mode = S_IFCHR;
  return 0;
}

/*----------------------------------------------------------------------*/
/* close                                                                */
/*----------------------------------------------------------------------*/
/* Close a file. */

int close( int file ) 
{
  return -1;
}

/*----------------------------------------------------------------------*/
/* link                                                                 */
/*----------------------------------------------------------------------*/
/* Establish a new name for an existing file. */

int link( char* old_name, char* new_name )
{
  errno = EMLINK;
  return -1;
}


/*----------------------------------------------------------------------*/
/* unlink                                                               */
/*----------------------------------------------------------------------*/
/* Remove a file's directory entry. */

int unlink( char* name )
{
  errno = ENOENT;
  return -1;
}

/*----------------------------------------------------------------------*/
/* execve                                                               */
/*----------------------------------------------------------------------*/
/* Transfer control to a new process. Minimal implementation for a
   system without processes from newlib documentation. */

int execve( char* name, char** argv, char** env )
{
  errno = ENOMEM;
  return -1;
}

/*----------------------------------------------------------------------*/
/* fork                                                                 */
/*----------------------------------------------------------------------*/
/* Create a new process. Minimal implementation for a system without
   processes from newlib documentation. */

int fork() 
{
  errno = EAGAIN;
  return -1;
}

/*----------------------------------------------------------------------*/
/* getpid                                                               */
/*----------------------------------------------------------------------*/
/* Get process id. This is sometimes used to generate strings unlikely
   to conflict with other processes. Minimal implementation for a
   system without processes just returns 1. */

int getpid() 
{
  return 1;
}

/*----------------------------------------------------------------------*/
/* kill                                                                 */
/*----------------------------------------------------------------------*/
/* Send a signal. Minimal implementation for a system without processes
   just causes and error. */

int kill( int pid, int sig )
{
  errno = EINVAL;
  return -1;
}

/*----------------------------------------------------------------------*/
/* wait                                                                 */
/*----------------------------------------------------------------------*/
/* Wait for a child process. Minimal implementation for a system without
   processes just causes and error. */

int wait( int* status )
{
  errno = ECHILD;
  return -1;
}

/*----------------------------------------------------------------------*/
/* isatty                                                               */
/*----------------------------------------------------------------------*/
/* Query whether output stream is a terminal. For consistency with the
   other minimal implementations, which only support output to stdout,
   this minimal implementation is suggested by the newlib docs. */

int isatty( int file )
{
  return 1;
}

/*----------------------------------------------------------------------*/
/* times                                                                */
/*----------------------------------------------------------------------*/
/* Timing information for current process. */

clock_t times( struct tms* buf )
{
  return -1;
}

/*----------------------------------------------------------------------*/
/* sbrk                                                                 */
/*----------------------------------------------------------------------*/
/* Increase program data space. As malloc and related functions depend
   on this, it is useful to have a working implementation. The following
   is suggested by the newlib docs and suffices for a standalone
   system. */

caddr_t sbrk( int incr )
{
  extern char _end; /* Defined by linker */
  static char* heap_end;
  char* prev_heap_end;

  if ( heap_end == 0 )
    heap_end = &_end;

  prev_heap_end = heap_end;
  heap_end += incr;
  return (caddr_t) prev_heap_end;
}

/*----------------------------------------------------------------------*/
/* _exit                                                                */
/*----------------------------------------------------------------------*/
/* Exit a program without cleaning up files. Not really sure what to put
   here yet. libnosys causes a divide by zero exception. */

void _exit( int rc )
{

  __asm__ ( "li $v0, 4001; syscall" );

  /* Convince GCC that this function never returns. */
  for (;;)
    ;
}
