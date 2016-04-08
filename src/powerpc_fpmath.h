/*-
 * Copyright (c) 2003 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

union IEEEl2bits {
	long double	e;
	struct {
		unsigned int		sign	:1;
		unsigned int		exp	:11;
		unsigned int		manh	:20;
		unsigned int		manl	:32;
	} bits;
};

#define	mask_nbit_l(u)	((void)0)
#define	LDBL_IMPLICIT_NBIT
#define	LDBL_NBIT	0

#define	LDBL_MANH_SIZE	20
#define	LDBL_MANL_SIZE	32

#define	LDBL_TO_ARRAY32(u, a) do {			\
	(a)[0] = (uint32_t)(u).bits.manl;		\
	(a)[1] = (uint32_t)(u).bits.manh;		\
} while(0)

#ifdef __POWER8_VECTOR__
/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
	do {							\
		double tmp;					\
		__asm__ (					\
			"xscvdpspn %1, %2\n\t"			\
			"xxspltw %1, %1, 0\n\t"			\
			"mfvsrwz %0, %1\n\t"			\
			: "=r" ((uint32_t) i),			\
			  "=d" (tmp)				\
			: "d" ((float) d) );			\
	} while(0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
	do {							\
		__asm__ (					\
			"mtvsrwz %0, %1\n\t"			\
			"xxspltw %0, %0, 1\n\t"			\
			"xscvspdp %0, %0\n\t"			\
			: "=d" ((float) d)			\
			: "r" ((uint32_t) i) );			\
	} while(0)
#endif
