#ifndef _SH_ATOMIC_H
#define _SH_ATOMIC_H

#define SH_A_GUSA 0
#define SH_A_LLSC 1
#define SH_A_CAS 2
#if !defined(__SH3__) && !defined(__SH4__)
#define SH_A_IMASK 3
#else
#define SH_A_IMASK -1LL /* unmatchable by unsigned int */
#endif

extern __attribute__((__visibility__("hidden"))) unsigned __sh_atomic_model;

#endif
