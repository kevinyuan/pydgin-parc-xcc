// Low-level type for atomic operations on MAVEN

#ifndef _GLIBCXX_ATOMIC_WORD_H
#define _GLIBCXX_ATOMIC_WORD_H	1

  typedef int _Atomic_word;

// todo: add something along the lines of the following to properly implement memory barriers

/*
#define _GLIBCXX_READ_MEM_BARRIER __asm __volatile ("isync":::"memory")
#ifdef __NO_LWSYNC__
#define _GLIBCXX_WRITE_MEM_BARRIER __asm __volatile ("sync":::"memory")
#else
#define _GLIBCXX_WRITE_MEM_BARRIER __asm __volatile ("lwsync":::"memory")
#endif
*/

#endif 
