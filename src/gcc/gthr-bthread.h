#ifndef GCC_GTHR_BTHREAD_H
#define GCC_GTHR_BTHREAD_H

#define __GTHREADS 1

#include <machine/syscfg.h>
//#include <bthread.h>
// manually including bthread.h

#define __BTHREAD_MUTEX_INIT 0
#define __BTHREAD_ONCE_INIT  0

//typedef unsigned int uint32;

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int __bthread_t;
typedef unsigned int __bthread_key_t;
typedef unsigned int __bthread_once_t;
typedef unsigned int __bthread_mutex_t;

static inline int __bthread_syscall(int num, int arg0, int arg1, int arg2)
{
  register int err asm ("a3");
  register int ret asm ("v0");
  __asm__ __volatile__ ("move $v0, %0\n"
      "move  $a0, %1\n"
      "move  $a1, %2\n"
      "move  $a2, %3\n"
      "syscall\n"
      : : "r"(num),"r"(arg0),"r"(arg1),"r"(arg2)
      : "v0","a0","a1","a2","a3");
  if (err)
  {
     // errno = ret;
     return -1;
  }
  else
     return ret;
}

static inline __bthread_t __bthread_self()
{
  unsigned int procid;
  __asm__ ( "mfc0 %0, $%1" : "=r"(procid) 
                           : "i"(MAVEN_SYSCFG_REGDEF_COP0_CORE_ID) );
  return procid;
}

// returns true if there is more than 1 core in the system
static inline int __bthread_threading()
{
  unsigned int temp;
  temp = __bthread_syscall( MAVEN_SYSCFG_SYSCALL_NUMCORES, 0, 0, 0 );

  return (temp > 1);
}

static inline int __bthread_mutex_trylock(__bthread_mutex_t* lock)
{
  int val = 1;
  __asm__ __volatile__ ("amo.or %0, %1, %2" : "=r"(val) : "r"(lock), "0"(val) : "memory");
  return val;
}

static inline int __bthread_mutex_lock(__bthread_mutex_t* lock)
{
  while(__bthread_mutex_trylock(lock))
    while(*(volatile int*)lock);
    
  return 0;
}

static inline int __bthread_mutex_unlock(__bthread_mutex_t* lock)
{
  *lock = 0;
  return 0;
}

static inline int 
__bthread_once( __bthread_once_t* __once, void (*__func)(void) )
{
  int ret;
  ret = __bthread_syscall( MAVEN_SYSCFG_SYSCALL_BTHREAD_ONCE, 
                          (int)__once, 0, 0 );
  if ( ret == 0 )
    __func();
  return 0;
  
}

static inline int 
__bthread_key_create( __bthread_key_t* __key, void (*__dtor) (void*) )
                                        
{
  return __bthread_syscall( MAVEN_SYSCFG_SYSCALL_BTHREAD_KEY_CREATE, 
                            (int)__key, (int)__dtor, 0);
}

static inline int
__bthread_key_delete( __bthread_key_t __key )
{
  return __bthread_syscall( MAVEN_SYSCFG_SYSCALL_BTHREAD_KEY_DELETE, 
                            (int)__key, 0, 0);
}

static inline int
__bthread_setspecific( __bthread_key_t __key, const void* __ptr )
{
  return __bthread_syscall(
           MAVEN_SYSCFG_SYSCALL_BTHREAD_KEY_SETSPECIFIC, 
           (int)__key, (int)__ptr, (int)__bthread_self());
}

static inline void* 
__bthread_getspecific( __bthread_key_t __key )
{
  return (void *) __bthread_syscall( MAVEN_SYSCFG_SYSCALL_BTHREAD_KEY_GETSPECIFIC, 
                                     (int)__key, __bthread_self(), 0);
}

#ifdef __cplusplus
}
#endif

// bthread.h

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef __bthread_key_t __gthread_key_t;
typedef __bthread_once_t __gthread_once_t;
typedef __bthread_mutex_t __gthread_mutex_t;

typedef struct {
  long depth;
  __bthread_t owner;
  __bthread_mutex_t actual;
} __gthread_recursive_mutex_t;

#define __GTHREAD_MUTEX_INIT __BTHREAD_MUTEX_INIT
#define __GTHREAD_ONCE_INIT __BTHREAD_ONCE_INIT

static inline int
__gthread_recursive_mutex_init_function(__gthread_recursive_mutex_t *__mutex);
#define __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION __gthread_recursive_mutex_init_function

#if SUPPORTS_WEAK && GTHREAD_USE_WEAK
# define __gthrw(name) \
  static __typeof(name) __gthrw_ ## name __attribute__ ((__weakref__(#name)));
# define __gthrw_(name) __gthrw_ ## name
#else
# define __gthrw(name)
# define __gthrw_(name) name
#endif

__gthrw(__bthread_once)
__gthrw(__bthread_key_create)
__gthrw(__bthread_key_delete)
__gthrw(__bthread_getspecific)
__gthrw(__bthread_setspecific)

__gthrw(__bthread_self)

__gthrw(__bthread_mutex_lock)
__gthrw(__bthread_mutex_trylock)
__gthrw(__bthread_mutex_unlock)

__gthrw(__bthread_threading)

// todo: verify this works as intended
#if SUPPORTS_WEAK && GTHREAD_USE_WEAK

static inline int
__gthread_active_p (void)
{
  return (__bthread_threading)();
}

#else /* not SUPPORTS_WEAK */

static inline int
__gthread_active_p (void)
{
  return 1;
}

#endif /* SUPPORTS_WEAK */

static inline int
__gthread_once (__gthread_once_t *__once, void (*__func) (void))
{
  if (__gthread_active_p ())
    return __gthrw_(__bthread_once) (__once, __func);
  else
    return -1;
}

static inline int
__gthread_key_create (__gthread_key_t *__key, void (*__dtor) (void *))
{
  return __gthrw_(__bthread_key_create) (__key, __dtor);
}

static inline int
__gthread_key_delete (__gthread_key_t __key)
{
  return __gthrw_(__bthread_key_delete) (__key);
}

static inline void *
__gthread_getspecific (__gthread_key_t __key)
{
  return __gthrw_(__bthread_getspecific) (__key);
}

static inline int
__gthread_setspecific (__gthread_key_t __key, const void *__ptr)
{
  return __gthrw_(__bthread_setspecific) (__key, __ptr);
}

static inline int
__gthread_mutex_destroy (__gthread_mutex_t *__mutex)
{
  return 0;
}

static inline int
__gthread_mutex_lock (__gthread_mutex_t *__mutex)
{
  if (__gthread_active_p ())
    return __gthrw_(__bthread_mutex_lock) (__mutex);
  else
    return 0;
}

static inline int
__gthread_mutex_trylock (__gthread_mutex_t *__mutex)
{
  if (__gthread_active_p ())
    return __gthrw_(__bthread_mutex_trylock) (__mutex);
  else
    return 0;
}

static inline int
__gthread_mutex_unlock (__gthread_mutex_t *__mutex)
{
  if (__gthread_active_p ())
    return __gthrw_(__bthread_mutex_unlock) (__mutex);
  else
    return 0;
}

static inline int
__gthread_recursive_mutex_init_function (__gthread_recursive_mutex_t *__mutex)
{
  __mutex->depth = 0;
  __mutex->owner = __gthrw_(__bthread_self) ();
  __mutex->actual = __BTHREAD_MUTEX_INIT;
  return 0;
}

static inline int
__gthread_recursive_mutex_lock (__gthread_recursive_mutex_t *__mutex)
{
  if (__gthread_active_p ())
    {
      __bthread_t __me = __gthrw_(__bthread_self) ();

      if (__mutex->owner != __me)
	{
	  __gthrw_(__bthread_mutex_lock) (&__mutex->actual);
	  __mutex->owner = __me;
	}

      __mutex->depth++;
    }
  return 0;
}

static inline int
__gthread_recursive_mutex_trylock (__gthread_recursive_mutex_t *__mutex)
{
  if (__gthread_active_p ())
    {
      __bthread_t __me = __gthrw_(__bthread_self) ();

      if (__mutex->owner != __me)
	{
	  if (__gthrw_(__bthread_mutex_trylock) (&__mutex->actual))
	    return 1;
	  __mutex->owner = __me;
	}

      __mutex->depth++;
    }
  return 0;
}

// todo: add error checking - what to do if this is called when
// we aren't the owner of the lock?
static inline int
__gthread_recursive_mutex_unlock (__gthread_recursive_mutex_t *__mutex)
{
  if (__gthread_active_p ())
    {
      if (--__mutex->depth == 0)
	{
	   __mutex->owner = (__bthread_t) 0;
	   __gthrw_(__bthread_mutex_unlock) (&__mutex->actual);
	}
    }
  return 0;
}

#ifdef __cplusplus
}
#endif

#endif
