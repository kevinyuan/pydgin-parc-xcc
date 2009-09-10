// Low-level functions for atomic operations on MAVEN

#include <ext/atomicity.h>

_GLIBCXX_BEGIN_NAMESPACE(__gnu_cxx)

  _Atomic_word
  __attribute__ ((__unused__))
  __exchange_and_add(volatile _Atomic_word* __mem, int __val)
  {
    _Atomic_word __result;
    
    __asm__ __volatile__ ("/* Inline exchange & add */\n\t"
                          "amo.add %0, %1, %2" : "=r"(__result) : "r"(__mem), "r"(__val) : "memory" );
    
    return __result;
  }
  
  void
  __attribute__ ((__unused__))
  __atomic_add(volatile _Atomic_word* __mem, int __val)
  {
    _Atomic_word __result;
    
    __asm__ __volatile__ ("/* Inline atomic add */\n\t"
                          "amo.add %0, %1, %2" : "=r"(__result) : "r"(__mem), "r"(__val) : "memory" );
    
  }

_GLIBCXX_END_NAMESPACE
