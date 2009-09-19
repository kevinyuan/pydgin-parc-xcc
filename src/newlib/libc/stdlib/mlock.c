
#include <sys/lock.h>
#include <machine/syscfg.h>

#pragma weak hart_self
int hart_self()
{
  unsigned int procid;
  __asm__ ( "mfc0 %0, $%1" : "=r"(procid) 
                           : "i"(MAVEN_SYSCFG_REGDEF_COP0_CORE_ID) );
  return procid;
}

#pragma weak newlib_trylock
int newlib_trylock(int* lock)
{
  int reg = 1;
  __asm__ __volatile__ ("amo.or %0, %1, %2" : "=r"(reg) : "r"(lock), "0"(reg) : "memory");
//  __asm__ __volatile__ ("amo.swap %0, 0(%2)" : "=r"(reg) : "r"(reg), "r"(lock) : "memory");
//  __asm__ __volatile__ ("swap [%2],%0" : "=r"(reg) : "0"(reg),"r"(lock) : "memory");
  return reg;
}

#pragma weak newlib_lock
void newlib_lock(int* lock)
{
  while(newlib_trylock(lock))
    while(*(volatile int*)lock);
}

#pragma weak newlib_is_locked
int newlib_is_locked(int* lock)
{
  return *(volatile int*)lock;
}

#pragma weak newlib_recursive_trylock
int newlib_recursive_trylock(long long* lock)
{
  unsigned int lower = *(volatile unsigned int*)lock;
  unsigned int id = hart_self();
  if(newlib_is_locked((int*)lock+1) && id+1 == (lower >> 8))
  {
    if((lower & 0xFF) == 0xFF)
        __asm__ __volatile__ ("li $v0, %0; syscall" : : "i"(65535) : "v0"); //cause a panic
//      __asm__ __volatile__("ta 0x7f"); /* too many recursive acquisitions */
    *(volatile unsigned int*)lock = lower+1;
    return 0;
  }
  else if(newlib_trylock((int*)lock+1) == 0)
  {
    *(volatile unsigned int*)lock = ((id+1)<<8)+1;
    return 0;
  }
  else return -1;
}

#pragma weak newlib_recursive_lock
void newlib_recursive_lock(long long* lock)
{
  if(newlib_recursive_trylock(lock) != 0)
  {
    newlib_lock((int*)lock+1);
    unsigned int id = hart_self();
    *(volatile unsigned int*)lock = ((id+1)<<8)+1;
  }
}

#pragma weak newlib_recursive_unlock
void newlib_recursive_unlock(long long* lock)
{
  int id = hart_self();
  unsigned int lower = *(volatile unsigned int*)lock;
  if(!(newlib_is_locked((int*)lock+1) && id+1 == (lower >> 8)))
    __asm__ __volatile__ ("li $v0, %0; syscall" : : "i"(65535) : "v0"); //cause a panic
//  __asm__ __volatile__("ta 0x7f"); /* oops, someone released a free or unowned lock */
  else if((lower & 0xFF) == 1)
    *(volatile long long*)lock = 0;
  else
    *(volatile unsigned int*)lock = lower-1;
}

#ifndef MALLOC_PROVIDED
/*
FUNCTION
<<__malloc_lock>>, <<__malloc_unlock>>---lock malloc pool

INDEX
	__malloc_lock
INDEX
	__malloc_unlock

ANSI_SYNOPSIS
	#include <malloc.h>
	void __malloc_lock (struct _reent *<[reent]>);
	void __malloc_unlock (struct _reent *<[reent]>);

TRAD_SYNOPSIS
	void __malloc_lock(<[reent]>)
	struct _reent *<[reent]>;

	void __malloc_unlock(<[reent]>)
	struct _reent *<[reent]>;

DESCRIPTION
The <<malloc>> family of routines call these functions when they need to lock
the memory pool.  The version of these routines supplied in the library use
the lock API defined in sys/lock.h.  If multiple threads of execution can
call <<malloc>>, or if <<malloc>> can be called reentrantly, then you need to
define your own versions of these functions in order to safely lock the
memory pool during a call.  If you do not, the memory pool may become
corrupted.

A call to <<malloc>> may call <<__malloc_lock>> recursively; that is,
the sequence of calls may go <<__malloc_lock>>, <<__malloc_lock>>,
<<__malloc_unlock>>, <<__malloc_unlock>>.  Any implementation of these
routines must be careful to avoid causing a thread to wait for a lock
that it already holds.
*/

#include <malloc.h>
#include <sys/lock.h>

#ifndef __SINGLE_THREAD__
__LOCK_INIT_RECURSIVE(static, __malloc_lock_object);
#endif

void
__malloc_lock (ptr)
     struct _reent *ptr;
{
#ifndef __SINGLE_THREAD__
  __lock_acquire_recursive (__malloc_lock_object);
#endif
}

void
__malloc_unlock (ptr)
     struct _reent *ptr;
{
#ifndef __SINGLE_THREAD__
  __lock_release_recursive (__malloc_lock_object);
#endif
}

#endif
