// reentrancy support for maven

#include <machine/syscfg.h>
#include <reent.h>

struct _reent _reent_data[MAVEN_SYSCFG_MAX_PROCS];

struct _reent *__getreent()
{
  int procid;
  __asm__ ( "mfc0 %0, $%1" : "=r"(procid)
                           : "i"(MAVEN_SYSCFG_REGDEF_COP0_CORE_ID) );
  if (procid == 0)
    return _impure_ptr;
    
  return(&_reent_data[procid]);
}