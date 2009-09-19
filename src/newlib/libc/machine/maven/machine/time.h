//========================================================================
// time.h : Machine dependent code for time                             
//========================================================================

#ifndef MAVEN_TIME_H
#define MAVEN_TIME_H

//------------------------------------------------------------------------
// CLOCKS_PER_SEC
//------------------------------------------------------------------------
// This macro specifies how many clock ticks (as returned by the clock
// function) are in a second. More specifically the _CLOCKS_PER_SEC_
// version is used to set CLOCKS_PER_SEC in
// xcc/src/newlib/libc/include/time.h 
//
// We will be using the maven cycle counter to return the number of
// clock ticks. So we should set this macro such that we can ideally
// enable microsecond resolution.

#define _CLOCKS_PER_SEC_ 1000000

#endif /* MAVEN_TIME_H */
